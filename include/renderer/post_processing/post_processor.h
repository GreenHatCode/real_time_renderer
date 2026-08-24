#pragma once 
#include "renderer/frame_buffer.h"
#include "shader/shader.h"
#include <map>

namespace renderer {

    enum class PostProcessingTypes {
        NO_POST_PROCESSING,
        INVERSION,
        GRAYSCALE, 
        BLUR, 
        EDGE_DETECTION
    };

    class PostProcessor final {
        static constexpr std::string_view INVERSION_VERTEX_SHADER = "shaders_sources/post_processing/inversion/vertex_shader.vert";
        static constexpr std::string_view INVERSION_FRAGMENT_SHADER = "shaders_sources/post_processing/inversion/fragment_shader.frag";

        FrameBuffer framebuffer_;
        std::map<PostProcessingTypes, Shader> post_processing_shaders_;

        GLuint vao_, vbo_;

    public:
        PostProcessor(GLsizei width, GLsizei height);
        ~PostProcessor();
        void enable_off_sceen_rendering(GLsizei scr_width, GLsizei scr_height);
        void do_post_process(PostProcessingTypes type);

        
    };
} // namespace renderer