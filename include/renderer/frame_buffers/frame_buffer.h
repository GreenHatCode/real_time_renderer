#pragma once
#include "opengl.h"
#include <glm/glm.hpp>

namespace renderer
{
    namespace framebuffers
    {
        struct framebuffer_config {
            GLsizei width = 0;
            GLsizei height = 0;
            GLenum color_internal_format = GL_RGBA16F;
            GLenum min_filter = GL_LINEAR;
            GLenum mag_filter = GL_LINEAR;
            bool has_depth_stencil = true;
        };

        class FrameBuffer final {
            GLuint fbo_;
            GLuint texture_;
            GLuint rbo_;

            framebuffer_config config_;

            void delete_buffers();
            void create_buffers();
            void check_fbo_status();

        public:
            static inline GLsizei MIN_FRAMEBUFFER_WIDTH = 1;
            static inline GLsizei MIN_FRAMEBUFFER_HEIGHT = 1;

            FrameBuffer(const framebuffer_config& config);
            FrameBuffer(const FrameBuffer& rhs) = delete;
            FrameBuffer& operator=(const FrameBuffer& rhs) = delete;
            FrameBuffer(FrameBuffer&& rhs);
            FrameBuffer& operator=(FrameBuffer&& rhs);

            static FrameBuffer create_post_processing(GLsizei width = MIN_FRAMEBUFFER_WIDTH, GLsizei height = MIN_FRAMEBUFFER_HEIGHT);
            static FrameBuffer create_ssao(GLsizei width = MIN_FRAMEBUFFER_WIDTH, GLsizei height = MIN_FRAMEBUFFER_HEIGHT);
            static FrameBuffer create_ssao_blur(GLsizei width = MIN_FRAMEBUFFER_WIDTH, GLsizei height = MIN_FRAMEBUFFER_HEIGHT);

            GLuint get_frame_texture();
            void rescale(GLsizei width, GLsizei height);
            void bind() const;
            void unbind() const;
            
            ~FrameBuffer();

        private:

        };
    } // namespace framebuffers 
} // namespace renderer