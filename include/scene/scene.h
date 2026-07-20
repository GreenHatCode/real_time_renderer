#pragma once
#include <cstdint>
#include "scene/scene_objects/scene_object.h"
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
        std::vector<std::pair<std::unique_ptr<SceneObject>, std::shared_ptr<Shader>>> scene_objects_;
        std::map<std::filesystem::path, std::shared_ptr<Shader>> objects_shader_;

//  all objects on the scene must have properties of SceneObject, but
//  during render stage, every object: ModelObject, LightSource ... will do different things in different stages
//  so, here we store them separately and use through SceneObject only when need to edit it's properties

    public:
        Scene();
        void add_scene_object(const std::filesystem::path& model_file_path);
        void add_shader(const std::filesystem::path& vert_shader_file_path, const std::filesystem::path& frag_shader_file_path);
        void set_shader()        
        

    };



} // namespace scene
