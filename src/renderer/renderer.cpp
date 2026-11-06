#include "renderer/renderer.h"
#include "glm/gtx/norm.hpp"
#include "scene/lights/spot_light.h"
#include "renderer/frame_buffers/shadow_cubemap_frame_buffer.h"
#include "renderer/frame_buffers/shadow_frame_buffer.h"
#include "assimp_wrapper/model.h"
#include "renderer/constants.h"

using namespace renderer;

Renderer::Renderer(std::shared_ptr<const scene::Scene> scene)
    : scene_(scene)
    , clear_color_rgba_(0.0)
    , curr_shader_ptr_(nullptr)
    , view_mat_(1.0)
    , projection_mat_(1.0)
    , camera_position_(1.0) 
    , post_processor_(1, 1)
    , post_processing_type_(PostProcessingTypes::NO_POST_PROCESSING)   
    , geometry_processor_()
    , geometry_processing_type_(GeometryProcessingTypes::NO_GEOMETRY_PROCESSING)
    , g_buffer_(1, 1)
    , internal_objects_id_mask_(0)
    , framebuffer_width_(0)
    , framebuffer_height_(0)
{

    shadow_frame_buffers_.emplace(
        scene::lights::LightType::DIRECTIONAL_LIGHT, 
        std::make_unique<framebuffers::ShadowFrameBuffer>(framebuffers::ShadowFrameBuffer(framebuffers::DEFAULT_SHADOW_MAP_WIDTH, framebuffers::DEFAULT_SHADOW_MAP_HEIGHT))
    );

    shadow_frame_buffers_.emplace(
        scene::lights::LightType::SPOT_LIGHT, 
        std::make_unique<framebuffers::ShadowFrameBuffer>(framebuffers::ShadowFrameBuffer(framebuffers::DEFAULT_SHADOW_MAP_WIDTH, framebuffers::DEFAULT_SHADOW_MAP_HEIGHT))
    );

    shadow_frame_buffers_.emplace(
        scene::lights::LightType::POINT_LIGHT, 
        std::make_unique<framebuffers::ShadowCubemapFrameBuffer>(framebuffers::ShadowCubemapFrameBuffer(framebuffers::DEFAULT_SHADOW_MAP_WIDTH, framebuffers::DEFAULT_SHADOW_MAP_HEIGHT))
    );





    // OpenGL configuration
    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0x00);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Renderer::set_general_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr)
{
    shader_ptr->set_uniform("u_view_mat", view_mat_);
    shader_ptr->set_uniform("u_projection_mat", projection_mat_);
    shader_ptr->set_uniform("u_camera_pos", camera_position_);


    shader_ptr->set_uniform("u_directional_lights_shadow_map_array", bind_texture_units::DIRECTIONAL_LIGHTS_SHADOW_MAP_TEXTURE_UNIT);
    glBindTextureUnit(bind_texture_units::DIRECTIONAL_LIGHTS_SHADOW_MAP_TEXTURE_UNIT, shadow_frame_buffers_.at(scene::lights::LightType::DIRECTIONAL_LIGHT)->get_depth_map());
    shader_ptr->set_uniform("u_spot_lights_shadow_map_array", bind_texture_units::SPOT_LIGHTS_SHADOW_MAP_TEXTURE_UNIT);
    glBindTextureUnit(bind_texture_units::SPOT_LIGHTS_SHADOW_MAP_TEXTURE_UNIT, shadow_frame_buffers_.at(scene::lights::LightType::SPOT_LIGHT)->get_depth_map());
    shader_ptr->set_uniform("u_point_lights_shadow_map_array", bind_texture_units::POINT_LIGHTS_SHADOW_MAP_TEXTURE_UNIT);
    glBindTextureUnit(bind_texture_units::POINT_LIGHTS_SHADOW_MAP_TEXTURE_UNIT, shadow_frame_buffers_.at(scene::lights::LightType::POINT_LIGHT)->get_depth_map());


    shader_ptr->set_uniform("frag_coords_texture", bind_texture_units::FRAG_COORDS_TEXTURE_UNIT);
    glBindTextureUnit(bind_texture_units::FRAG_COORDS_TEXTURE_UNIT, g_buffer_.get_position_buffer_texture());
    shader_ptr->set_uniform("frag_normal_texture", bind_texture_units::FRAG_NORMAL_TEXTURE_UNIT);
    glBindTextureUnit(bind_texture_units::FRAG_NORMAL_TEXTURE_UNIT, g_buffer_.get_normal_buffer_texture());
    shader_ptr->set_uniform("frag_color_texture", bind_texture_units::FRAG_COLOR_TEXTURE_UNIT);
    glBindTextureUnit(bind_texture_units::FRAG_COLOR_TEXTURE_UNIT, g_buffer_.get_albedospec_buffer_texture());
  

}

