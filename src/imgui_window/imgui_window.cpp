#include "imgui_window/imgui_window.h"

ImGuiWindow::ImGuiWindow(const std::shared_ptr<scene::Scene> &scene)
    : scene_(scene)
{
}

void ImGuiWindow::render_window()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);

    ImGui::Text("Read-Timer Renderer");
    ImGui::ColorEdit4("clear color", glm::value_ptr(clear_color));

    ImGui::Separator();
    ImGui::Text("App average FPS: %.2f", calculate_fps());
}

glm::vec4 ImGuiWindow::get_clear_color() const
{
    return clear_color;
}

float ImGuiWindow::get_delta_time() const
{
    return static_cast<float>(delta_time);
}
