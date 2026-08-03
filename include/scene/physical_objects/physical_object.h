#pragma once
#include "glm/glm.hpp"
#include "shader/shader.h"
#include <memory>

namespace scene::physical_objects
{   
    class PhysicalObject {
        glm::vec3 move_vector_;
        float rotation_angle_cw_; // radians
        glm::vec3 rotation_axes_;
        glm::vec3 scaling_vec_;

    public:
        PhysicalObject();
        void move(glm::vec3 move_vector);
        void rotate(float angle_rad, glm::vec3 axes_vec); // CW direction
        void scale(glm::vec3 prescaling_vec);

        glm::vec3 get_move_vector() const;
        float get_rotation_angle_cw() const;
        glm::vec3 get_rotation_axes() const;
        glm::vec3 get_scaling_vector() const;

        glm::mat4 model_matrix() const;

        virtual ~PhysicalObject() = 0;
    };
} // namespace scene

