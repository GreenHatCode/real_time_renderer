#pragma once
#include "glm/glm.hpp"

namespace scene
{   
    class SceneObject {
        glm::vec3 move_vector_;
        float rotation_angle_cw_;
        glm::vec3 rotation_axes_;
        float scaling_prescaler_;
        glm::vec3 scaling_axes_;

    public:
        void move(glm::vec3 move_vector);
        void rotate(float angle_rad, glm::vec3 axes_vec); // CW direction
        void scale(float prescaler, glm::vec3 axes_vec);

        glm::vec3 get_move_vector() const;
        float get_rotation_angle_cw() const;
        glm::vec3 get_rotation_axes() const;
        float get_scaling_prescaler() const;
        glm::vec3 get_scaling_axes() const;

        glm::mat4 model_matrix() const;

        virtual ~SceneObject();
    };
} // namespace scene

