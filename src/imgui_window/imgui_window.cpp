#include "imgui_window/imgui_window.h"
#include "misc/cpp/imgui_stdlib.h"

ImGuiWindow::ImGuiWindow(const std::shared_ptr<scene::Scene> &scene)
    : scene_(scene)
    , add_model_dialog_state_{.model_file_path="", .shader_file_path="", .object_name=""}
{
}

void ImGuiWindow::render_window()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);

    // rendering options
    ImGui::Text("Read-Timer Renderer");
    ImGui::ColorEdit4("clear color", glm::value_ptr(clear_color_));
    ImGui::Separator();

    // loading models and shaders
    
    if (ImGui::Button("add model")) {
        ImGui::OpenPopup("add model");
    }
    render_add_model_dialog();
    
    ImGui::SameLine();
    
    if (ImGui::Button("add shader")) {
        ImGui::OpenPopup("add shader");
    }
    render_add_shader_dialog();

    ImGui::Separator();


    // rendering statistics
    ImGui::Text("App average FPS: %.2f", calculate_fps());
    ImGui::Text("App frame time: %.2f", delta_time_);
}

glm::vec4 ImGuiWindow::get_clear_color() const
{
    return clear_color_;
}

float ImGuiWindow::get_delta_time() const
{
    return static_cast<float>(delta_time_);
}

double ImGuiWindow::calculate_fps()
{
    double curr_s = glfwGetTime();
    delta_time_ = curr_s - prev_s_;
    prev_s_ = curr_s;

    title_countdown_s_ -= delta_time_;
    if (title_countdown_s_ <= 0.0 && delta_time_ > 0.0) {
        fps_ = 1.0 / delta_time_;
        title_countdown_s_ = 0.1;
        return fps_;
    }
    return fps_;
}

void ImGuiWindow::render_add_model_dialog()
{
    if (ImGui::BeginPopupModal("add model", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Load a model into the scene");
        ImGui::Separator();
        ImGui::InputText("File", &add_model_dialog_state_.model_file_path);
        ImGui::InputText("Shader file", &add_model_dialog_state_.shader_file_path);
        ImGui::InputText("Object name", &add_model_dialog_state_.object_name);
        ImGui::Spacing();

        if (ImGui::Button("Load"))
        {
            // add model to scene
            add_model_dialog_state_ = {};
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            add_model_dialog_state_ = {};
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

}

void ImGuiWindow::render_add_shader_dialog()
{
    if (ImGui::BeginPopupModal("add shader", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Load a shader into the scene");
        ImGui::Separator();
        ImGui::InputText("File", &add_model_dialog_state_.model_file_path);
        ImGui::InputText("Shader file", &add_model_dialog_state_.shader_file_path);
        ImGui::Spacing();

        if (ImGui::Button("Load"))
        {
            // add shader to scene
            add_model_dialog_state_ = {};
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            add_model_dialog_state_ = {};
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
