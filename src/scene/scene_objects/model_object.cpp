#include "scene/scene_objects/model_object.h"

scene::ModelObject::ModelObject(assimp_wrapper::Model&& loaded_model, const std::string& object_name, std::shared_ptr<Shader> shader)
    : SceneObject(object_name, shader)
    , model_(std::move(loaded_model))
{
}

void scene::ModelObject::render() const 
{
    shader_->set_uniform("u_model_mat", model_matrix());
    model_.draw(*shader_);
}
