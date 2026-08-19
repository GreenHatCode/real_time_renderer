#pragma once
#include "scene/physical_objects/physical_light_object.h"
#include "scene/i_scene_object.h"
#include "assimp_wrapper/model.h"

namespace scene::physical_objects
{

    class LightModelObject final: public PhysicalLightObject {
        assimp_wrapper::Model model_;
        std::shared_ptr<Shader> shader_;
        std::string name_;
        glm::vec3 color_;

    public:
        LightModelObject(const std::string& object_name, const std::filesystem::path& file_path, glm::vec3 color, const std::shared_ptr<Shader>& shader = nullptr);
        uint32_t get_vertices_count() const override;
        uint32_t get_faces_count() const override;
        void set_shader(const std::shared_ptr<Shader>& shader) override;
        std::shared_ptr<Shader> get_shader() const override;
        std::string get_name() const override;
        void render(const std::shared_ptr<Shader>& specific_shader = nullptr) const override;
        bool is_transparent() const override;

        void set_color(glm::vec3 color) override;
    };

} // namespace scene::physical_objects

