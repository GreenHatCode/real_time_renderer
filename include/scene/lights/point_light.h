#pragma once 
#include "scene/lights/light_base.h"
#include <string>

namespace scene::lights
{
    class PointLight final: public LightBase {
        glm::vec3 position_;
        std::string name_;
        LightType type_;

        // attenuation parameters
        float constant_;
        float linear_;
        float quadratic_;

    public:
        PointLight(const std::string& name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, 
            glm::vec3 rgb_color, glm::vec3 position, float constant, float linear, float quadratic);
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
    };


} // namespace scene::lights
