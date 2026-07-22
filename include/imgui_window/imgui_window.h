#pragma once
#include <GLFW/glfw3.h>
#include <cstdio>
#include "imgui.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "scene/scene.h"

class ImGuiWindow final {

    // do not expose outside
    std::shared_ptr<scene::Scene> scene_;

    double prev_s = 0.0;
    double title_countdown_s = 0.1;
    double fps = 0.0;
    

    // should be accessible through getters and setters respectively
    float f = 0.0f;
    int counter = 0;
    glm::vec4 clear_color;
    double delta_time = 0.0f;

public:
    ImGuiWindow(const std::shared_ptr<scene::Scene>& scene);
    void render_window();
    glm::vec4 get_clear_color() const;
    float get_delta_time() const;

private:
    double calculate_fps() {
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

};