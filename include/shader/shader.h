#pragma once
#include <cstdint>
#include <string>
#include "shader/uniform_uploader.h"

class Shader
{
private:
    uint32_t ID;

public:
    Shader(const char *vertexPath, const char *fragmentPath);
    void use();
    ~Shader();

    template <typename T>
    void set_uniform(const std::string &name, const T &value) const
    {
        int32_t uniform_location = glGetUniformLocation(ID, name.c_str());
        uniform_traits::UniformUploader<T>::upload(uniform_location, value);
    }

    

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};
