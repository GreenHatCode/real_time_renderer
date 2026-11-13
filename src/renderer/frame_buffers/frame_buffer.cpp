#include "renderer/frame_buffers/frame_buffer.h"
#include "spdlog/spdlog.h"
#include <exception>
#include <sstream>

using namespace renderer::framebuffers;

void FrameBuffer::delete_buffers()
{
	if (texture_) glDeleteTextures(1, &texture_);
	if (rbo_) glDeleteRenderbuffers(1, &rbo_);
}

void FrameBuffer::create_buffers()
{
	glCreateTextures(GL_TEXTURE_2D, 1, &texture_);
	glTextureStorage2D(texture_, 1, config_.color_internal_format, config_.width, config_.height);
	glTextureParameteri(texture_, GL_TEXTURE_MIN_FILTER, config_.min_filter);
	glTextureParameteri(texture_, GL_TEXTURE_MAG_FILTER, config_.mag_filter);
	glNamedFramebufferTexture(fbo_, GL_COLOR_ATTACHMENT0, texture_, 0);

	if (config_.has_depth_stencil) {	
		glCreateRenderbuffers(1, &rbo_);
		glNamedRenderbufferStorage(rbo_, GL_DEPTH24_STENCIL8, config_.width, config_.height);
		glNamedFramebufferRenderbuffer(fbo_, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);
	}

	check_fbo_status();
}

void FrameBuffer::check_fbo_status()
{
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
}

FrameBuffer::FrameBuffer(const framebuffer_config& config)
    : config_(config)
{
	glCreateFramebuffers(1, &fbo_);
	create_buffers();
}

FrameBuffer::FrameBuffer(FrameBuffer &&rhs)
	: fbo_(rhs.fbo_)
	, texture_(rhs.texture_)
	, rbo_(rhs.rbo_)
	, config_(rhs.config_)
{
	rhs.fbo_ = 0;
	rhs.texture_ = 0;
	rhs.rbo_ = 0;
}

FrameBuffer &FrameBuffer::operator=(FrameBuffer &&rhs)
{
    if (this == &rhs) return *this;

	std::swap(fbo_, rhs.fbo_);
	std::swap(texture_, rhs.texture_);
	std::swap(rbo_, rhs.rbo_);
	config_ = rhs.config_;
	return *this;
}

FrameBuffer renderer::framebuffers::FrameBuffer::create_post_processing(GLsizei width, GLsizei height)
{
	framebuffer_config config;
    config.width = width;
    config.height = height;
    config.color_internal_format = GL_RGBA16F;
    config.min_filter = GL_LINEAR;
    config.mag_filter = GL_LINEAR;
    config.has_depth_stencil = true; 
    return FrameBuffer(config);
}

FrameBuffer renderer::framebuffers::FrameBuffer::create_ssao(GLsizei width, GLsizei height)
{
	framebuffer_config config;
    config.width = width;
    config.height = height;
    config.color_internal_format = GL_R8;
    config.min_filter = GL_NEAREST;
    config.mag_filter = GL_NEAREST;
    config.has_depth_stencil = false; 
    return FrameBuffer(config);
}

FrameBuffer renderer::framebuffers::FrameBuffer::create_ssao_blur(GLsizei width, GLsizei height)
{
	framebuffer_config config;
    config.width = width;
    config.height = height;
    config.color_internal_format = GL_R8;
    config.min_filter = GL_NEAREST;
    config.mag_filter = GL_NEAREST;
    config.has_depth_stencil = false; 
    return FrameBuffer(config);
}

FrameBuffer::~FrameBuffer()
{
	if (fbo_) {
		glDeleteFramebuffers(1, &fbo_);
		delete_buffers();
	}
}

GLuint FrameBuffer::get_frame_texture()
{
    return texture_;
}

void FrameBuffer::rescale(GLsizei width, GLsizei height)
{
	if (config_.width == width && config_.height == height) return;

	config_.width = width;
	config_.height = height;
	
	delete_buffers();
	create_buffers();
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
