#pragma once
#include <cstdint>
#include "scene/physical_objects/physical_object.h"
#include "scene/physical_objects/light_model_object.h"
#include "scene/physical_objects/model_object.h"
#include "scene/lights/light_base.h"
#include <memory>
#include <map>
#include <filesystem>
#include "shader/shader.h"
#include <vector>

namespace scene
{
    enum class InternalShadersIDS {
        SHADOW_MAP_EMPTY_FRAG_SHADER, 
        SHADOW_MAP_POINT_LIGHT_SHADER
    };

    class Scene {

    public:
        struct internal_physical_object {
            std::shared_ptr<scene::physical_objects::PhysicalObject> ptr;
            uint8_t id;
            GLenum draw_mode;
        };
        
    private:

        std::vector<std::shared_ptr<scene::physical_objects::PhysicalObject>> physical_objects_;
        std::vector<internal_physical_object> internal_physical_objects_;
        std::vector<std::shared_ptr<scene::lights::LightBase>> lights_;
        std::vector<std::shared_ptr<Shader>> shaders_;
        std::map<InternalShadersIDS, std::shared_ptr<Shader>> internal_shaders_;
    
        uint32_t total_vertices_count_;
        uint32_t total_faces_count_;

    public:
        Scene();
        void add_object(const std::shared_ptr<scene::physical_objects::PhysicalObject>& ptr);
        void add_light(const std::shared_ptr<scene::lights::LightBase>& ptr);
        void add_shader(const std::shared_ptr<Shader>& ptr);
        bool delete_scene_object(const std::string& name);
        bool delete_shader(const std::string &name);

        std::vector<std::shared_ptr<scene::physical_objects::PhysicalObject>> get_objects() const; // returns scene physical objects that the user can edit
        std::vector<internal_physical_object> get_internal_objects() const;
        std::vector<std::shared_ptr<scene::lights::LightBase>> get_lights() const;
        std::vector<std::shared_ptr<Shader>> get_shaders() const;
        std::shared_ptr<scene::physical_objects::PhysicalObject> get_object(const std::string &name) const;
        std::shared_ptr<scene::lights::LightBase> get_light(const std::string &name) const;
        std::shared_ptr<Shader> get_shader(const std::string& name) const;
        std::shared_ptr<Shader> get_internal_shader(InternalShadersIDS id) const;

        uint32_t get_total_vertices_count() const;
        uint32_t get_total_faces_count() const;
    };
} // namespace scene
