#pragma once
#include "opengl.h"


namespace renderer
{
    namespace framebuffers
    {
        class GFrameBuffer final {
        public:
            GFrameBuffer(GLsizei width, GLsizei height);
            GFrameBuffer(const GFrameBuffer& rhs) = delete;
            GFrameBuffer& operator=(const GFrameBuffer& rhs) = delete;
            GFrameBuffer(GFrameBuffer&& rhs) = delete;
            GFrameBuffer& operator=(GFrameBuffer&& rhs) = delete;

            void blit_depth_to_framebuffer(GLsizei width, GLsizei height, GLint framebuffer_id);
            GLuint get_position_buffer_texture();
            GLuint get_normal_buffer_texture();
            GLuint get_albedospec_buffer_texture();
            void rescale(GLsizei width, GLsizei height);
            void bind() const;
            void unbind() const;
            
            ~GFrameBuffer();

        private:
            void delete_buffers();
            void create_buffers();
            void check_fbo_status();

            GLuint fbo_;
            GLuint texture_position_;
            GLuint texture_normal_;
            GLuint texture_albedospec_;
            GLuint rbo_;

            GLsizei width_;
            GLsizei height_;

        };
    } // namespace framebuffers 
} // namespace renderer
