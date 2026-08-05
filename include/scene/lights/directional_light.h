#pragma once
#include "scene/lights/light_base.h"
#include <string>

namespace scene::lights
{
    class DirectionalLight final: public LightBase {
        glm::vec3 direction_;
        std::string name_;
        LightType type_;

    public:
        DirectionalLight(const std::string& name, glm::vec3 ambient, glm::vec3 diffuse, 
            glm::vec3 specular, glm::vec3 rgb_color, glm::vec3 direction);
        void set_direction(glm::vec3 direction);
        glm::vec3 get_direction() const;
        std::string get_name() const override;
        LightType get_type() const override;
        void upload_components(Shader &obj_shader, size_t light_idx) const override;
    };

} // namespace scene::lights
