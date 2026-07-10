#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <stdexcept>

class Projection {
public:
    enum ProjectionType
    {
        ORTHOGRAPHIC,
        PERSPECTIVE
    };

    // view_size depends on the projection type: for PERSPECTIVE is it FOV; for ORTHOGRAPHIC - orthographic size by each axis in both directions
    Projection(ProjectionType type = PERSPECTIVE, float projection_parameter, float near, float far)
        : projection_type_(type), near_(near), far_(far)
    {
        if (near <= 0.0f || far < near) {
            std::runtime_error("Incorrect values for near and far");
        }
    }

    ProjectionType get_type() const { return projection_type_; }

    void set_type(ProjectionType type) {
        projection_type_ = type;
    }

    glm::mat4 projection_matrix(float width, float height) const {
        if (projection_type_ == PERSPECTIVE) {
            return glm::perspective(glm::radians(field_of_view_), width / height, near_, far_);
        }
        else {
            float aspect = width / height;
            return glm::ortho(-(orthographic_size_ * aspect), orthographic_size_ * aspect, -orthographic_size_, orthographic_size_, near_, far_);
        }
    }

private:
    ProjectionType projection_type_;
    float field_of_view_;
    float orthographic_size_;
    float near_;
    float far_;
};