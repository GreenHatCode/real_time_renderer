#pragma once
#include "scene/lights/light_base.h"
#include <string>

namespace scene::lights
{
    class DirectionalLight final: public LightBase {
        glm::vec3 direction_;
        std::string name_;
        LightType type_;
        glm::vec3 color_rgb_;
        glm::mat4 ortho_mat_;

        inline static constexpr glm::vec3 LOOK_POINT = glm::vec3(0.0f);
        inline static constexpr glm::vec3 DISTANCE_FROM_DIRECTION_PRESCALER = glm::vec3(50.0f);
        inline static constexpr size_t VIEW_COUNT = 1;
    public:
        DirectionalLight(
            const std::string& name, 
            glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f), 
            glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f), 
            glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f), 
            glm::vec3 rgb_color = glm::vec3(1.0f, 1.0f, 1.0f), 
            glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f)
        );

        void set_color(glm::vec3 rgb_color) override;
        glm::vec3 get_color() const override;
        void set_direction(glm::vec3 direction);
        glm::vec3 get_direction() const;


        // shadow mapping
        size_t get_view_count() const override;
        glm::mat4 view_matrix(size_t face_idx = 0) const override;
        glm::mat4 projection_matrix(float framebuffer_width, float framebuffer_height) const;

        std::string get_name() const override;
        LightType get_type() const override;
        void upload_components(Shader &obj_shader, size_t light_idx) const override;
    };

} // namespace scene::lights
