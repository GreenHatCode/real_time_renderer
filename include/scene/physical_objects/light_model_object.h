#pragma once
#include "scene/i_scene_object.h"
#include "assimp_wrapper/model.h"
#include "scene/physical_objects/physical_object.h"

namespace scene::physical_objects
{
    namespace physical_light_object_constants
    {
        inline constexpr std::string_view LIGHT_SPHERE_MODEL_FILE = "resources/point_light_sphere/tinker.obj";
        inline constexpr std::string_view LIGHT_CONE_MODEL_FILE = "resources/light_cone.obj";
        inline constexpr std::string_view LIGHT_MODEL_OBJECT_SHADER_NAME = "physical_light_object_shader";
        inline constexpr std::string_view LIGHT_MODEL_OBJECT_VERTEX_SHADER = "shaders_sources/light_object/light.vert";
        inline constexpr std::string_view LIGHT_MODEL_OBJECT_FRAGMENT_SHADER = "shaders_sources/light_object/light.frag";
        inline constexpr glm::vec3 SPOT_LIGHT_MODEL_DEFAULT_FORWARD = glm::vec3(0.0f, -1.0f, 0.0f);
        inline constexpr std::string_view LIGHT_MODEL_OBJECT_NAME_SUFFIX = "_light_model_obj";
    }

    class LightModelObject final: public PhysicalObject {
        assimp_wrapper::model_t model_;
        std::shared_ptr<Shader> shader_;
        std::string name_;
        glm::vec3 color_;

    public:
        LightModelObject(assimp_wrapper::model_t&& loaded_model, const std::string& object_name, glm::vec3 color, const std::shared_ptr<Shader>& shader = nullptr);
        uint32_t get_vertices_count() const override;
        uint32_t get_faces_count() const override;
        void set_shader(const std::shared_ptr<Shader>& shader) override;
        std::shared_ptr<Shader> get_shader() const override;
        std::string get_name() const override;
        void render(GLenum draw_mode, bool render_transparent_meshes, const std::shared_ptr<Shader>& specific_shader = nullptr) const override;
        assimp_wrapper::ModelType model_type() const override;

        void set_color(glm::vec3 color);
    };

} // namespace scene::physical_objects

