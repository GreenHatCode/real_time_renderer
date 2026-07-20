#pragma once
#include "scene/scene_objects/scene_object.h"
#include "assimp_wrapper/model.h"

namespace scene
{
    class ModelObject : public SceneObject {
        assimp_wrapper::Model model_;

    public:
        ModelObject(const assimp_wrapper::Model& loaded_model);
        void render(Shader &shader_program, const glm::mat4& view_mat, const glm::mat4& projection_mat) override;
    };

} // namespace scene
