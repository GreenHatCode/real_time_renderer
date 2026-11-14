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
    struct UniformUploader<uint32_t>
    {
        static void upload(int32_t uniform_location, uint32_t value) {
            glUniform1ui(uniform_location, value);
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

    template <> 
    struct UniformUploader<glm::mat3>
    {
        static void upload(int32_t uniform_location, const glm::mat3& value) {
            glUniformMatrix3fv(uniform_location, 1, GL_FALSE, glm::value_ptr(value));
        }
    };

    template <> 
    struct UniformUploader<glm::vec2>
    {
        static void upload(int32_t uniform_location, const glm::vec2& value) {
            glUniform2fv(uniform_location, 1, glm::value_ptr(value));
        }
    };

    template <> 
    struct UniformUploader<glm::vec3>
    {
        static void upload(int32_t uniform_location, const glm::vec3& value) {
            glUniform3fv(uniform_location, 1, glm::value_ptr(value));
        }
    };

    template <> 
    struct UniformUploader<glm::vec4>
    {
        static void upload(int32_t uniform_location, const glm::vec4& value) {
            glUniform4fv(uniform_location, 1, glm::value_ptr(value));
        }
    };

}
// namespace uniform_traits
