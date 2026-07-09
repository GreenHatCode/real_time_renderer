#pragma once
#include <GLFW/glfw3.h>
#include <cstdio>
#include "imgui.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

class ImGuiWindow final {

    // do not expose outside
    double prev_s = 0.0;
    double title_countdown_s = 0.1;
    double fps = 0.0;
    
    // should be accessible through getters and setters respectively
    float f = 0.0f;
    int counter = 0;
    glm::vec4 clear_color;

public:
    void render_window() {
        ImGui::Text("Hello, World!");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit4("clear color", glm::value_ptr(clear_color));

        if (ImGui::Button("Button")) counter++;

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGui::Text("App average FPS: %.2f", calculate_fps());
    }

    glm::vec4 get_clear_color() const { return clear_color; }


private:
    double calculate_fps() {
        double curr_s = glfwGetTime();
        double elapsed_s = curr_s - prev_s;
        prev_s = curr_s;

        title_countdown_s -= elapsed_s;
        if (title_countdown_s <= 0.0 && elapsed_s > 0.0) {
            fps = 1.0 / elapsed_s;
            title_countdown_s = 0.1;
            return fps;
        }
        return fps;
    }

};