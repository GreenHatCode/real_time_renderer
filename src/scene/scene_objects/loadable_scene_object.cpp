#include "scene/scene_objects/loadable_scene_object.h"

scene::LoadableSceneObject::LoadableSceneObject(const assimp_wrapper::Model &loaded_model, uint32_t shader_program_id)
    : SceneObject()
    , shader_program_id_(shader_program_id)
    , model_(loaded_model)
{
}

void scene::LoadableSceneObject::render(Shader &shader_program) const
{
    model_.draw(shader_program);
}

uint32_t scene::LoadableSceneObject::shader_program_id() const
{
    return shader_program_id_;
}
