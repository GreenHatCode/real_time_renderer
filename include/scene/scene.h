#pragma once
#include <cstdint>
#include "scene/physical_objects/physical_object.h"
#include "scene/physical_objects/model_object.h"
#include "scene/lights/directional_light.h"
#include <memory>
#include <map>
#include <filesystem>
#include "shader/shader.h"
#include <utility>
#include <vector>

namespace scene
{
    // all objects on the scene ought to have unique names

    class Scene {
        std::unordered_map<std::string, std::shared_ptr<scene::physical_objects::ModelObject>> model_objects_map_;
        std::unordered_map<std::string, std::shared_ptr<scene::lights::DirectionalLight>> directional_lights_map_;
        std::unordered_map<std::string, std::shared_ptr<Shader>> shaders_map_;
        uint32_t total_vertices_count;
        uint32_t total_faces_count;

    public:
        std::shared_ptr<scene::physical_objects::ModelObject> add_model_object(const std::string& name, const std::filesystem::path& model_file_path);
        std::shared_ptr<scene::lights::DirectionalLight> add_directional_light(/*TODO: define in future*/);
        std::shared_ptr<Shader> add_shader(const std::string& name, const std::filesystem::path& vert_shader_file_path, const std::filesystem::path& frag_shader_file_path);
        bool delete_scene_object(const std::string& name);
        bool delete_shader(const std::string& name);
        
        std::vector<std::shared_ptr<scene::physical_objects::ModelObject>> get_model_objects() const;
        std::vector<std::shared_ptr<scene::lights::DirectionalLight>> get_directional_lights() const;
        std::vector<std::shared_ptr<Shader>> get_shaders() const;
        std::shared_ptr<scene::physical_objects::ModelObject> get_model_object(const std::string &name) const;
        std::shared_ptr<scene::lights::DirectionalLight> get_directional_light(const std::string &name) const;
        std::shared_ptr<Shader> get_shader(const std::string& name) const;

        uint32_t get_total_vertices_count() const;
        uint32_t get_total_faces_count() const;

    private:
        bool valid_name(const std::string& name);
    };
} // namespace scene
