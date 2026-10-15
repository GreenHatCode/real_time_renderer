#pragma once 
#include "renderer/frame_buffers/frame_buffer.h"
#include "shader/shader.h"
#include <map>

namespace renderer {

    inline constexpr float MIN_GAMMA = 0.1f;
    inline constexpr float MAX_GAMMA = 5.0f;

    enum class PostProcessingTypes {
        NO_POST_PROCESSING,
        INVERSION,
        GRAYSCALE, 
        BLUR, 
        EDGE_DETECTION,
        GAMMA_CORRECTION, 
        COUNT
    };

    inline const char* const* post_processing_types_names() 
    {
        static const char* names[] = {
            "No post processing",
            "Inversion",
            "Grayscale",
            "Blur",
            "Edge detection",
            "Gamma correction"
        };
        return names;
    }

    class PostProcessor final {
        framebuffers::FrameBuffer framebuffer_;
        std::map<PostProcessingTypes, Shader> post_processing_shaders_;
        GLuint vao_, vbo_;
        float gamma_;

    public:
        PostProcessor(GLsizei width, GLsizei height);
        ~PostProcessor();
        void enable_off_sceen_rendering(GLsizei scr_width, GLsizei scr_height);
        void do_post_process(PostProcessingTypes type); 
        void set_gamma(float value);
    };
} // namespace renderer