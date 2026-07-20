#pragma once
#include "scene/scene_objects/scene_object.h"
#include "assimp_wrapper/model.h"

namespace scene
{
    class LoadableSceneObject : public SceneObject {
        assimp_wrapper::Model model_;
        uint32_t shader_program_id_;

    public:
        LoadableSceneObject(const assimp_wrapper::Model& loaded_model, uint32_t shader_program_id);
        void render(Shader& shader_program) const;
        uint32_t shader_program_id() const;
    };

} // namespace scene
