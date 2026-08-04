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
        std::unordered_map<std::string, std::shared_ptr<scene::physical_objects::PhysicalObject>> renderable_objects_map_;
        std::unordered_map<std::string, std::shared_ptr<scene::lights::ILight>> lights_map_;
        std::unordered_map<std::string, std::shared_ptr<Shader>> shaders_map_;
        uint32_t total_vertices_count;
        uint32_t total_faces_count;

    public:
        void add_object(const std::string& name, const std::shared_ptr<scene::physical_objects::PhysicalObject>& ptr);
        void add_light(const std::string& name, const std::shared_ptr<scene::lights::ILight>& ptr);
        void add_shader(const std::string& name, const std::shared_ptr<Shader>& ptr);
        bool delete_scene_object(const std::string& name);
        bool delete_shader(const std::string &name);

        std::vector<std::shared_ptr<scene::physical_objects::PhysicalObject>> get_objects() const;
        std::vector<std::shared_ptr<scene::lights::ILight>> get_lights() const;
        std::vector<std::shared_ptr<Shader>> get_shaders() const;
        std::shared_ptr<scene::physical_objects::PhysicalObject> get_object(const std::string &name) const;
        std::shared_ptr<scene::lights::ILight> get_light(const std::string &name) const;
        std::shared_ptr<Shader> get_shader(const std::string& name) const;

        uint32_t get_total_vertices_count() const;
        uint32_t get_total_faces_count() const;
    };
} // namespace scene