void Renderer::set_lighting_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr)
{
    uint32_t directional_light_idx = 0;
    uint32_t point_light_idx = 0;
    uint32_t spot_light_idx = 0;

    for (auto&& light: scene_lights_buffer_) {
        switch (light->get_type())
        {
        case scene::lights::LightType::DIRECTIONAL_LIGHT:
            light->upload_components(*shader_ptr, directional_light_idx++);
            break;
        case scene::lights::LightType::POINT_LIGHT:
            light->upload_components(*shader_ptr, point_light_idx++);
            break;
        case scene::lights::LightType::SPOT_LIGHT:
            light->upload_components(*shader_ptr, spot_light_idx++);
            break;
        default:
            spdlog::error("Unknown light type, type");
            assert(false);
            break;
        }   
    }

    shader_ptr->set_uniform("u_directional_lights_count", directional_light_idx);
    shader_ptr->set_uniform("u_point_lights_count", point_light_idx);
    shader_ptr->set_uniform("u_spot_lights_count", spot_light_idx);
}

void renderer::Renderer::build_rendering_queues()
{
    opaque_objects_render_queue_.clear();
    transparent_objects_render_queue_.clear();
    forward_render_render_queue_.clear();

    // special objects
    for (const auto &obj : scene_->get_internal_objects()) {
        if (internal_objects_id_mask_ & obj.id) {
            obj.ptr->move(glm::vec3(camera_position_.x, obj.ptr->get_move_vector().y, camera_position_.z));
            
            if (obj.id == renderer::defaults::XZ_FACE_SOLID_ID) // opaque plane where shadows can be rendered
                opaque_objects_render_queue_.emplace_back(RenderQueueObject{.obj_ptr=obj.ptr, .distance_to_viewer=0.0f, .draw_mode=obj.draw_mode});
            else 
                forward_render_render_queue_.emplace_back(RenderQueueObject{.obj_ptr=obj.ptr, .distance_to_viewer=0.0f, .draw_mode=obj.draw_mode});
        }
    }

    // lights models
    for (auto&& obj : scene_->get_light_objects()) {
        forward_render_render_queue_.emplace_back(RenderQueueObject{.obj_ptr=obj, .distance_to_viewer=0.0f, .draw_mode=assimp_wrapper::DEFAULT_DRAW_MODE});
    }

    for (const auto &obj : scene_->get_model_objects())
    {
        switch (obj->model_type())
        {
        case assimp_wrapper::ModelType::OPAQUE:
            opaque_objects_render_queue_.emplace_back(RenderQueueObject{.obj_ptr=obj, .distance_to_viewer=0.0f, .draw_mode=assimp_wrapper::DEFAULT_DRAW_MODE});
            break;
        case assimp_wrapper::ModelType::TRANSPARENT:
        {
            float distance = glm::length2(camera_position_ - obj->get_move_vector());
            transparent_objects_render_queue_.emplace_back(RenderQueueObject{.obj_ptr=obj, .distance_to_viewer=distance, .draw_mode=assimp_wrapper::DEFAULT_DRAW_MODE});
        }
            break;
        case assimp_wrapper::ModelType::OPAQUE_AND_TRANSPARENT:
        {
            float distance = glm::length2(camera_position_ - obj->get_move_vector());
            transparent_objects_render_queue_.emplace_back(RenderQueueObject{.obj_ptr=obj, .distance_to_viewer=distance, .draw_mode=assimp_wrapper::DEFAULT_DRAW_MODE});
            opaque_objects_render_queue_.emplace_back(RenderQueueObject{.obj_ptr=obj, .distance_to_viewer=0.0f, .draw_mode=assimp_wrapper::DEFAULT_DRAW_MODE});
        }
            break;
        
        default:
            spdlog::error("Unknown model_type passed to renderer: {}", static_cast<int>(obj->model_type()));
            assert(false);
            break;
        }
    }

    std::sort(
        transparent_objects_render_queue_.begin(), 
        transparent_objects_render_queue_.end(), 
        [](const RenderQueueObject& lhs, const RenderQueueObject& rhs) 
        { 
            return lhs.distance_to_viewer > rhs.distance_to_viewer; 
        }
    );
}

