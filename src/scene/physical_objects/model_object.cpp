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


void scene::physical_objects::ModelObject::render() const 
{
    glm::mat4 model = model_matrix();
    shader_->set_uniform("u_model_mat", model);

    model = glm::inverse(model);
    model = glm::transpose(model);
    shader_->set_uniform("u_normal_mat", glm::mat3(model));
    model_.draw(*shader_);
}


