#pragma once
#include "scene/physical_objects/physical_object.h"


namespace scene::physical_objects
{
    namespace physical_light_object_constants
    {
        constexpr std::string_view LightSphereModel = "resources/light_sphere.obj";
        constexpr std::string_view LightConeModel = "resources/light_cone.obj";
        constexpr std::string_view ShaderName = "physical_light_object_shader";
        constexpr std::string_view LightVertexShader = "shaders_sources/light_object/light.vert";
        constexpr std::string_view LightFragmentShader = "shaders_sources/light_object/light.frag";
    }

    class PhysicalLightObject: public PhysicalObject {
        virtual void set_color(glm::vec3 color) = 0;
    };

} // namespace scene::physical_objects
