#pragma once 
#include "scene/lights/light_base.h"
#include <string>
#include "scene/physical_objects/light_model_object.h"

namespace scene::lights
{
    class PointLight final: public LightBase {
        glm::vec3 position_;
        std::string name_;
        LightType type_;
        std::weak_ptr<scene::physical_objects::LightModelObject> light_sphere_;

        // attenuation parameters
        float constant_;
        float linear_;
        float quadratic_;

    public:
        PointLight(
            const std::string& name, 
            const std::weak_ptr<scene::physical_objects::LightModelObject>& sphere_model = {}, 
            glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f), 
            glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f), 
            glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f), 
            glm::vec3 rgb_color = glm::vec3(1.0f, 1.0f, 1.0f), 
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
            float constant = 1.0f, 
            float linear = 0.09f, 
            float quadratic = 0.032f
        );
        std::string get_name() const override;
        LightType get_type() const override;
        void upload_components(Shader &obj_shader, size_t light_idx) const override;

        void set_position(glm::vec3 position);
        glm::vec3 get_position() const;  
        void set_constant(float value); 
        float get_constant() const;
        void set_linear(float value);
        float get_linear() const;
        void set_quadratic(float value);
        float get_quadratic() const;

        void set_model_ptr(const std::weak_ptr<scene::physical_objects::LightModelObject>& value);
        std::weak_ptr<scene::physical_objects::LightModelObject> get_model_ptr() const;
    };

} // namespace scene::lights
