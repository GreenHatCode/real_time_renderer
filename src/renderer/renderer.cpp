#include "renderer/renderer.h"

void Renderer::set_general_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr)
{
    shader_ptr->set_uniform("u_view_mat", view_mat_);
    shader_ptr->set_uniform("u_projection_mat", projection_mat_);
}

void Renderer::set_model_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr)
{
    // add setting lights uniforms (preferably using structs or glbuffers)
}

void Renderer::render_scene_object(const std::shared_ptr<const scene::ModelObject> &obj)
{
    std::shared_ptr<Shader> shader_ptr = obj->get_shader();
    if (shader_ptr.get() != curr_shader_ptr_) 
    {
        curr_shader_ptr_ = shader_ptr.get();
        shader_ptr->use();
    }

    set_general_shader_uniforms(shader_ptr);
    set_model_shader_uniforms(shader_ptr);
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
    
    for (const auto& obj : scene_->get_models()) { 
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
