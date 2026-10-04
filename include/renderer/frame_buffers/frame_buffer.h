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
            ~FrameBuffer();
            GLuint get_frame_texture();
            void rescale(GLsizei width, GLsizei height);
            void bind() const;
            void unbind() const;

        private:
            GLuint fbo_;
            GLuint texture_;
            GLuint rbo_;

            GLsizei width_;
            GLsizei height_;

        };
    } // namespace framebuffers 
} // namespace renderer