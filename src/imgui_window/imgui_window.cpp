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
    ImGui::Text("App frame time: %.2f", delta_time);
}

glm::vec4 ImGuiWindow::get_clear_color() const
{
    return clear_color;
}

float ImGuiWindow::get_delta_time() const
{
    return static_cast<float>(delta_time);
}

double ImGuiWindow::calculate_fps()
{
    double curr_s = glfwGetTime();
    delta_time = curr_s - prev_s;
    prev_s = curr_s;

    title_countdown_s -= delta_time;
    if (title_countdown_s <= 0.0 && delta_time > 0.0) {
        fps = 1.0 / delta_time;
        title_countdown_s = 0.1;
        return fps;
    }
    return fps;
}
