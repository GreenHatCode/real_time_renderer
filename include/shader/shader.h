#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include "shader/uniform_uploader.h"

class Shader
{
private:
    uint32_t shader_program_id_;
    std::unordered_map<std::string, int32_t> cached_uniform_locations_;
    std::string name_;

public:
    Shader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path = "");
    void use();
    std::string get_name() const;
    ~Shader();

    template <typename T>
    void set_uniform(const std::string &name, const T &value)
    {
        detail::UniformUploader<T>::upload(get_uniform_location(name), value);
    }

private:
    void checkCompileErrors(unsigned int shader, GLenum type);
    GLint get_uniform_location(const std::string &name);
    uint32_t create_shader(const char *file, GLenum type);

    static const char* shader_type_to_string(GLenum type);
};
