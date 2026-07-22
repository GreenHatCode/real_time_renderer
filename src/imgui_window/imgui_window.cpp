#include "imgui_window.h"

ImGuiWindow::ImGuiWindow(const std::shared_ptr<scene::Scene> &scene)
    : scene_(scene)
{
}

void ImGuiWindow::render_window()
{
    ImGui::Text("Hello, World!");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit4("clear color", glm::value_ptr(clear_color));

    if (ImGui::Button("Button")) counter++;

    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
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
