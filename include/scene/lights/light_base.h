#pragma once
#include "glm/glm.hpp"

namespace scene::lights {

    namespace lighting_constants {
        inline constexpr float MIN_COLOR = 0.0f;
        inline constexpr float MAX_COLOR = 1.0f;
        inline constexpr float MIN_LIGHTING_COMPONENT_VALUE = 0.0f;
        inline constexpr float MAX_LIGHTING_COMPONENT_VALUE = 1.0f;
    }

    class LightBase {
        glm::vec3 ambient_;
        glm::vec3 diffuse_;
        glm::vec3 specular_;
        glm::vec4 color_rgba_;
    
    public:
        LightBase(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec4 rgba_color);
        void set_color(glm::vec4 rgba_color);
        glm::vec4 get_color() const;
        void set_ambient(glm::vec3 value);
        glm::vec3 get_ambient() const;
        void set_diffuse(glm::vec3 value);
        glm::vec3 get_diffuse() const;
        void set_specular(glm::vec3 value);
        glm::vec3 get_specular() const;
        virtual ~LightBase() = 0;
    };

} // namespace scene::lights