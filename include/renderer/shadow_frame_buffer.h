#pragma once
#include "opengl.h"
#include "glm/glm.hpp"

namespace renderer
{
    class ShadowFrameBuffer {
        GLuint fbo_;
        GLuint depth_map_;
        GLuint rbo_;

        GLsizei width_;
        GLsizei height_;

    public:
        ShadowFrameBuffer(GLsizei width, GLsizei height);
        GLsizei get_depth_map();
        void rescale(GLsizei width, GLsizei height);
        void bind() const;
        void unbind() const;
        ~ShadowFrameBuffer();

    };
} // namespace renderer
