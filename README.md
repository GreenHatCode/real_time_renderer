# Lightweight Real-Time OpenGL Renderer & Scene Editor

A custom-built, real-time 3D rendering framework and dynamic scene editor powered by Modern OpenGL. Designed as a high-performance playground for 3D graphics, this project features dynamic model loading, hot shader reloading, and a comprehensive user interface for real-time scene manipulation and performance profiling.

## Key Features
### Dynamic Scene Management
- **Asset Loading**: Dynamically load meshes and textures at runtime. Supports standard formats including .obj, .stl, and .glTF (with full support for both external and embedded textures).

- **Real-Time Manipulation**: Add or remove meshes on the fly. Transform objects effortlessly with dynamic scaling, rotation, and translation controls.

- **Scene Hierarchy**: Inspect and manage the scene structure through an intuitive, integrated object hierarchy tree.

### Advanced Camera System
- **Dual Movement Modes**: Switch seamlessly between Orbit (focusing on a specific target) and Free (first-person style flight) camera behaviors.

- **Flexible Projection**: Supports both Orthographic and Perspective projection modes to suit different rendering needs.

### Rendering & Materials
- **Hot Shader Reloading**: Modify, compile, and apply shaders at runtime without restarting the application.

- **Dynamic Shader Assignment**: Load a mesh with a generic shader and dynamically assign custom, specialized shaders to individual objects directly from the UI.

### Real-Time Profiling & UI
Fully wrapped and controlled using Dear ImGui, featuring a detailed render statistics dashboard:

- **Performance Metrics**: Track FPS, Frame Time, CPU Time, and GPU Time.

- **Geometry Stats**: Monitor total vertex and triangle counts in real-time.

- **Generic Controllers**: ImGui is deeply integrated not just as a wrapper, but as a library for all generic user controllers and inputs.

## Tech Stack
- **Language**: C++
- **Graphics API**: Modern OpenGL4+
- **UI Framework**: Dear ImGui
- **Asset Loading**: Assimp
- **Math**: GLM
- **Logging**: spdlog

## Build and Run

### Prerequisites

- C++20 or newer
- Installed and configured OpenGL4+
- CMake 3.14 or newer
- GCC 12 or newer

1. Clone the repository:
    ```bash
    git@github.com:GreenHatCode/real_time_renderer.git
    cd real_time_renderer
    ```
2.  Change git remote url to avoid accidental pushes to base project
    ```bash
    git remote set-url origin github_username/repo_name
    git remote -v # confirm the changes
    ```
3. Build and run (after successful build the app will start)
    ```bash
    chmod +x u_build_and_run.sh
    ./u_build_and_run.sh
    ```

## Usage examples

### Blending transparent objects with lighting
![Failed to load file "./pics/blending_transparent_objects_with_lighting.png"](./pics/blending_transparent_objects_with_lighting.png)

### Blur post processing
![Failed to load file "./pics/blur_post_processing_via_framebuffer.png"](./pics/blur_post_processing_via_framebuffer.png)

### Edge detection post processing
![Failed to load file "./pics/edge_detection_post_processing_via_framebuffer.png"](./pics/edge_detection_post_processing_via_framebuffer.png)

### Grayscale post processing
![Failed to load file "./pics/greyscale_post_processing_via_framebuffers.png"](./pics/greyscale_post_processing_via_framebuffers.png)

### Rendering different models
![Failed to load file "./pics/different_models_example.png"](./pics/different_models_example.png)

### Wireframe geometry post processing
![Failed to load file "./pics/drawing_in_wireframe_mode_using_geometry_shader.png"](./pics/drawing_in_wireframe_mode_using_geometry_shader.png)

### Drawing normals geometry post processing
![Failed to load file "./pics/diplaying_normals.png"](./pics/diplaying_normals.png)

### Lighting many objects
![Failed to load file "./pics/multiple_light_objects.png"](./pics/multiple_light_objects.png)

### Multiple lights
![Failed to load file "./pics/multiple_lights_example.png"](./pics/multiple_lights_example.png)

### Stencil outline
![Failed to load file "./pics/stencil_outline.png"](./pics/stencil_outline.png)
