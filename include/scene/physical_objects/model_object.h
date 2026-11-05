#pragma once
#include "scene/physical_objects/physical_object.h"
#include "scene/i_scene_object.h"
#include "assimp_wrapper/model.h"

namespace scene::physical_objects
{
    class ModelObject final: public PhysicalObject {
        assimp_wrapper::model_t model_;
        std::shared_ptr<Shader> g_path_shader_;
        std::shared_ptr<Shader> shader_;
        std::string name_;
        
    public:
        ModelObject(
            assimp_wrapper::model_t&& loaded_model, 
            const std::string& object_name,
            const std::shared_ptr<Shader>& g_path_shader, 
            const std::shared_ptr<Shader>& shader = nullptr);
        uint32_t get_vertices_count() const override;
        uint32_t get_faces_count() const override;
        void set_shader(const std::shared_ptr<Shader>& shader) override;
        std::shared_ptr<Shader> get_shader() const override;
        std::shared_ptr<Shader> get_geometry_path_shader() const override;
        std::string get_name() const override;
        void render(GLenum draw_mode, bool render_transparent_meshes, const std::shared_ptr<Shader>& specific_shader = nullptr) const override;
        void render_geometry_path(GLenum draw_mode) const override;
        assimp_wrapper::ModelType model_type() const override;
    };

} // namespace scene::physical_objects
