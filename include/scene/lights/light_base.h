#pragma once
#include "glm/glm.hpp"
#include "scene/i_scene_object.h"
#include "shader/shader.h"

namespace scene::lights {

    namespace lighting_constants {
        inline constexpr float MIN_COLOR = 0.0f;
        inline constexpr float MAX_COLOR = 1.0f;
        inline constexpr float MIN_LIGHTING_COMPONENT_VALUE = 0.0f;
        inline constexpr float MAX_LIGHTING_COMPONENT_VALUE = 1.0f;
        inline constexpr float LIGHT_CONSTANT = 1.0f;
        inline constexpr float MIN_LIGHT_LINEAR = 0.0014f;
        inline constexpr float MAX_LIGHT_LINEAR = 1.0f;
        inline constexpr float MIN_LIGHT_QUADRATIC = 0.000007f;
        inline constexpr float MAX_LIGHT_QUADRATIC = 1.0f;
        inline constexpr float MIN_SPOT_CUTOFF = 1.0f;
        inline constexpr float MAX_SPOT_CUTOFF = 89.0f;

        // shadow mapping
        inline constexpr float SPOT_LIGHT_DEFAULT_NEAR_PLANE = 0.1f;
        inline constexpr float CAMERA_DEFAULT_NEAR_PLANE = -100.0f;
        inline constexpr float LIGHTS_ORTHO_SIDE_SIZE = 120.0f;
        inline constexpr float DIRECTIONAL_LIGHT_DEFAULT_FAR_PLANE = 300.0f;
    }

    enum class LightType
    {
        DIRECTIONAL_LIGHT,
        POINT_LIGHT,
        SPOT_LIGHT,
        COUNT
    };

    inline const char* const* get_light_type_names() 
    {
        static const char* names[] = {
            "Directional Light",
            "Point Light",
            "Spot Light"
        };
        return names;
    }

    class LightBase: public ISceneObject {
        glm::vec3 ambient_;
        glm::vec3 diffuse_;
        glm::vec3 specular_;
        glm::vec3 color_rgb_;
    
    public:
        LightBase(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 rgb_color);
        
        // general light usage
        void set_color(glm::vec3 rgb_color);
        glm::vec3 get_color() const;
        void set_ambient(glm::vec3 value);
        glm::vec3 get_ambient() const;
        void set_diffuse(glm::vec3 value);
        glm::vec3 get_diffuse() const;
        void set_specular(glm::vec3 value);
        glm::vec3 get_specular() const;

        // shadow mapping
        virtual size_t get_view_count() const = 0;
        virtual glm::mat4 view_matrix(size_t face_idx = 0) const = 0;
        virtual glm::mat4 projection_matrix(float framebuffer_width, float framebuffer_height) const = 0;

        virtual void upload_components(Shader& obj_shader, size_t light_idx) const = 0; // push light's data to shader
        virtual LightType get_type() const = 0;

        virtual ~LightBase() = default;
    };

} // namespace scene::lights