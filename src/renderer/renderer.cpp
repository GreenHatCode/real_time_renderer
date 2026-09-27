#include "renderer/renderer.h"
#include "glm/gtx/norm.hpp"
#include "scene/lights/spot_light.h"

using namespace renderer;

Renderer::Renderer(std::shared_ptr<const scene::Scene> scene)
    : scene_(scene)
    , clear_color_rgba_(0.0)
    , view_mat_(1.0)
    , projection_mat_(1.0)
    , camera_position_(1.0) 
    , post_processor_(1, 1)
    , post_processing_type_(PostProcessingTypes::NO_POST_PROCESSING)   
    , geometry_processor_()
    , geometry_processing_type_(GeometryProcessingTypes::NO_GEOMETRY_PROCESSING)
    , internal_objects_id_mask_(0)
    , shadow_framebuffer_(1, 1)
{
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


    glActiveTexture(GL_TEXTURE0);
    shader_ptr->set_uniform("u_light_space_mat", light_space_mat_);
    shader_ptr->set_uniform("u_shadow_map", 0);
    glBindTexture(GL_TEXTURE_2D, shadow_framebuffer_.get_depth_map());
}

void Renderer::set_lighting_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr)
{
    uint32_t directional_light_idx = 0;
    uint32_t point_light_idx = 0;
    uint32_t spot_light_idx = 0;

    for (const auto& light: scene_lights_buffer_) {
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
    opaque_objects_.clear();
    transparent_objects_.clear();

    for (const auto &obj : scene_->get_internal_objects()) {
        if (internal_objects_id_mask_ & obj.id) {
            obj.ptr->move(glm::vec3(camera_position_.x, obj.ptr->get_move_vector().y, camera_position_.z));
            
            opaque_objects_.emplace_back(RenderQueueObject{.obj_ptr=obj.ptr, .distance_to_viewer=0.0f, .draw_mode=obj.draw_mode});
        }
    }

    for (const auto &obj : scene_->get_objects())
    {
        if (obj->is_transparent()) {
            float distance = glm::length2(camera_position_ - obj->get_move_vector());
            transparent_objects_.emplace_back(RenderQueueObject{.obj_ptr=obj, .distance_to_viewer=distance, .draw_mode=assimp_wrapper::DEFAULT_DRAW_MODE});
        }
        else {
            opaque_objects_.emplace_back(RenderQueueObject{.obj_ptr=obj, .distance_to_viewer=0.0f, .draw_mode=assimp_wrapper::DEFAULT_DRAW_MODE});
        }
    }

    std::sort(
        transparent_objects_.begin(), 
        transparent_objects_.end(), 
        [](const RenderQueueObject& lhs, const RenderQueueObject& rhs) 
        { 
            return lhs.distance_to_viewer > rhs.distance_to_viewer; 
        }
    );
}

void renderer::Renderer::do_shadow_map_render(float framebuffer_width, float framebuffer_height)
{
    std::shared_ptr<scene::lights::LightBase> light = scene_lights_buffer_[0];
    // loop and push rendered framebuffer to vector

    light_space_mat_ = light->projection_matrix(framebuffer_width, framebuffer_height) * light->view_matrix();

    // bind shadow framebuffer (for a particular light) 
    shadow_framebuffer_.rescale(framebuffer_width, framebuffer_height);
    shadow_framebuffer_.bind();
    
    std::shared_ptr<Shader> shader = scene_->get_internal_shader(scene::InternalShadersIDS::SHADOW_MAP_EMPTY_FRAG_SHADER);
    curr_shader_ptr_ = shader.get();
    curr_shader_ptr_->use();
    curr_shader_ptr_->set_uniform("u_light_space_mat", light_space_mat_);

    glClearColor(clear_color_rgba_.r, clear_color_rgba_.g, clear_color_rgba_.b, clear_color_rgba_.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // render opaque objects (without anything)
    for (const auto& obj: opaque_objects_) {
        obj.obj_ptr->render(obj.draw_mode, false, shader); 
    }

    // render transparent objects (in two steps)
    for (const auto& obj: transparent_objects_) {
        obj.obj_ptr->render(obj.draw_mode, false);       
    }

    for (const auto& obj: transparent_objects_) {
        glDepthMask(GL_FALSE);
        obj.obj_ptr->render(obj.draw_mode, true); 
        glDepthMask(GL_TRUE);
    }


    shadow_framebuffer_.unbind();
}

void renderer::Renderer::do_render()
{
    glClearColor(clear_color_rgba_.r, clear_color_rgba_.g, clear_color_rgba_.b, clear_color_rgba_.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    render_opaque_scene_objects();
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
        for (const auto& obj: opaque_objects_) {
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
        for (const auto& obj: opaque_objects_) {
            use_shader(obj.obj_ptr);
            obj.obj_ptr->render(obj.draw_mode, false, processing_shader); 
            use_shader(processing_shader);
            obj.obj_ptr->render(obj.draw_mode, false, processing_shader); 
        }
    } else {
        auto processing_shader = geometry_processor_.get_geometry_shader(geometry_processing_type_);
        use_shader(processing_shader);
        for (const auto& obj: opaque_objects_) {
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
        for (const auto& obj: transparent_objects_) {
            use_shader(obj.obj_ptr);
            if (obj.obj_ptr->is_selected()) {
                render_scene_object_with_outline(obj); 
            }
            else {
                obj.obj_ptr->render(obj.draw_mode, false); 
            }      
        }

        for (const auto& obj: transparent_objects_) {
            use_shader(obj.obj_ptr);
            glDepthMask(GL_FALSE);
            obj.obj_ptr->render(obj.draw_mode, true); 
            glDepthMask(GL_TRUE);
        }
    } else if (geometry_processing_type_ == GeometryProcessingTypes::NORMALS_VISUALIZATIONS) {
        auto processing_shader = geometry_processor_.get_geometry_shader(geometry_processing_type_);
        for (const auto& obj: transparent_objects_) {
            use_shader(obj.obj_ptr);
            obj.obj_ptr->render(obj.draw_mode, false); 
            use_shader(processing_shader);
            obj.obj_ptr->render(obj.draw_mode, false, processing_shader); 
        }

        for (const auto& obj: transparent_objects_) {
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
        for (const auto& obj: transparent_objects_) {
            obj.obj_ptr->render(obj.draw_mode, false, processing_shader);     
        }

        for (const auto& obj: transparent_objects_) {
            glDepthMask(GL_FALSE);
            obj.obj_ptr->render(obj.draw_mode, true, processing_shader); 
            glDepthMask(GL_TRUE);
        }
    }
}

void Renderer::render_scene_object_with_outline(const RenderQueueObject& obj)
{
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

void Renderer::render_scene(GLsizei framebuffer_width, GLsizei framebuffer_height)
{
    profiler_.begin_cpu_profiling_query();
    profiler_.begin_gpu_profiling_query();

    scene_lights_buffer_ = scene_->get_lights();
    build_rendering_queues();

    do_shadow_map_render(framebuffer_width, framebuffer_height);

#if 0
    post_processor_.do_post_process_debug(PostProcessingTypes::GAMMA_CORRECTION, shadow_framebuffer_.get_depth_map());
    curr_shader_ptr_->use(); // restore renderer's shader
    
#else

    if (post_processing_type_ != PostProcessingTypes::NO_POST_PROCESSING) {
        post_processor_.enable_off_sceen_rendering(framebuffer_width, framebuffer_height);
    }

    do_render();

    if (post_processing_type_ != PostProcessingTypes::NO_POST_PROCESSING) {
        post_processor_.do_post_process(post_processing_type_);
        curr_shader_ptr_->use(); // restore renderer's shader
    }
#endif


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
