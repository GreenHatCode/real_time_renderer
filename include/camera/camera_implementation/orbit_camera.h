#pragma once

#include "opengl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera/i_camera.h"
#include "camera/constants_and_defaults.h"
#include "camera/projection.h"
#include <stdexcept>

namespace OrbitCameraLimits 
{
    inline constexpr float MaxPitch = 89.0f;
    inline constexpr float MinPitch = -89.0f;
}

class OrbitCamera final : public ICamera {
    Projection projection_;
    glm::vec3 target_;

    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 camera_up_;
    glm::vec3 right_;
    glm::vec3 world_up_;

    float yaw_;
    float pitch_;

    float radius_;
    float movement_speed_;
    float sensitivity_;

public:
    OrbitCamera(Projection projection, float radius = 3.0f, glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f), 
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f), 
    float yaw = CameraDefaults::YAW, float pitch = CameraDefaults::PITCH) 
    : projection_(projection)
    , target_(target)
    , radius_(radius)
    , front_(direction)
    , position_(target - direction * radius)
    , world_up_(world_up)
    , yaw_(yaw)
    , pitch_(pitch)
    , movement_speed_(CameraDefaults::SPEED)
    , sensitivity_(CameraDefaults::SENSITIVITY)
    {
        update_camera_vectors();
    }

    glm::mat4 view_matrix() const override { return glm::lookAt(position_, target_, camera_up_); }

    float get_movement_speed() const override{ return movement_speed_; }

    float get_sensitivity() const { return sensitivity_; }

    Projection::ProjectionType get_projection_type() const override { return projection_.get_type(); }

    Projection::ProjectionInfo_t get_projection_info() const override {return projection_.get_zoom(); }

    glm::mat4 projection_matrix(float scr_width, float scr_height) const override
    {
        return projection_.matrix(scr_width, scr_height);
    }

public: // setters and modifiers
    
    void set_projection_type(Projection::ProjectionType type) override
    {
        projection_.set_type(type);
    }

    void set_movement_speed(float movement_speed) override
    {
        if (movement_speed <= 0.0f)
            throw std::runtime_error("Camera movement speed should be greater than zero.");

        movement_speed_ = movement_speed;
    }

    void set_sensitivity(float sensitivity) 
    {
        if (sensitivity <= 0.0f)
            throw std::runtime_error("Camera sensitivity should be greater than zero.");

        sensitivity_ = sensitivity;
    }

    // changes target position (where camera looks at)
    void move(CameraMovement direction, float deltaTime) override
    {
        float velocity = movement_speed_ * deltaTime;
        if (direction == CameraMovement::FORWARD)
            target_ += glm::vec3(front_.x, 0.0f, front_.z) * velocity;
        if (direction == CameraMovement::BACKWARD)
            target_ -= glm::vec3(front_.x, 0.0f, front_.z) * velocity;
        if (direction == CameraMovement::LEFT)
            target_ -= glm::vec3(right_.x, 0.0f, right_.z) * velocity;
        if (direction == CameraMovement::RIGHT)
            target_ += glm::vec3(right_.x, 0.0f, right_.z) * velocity;

        update_camera_vectors();
    }

    // changes camera direction thus changes position (move and rotate camera)
    void rotate(float xoffset, float yoffset) override
    {
        xoffset *= sensitivity_;
        yoffset *= sensitivity_;

        yaw_   += xoffset;
        pitch_ += yoffset;

        if (pitch_ > OrbitCameraLimits::MaxPitch)
            pitch_ = OrbitCameraLimits::MaxPitch;
        if (pitch_ < OrbitCameraLimits::MinPitch)
            pitch_ = OrbitCameraLimits::MinPitch;
        
        update_camera_vectors();
    }

    void zoom(float delta) override
    {
        projection_.zoom(-delta);
    }

private:
    void update_camera_vectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front.y = sin(glm::radians(pitch_));
        front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front_ = glm::normalize(front);
        right_ = glm::normalize(glm::cross(front_, world_up_));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        camera_up_ = glm::normalize(glm::cross(right_, front_));
        position_ = target_ - front_ * radius_;
    }
};