void renderer::Renderer::do_shadow_map_render()
{
    std::shared_ptr<Shader> shader = nullptr;
    
    uint32_t directional_light_idx = 0;
    uint32_t point_light_idx = 0;
    uint32_t spot_light_idx = 0;

    for (auto&& light: scene_lights_buffer_) {

        const auto type = light->get_type();
        framebuffers::IShadowFrameBuffer* fb = shadow_frame_buffers_.at(type).get(); // todo: raw pointer
        glViewport(0, 0, fb->get_width(), fb->get_height());

        std::shared_ptr<Shader> shader = (type == scene::lights::LightType::POINT_LIGHT)
                ? scene_->get_internal_shader(scene::InternalShadersIDS::SHADOW_MAP_POINT_LIGHT_SHADER)
                : scene_->get_internal_shader(scene::InternalShadersIDS::SHADOW_MAP_EMPTY_FRAG_SHADER);

        shader->use();


        uint32_t base_slice;
        if (type == scene::lights::LightType::DIRECTIONAL_LIGHT) base_slice = directional_light_idx++;
        else if (type == scene::lights::LightType::SPOT_LIGHT) base_slice = spot_light_idx++;
        else if (type == scene::lights::LightType::POINT_LIGHT) base_slice = point_light_idx++ * framebuffers::ShadowCubemapFrameBuffer::CUBEMAP_LAYERS_MULTIPLICATOR;
        else assert(false && "Unsupported light type");

        const size_t view_count = light->get_view_count();
        for (size_t face = 0; face < view_count; ++face) {
            fb->bind(base_slice + face);

            shader->set_uniform("u_light_space_mat", light->projection_matrix(fb->get_width(), fb->get_height()) * light->view_matrix(face));

            if (type == scene::lights::LightType::POINT_LIGHT) {
                light->upload_components(*shader, 0); // upload additional components

            }

            render_shadow_depth_buffer(shader);

        }
        
        fb->unbind();
    }

    glViewport(0, 0, framebuffer_width_, framebuffer_height_);
    if (curr_shader_ptr_ != nullptr) curr_shader_ptr_->use(); // restore last used shader
}

