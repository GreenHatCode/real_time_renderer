#pragma once
#include "scene/physical_objects/physical_object.h"


namespace scene::physical_objects
{
    namespace physical_light_object_constants
    {
        inline constexpr std::string_view LIGHT_SPHERE_MODEL_FILE = "resources/point_light_sphere/tinker.obj";
        inline constexpr std::string_view LIGHT_CONE_MODEL_FILE = "resources/light_cone.obj";
        inline constexpr std::string_view LIGHT_MODEL_OBJECT_SHADER_NAME = "physical_light_object_shader";
        inline constexpr std::string_view LIGHT_MODEL_OBJECT_VERTEX_SHADER = "shaders_sources/light_object/light.vert";
        inline constexpr std::string_view LIGHT_MODEL_OBJECT_FRAGMENT_SHADER = "shaders_sources/light_object/light.frag";
        inline constexpr glm::vec3 SPOT_LIGHT_MODEL_DEFAULT_FORWARD = glm::vec3(0.0f, -1.0f, 0.0f);
        inline constexpr std::string_view LIGHT_MODEL_OBJECT_NAME_SUFFIX = "_light_model_obj";
    }

    class PhysicalLightObject: public PhysicalObject {
        virtual void set_color(glm::vec3 color) = 0;
    };

} // namespace scene::physical_objects
