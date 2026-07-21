#include "renderer/renderer.h"

void Renderer::set_general_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr)
{
    shader_ptr->set_uniform("view_mat", view_mat_);
    shader_ptr->set_uniform("projection_mat", projection_mat_);
}

void Renderer::set_model_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr)
{
    // add setting lights uniforms (preferably using structs or glbuffers)
}

void Renderer::render_scene_object(const std::shared_ptr<scene::SceneObject> &obj)
{
    const std::shared_ptr<Shader> shader_ptr = obj.get_shader();
    if (shader_ptr!= curr_shader_ptr_) 
    {
        curr_shader_ptr_ = shader_ptr;
        curr_shader_ptr_->use();
    }

    set_general_shader_uniforms(curr_shader_ptr_);
    set_model_shader_uniforms(curr_shader_ptr_);
    obj->render(*curr_shader_ptr_);
}

Renderer::Renderer(const std::shared_ptr<Scene> &scene, uint32_t screen_width, uint32_t screen_height)
    : scene_(scene), scr_width_(screen_width), scr_height_(screen_height)
{
    // OpenGL configuration
    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


}

void Renderer::render_scene(const std::shared_ptr<ICamera> &camera)
{
    view_mat_ = camera->view_matrix();
    projection_mat_ = camera->projection_matrix(scr_width_, scr_height_);

    for (const auto& obj : scene_.get_models()) {
        render_scene_object(obj);
    }
}
