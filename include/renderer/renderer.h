#pragma once
#include "opengl.h"
#include "scene/scene.h"
#include "camera/i_camera.h"
#include <memory>
#include <vector>
#include "shader/shader.h"
#include "scene/scene_objects/scene_object.h"

class Renderer final {

    std::shared_ptr<scene::Scene> scene_;
    const uint32_t scr_width_;
    const uint32_t scr_height_;
    std::shared_ptr<Shader> curr_shader_ptr_;

    glm::mat4 view_mat_;
    glm::mat4 projection_mat_;

    // uniforms and shaders
    void set_general_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr);
    void set_model_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr);

    void render_scene_object(const std::shared_ptr<scene::ModelObject>& obj);

public:
    Renderer(const std::shared_ptr<scene::Scene>& scene, uint32_t screen_width, uint32_t screen_height);
    void render_scene(const std::shared_ptr<ICamera>& camera);
};