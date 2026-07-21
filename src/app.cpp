#include "app.h"
#include "spdlog/spdlog.h"

int32_t App::initialize_opengl()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Read-Time Renderer", NULL, NULL);
    if (window == NULL)
    {
        spdlog::error("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // add callbacks namespace
    glfwSetCursorPosCallback(window, mouse_callback); // add callbacks namespace
    glfwSetScrollCallback(window, scroll_callback); // add callbacks namespace

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    return 0;
}

void App::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void App::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_->move(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_->move(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_->move(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_->move(CameraMovement::RIGHT, deltaTime);
}

void App::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (first_mouse_)
    {
        lastX = xpos;
        lastY = ypos;
        first_mouse_ = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera_->rotate(xoffset, yoffset);
}

void App::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera_->zoom(static_cast<float>(yoffset));

}

App::App(uint32_t screen_width, uint32_t screen_height)
    : window_(NULL)
    , first_mouse_(false)
{
    // camera_ = std::make_unique<FreeCamera>(free_cam_projection, glm::vec3(0.0f, 0.0f, 3.0f));
    camera_ = std::make_shared<OrbitCamera>(Projection::Orthographic(95.0f, -100.0f, 300.0f););

    scene_ = std::make_shared<Scene>(/ ... /);

    imgui_window_ = std::unique_ptr<ImGuiWindow>(scene_);
    renderer_ = std::unique_ptr<Renderer>(scene_, screen_width, screen_height);
}

int32_t App::run()
{
    if (initialize_opengl() != 0) return -1;

    while (true)
    {
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
