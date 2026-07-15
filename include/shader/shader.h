#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include "shader/uniform_uploader.h"

class Shader
{
private:
    uint32_t shader_program_id;
    std::unordered_map<std::string, int32_t> cached_uniform_locations;

public:
    Shader(const char *vertexPath, const char *fragmentPath);
    void use();
    ~Shader();

    template <typename T>
    void set_uniform(const std::string &name, const T &value)
    {
        detail::UniformUploader<T>::upload(get_uniform_location(name), value);
    }

private:
    void checkCompileErrors(unsigned int shader, std::string type);
    GLint get_uniform_location(const std::string &name);
};
