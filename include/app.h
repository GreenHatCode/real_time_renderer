#pragma once
#include "camera/i_camera.h"
#include "renderer/renderer.h"
#include "scene/scene.h"
#include "imgui_window/imgui_window.h"

class App {

    std::shared_ptr<ICamera> camera_;
    std::shared_ptr<Scene> scene_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<ImGuiWindow> imgui_window_;

    GLFWwindow *window_;
    bool first_mouse_;

    int32_t initialize_opengl();

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow *window);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


public:
    App(uint32_t screen_width = 800, uint32_t screen_height = 600);
    int32_t run();
};