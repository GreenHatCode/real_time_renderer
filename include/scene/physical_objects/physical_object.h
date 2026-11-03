#pragma once
#include "glm/glm.hpp"
#include "shader/shader.h"
#include "scene/physical_objects/i_render.h"
#include "assimp_wrapper/model.h"

namespace scene::physical_objects
{   
    class PhysicalObject: public IRender {
        glm::vec3 move_vector_;
        glm::quat rotation_quat_;
        glm::vec3 scaling_vec_;

        bool selected_;

    public:
        PhysicalObject();
        void move(glm::vec3 move_vector);
        void set_rotation(const glm::quat& rotation_quat); // CW direction
        void scale(glm::vec3 prescaling_vec);

        glm::vec3 get_move_vector() const;
        glm::quat get_rotation() const;
        glm::vec3 get_scaling_vector() const;

        glm::mat4 model_matrix() const;
        glm::mat3 normal_matrix() const;

        bool is_selected() const;
        void set_selected(bool selected);
        
        virtual assimp_wrapper::ModelType model_type() const = 0;
        virtual ~PhysicalObject() = default;
    };
} // namespace scene::physical_objects

