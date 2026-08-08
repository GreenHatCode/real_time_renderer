#pragma once
#include "glm/glm.hpp"
#include "shader/shader.h"
#include "scene/physical_objects/i_render.h"
#include <memory>
#include "glm/gtc/quaternion.hpp"

namespace scene::physical_objects
{   
    class PhysicalObject: public IRender {
        glm::vec3 move_vector_;
        glm::quat rotation_quat_;
        glm::vec3 scaling_vec_;

    public:
        PhysicalObject();
        void move(glm::vec3 move_vector);
        void set_rotation(const glm::quat& rotation_quat); // CW direction
        void scale(glm::vec3 prescaling_vec);

        glm::vec3 get_move_vector() const;
        glm::quat get_rotation() const;
        glm::vec3 get_scaling_vector() const;

        glm::mat4 model_matrix() const;

        virtual ~PhysicalObject() = default;
    };
} // namespace scene::physical_objects

