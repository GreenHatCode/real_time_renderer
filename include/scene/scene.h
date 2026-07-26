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
        std::unordered_map<std::string, std::shared_ptr<ModelObject>> models_map_;
        std::unordered_map<std::string, std::shared_ptr<Shader>> shaders_map_;

//  all objects on the scene must have properties of SceneObject, but
//  during render stage, every object: ModelObject, LightSource ... will do different things in different stages
//  so, here we store them separately and use through SceneObject only when need to edit it's properties

    public:
        std::shared_ptr<ModelObject> add_model(const std::string& name, const std::filesystem::path& model_file_path);
        std::shared_ptr<Shader> add_shader(const std::string& name, const std::filesystem::path& vert_shader_file_path, const std::filesystem::path& frag_shader_file_path);
        bool delete_model(const std::string& name);
        bool delete_shader(const std::string& name);
        
        std::vector<std::shared_ptr<ModelObject>> get_models() const;
        std::vector<std::shared_ptr<Shader>> get_shaders() const;
        std::shared_ptr<Shader> get_shader(const std::string& name) const;
        std::shared_ptr<ModelObject> get_model(const std::string &name) const;
    };



} // namespace scene
