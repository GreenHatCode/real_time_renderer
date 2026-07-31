#pragma once
#include "opengl.h"
#include "scene/scene.h"
#include "camera/i_camera.h"
#include <memory>
#include <vector>
#include "shader/shader.h"
#include "scene/scene_objects/scene_object.h"
#include "renderer/simple_profiler.h"

class Renderer final {

    profiling::SimpleProfiler profiler_;
    const std::shared_ptr<const scene::Scene> scene_;
    Shader* curr_shader_ptr_; // can be a dangling pointeri

    glm::mat4 view_mat_;
    glm::mat4 projection_mat_;

    double profile_gpu_time_;
    double profile_cpu_time_;

    // uniforms and shaders
    void set_general_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr);
    void set_model_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr);

    void render_scene_object(const std::shared_ptr<const scene::ModelObject>& obj);

public:
    Renderer(std::shared_ptr<const scene::Scene> scene);
    void render_scene(const glm::vec4& clear_color_rgba, const glm::mat4& view_mat, const glm::mat4& projection_mat);
    double get_gpu_time() const;
    double get_cpu_time() const;
};