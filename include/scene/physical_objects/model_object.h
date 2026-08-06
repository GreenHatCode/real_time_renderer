#pragma once
#include "scene/physical_objects/physical_object.h"
#include "scene/i_scene_object.h"
#include "assimp_wrapper/model.h"

namespace scene::physical_objects
{
    class ModelObject final: public PhysicalObject {
        assimp_wrapper::Model model_;
        std::shared_ptr<Shader> shader_;
        std::string name_;
        
    public:
        ModelObject(assimp_wrapper::Model&& loaded_model, const std::string& object_name, std::shared_ptr<Shader> shader = nullptr);
        uint32_t get_vertices_count() const override;
        uint32_t get_faces_count() const override;
        void set_shader(const std::shared_ptr<Shader>& shader) override;
        std::shared_ptr<Shader> get_shader() const override;
        std::string get_name() const override;
        void render() const override;

    };

} // namespace scene::physical_objects
