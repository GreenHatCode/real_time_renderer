#include "renderer/frame_buffers/frame_buffer.h"
#include "spdlog/spdlog.h"
#include <exception>
#include <sstream>
// todo: to opengl4+
renderer::framebuffers::FrameBuffer::FrameBuffer(GLsizei width, GLsizei height)
	: width_(width), height_(height)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &texture_);
	glTextureStorage2D(texture_, 1, GL_RGB8, width_, height_);
	glTextureParameteri(texture_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texture_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glCreateFramebuffers(1, &fbo_);
	glNamedFramebufferTexture(fbo_, GL_COLOR_ATTACHMENT0, texture_, 0);

	glCreateRenderbuffers(1, &rbo_);
	glNamedRenderbufferStorage(rbo_, GL_DEPTH32F_STENCIL8, width_, height_);
	glNamedFramebufferRenderbuffer(fbo_, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

	GLenum status = glCheckNamedFramebufferStatus(fbo_, GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::stringstream ss;
		ss << "FRAMEBUFFER: Framebuffer is not complete! Status: ";

		switch (status)
		{
		case GL_FRAMEBUFFER_UNDEFINED:
			ss << "GL_FRAMEBUFFER_UNDEFINED";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			ss << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			ss << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			ss << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			ss << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			ss << "GL_FRAMEBUFFER_UNSUPPORTED";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			ss << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			ss << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
			break;
		default:
			spdlog::error("Unknown glCheckNamedFramebufferStatus");
			assert(false);
			break;
		}

		throw std::runtime_error(ss.str());
	}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

renderer::framebuffers::FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &fbo_);
	glDeleteTextures(1, &texture_);
	glDeleteRenderbuffers(1, &rbo_);
}

GLuint renderer::framebuffers::FrameBuffer::get_frame_texture()
{
    return texture_;
}

void renderer::framebuffers::FrameBuffer::rescale(GLsizei width, GLsizei height)
{
	if (width_ == width && height_ == height) return;

	width_ = width;
	height_ = height;
	glDeleteTextures(1, &texture_);
	glDeleteRenderbuffers(1, &rbo_);


	glCreateTextures(GL_TEXTURE_2D, 1, &texture_);
	glTextureStorage2D(texture_, 1, GL_RGB8, width_, height_);
	glTextureParameteri(texture_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texture_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glNamedFramebufferTexture(fbo_, GL_COLOR_ATTACHMENT0, texture_, 0);

	glCreateRenderbuffers(1, &rbo_);
	glNamedRenderbufferStorage(rbo_, GL_DEPTH32F_STENCIL8, width_, height_);
	glNamedFramebufferRenderbuffer(fbo_, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

}

void renderer::framebuffers::FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void renderer::framebuffers::FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
