#pragma once 
#include "scene/lights/light_base.h"
#include <string>
#include "scene/physical_objects/light_sphere.h"

namespace scene::lights
{
    class SpotLight final: public LightBase {
        glm::vec3 position_;
        glm::vec3 direction_;
        std::string name_;
        LightType type_;
        std::weak_ptr<scene::physical_objects::LightModelObject> light_cone_;

        // attenuation parameters
        float constant_;
        float linear_;
        float quadratic_;
        float cut_off_rad_;
        float outer_cut_off_rad_;

    public:
        SpotLight(const std::string& name, const std::weak_ptr<scene::physical_objects::LightModelObject>& cone_model, 
            glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 rgb_color, glm::vec3 position, glm::vec3 direction_,
            float constant, float linear, float quadratic, float cut_off_rad, float outer_cut_off_rad);
        std::string get_name() const override;
        LightType get_type() const override;
        void upload_components(Shader &obj_shader, size_t light_idx) const override;

        void set_position(glm::vec3 position);
        glm::vec3 get_position() const;
        void set_direction(glm::vec3 direction);
        glm::vec3 get_direction() const;
        void set_constant(float value);
        float get_constant() const;
        void set_linear(float value);
        float get_linear() const;
        void set_quadratic(float value);
        float get_quadratic() const;
        void set_cut_off(float value);
        float get_cut_off() const;
        void set_outer_cut_off(float value);
        float get_outer_cut_off() const;

        std::weak_ptr<scene::physical_objects::LightModelObject> get_model_ptr() const;
    };

} // namespace scene::lights
