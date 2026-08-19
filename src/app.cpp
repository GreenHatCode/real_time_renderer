#include "app.h"
#include "spdlog/spdlog.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "camera/camera_implementation/orbit_camera.h"
#include "camera/camera_implementation/free_camera.h"
#include "scene/lights/point_light.h"

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
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window_);
    glfwSetWindowUserPointer(window_, this);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback); // add callbacks namespace
    glfwSetCursorPosCallback(window_, mouse_callback); // add callbacks namespace
    glfwSetScrollCallback(window_, scroll_callback); // add callbacks namespace

    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    spdlog::info("Initialized opengl");


    // platform debug info
    GLint maxTextureUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    spdlog::info("GPU maximum fragment samplers: {}", maxTextureUnits);
}

void App::load_default_content()
{
    imgui_window_->load_shader(
        std::string(scene::physical_objects::physical_light_object_constants::LIGHT_MODEL_OBJECT_SHADER_NAME), 
        std::string(scene::physical_objects::physical_light_object_constants::LIGHT_MODEL_OBJECT_VERTEX_SHADER), 
        std::string(scene::physical_objects::physical_light_object_constants::LIGHT_MODEL_OBJECT_FRAGMENT_SHADER)
    );

    imgui_window_->load_shader(
        std::string(renderer::defaults::OUTLINE_SHADER_NAME), 
        std::string(renderer::defaults::OUTLINE_VERTEX_SHADER), 
        std::string(renderer::defaults::OUTLINE_FRAGMENT_SHADER)
    );


    spdlog::info("Loaded default content, please do not delete it");
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
    // quit app
    bool quit_app = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
    if (quit_app) glfwSetWindowShouldClose(window, true);

    bool escape_pressed = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    if (escape_pressed && input_mode_ != InputMode::UI) {
        enter_ui_mode();
    }

    bool lbt_pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    bool imgui_wants_mouse = ImGui::GetIO().WantCaptureMouse;
    if (input_mode_ != InputMode::Viewport && !imgui_wants_mouse && lbt_pressed)
    {
        enter_viewport_mode();
    }

    // viewport mode functions
    if (input_mode_ != InputMode::Viewport) return;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_->move(CameraMovement::FORWARD, imgui_window_->get_delta_time());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_->move(CameraMovement::BACKWARD, imgui_window_->get_delta_time());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_->move(CameraMovement::LEFT, imgui_window_->get_delta_time());
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_->move(CameraMovement::RIGHT, imgui_window_->get_delta_time());
}

void App::enter_viewport_mode()
{
    input_mode_ = InputMode::Viewport;
    first_mouse_ = true;
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    spdlog::info("Enter viewport mode");
}

void App::enter_ui_mode()
{
    input_mode_ = InputMode::UI;
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    spdlog::info("Enter ui mode");
}

App::~App()
{
    renderer_.reset();
    imgui_window_.reset();
    scene_.reset();
    camera_.reset();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (window_) {
        glfwDestroyWindow(window_);
    }
    glfwTerminate();
}

void App::mouse_callback_impl(double xpos_in, double ypos_in)
{
    if (input_mode_ != InputMode::Viewport) return;

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
    if (input_mode_ != InputMode::Viewport) return;
    camera_->zoom(static_cast<float>(yoffset));
}

App::App(uint32_t screen_width, uint32_t screen_height)
    : window_(NULL)
    , first_mouse_(true)
    , last_x_(screen_width/2.0f)
    , last_y_(screen_height/2.0f)
    , screen_width_(screen_width)
    , screen_height_(screen_height)
    , input_mode_(InputMode::UI)
{

    initialize_opengl();

    //configure logging
    spdlog::set_level(spdlog::level::debug);
    

    // camera_ = std::make_unique<FreeCamera>(
    //     Projection::Perspective(45.0f, 0.1f, 300.0f), 
    //     glm::vec3(0.0f, 0.0f, -10.0f),
    //     glm::vec3(0.0f, 0.0f, -1.0f)
    // );
    camera_ = std::make_unique<OrbitCamera>(Projection::Orthographic(95.0f, -100.0f, 300.0f));

    scene_ = std::make_shared<scene::Scene>();
    imgui_window_ = std::make_unique<UIWindow>(scene_);
    renderer_ = std::make_unique<renderer::Renderer>(scene_);

    spdlog::info("Created app with screen size: width={}, height={}", screen_width, screen_height);

    load_default_content();
}

void App::load_sources_debug() 
{
    imgui_window_->load_shader("simple", "shaders_sources/simple/vertex_shader.vert", "shaders_sources/simple/fragment_shader.frag");
    imgui_window_->load_shader("lighting", "shaders_sources/lighting/vertex_shader.vert", "shaders_sources/lighting/fragment_shader.frag");
    imgui_window_->load_shader("assimp", "shaders_sources/assimp/spider_vertex_shader.vert", "shaders_sources/assimp/spider_fragment_shader.frag");
    imgui_window_->load_shader("blending", "shaders_sources/blending/vertex_shader.vert", "shaders_sources/blending/fragment_shader.frag");


    // imgui_window_->load_model("lighting", "spider", "resources/assimp-mdb/model-db/Obj/Spider/spider.obj");   
    // imgui_window_->load_model("lighting", "wuson", "resources/assimp-mdb/model-db/Obj/Wuson/WusonOBJ.obj");
    imgui_window_->load_model("lighting", "glass", "resources/multi_cube/tinker.obj");

    imgui_window_->add_light("spot_light", scene::lights::LightType::SPOT_LIGHT);
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
        
        renderer_->render_scene(imgui_window_->get_clear_color(), camera_->view_matrix(), camera_->projection_matrix(screen_width_, screen_height_), camera_->get_position()); 


        imgui_window_->set_gpu_time_to_display(renderer_->get_gpu_time());
        imgui_window_->set_cpu_time_to_display(renderer_->get_cpu_time());


        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

    return 0;
}
