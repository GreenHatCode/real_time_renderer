#include "scene/physical_objects/model_object.h"

scene::physical_objects::ModelObject::ModelObject(assimp_wrapper::Model&& loaded_model, const std::string& object_name, const std::shared_ptr<Shader>& shader)
    : PhysicalObject()
    , model_(std::move(loaded_model))
    , name_(object_name)
    , shader_(shader)
{
}

uint32_t scene::physical_objects::ModelObject::get_vertices_count() const
{
    return model_.get_vertices_count();
}

uint32_t scene::physical_objects::ModelObject::get_faces_count() const
{
    return model_.get_faces_count();
}

void scene::physical_objects::ModelObject::set_shader(const std::shared_ptr<Shader> &shader)
{
    shader_ = shader;
}

std::shared_ptr<Shader> scene::physical_objects::ModelObject::get_shader() const
{
    return shader_;
}

std::string scene::physical_objects::ModelObject::get_name() const
{
    return name_;
}

void scene::physical_objects::ModelObject::render(bool render_transparent_meshes, const std::shared_ptr<Shader> &specific_shader) const
{
    glm::mat4 model = model_matrix();
    glm::mat3 normal_mat = glm::mat3(normal_matrix());    

    if (specific_shader == nullptr) { // render using model's shader
        shader_->set_uniform("u_model_mat", model);
        shader_->set_uniform("u_normal_mat", normal_mat);
        model_.draw(render_transparent_meshes, *shader_);
    } else {
        specific_shader->set_uniform("u_model_mat", model);
        specific_shader->set_uniform("u_normal_mat", normal_mat);
        model_.draw(render_transparent_meshes, *specific_shader);
    } 
}

bool scene::physical_objects::ModelObject::is_transparent() const
{
    return model_.is_transparent();
}
