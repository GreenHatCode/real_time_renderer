#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

namespace CameraDefaults
{
    inline constexpr float YAW         = -90.0f;
    inline constexpr float PITCH       = 0.0f;
    inline constexpr float SPEED       = 2.5f;
    inline constexpr float SENSITIVITY = 0.1f;
    inline constexpr float ZOOM        = 45.0f;
}

class FlyCamera
{
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 camera_up_;
    glm::vec3 right_;
    glm::vec3 world_up_;

    float yaw_;
    float pitch_;

    float movement_speed_;
    float sensitivity_;
    float field_of_view_;

public:
    FlyCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = CameraDefaults::YAW, float pitch = CameraDefaults::PITCH) 
    : front_(direction)
    , position_(position)
    , world_up_(world_up)
    , yaw_(yaw)
    , pitch_(pitch)
    , movement_speed_(CameraDefaults::SPEED)
    , sensitivity_(CameraDefaults::SENSITIVITY)
    , field_of_view_(CameraDefaults::ZOOM)
    {
        update_camera_vectors();
    }

    glm::mat4 get_view_matrix() { return glm::lookAt(position_, position_ + front_, camera_up_); }

    float get_movement_speed() const { return movement_speed_; }

    float get_fov() const { return field_of_view_; }

    float get_sensitivity() const { return sensitivity_; }

public: // setters and modifiers
    void set_movement_speed(float movement_speed) 
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

    void move(Camera_Movement direction, float deltaTime)
    {
        float velocity = movement_speed_ * deltaTime;
        if (direction == FORWARD)
            position_ += front_ * velocity;
        if (direction == BACKWARD)
            position_ -= front_ * velocity;
        if (direction == LEFT)
            position_ -= right_ * velocity;
        if (direction == RIGHT)
            position_ += right_ * velocity;
    }

    void change_direction(float xoffset, float yoffset, GLboolean constrain_pitch = true)
    {
        xoffset *= sensitivity_;
        yoffset *= sensitivity_;

        yaw_   += xoffset;
        pitch_ += yoffset;

        if (constrain_pitch)
        {
            if (pitch_ > 89.0f)
                pitch_ = 89.0f;
            if (pitch_ < -89.0f)
                pitch_ = -89.0f;
        }

        update_camera_vectors();
    }

    void change_fov(float yoffset)
    {
        field_of_view_ -= (float)yoffset;
        if (field_of_view_ < 1.0f)
            field_of_view_ = 1.0f;
        if (field_of_view_ > 45.0f)
            field_of_view_ = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void update_camera_vectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front.y = sin(glm::radians(pitch_));
        front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front_ = glm::normalize(front);
        right_ = glm::normalize(glm::cross(front_, world_up_));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        camera_up_ = glm::normalize(glm::cross(right_, front_));
    }
};

