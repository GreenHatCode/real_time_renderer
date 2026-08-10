#pragma once
#include "opengl.h"
#include "scene/scene.h"
#include "camera/i_camera.h"
#include <memory>
#include <vector>
#include "shader/shader.h"
#include "scene/physical_objects/i_render.h"
#include "renderer/simple_profiler.h"

namespace renderer {
    namespace defaults {
        inline constexpr std::string_view OUTLINE_SHADER_NAME = "stencil_outline";
        inline constexpr std::string_view OUTLINE_VERTEX_SHADER = "shaders_sources/stencil_outline/vertex_shader.vert";
        inline constexpr std::string_view OUTLINE_FRAGMENT_SHADER = "shaders_sources/stencil_outline/fragment_shader.frag";
        inline constexpr float OUTLINE_WIDTH = 0.06f;
        inline constexpr glm::vec3 OUTLINE_COLOR = glm::vec3(0.0f, 0.8f, 1.0f);
    } // namespace defaults

    class Renderer final {

        profiling::SimpleProfiler profiler_;
        const std::shared_ptr<const scene::Scene> scene_;
        Shader* curr_shader_ptr_; // can be a dangling pointer
        std::vector<std::shared_ptr<scene::lights::LightBase>> scene_lights_buffer_;

        glm::mat4 view_mat_;
        glm::mat4 projection_mat_;
        glm::vec3 camera_position_;

        double profile_gpu_time_;
        double profile_cpu_time_;

        // uniforms and shaders
        void set_general_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr);
        void set_lighting_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr);

        void render_scene_object(const std::shared_ptr<const scene::physical_objects::IRender>& obj);
        void render_scene_object_with_outline(const std::shared_ptr<const scene::physical_objects::IRender>& obj);

    public:
        Renderer(std::shared_ptr<const scene::Scene> scene);
        void render_scene(const glm::vec4& clear_color_rgba, const glm::mat4& view_mat, const glm::mat4& projection_mat, const glm::vec3& camera_position);
        double get_gpu_time() const;
        double get_cpu_time() const;
    };

} // namespace renderer

