#pragma once
#include "scene/scene_objects/scene_object.h"
#include "assimp_wrapper/model.h"

namespace scene
{
    class ModelObject : public SceneObject {
        assimp_wrapper::Model model_;

    public:
        ModelObject(assimp_wrapper::Model&& loaded_model, std::shared_ptr<Shader> shader = nullptr);
        void render() override;
    };

} // namespace scene
