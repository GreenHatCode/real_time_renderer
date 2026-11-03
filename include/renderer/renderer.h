#pragma once
#include "opengl.h"
#include "scene/scene.h"
#include "camera/i_camera.h"
#include <memory>
#include <vector>
#include "shader/shader.h"
#include "scene/physical_objects/i_render.h"
#include "renderer/simple_profiler.h"
#include "renderer/post_processing/post_processor.h"
#include "renderer/post_processing/geometry_processor.h"
#include "renderer/frame_buffers/i_shadow_frame_buffer.h"
#include "scene/lights/light_base.h"
#include "renderer/frame_buffers/g_framebuffer.h"

namespace renderer {
    namespace defaults {
        inline constexpr std::string_view OUTLINE_SHADER_NAME = "stencil_outline";
        inline constexpr std::string_view OUTLINE_VERTEX_SHADER = "shaders_sources/stencil_outline/vertex_shader.vert";
        inline constexpr std::string_view OUTLINE_FRAGMENT_SHADER = "shaders_sources/stencil_outline/fragment_shader.frag";
        inline constexpr float OUTLINE_WIDTH = 0.1f;
        inline constexpr glm::vec3 OUTLINE_COLOR = glm::vec3(0.0f, 0.8f, 1.0f);
        
        inline constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
        inline constexpr glm::vec3 FALLBACK_WORLD_UP = glm::vec3(0.0f, 0.0f, 1.0f);

        inline constexpr uint8_t AXES_LINES_ID = 0x01; // 0b00000001
        inline constexpr uint8_t XZ_FACE_GRID_ID = 0x02; // 0b000010
        inline constexpr uint8_t XZ_FACE_SOLID_ID = 0x04; // 0b00100
    } // namespace defaults

    class Renderer final {

        struct RenderQueueObject {
            std::shared_ptr<const scene::physical_objects::IRender> obj_ptr;
            float distance_to_viewer;
            GLenum draw_mode;
        };

    private:
        profiling::SimpleProfiler profiler_;
        const std::shared_ptr<const scene::Scene> scene_;
        Shader* curr_shader_ptr_; // can be a dangling pointer
        std::vector<std::shared_ptr<scene::lights::LightBase>> scene_lights_buffer_;

        PostProcessor post_processor_;
        PostProcessingTypes post_processing_type_;
        GeometryProcessor geometry_processor_;
        GeometryProcessingTypes geometry_processing_type_;

        framebuffers::GFrameBuffer g_buffer_;

        std::vector<RenderQueueObject> opaque_objects_;
        std::vector<RenderQueueObject> transparent_objects_;

        uint8_t internal_objects_id_mask_;

        glm::vec4 clear_color_rgba_;
        glm::mat4 view_mat_;
        glm::mat4 projection_mat_;
        glm::vec3 camera_position_;

        double profile_gpu_time_;
        double profile_cpu_time_;

        GLsizei framebuffer_width_;
        GLsizei framebuffer_height_;

        std::map<scene::lights::LightType, std::unique_ptr<framebuffers::IShadowFrameBuffer>> shadow_frame_buffers_;

        // uniforms and shaders
        void set_general_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr);
        void set_lighting_shader_uniforms(const std::shared_ptr<Shader> &shader_ptr);

        void build_rendering_queues();
        void do_shadow_map_render();
        void do_geometry_path_render();
        void do_render();
        void use_shader(const std::shared_ptr<const scene::physical_objects::IRender>& obj);
        void use_shader(const std::shared_ptr<Shader> &shader_ptr);
        void render_opaque_scene_objects();
        void render_transparent_scene_objects();
        
        void render_scene_object_with_outline(const RenderQueueObject& obj);

        void render_shadow_depth_buffer(const std::shared_ptr<Shader> &shader_ptr);

    public:
        Renderer(std::shared_ptr<const scene::Scene> scene);
        void render_scene(GLsizei framebuffer_width, GLsizei framebuffer_height);
        double get_gpu_time() const;
        double get_cpu_time() const;

        void set_clear_color(const glm::vec4& clear_color_rgba);
        void set_view_mat(const glm::mat4& view_mat);
        void set_projection_mat(const glm::mat4& projection_mat);
        void set_camera_position(const glm::vec3& camera_position);
        void set_internal_objects_id_mask(uint8_t internal_objects_id_mask);

        void set_post_processing(PostProcessingTypes type = PostProcessingTypes::NO_POST_PROCESSING);
        void set_geometry_post_processing(GeometryProcessingTypes type = GeometryProcessingTypes::NO_GEOMETRY_PROCESSING);
        
        void set_gamma(float value);
    };

} // namespace renderer

