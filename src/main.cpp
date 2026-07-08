#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader/shader.h"
#include <iostream>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void calculate_fps(GLFWwindow* window);

void helloWorld() {
	ImGui::Begin("My DearImGui Window");
	ImGui::Text("hello, world");
	ImGui::End();
}


// settings
const uint32_t SCR_WIDTH = 800;
const uint32_t SCR_HEIGHT = 600;
double prev_s = 0.0;
double title_countdown_s = 0.1;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Read-Time Renderer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    const char* glsl_version = "#version 460";
	ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    float triangle_1_vertices[] = {
        // first triangle
        -0.9f,
        -0.5f,
        0.0f, // left
        -0.0f,
        -0.5f,
        0.0f, // right
        -0.45f,
        0.5f,
        0.0f, // top
    };

    float triangle_2_vertices[] = {
        // second triangle
        0.0f, -0.5f, 0.0f, // left
        0.9f, -0.5f, 0.0f, // right
        0.45f, 0.5f, 0.0f  // top
    };

    float triangle_2_vertex_colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    GLuint triangle_1_vbo, triangle_1_vao;
    glGenVertexArrays(1, &triangle_1_vao);
    glGenBuffers(1, &triangle_1_vbo);

    glBindVertexArray(triangle_1_vao);
    glBindBuffer(GL_ARRAY_BUFFER, triangle_1_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(triangle_1_vertices), triangle_1_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // second triangle
    GLuint triangle_2_vao, triangle_2_vbo, triangle_2_vertex_color_vbo;
    glGenVertexArrays(1, &triangle_2_vao);
    glGenBuffers(1, &triangle_2_vbo);
    glGenBuffers(1, &triangle_2_vertex_color_vbo);
    glBindVertexArray(triangle_2_vao);

    glBindBuffer(GL_ARRAY_BUFFER, triangle_2_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(triangle_2_vertices), triangle_2_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, triangle_2_vertex_color_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(triangle_2_vertex_colors), triangle_2_vertex_colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    {
        Shader simple_shader("shader/simple/vertex_shader.vert", "shader/simple/fragment_shader.frag");
        Shader coloured_shader("shader/coloured/vertex_shader.vert", "shader/coloured/fragment_shader.frag");
        
        // render loop
        prev_s = glfwGetTime();
        while (!glfwWindowShouldClose(window))
        {
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Run our Dear ImGui application
            helloWorld();




            // Rendering
            ImGui::Render();
            calculate_fps(window);
            processInput(window);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            simple_shader.use();
            glBindVertexArray(triangle_1_vao); // seeing as we only have a single triangle_1_vao there's no need to bind it every time, but we'll do so to keep things a bit more organized
            glDrawArrays(GL_TRIANGLES, 0, 3); // set the count to 6 since we're drawing 6 vertices now (2 triangles); not 3!

            coloured_shader.use();
            glBindVertexArray(triangle_2_vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &triangle_1_vao);
        glDeleteBuffers(1, &triangle_1_vbo);

    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void calculate_fps(GLFWwindow* window) {
    double curr_s = glfwGetTime();
    double elapsed_s = curr_s - prev_s;
    prev_s = curr_s;

    title_countdown_s -= elapsed_s;
    if (title_countdown_s <= 0.0 && elapsed_s > 0.0) {
        double fps = 1.0 / elapsed_s;

        char tmp[256];
        sprintf(tmp, "FPS: %.2lf", fps);
        glfwSetWindowTitle(window, tmp);
        title_countdown_s = 0.1;
    }
}