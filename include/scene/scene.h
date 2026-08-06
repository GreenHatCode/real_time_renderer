#pragma once
#include <cstdint>
#include "scene/physical_objects/physical_object.h"
#include "scene/physical_objects/physical_light_object.h"
#include "scene/physical_objects/model_object.h"
#include "scene/lights/light_base.h"
#include <memory>
#include <map>
#include <filesystem>
#include "shader/shader.h"
#include <utility>
#include <vector>

namespace scene
{
    // TODO: consider using buffers to avoid allocation for every call of get_objects, get_lights, get_shaders

    class Scene {
        std::unordered_map<std::string, std::shared_ptr<scene::physical_objects::PhysicalObject>> physical_objects_map_;
        std::unordered_map<std::string, std::shared_ptr<scene::physical_objects::PhysicalLightObject>> physical_light_objects_map_;
        std::unordered_map<std::string, std::shared_ptr<scene::lights::LightBase>> lights_map_;
        std::unordered_map<std::string, std::shared_ptr<Shader>> shaders_map_;
        uint32_t total_vertices_count;
        uint32_t total_faces_count;

    public:
        void add_object(const std::string& name, const std::shared_ptr<scene::physical_objects::PhysicalObject>& ptr);
        void add_object(const std::string& name, const std::shared_ptr<scene::physical_objects::PhysicalLightObject>& ptr);
        void add_light(const std::string& name, const std::shared_ptr<scene::lights::LightBase>& ptr);
        void add_shader(const std::string& name, const std::shared_ptr<Shader>& ptr);
        bool delete_scene_object(const std::string& name);
        bool delete_shader(const std::string &name);

        std::vector<std::shared_ptr<scene::physical_objects::PhysicalObject>> get_objects(bool lights_models = false) const;
        std::vector<std::shared_ptr<scene::lights::LightBase>> get_lights() const;
        std::vector<std::shared_ptr<Shader>> get_shaders() const;
        std::shared_ptr<scene::physical_objects::PhysicalObject> get_object(const std::string &name) const;
        std::shared_ptr<scene::lights::LightBase> get_light(const std::string &name) const;
        std::shared_ptr<Shader> get_shader(const std::string& name) const;

        uint32_t get_total_vertices_count() const;
        uint32_t get_total_faces_count() const;
    };
} // namespace scene
