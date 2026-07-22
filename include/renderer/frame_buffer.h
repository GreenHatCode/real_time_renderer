#pragma once

#include <iostream>
#include "opengl.h"
#include <glm/glm.hpp>

class FrameBuffer final {
public:
    FrameBuffer(float width, float height);
    ~FrameBuffer();
    GLuint get_frame_texture();
    void rescale(float width, float height);
    void bind() const;
    void unbind() const;

private:
    GLuint fbo_;
    GLuint texture_;
    GLuint rbo_;

};