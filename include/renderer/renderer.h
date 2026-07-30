#pragma once
#include "opengl.h"
#include "scene/scene.h"
#include "camera/i_camera.h"
#include <memory>
#include <vector>
#include "shader/shader.h"
#include "scene/scene_objects/scene_object.h"

class Renderer final {

    const std::shared_ptr<const scene::Scene> scene_;
    std::shared_ptr<Shader> curr_shader_ptr_;

    glm::mat4 view_mat_;
    glm::mat4 projection_mat_;

    uint64_t profile_gpu_time_;
    uint64_t profile_cpu_time_;

    // uniforms and shaders
    void set_general_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr);
    void set_model_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr);

    void render_scene_object(const std::shared_ptr<const scene::ModelObject>& obj);

public:
    Renderer(std::shared_ptr<const scene::Scene> scene);
    void render_scene(const glm::vec4& clear_color_rgba, const glm::mat4& view_mat, const glm::mat4& projection_mat);
    uint64_t get_gpu_time() const;
    uint64_t get_cpu_time() const;
};