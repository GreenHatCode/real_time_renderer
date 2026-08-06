#pragma once
#include "scene/physical_objects/physical_object.h"


namespace scene::physical_objects
{
    namespace physical_light_object_constants
    {
        constexpr std::string_view LightSphereModel = "resources/Sphere_with_Grid_v1_L1.123c4428b35e-a400-4b6c-8930-ddff4fb80cc1/15977_Sphere_with_Grid_v1.obj";
        constexpr std::string_view ShaderName = "physical_light_object_shader";
        constexpr std::string_view LightVertexShader = "shaders_sources/light_object/light.vert";
        constexpr std::string_view LightFragmentShader = "shaders_sources/light_object/light.frag";
    }

    class PhysicalLightObject: public PhysicalObject {

        virtual void set_color(glm::vec3 color) = 0;
        virtual void move_to(glm::vec3 target) = 0;

    };

} // namespace scene::physical_objects
