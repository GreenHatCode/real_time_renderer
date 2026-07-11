#pragma once
#include "glm/glm.hpp"
#include "camera/projection.h"
#include "constants_and_defaults.h"

class ICamera {
public:
    virtual glm::mat4 view_matrix() const = 0;
    virtual float get_movement_speed() const = 0;
    virtual void set_movement_speed(float movement_speed) = 0;
    virtual Projection::ProjectionType get_projection_type() const = 0;
    virtual void set_projection_type(Projection::ProjectionType type) = 0;
    virtual Projection::ProjectionInfo_t get_projection_info() const = 0;
    virtual glm::mat4 projection_matrix(float scr_width, float scr_height) const = 0;
    virtual void move(CameraMovement direction, float deltaTime) = 0;
    virtual void rotate(float xoffset, float yoffset) = 0;
    virtual void zoom(float delta) = 0;    
    
    virtual ~ICamera() = default;
};