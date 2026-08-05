#pragma once
#include "glm/glm.hpp"
#include "scene/lights/i_light.h"

namespace scene::lights {

    namespace lighting_constants {
        inline constexpr float MIN_COLOR = 0.0f;
        inline constexpr float MAX_COLOR = 1.0f;
        inline constexpr float MIN_LIGHTING_COMPONENT_VALUE = 0.0f;
        inline constexpr float MAX_LIGHTING_COMPONENT_VALUE = 1.0f;
        inline constexpr float POINT_LIGHT_MIN_CONSTANT = 1.0f;
        inline constexpr float POINT_LIGHT_MIN_LINEAR = 0.0f;
        inline constexpr float POINT_LIGHT_MIN_QUADRATIC = 0.0f;
    }

    class LightBase: public ILight {
        glm::vec3 ambient_;
        glm::vec3 diffuse_;
        glm::vec3 specular_;
        glm::vec3 color_rgb_;
    
    public:
        LightBase(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 rgb_color);
        void set_color(glm::vec3 rgb_color);
        glm::vec3 get_color() const;
        void set_ambient(glm::vec3 value);
        glm::vec3 get_ambient() const;
        void set_diffuse(glm::vec3 value);
        glm::vec3 get_diffuse() const;
        void set_specular(glm::vec3 value);
        glm::vec3 get_specular() const;
        virtual ~LightBase() = default;
    };

} // namespace scene::lights