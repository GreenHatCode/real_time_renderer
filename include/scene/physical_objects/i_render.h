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
        virtual void render(GLenum draw_mode, bool render_transparent_meshes, const std::shared_ptr<Shader>& specific_shader = nullptr) const = 0;
        virtual uint32_t get_vertices_count() const = 0;
        virtual uint32_t get_faces_count() const = 0;
        virtual ~IRender() = default;

        virtual bool is_selected() const = 0;
        virtual void set_selected(bool selected) = 0;

    };
    
} // namespace scene::physical_objects
