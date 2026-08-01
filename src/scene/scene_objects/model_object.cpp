#include "scene/scene_objects/model_object.h"

scene::ModelObject::ModelObject(assimp_wrapper::Model&& loaded_model, const std::string& object_name, std::shared_ptr<Shader> shader)
    : SceneObject(object_name, shader)
    , model_(std::move(loaded_model))
{
}

void scene::ModelObject::render() const 
{
    glm::mat4 model = model_matrix();
    shader_->set_uniform("u_model_mat", model);

    model = glm::inverse(model);
    model = glm::transpose(model);
    shader_->set_uniform("u_normal_mat", glm::mat3(model));
    model_.draw(*shader_);
}

uint32_t scene::ModelObject::get_vertices_count() const
{
    return model_.get_vertices_count();
}

uint32_t scene::ModelObject::get_faces_count() const
{
    return model_.get_faces_count();
}
