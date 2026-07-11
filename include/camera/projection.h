#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <stdexcept>
#include <cassert>

namespace ProjectionLimits 
{
    inline constexpr float MIN_NEAR = 0.001f;
    inline constexpr float MIN_FOV = 1.0f;
    inline constexpr float MAX_FOV = 120.0f;
    inline constexpr float MIN_ORTHOGRAPHIC_SIZE = 1.0f;
    inline constexpr float MAX_ORTHOGRAPHIC_SIZE = 100.0f;
}

namespace ProjectionDefaults 
{
    inline constexpr float DEFAULT_FOV = 45.0f;
    inline constexpr float DEFAULT_ORTHOGRAPHIC_SIZE = 10.0f;
}

class Projection {
public:
    enum class ProjectionType
    {
        ORTHOGRAPHIC,
        PERSPECTIVE
    };

private:
    ProjectionType projection_type_;
    float field_of_view_;
    float orthographic_size_;
    float near_;
    float far_;

    Projection(ProjectionType type, float projection_parameter, float near, float far)
        : projection_type_(type)
        , field_of_view_(ProjectionDefaults::DEFAULT_FOV)
        , orthographic_size_(ProjectionDefaults::DEFAULT_ORTHOGRAPHIC_SIZE)
        , near_(near)
        , far_(far)
    {
        if (type == ProjectionType::PERSPECTIVE) field_of_view_ = projection_parameter;
        else orthographic_size_ = projection_parameter;
    }

    static bool validate_fov(float value) {
        return value >= ProjectionLimits::MIN_FOV && value <= ProjectionLimits::MAX_FOV;
    }

    static bool validate_ortho_size(float value) {
        return value >= ProjectionLimits::MIN_ORTHOGRAPHIC_SIZE && value <= ProjectionLimits::MAX_ORTHOGRAPHIC_SIZE;
    }

public:

    static Projection Perspective(float field_of_view, float near, float far) {
        if (near <= ProjectionLimits::MIN_NEAR || far <= near) {
            throw std::invalid_argument("Incorrect values for near and far");
        }

        if (!validate_fov(field_of_view)) {
            throw std::invalid_argument("Invalid value for field of view");
        }

        return Projection(ProjectionType::PERSPECTIVE, field_of_view, near, far);
    }

    static Projection Orthographic(float orthographic_size, float near, float far) {
        if (near <= ProjectionLimits::MIN_NEAR || far <= near) {
            throw std::invalid_argument("Incorrect values for near and far");
        }

        if (!validate_ortho_size(orthographic_size)) {
            throw std::invalid_argument("Invalid value for orthographic size");
        }

        return Projection(ProjectionType::ORTHOGRAPHIC, orthographic_size, near, far);
    }

    glm::mat4 matrix(float width, float height) const {

        assert(height > 0.0f);
        assert(width > 0.0f);
        if (projection_type_ == ProjectionType::PERSPECTIVE) {
            return glm::perspective(glm::radians(field_of_view_), width / height, near_, far_);
        }
        else {
            float aspect = width / height;
            return glm::ortho(-(orthographic_size_ * aspect), orthographic_size_ * aspect, -orthographic_size_, orthographic_size_, near_, far_);
        }
    }

    ProjectionType get_type() const { return projection_type_; }

    void set_type(ProjectionType type) {
        projection_type_ = type;
    }

    float get_field_of_view() const { return field_of_view_; }

    void set_field_of_view(float value) {
        if (!validate_fov(value)) {
            throw std::invalid_argument("Invalid new value for field of view");
        }

        field_of_view_ = value;
    }

    float get_orthographic_size() const { return orthographic_size_; }

    void set_orthographic_size(float value) {
        if (!validate_ortho_size(value)) {
            throw std::invalid_argument("Invalid new value for orthographic size");
        }
    
        orthographic_size_ = value;
    }

};