void renderer::Renderer::do_geometry_path_render()
{
    // render all opaque MESHES except lights
    g_buffer_.rescale(framebuffer_width_, framebuffer_height_);
    g_buffer_.bind();

    glClearColor(clear_color_rgba_.r, clear_color_rgba_.g, clear_color_rgba_.b, clear_color_rgba_.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glDisable(GL_BLEND);
    std::shared_ptr<Shader> sh = nullptr;
    for (const auto& obj: opaque_objects_render_queue_) {
        sh = obj.obj_ptr->get_geometry_path_shader();
        sh->use();

        sh->set_uniform("u_view_mat", view_mat_);
        sh->set_uniform("u_projection_mat", projection_mat_);
        sh->set_uniform("u_camera_pos", camera_position_);

        obj.obj_ptr->render_geometry_path(obj.draw_mode); 
    }
    glEnable(GL_BLEND);

    g_buffer_.unbind();
    if (curr_shader_ptr_ != nullptr) curr_shader_ptr_->use(); // restore last used shader
}

void renderer::Renderer::do_color_path_render()
{
    glClearColor(clear_color_rgba_.r, clear_color_rgba_.g, clear_color_rgba_.b, clear_color_rgba_.a);
    glClear(GL_DEPTH_BUFFER_BIT);

    use_shader(scene_->get_internal_shader(scene::InternalShadersIDS::COLOR_PATH_SHADER));

    glDisable(GL_DEPTH_TEST);
    post_processor_.render_quad();
    glEnable(GL_DEPTH_TEST);
}

void renderer::Renderer::do_forward_path_render()
{
    // copy depth buffer from g-buffer
    g_buffer_.blit_depth_to_default_buffer(framebuffer_width_, framebuffer_height_);
   
    // render lights
    for (auto&& obj : forward_render_render_queue_) {
        use_shader(obj.obj_ptr->get_shader());
        obj.obj_ptr->render(obj.draw_mode, false);
        obj.obj_ptr->render(obj.draw_mode, true);
    }

    render_transparent_scene_objects();
}

void renderer::Renderer::use_shader(const std::shared_ptr<const scene::physical_objects::IRender>& obj)
{
    std::shared_ptr<Shader> shader_ptr = obj->get_shader();
    if (shader_ptr == nullptr) {
        spdlog::warn("Failed to use model's shader, {} hasn't shader", obj->get_name());
        return;
    }

    use_shader(shader_ptr);
}

void renderer::Renderer::use_shader(const std::shared_ptr<Shader> &shader_ptr)
{
    if (shader_ptr.get() != curr_shader_ptr_) 
    {
        curr_shader_ptr_ = shader_ptr.get();
        shader_ptr->use();
    }

    set_general_shader_uniforms(shader_ptr);
    set_lighting_shader_uniforms(shader_ptr);
}

void Renderer::render_opaque_scene_objects()
{
    if (geometry_processing_type_ == GeometryProcessingTypes::NO_GEOMETRY_PROCESSING) {
        for (const auto& obj: opaque_objects_render_queue_) {
            use_shader(obj.obj_ptr);
            if (obj.obj_ptr->is_selected()) {
                render_scene_object_with_outline(obj); 
            }
            else {
                obj.obj_ptr->render(obj.draw_mode, false); 
            }
        }
    } else if (geometry_processing_type_ == GeometryProcessingTypes::NORMALS_VISUALIZATIONS) {
        auto processing_shader = geometry_processor_.get_geometry_shader(geometry_processing_type_);
        for (const auto& obj: opaque_objects_render_queue_) {
            use_shader(obj.obj_ptr);
            obj.obj_ptr->render(obj.draw_mode, false, processing_shader); 
            use_shader(processing_shader);
            obj.obj_ptr->render(obj.draw_mode, false, processing_shader); 
        }
    } else {
        auto processing_shader = geometry_processor_.get_geometry_shader(geometry_processing_type_);
        use_shader(processing_shader);
        for (const auto& obj: opaque_objects_render_queue_) {
            obj.obj_ptr->render(obj.draw_mode, false, processing_shader);
        } 
    }
}

void renderer::Renderer::render_transparent_scene_objects()
{
    // render all opaque meshes first, then transparent
    // the model can contain only transparent meshes,
    // but can contain a mix of opaque and transparent
    if (geometry_processing_type_ == GeometryProcessingTypes::NO_GEOMETRY_PROCESSING) {
        for (const auto& obj: transparent_objects_render_queue_) {
            use_shader(obj.obj_ptr);
            glDepthMask(GL_FALSE);
            obj.obj_ptr->render(obj.draw_mode, true); 
            glDepthMask(GL_TRUE);
        }
    } else if (geometry_processing_type_ == GeometryProcessingTypes::NORMALS_VISUALIZATIONS) {
        auto processing_shader = geometry_processor_.get_geometry_shader(geometry_processing_type_);
        for (const auto& obj: transparent_objects_render_queue_) {
            glDepthMask(GL_FALSE);
            use_shader(obj.obj_ptr);
            obj.obj_ptr->render(obj.draw_mode, true); 
            use_shader(processing_shader);
            obj.obj_ptr->render(obj.draw_mode, true, processing_shader); 
            glDepthMask(GL_TRUE);
        }

    } else {
        auto processing_shader = geometry_processor_.get_geometry_shader(geometry_processing_type_);
        use_shader(processing_shader);
        for (const auto& obj: transparent_objects_render_queue_) {
            glDepthMask(GL_FALSE);
            obj.obj_ptr->render(obj.draw_mode, true, processing_shader); 
            glDepthMask(GL_TRUE);
        }
    }
}

void Renderer::render_scene_object_with_outline(const RenderQueueObject& obj)
{
    // todo: fix outline
    use_shader(obj.obj_ptr);

    // render normal object with writing to stencil buffer
    // to mark pixels that we have painted 
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // doesn't discard any fragment
    glStencilMask(0xFF); // allow writing to stencil buffer
    obj.obj_ptr->render(obj.draw_mode, false);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    obj.obj_ptr->render(obj.draw_mode, true);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    // select outline shader
    std::shared_ptr<Shader> shader_ptr = scene_->get_shader(std::string(defaults::OUTLINE_SHADER_NAME.data()));
    if (shader_ptr == nullptr) {
        spdlog::warn("Scene hasn't shader {} to draw outline, skipping rendering model {}", defaults::OUTLINE_SHADER_NAME.data(), obj.obj_ptr->get_name());
        return;
    }
    curr_shader_ptr_ = shader_ptr.get();
    shader_ptr->use();

    set_general_shader_uniforms(shader_ptr);
    set_lighting_shader_uniforms(shader_ptr);
    shader_ptr->set_uniform("u_outline_width", defaults::OUTLINE_WIDTH);
    shader_ptr->set_uniform("u_outline_color", defaults::OUTLINE_COLOR);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // discards all fragments whose stencil value is equeal to 1 (we have already drawn a normal model there)
    glStencilMask(0x00); // prevent writing to stencil buffer

#ifdef MERGED_OUTLINES
    glDepthMask(GL_FALSE);
#endif
    // render object outline
    obj.obj_ptr->render(obj.draw_mode, false, shader_ptr);
    obj.obj_ptr->render(obj.draw_mode, true, shader_ptr);

    // return stencil settings to default
    glStencilMask(0xFF); // allow writing to stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // doesn't discard any fragment

#ifdef MERGED_OUTLINES
    glDepthMask(GL_TRUE);
#endif
}

void renderer::Renderer::render_shadow_depth_buffer(const std::shared_ptr<Shader> &shader_ptr)
{
        glCullFace(GL_FRONT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // render opaque objects (without anything)
        for (const auto& obj: opaque_objects_render_queue_) {
            obj.obj_ptr->render(obj.draw_mode, false, shader_ptr); 
        }

        // render transparent objects (in two steps)
        for (const auto& obj: transparent_objects_render_queue_) {
            obj.obj_ptr->render(obj.draw_mode, false, shader_ptr);       
        }

        for (const auto& obj: transparent_objects_render_queue_) {
            glDepthMask(GL_FALSE);
            obj.obj_ptr->render(obj.draw_mode, true, shader_ptr); 
            glDepthMask(GL_TRUE);
        }

        glCullFace(GL_BACK);
}

void Renderer::render_scene(GLsizei framebuffer_width, GLsizei framebuffer_height)
{
    profiler_.begin_cpu_profiling_query();
    profiler_.begin_gpu_profiling_query();

    framebuffer_width_ = framebuffer_width;
    framebuffer_height_ = framebuffer_height;

    // render preparations
    scene_lights_buffer_ = scene_->get_lights();

    shadow_frame_buffers_.at(scene::lights::LightType::DIRECTIONAL_LIGHT)->set_layers_count(
        std::count_if(scene_lights_buffer_.cbegin(), scene_lights_buffer_.cend(), 
        [](const std::shared_ptr<scene::lights::LightBase>& l){
            return l->get_type() == scene::lights::LightType::DIRECTIONAL_LIGHT;
        }
    ));
    
    shadow_frame_buffers_.at(scene::lights::LightType::SPOT_LIGHT)->set_layers_count(
        std::count_if(scene_lights_buffer_.cbegin(), scene_lights_buffer_.cend(), 
        [](const std::shared_ptr<scene::lights::LightBase>& l){
            return l->get_type() == scene::lights::LightType::SPOT_LIGHT;
        }
    ));
    
    shadow_frame_buffers_.at(scene::lights::LightType::POINT_LIGHT)->set_layers_count(
        std::count_if(scene_lights_buffer_.cbegin(), scene_lights_buffer_.cend(), 
        [](const std::shared_ptr<scene::lights::LightBase>& l){
            return l->get_type() == scene::lights::LightType::POINT_LIGHT;
        }
    ));

    build_rendering_queues(); // todo: remake


    // render
    do_shadow_map_render();
    do_geometry_path_render();

    if (post_processing_type_ != PostProcessingTypes::NO_POST_PROCESSING) {
        post_processor_.enable_off_sceen_rendering(framebuffer_width_, framebuffer_height_);
    }

    do_color_path_render();
    do_forward_path_render(); // transparent objects and light physical objects

    if (post_processing_type_ != PostProcessingTypes::NO_POST_PROCESSING) {
        post_processor_.do_post_process(post_processing_type_);
        curr_shader_ptr_->use(); // restore renderer's shader
    }

    profiler_.end_gpu_profiling_query();
    profiler_.end_cpu_profiling_query();
}

double Renderer::get_gpu_time() const
{
    return profiler_.gpu_elapsed_time();
}

double Renderer::get_cpu_time() const
{
    return profiler_.cpu_elapsed_time();
}

void renderer::Renderer::set_clear_color(const glm::vec4 &clear_color_rgba)
{
    clear_color_rgba_ = clear_color_rgba;
}

void renderer::Renderer::set_view_mat(const glm::mat4 &view_mat)
{
    view_mat_ = view_mat;
}

void renderer::Renderer::set_projection_mat(const glm::mat4 &projection_mat)
{
    projection_mat_ = projection_mat;
}

void renderer::Renderer::set_camera_position(const glm::vec3 &camera_position)
{
    camera_position_ = camera_position;
}

void renderer::Renderer::set_internal_objects_id_mask(uint8_t internal_objects_id_mask)
{
    internal_objects_id_mask_ = internal_objects_id_mask;
}

void renderer::Renderer::set_post_processing(PostProcessingTypes type)
{
    post_processing_type_ = type;
}

void renderer::Renderer::set_geometry_post_processing(GeometryProcessingTypes type)
{
    geometry_processing_type_ = type;
}

void renderer::Renderer::set_gamma(float value)
{
    post_processor_.set_gamma(value);
}
