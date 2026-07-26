#pragma once
#include "opengl.h"
#include <cstdio>
#include "imgui.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "scene/scene.h"
#include <filesystem>

class ImGuiWindow final {

    struct AddModelDialog_t {
        std::string model_file_path;
        std::string shader_name;
        std::string name;
    };

    struct AddShaderDialog_t {
        std::string vert_shader_file_path;
        std::string frag_shader_file_path;
        std::string name;
    };

    struct ModelInspectorState_t {
        std::shared_ptr<scene::ModelObject> selected_model_ptr;

    };

    // do not expose outside
    std::shared_ptr<scene::Scene> scene_;
    std::vector<std::string> loaded_models_names_;
    std::vector<std::string> loaded_shaders_names_;

    double prev_s_ = 0.0;
    double title_countdown_s_ = 0.1;
    double fps_ = 0.0;
    AddModelDialog_t model_dialog_state_;  
    AddShaderDialog_t shader_dialog_state_;
    ModelInspectorState_t model_inspector_state_;

    // should be accessible through getters and setters respectively
    glm::vec4 clear_color_;
    double delta_time_ = 0.0f;

public:
    ImGuiWindow(const std::shared_ptr<scene::Scene>& scene);
    void render_window();
    glm::vec4 get_clear_color() const;
    float get_delta_time() const;

private:
    double calculate_fps();
    void render_model_dialog();
    void render_shader_dialog();
    void render_model_inspector();
};