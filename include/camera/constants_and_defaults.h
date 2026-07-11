#pragma once

enum class CameraMovement 
{
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