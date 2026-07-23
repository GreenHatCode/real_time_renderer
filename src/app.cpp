#include "app.h"
#include "spdlog/spdlog.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "camera/camera_implementation/orbit_camera.h"
#include "camera/camera_implementation/free_camera.h"


void App::initialize_opengl()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(screen_width_, screen_height_, "Read-Time Renderer", NULL, NULL);
    if (window_ == NULL)
    {
        glfwTerminate();
        std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window_);
    glfwSetWindowUserPointer(window_, this);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback); // add callbacks namespace
    glfwSetCursorPosCallback(window_, mouse_callback); // add callbacks namespace
    glfwSetScrollCallback(window_, scroll_callback); // add callbacks namespace

    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::runtime_error("Failed to initialize GLAD");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    spdlog::info("Initialized opengl");
}

void App::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    App* app_ptr = static_cast<App*>(glfwGetWindowUserPointer(window));
    assert(app_ptr);
    app_ptr->framebuffer_size_callback_impl(width, height);
}

void App::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    App* app_ptr = static_cast<App*>(glfwGetWindowUserPointer(window));
    assert(app_ptr);
    app_ptr->mouse_callback_impl(xpos, ypos);
}

void App::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    App* app_ptr = static_cast<App*>(glfwGetWindowUserPointer(window));
    assert(app_ptr);
    app_ptr->scroll_callback_impl(xoffset, yoffset);
}

void App::framebuffer_size_callback_impl(int width, int height)
{
    glViewport(0, 0, width, height);
}

void App::process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_->move(CameraMovement::FORWARD, imgui_window_->get_delta_time());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_->move(CameraMovement::BACKWARD, imgui_window_->get_delta_time());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_->move(CameraMovement::LEFT, imgui_window_->get_delta_time());
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_->move(CameraMovement::RIGHT, imgui_window_->get_delta_time());
}

void App::mouse_callback_impl(double xpos_in, double ypos_in)
{
    float xpos = static_cast<float>(xpos_in);
    float ypos = static_cast<float>(ypos_in);

    if (first_mouse_)
    {
        last_x_ = xpos;
        last_y_ = ypos;
        first_mouse_ = false;
    }

    float xoffset = xpos - last_x_;
    float yoffset = last_y_ - ypos; // reversed since y-coordinates go from bottom to top

    last_x_ = xpos;
    last_y_ = ypos;

    camera_->rotate(xoffset, yoffset);
}

void App::scroll_callback_impl(double xoffset, double yoffset)
{
    camera_->zoom(static_cast<float>(yoffset));

}

App::App(uint32_t screen_width, uint32_t screen_height)
    : window_(NULL)
    , first_mouse_(true)
    , last_x_(screen_width/2.0f)
    , last_y_(screen_height/2.0f)
    , screen_width_(screen_width)
    , screen_height_(screen_height)
{

    initialize_opengl();

    // camera_ = std::make_shared<FreeCamera>(Projection::Orthographic(95.0f, -100.0f, 300.0f), glm::vec3(0.0f, 0.0f, 3.0f));
    camera_ = std::make_shared<OrbitCamera>(Projection::Orthographic(95.0f, -100.0f, 300.0f));

    scene_ = std::make_shared<scene::Scene>();
    imgui_window_ = std::make_unique<ImGuiWindow>(scene_);
    renderer_ = std::make_unique<Renderer>(scene_, screen_width, screen_height);

    spdlog::info("Created app with screen size: width={}, height={}", screen_width, screen_height);

    // tmp add objects and shaders

    auto spider = scene_->add_model("resources/assimp-mdb/model-db/Obj/Spider/spider.obj");
    auto spider_shader = scene_->add_shader("shaders_sources/assimp/spider_vertex_shader.vert", "shaders_sources/assimp/spider_fragment_shader.frag");
    spider->set_shader(spider_shader);
}

int32_t App::run()
{
    while (!glfwWindowShouldClose(window_))
    {
        process_input(window_);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        imgui_window_->render_window();
        ImGui::Render();
        // process imgui_window_ input and modify scene objects and camera
        // this includes:
        //      - add/delete object form scene
        //      - change scene object's shader
        //      - move camera
        //      - change camera/projection type
        //      - change other properties from ui

        
        renderer_->render_scene(camera_); 

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
