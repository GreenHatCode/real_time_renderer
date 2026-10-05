#pragma once
#include "opengl.h"
#include <glm/glm.hpp>

namespace renderer
{
    namespace framebuffers
    {
        class FrameBuffer final {
        public:
            FrameBuffer(GLsizei width, GLsizei height);
            FrameBuffer(const FrameBuffer& rhs) = delete;
            FrameBuffer& operator=(const FrameBuffer& rhs) = delete;
            FrameBuffer(FrameBuffer&& rhs) = delete;
            FrameBuffer& operator=(FrameBuffer&& rhs) = delete;

            GLuint get_frame_texture();
            void rescale(GLsizei width, GLsizei height);
            void bind() const;
            void unbind() const;
            
            ~FrameBuffer();

        private:
            GLuint fbo_;
            GLuint texture_;
            GLuint rbo_;

            GLsizei width_;
            GLsizei height_;

        };
    } // namespace framebuffers 
} // namespace renderer