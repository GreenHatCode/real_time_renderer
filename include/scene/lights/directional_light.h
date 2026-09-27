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
        DirectionalLight(
            const std::string& name, 
            glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f), 
            glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f), 
            glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f), 
            glm::vec3 rgb_color = glm::vec3(1.0f, 1.0f, 1.0f), 
            glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f)
        );
        void set_direction(glm::vec3 direction);
        glm::vec3 get_direction() const;


        // shadow mapping
        virtual glm::mat4 view_matrix() const override { return glm::mat4(1.0f); }
        virtual glm::mat4 projection_matrix(float framebuffer_width, float framebuffer_height) const override { return glm::mat4(1.0f); }




        std::string get_name() const override;
        LightType get_type() const override;
        void upload_components(Shader &obj_shader, size_t light_idx) const override;
    };

} // namespace scene::lights
