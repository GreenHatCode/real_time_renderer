#include "scene/scene_objects/model_object.h"

scene::ModelObject::ModelObject(assimp_wrapper::Model&& loaded_model)
    : SceneObject()
    , model_(std::move(loaded_model))
{
}

void scene::ModelObject::render()
{
    // shader_->set_uniform("model_mat", model_matrix());
    shader_->set_uniform("u_model_mat", glm::mat4(1.0f));
    model_.draw(*shader_);
}
