#pragma once
#include <cstdint>
#include "scene/scene_objects/model_object.h"
#include <memory>
#include <map>
#include <filesystem>
#include "shader/shader.h"
#include <utility>
#include <vector>

namespace scene
{

    class Scene {
    private:
        uint64_t vertices_count_;
        uint64_t faces_count_;
        std::vector<std::shared_ptr<ModelObject>> models_;
        std::vector<std::shared_ptr<Shader>> objects_shader_;

//  all objects on the scene must have properties of SceneObject, but
//  during render stage, every object: ModelObject, LightSource ... will do different things in different stages
//  so, here we store them separately and use through SceneObject only when need to edit it's properties

    public:
        std::weak_ptr<ModelObject> add_model(const std::filesystem::path& model_file_path);
        std::shared_ptr<Shader> add_shader(const std::filesystem::path& vert_shader_file_path, const std::filesystem::path& frag_shader_file_path);
        std::vector<std::weak_ptr<ModelObject>> get_models() const;     

    };



} // namespace scene
