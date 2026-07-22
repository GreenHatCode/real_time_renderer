#pragma once
#include "opengl.h"
#include <cstdint>
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace detail
{
    template <typename T>
    struct UniformUploader;

    template <> 
    struct UniformUploader<bool>
    {
        static void upload(int32_t uniform_location, bool value) {
            glUniform1i(uniform_location, (int)value);
        }
    };

    template <> 
    struct UniformUploader<int>
    {
        static void upload(int32_t uniform_location, int value) {
            glUniform1i(uniform_location, value);
        }
    };
    
    template <> 
    struct UniformUploader<float>
    {
        static void upload(int32_t uniform_location, float value) {
            glUniform1f(uniform_location, value);
        }
    };

    template <> 
    struct UniformUploader<glm::mat4>
    {
        static void upload(int32_t uniform_location, const glm::mat4& value) {
            glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(value));
        }
    };


}
// namespace uniform_traits
