#include "shader/shader.h"
#include "opengl.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "spdlog/spdlog.h"

Shader::Shader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path)
    : name_(name)
{
    shader_program_id_ = glCreateProgram();
    unsigned int vertex, fragment, geometry;

    vertex = create_shader(vertex_path.c_str(), GL_VERTEX_SHADER);
    fragment = create_shader(fragment_path.c_str(), GL_FRAGMENT_SHADER);

    // geometry shader is optional
    if (!geometry_path.empty()) geometry = create_shader(geometry_path.c_str(), GL_GEOMETRY_SHADER);

    glAttachShader(shader_program_id_, vertex);
    glAttachShader(shader_program_id_, fragment);
    if (!geometry_path.empty()) glAttachShader(shader_program_id_, geometry);

    glLinkProgram(shader_program_id_);
    checkCompileErrors(shader_program_id_, GL_PROGRAM);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if (!geometry_path.empty()) glDeleteShader(geometry);
}
void Shader::use()
{ 
    glUseProgram(shader_program_id_); 
}

std::string Shader::get_name() const
{
    return name_;
}
Shader::~Shader()
{
    glDeleteProgram(shader_program_id_);
}

void Shader::checkCompileErrors(unsigned int shader, GLenum type)
{
    int success;
    char infoLog[1024];
    std::stringstream ss;
    if (type == GL_PROGRAM) {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            ss << "PROGRAM_LINKING_ERROR of type: "
            << shader_type_to_string(type) << ". "
            << infoLog;
        }
    }
    else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            ss << "SHADER_COMPILATION_ERROR of type: "
            << shader_type_to_string(type) << ". "
            << infoLog;
        }
    }

    if (!success) {
        spdlog::error(ss.str());
        throw std::runtime_error(ss.str());
    }
}

GLint Shader::get_uniform_location(const std::string &name)
{
    auto iter = cached_uniform_locations_.find(name);
    
    if (iter == cached_uniform_locations_.end()) {
        int32_t uniform_location = glGetUniformLocation(shader_program_id_, name.c_str());
        cached_uniform_locations_.emplace(name, uniform_location);
        return uniform_location;
    }

    return iter->second;
}

uint32_t Shader::create_shader(const char *file, GLenum type)
{
    try
    {
        std::ifstream inf(file);
        inf.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        std::string source{std::istreambuf_iterator<char>(inf), std::istreambuf_iterator<char>()};
        const char *source_ptr = source.c_str();

        GLuint descriptor = glCreateShader(type);
        glShaderSource(descriptor, 1, &source_ptr, NULL);
        glCompileShader(descriptor);
        checkCompileErrors(descriptor, type);

        return descriptor;
    }
    catch(const std::exception& e) 
    {
        spdlog::error("Failed to create shader from file={}, what={}", file, e.what());
        throw;
    }
}

const char *Shader::shader_type_to_string(GLenum type)
{
    switch (type)
    {
    case GL_VERTEX_SHADER:
        return "VERTEX";
        break;
    case GL_FRAGMENT_SHADER:
        return "FRAGMENT";
        break;
    case GL_GEOMETRY_SHADER:
        return "GEOMETRY";
        break;
    case GL_PROGRAM:
        return "PROGRAM";
        break;
    default:
        spdlog::error("Unknown shader type to convert to string: type=", static_cast<int>(type));
        assert(false);
        break;
    }

    return nullptr;
}
