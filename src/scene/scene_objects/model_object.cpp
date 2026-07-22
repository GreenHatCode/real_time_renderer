#include "scene/scene_objects/model_object.h"

scene::ModelObject::ModelObject(assimp_wrapper::Model&& loaded_model)
    : SceneObject()
    , model_(std::move(loaded_model))
{
}

void scene::ModelObject::render()
{
    shader_->set_uniform("model_mat", model_matrix());
    model_.draw(*shader_);
}
