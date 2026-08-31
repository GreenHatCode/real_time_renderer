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
        EDGE_DETECTION, 
        COUNT
    };

    inline const char* const* post_processing_types_names() 
    {
        static const char* names[] = {
            "No post processing",
            "Inversion",
            "Grayscale",
            "Blur",
            "Edge detection"
        };
        return names;
    }

    class PostProcessor final {
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