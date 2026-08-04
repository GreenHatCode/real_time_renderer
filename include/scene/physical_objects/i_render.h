#pragma once
#include <memory>
#include "shader/shader.h"
#include "scene/i_scene_object.h"

namespace scene::physical_objects
{
    class IRender: public ISceneObject {
    public:
        virtual void set_shader(const std::shared_ptr<Shader>& shader) = 0;
        virtual std::shared_ptr<Shader> get_shader() const = 0;
        virtual void render() const = 0;
        virtual uint32_t get_vertices_count() const = 0;
        virtual uint32_t get_faces_count() const = 0;
        virtual ~IRender() = default;
    };
    
} // namespace scene::physical_objects
