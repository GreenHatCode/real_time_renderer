#pragma once
#include "scene/physical_objects/physical_object.h"


namespace scene::physical_objects
{
    class PhysicalLightObject: public PhysicalObject {

        virtual void set_color(glm::vec3 color) = 0;
        virtual void move_to(glm::vec3 target) = 0;

    };

} // namespace scene::physical_objects
