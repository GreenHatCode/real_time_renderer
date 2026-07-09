#include "frame_buffer.h"

FrameBuffer::FrameBuffer(float width, float height)
{
	glGenFramebuffers(1, &fbo_);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_, 0);

	glGenRenderbuffers(1, &rbo_);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);  
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &fbo_);
	glDeleteTextures(1, &texture_);
	glDeleteRenderbuffers(1, &rbo_);
}

GLuint FrameBuffer::get_frame_texture()
{
    return texture_;
}

void FrameBuffer::rescale(float width, float height)
{
	glBindTexture(GL_TEXTURE_2D, texture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
