#include "renderer/renderer.h"

using namespace renderer;

void Renderer::set_general_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr)
{
    shader_ptr->set_uniform("u_view_mat", view_mat_);
    shader_ptr->set_uniform("u_projection_mat", projection_mat_);
    shader_ptr->set_uniform("u_camera_pos", camera_position_);
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

void Renderer::render_scene_object(const std::shared_ptr<const scene::physical_objects::IRender>& obj)
{
    std::shared_ptr<Shader> shader_ptr = obj->get_shader();
    if (shader_ptr == nullptr) {
        spdlog::warn("Model {} hasn't shader, skipping rendering this model", obj->get_name());
        return;
    }

    if (shader_ptr.get() != curr_shader_ptr_) 
    {
        curr_shader_ptr_ = shader_ptr.get();
        shader_ptr->use();
    }

    set_general_shader_uniforms(shader_ptr);
    set_lighting_shader_uniforms(shader_ptr);


    if (obj->is_selected()) { // render with outline
        render_scene_object_with_outline(obj);
    } else { // normal render
        obj->render();
    }


}

void Renderer::render_scene_object_with_outline(const std::shared_ptr<const scene::physical_objects::IRender> &obj)
{
    // render normal object with writing to stencil buffer
    // to mark pixels that we have painted 
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // doesn't discard any fragment
    glStencilMask(0xFF); // allow writing to stencil buffer
    obj->render();

    // select outline shader
    std::shared_ptr<Shader> shader_ptr = scene_->get_shader(std::string(defaults::OUTLINE_SHADER_NAME.data()));
    if (shader_ptr == nullptr) {
        spdlog::warn("Scene hasn't shader {} to draw outline, skipping rendering model {}", defaults::OUTLINE_SHADER_NAME.data(), obj->get_name());
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
    obj->render(shader_ptr);

    // return stencil settings to default
    glStencilMask(0xFF); // allow writing to stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // doesn't discard any fragment

#ifdef MERGED_OUTLINES
    glDepthMask(GL_TRUE);
#endif
}   

Renderer::Renderer(std::shared_ptr<const scene::Scene> scene)
    : scene_(scene)
{
    // OpenGL configuration
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void Renderer::render_scene(const glm::vec4& clear_color_rgba, const glm::mat4& view_mat, const glm::mat4& projection_mat, const glm::vec3& camera_position)
{
    profiler_.begin_cpu_profiling_query();

    view_mat_ = view_mat;
    projection_mat_ = projection_mat;
    camera_position_ = camera_position;

    profiler_.begin_gpu_profiling_query();
    glClearColor(clear_color_rgba.r, clear_color_rgba.g, clear_color_rgba.b, clear_color_rgba.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    scene_lights_buffer_ = scene_->get_lights();
    for (const auto &obj : scene_->get_objects())
    {
        render_scene_object(obj);
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
