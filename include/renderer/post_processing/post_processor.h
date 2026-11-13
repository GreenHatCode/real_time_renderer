#pragma once 
#include "renderer/frame_buffers/frame_buffer.h"
#include "shader/shader.h"
#include <map>
#include "renderer/post_processing/common.h"

namespace renderer {
    namespace post_processing {

        class PostProcessor final {
            framebuffers::FrameBuffer framebuffer_;
            std::map<PostProcessingTypes, Shader> post_processing_shaders_;
            float gamma_;

        public:
            PostProcessor();
            void enable_off_sceen_rendering(GLsizei scr_width, GLsizei scr_height);
            void do_post_process(PostProcessingTypes type); 
            void set_gamma(float value);
        };

    } // namespace post_processing
} // namespace renderer