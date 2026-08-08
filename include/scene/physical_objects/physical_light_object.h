#pragma once
#include "scene/physical_objects/physical_object.h"


namespace scene::physical_objects
{
    namespace physical_light_object_constants
    {
        inline constexpr std::string_view LightSphereModel = "resources/light_sphere.obj";
        inline constexpr std::string_view LightConeModel = "resources/light_cone.obj";
        inline constexpr std::string_view ShaderName = "physical_light_object_shader";
        inline constexpr std::string_view LightVertexShader = "shaders_sources/light_object/light.vert";
        inline constexpr std::string_view LightFragmentShader = "shaders_sources/light_object/light.frag";
        inline constexpr glm::vec3 SPOT_LIGHT_MODEL_DEFAULT_FORWARD = glm::vec3(0.0f, -1.0f, 0.0f);
    }

    class PhysicalLightObject: public PhysicalObject {
        virtual void set_color(glm::vec3 color) = 0;
    };

} // namespace scene::physical_objects
