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
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    window_ = glfwCreateWindow(framebuffer_width_, framebuffer_height_, "Read-Time Renderer", NULL, NULL);
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

    glfwGetFramebufferSize(window_, &framebuffer_width_, &framebuffer_height_);


    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        
        // Ensures callback runs on the thread that triggered the error,
        // making it easy to hit breakpoints directly on the bad GL call.
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        
        glDebugMessageCallback(openglDebugCallback, nullptr);
        
        // Enable all message groups
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
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
    framebuffer_width_ = width;
    framebuffer_height_ = height;
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
    if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS )) {
        if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) scroll_callback_impl(0, KEYBOARD_SCROLL_DIFF);
        else if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) scroll_callback_impl(0, -KEYBOARD_SCROLL_DIFF);
    }
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
    , framebuffer_width_(screen_width)
    , framebuffer_height_(screen_height)
    , input_mode_(InputMode::UI)
{

    initialize_opengl();

    //configure logging
    spdlog::set_level(spdlog::level::debug);
    
#ifdef FREE_CAMERA
    camera_ = std::make_unique<FreeCamera>(
        Projection::Perspective(45.0f, 0.1f, scene::lights::lighting_constants::DIRECTIONAL_LIGHT_DEFAULT_FAR_PLANE), 
        glm::vec3(0.0f, 0.0f, -10.0f),
        glm::vec3(0.0f, 0.0f, -1.0f)
    );
#else
    camera_ = std::make_unique<OrbitCamera>(Projection::Orthographic(
        95.0f, // ortho size
        scene::lights::lighting_constants::CAMERA_DEFAULT_NEAR_PLANE, 
        scene::lights::lighting_constants::DIRECTIONAL_LIGHT_DEFAULT_FAR_PLANE));
#endif

    scene_ = std::make_shared<scene::Scene>();
    imgui_window_ = std::make_unique<UIWindow>(scene_);
    renderer_ = std::make_unique<renderer::Renderer>(scene_);

    spdlog::info("Created app with screen size: width={}, height={}", screen_width, screen_height);

    load_default_content();
}

void App::load_sources_debug() 
{
    // mostly incorrect load 
    // normal work only from ui

    imgui_window_->load_shader("simple", "shaders_sources/simple/vertex_shader.vert", "shaders_sources/simple/fragment_shader.frag");
    imgui_window_->load_shader("lighting", "shaders_sources/lighting/vertex_shader.vert", "shaders_sources/lighting/fragment_shader.frag");
    imgui_window_->load_shader("assimp", "shaders_sources/assimp/spider_vertex_shader.vert", "shaders_sources/assimp/spider_fragment_shader.frag");
    imgui_window_->load_shader("blending", "shaders_sources/blending/vertex_shader.vert", "shaders_sources/blending/fragment_shader.frag");

    // imgui_window_->load_model("assimp", "spider", "resources/assimp-mdb/model-db/Obj/Spider/spider.obj");   
    // imgui_window_->load_model("blending", "window", "resources/transparent_window/tinker.obj");
    // imgui_window_->load_model("blending", "window2", "resources/transparent_window/tinker.obj");
    // imgui_window_->load_model("blending", "window3", "resources/transparent_window/tinker.obj");

    // imgui_window_->load_model("lighting", "multi_cube", "resources/multi_cube/tinker.obj");
    imgui_window_->load_model("lighting", "sphere", "resources/light_sphere.obj");

    imgui_window_->add_light("s1", scene::lights::LightType::SPOT_LIGHT);
    // imgui_window_->add_light("p1", scene::lights::LightType::POINT_LIGHT);
    // imgui_window_->add_light("dir1", scene::lights::LightType::DIRECTIONAL_LIGHT);
    // imgui_window_->add_light("dir2", scene::lights::LightType::DIRECTIONAL_LIGHT);

#if 0
    for (int i = 0; i < 10; i++) {
            imgui_window_->add_light("dir" + std::to_string(i), scene::lights::LightType::DIRECTIONAL_LIGHT);
    }

#endif
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
        

        renderer_->set_clear_color(imgui_window_->get_clear_color());
        renderer_->set_view_mat(camera_->view_matrix());
        renderer_->set_projection_mat(camera_->projection_matrix(framebuffer_width_, framebuffer_height_));
        renderer_->set_camera_position(camera_->get_position());
        renderer_->set_post_processing(imgui_window_->get_post_processing_type());
        renderer_->set_geometry_post_processing(imgui_window_->get_geometry_processing_type());
        renderer_->set_internal_objects_id_mask(imgui_window_->get_selected_internal_objects_id_mask());
        renderer_->set_gamma(imgui_window_->get_gamma());

        renderer_->render_scene(framebuffer_width_, framebuffer_height_);

        imgui_window_->set_gpu_time_to_display(renderer_->get_gpu_time());
        imgui_window_->set_cpu_time_to_display(renderer_->get_cpu_time());


        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

    return 0;
}
