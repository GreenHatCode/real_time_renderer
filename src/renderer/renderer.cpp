#include "renderer/renderer.h"

void Renderer::set_general_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr)
{
    shader_ptr->set_uniform("u_view_mat", view_mat_);
    shader_ptr->set_uniform("u_projection_mat", projection_mat_);
}

void Renderer::set_lighting_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr)
{
    // add setting lights uniforms (preferably using structs or glbuffers)

#if 1
    shader_ptr->set_uniform("u_object_color", glm::vec3(1.0f, 0.5f, 0.31f));
    shader_ptr->set_uniform("u_light_pos", glm::vec3(glm::vec3(1.2f, 50.0f, 2.0f) * glm::mat3(1.0)));
    shader_ptr->set_uniform("u_light_color", glm::vec3(1.0f, 1.0f, 1.0f));

#else

    size_t directional_light_idx = 0;
    size_t point_light_idx = 0;
    size_t spot_light_idx = 0;

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
#endif
}

void Renderer::render_scene_object(const std::shared_ptr<const scene::physical_objects::IRender>& obj)
{
    std::shared_ptr<Shader> shader_ptr = obj->get_shader();
    if (shader_ptr.get() != curr_shader_ptr_) 
    {
        curr_shader_ptr_ = shader_ptr.get();
        shader_ptr->use();
    }

    set_general_shader_uniforms(shader_ptr);
    set_lighting_shader_uniforms(shader_ptr);
    obj->render();
}

Renderer::Renderer(std::shared_ptr<const scene::Scene> scene)
    : scene_(scene)
{
    // OpenGL configuration
    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


}

void Renderer::render_scene(const glm::vec4& clear_color_rgba, const glm::mat4& view_mat, const glm::mat4& projection_mat)
{
    profiler_.begin_cpu_profiling_query();

    view_mat_ = view_mat;
    projection_mat_ = projection_mat;

    profiler_.begin_gpu_profiling_query();
    glClearColor(clear_color_rgba.r, clear_color_rgba.g, clear_color_rgba.b, clear_color_rgba.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
