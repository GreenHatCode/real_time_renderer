#pragma once
#include "camera/i_camera.h"
#include "renderer/renderer.h"
#include "scene/scene.h"
#include "ui_window/ui_window.h"

class App {

    enum class InputMode
    {
        Viewport,
        UI
    };

    std::unique_ptr<ICamera> camera_;
    std::shared_ptr<scene::Scene> scene_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<UIWindow> imgui_window_;

    GLFWwindow *window_;
    bool first_mouse_;
    float last_x_, last_y_;
    float screen_width_, screen_height_;
    InputMode input_mode_;

    // initial configuration
    void initialize_opengl();

    // wrappers for C OpenGL api
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    void framebuffer_size_callback_impl(int width, int height);
    void mouse_callback_impl(double xpos, double ypos);
    void scroll_callback_impl(double xoffset, double yoffset);
    void process_input(GLFWwindow *window);

    // ui behavior
    void enter_viewport_mode();
    void enter_ui_mode();



public:
    App(uint32_t screen_width = 800, uint32_t screen_height = 600);
    int32_t run();
    void load_sources_debug();
    ~App();
};