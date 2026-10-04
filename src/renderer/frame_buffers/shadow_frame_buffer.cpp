#include "renderer/frame_buffers/shadow_frame_buffer.h"
#include <exception>
#include "spdlog/spdlog.h"
#include <sstream>

renderer::framebuffers::ShadowFrameBuffer::ShadowFrameBuffer(GLsizei width, GLsizei height, size_t layers)
    : width_(width), height_(height), layers_(layers)
{
    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &depth_map_);
	glTextureStorage3D(depth_map_, 1, GL_DEPTH_COMPONENT24, width_, height_, layers_);

    glTextureParameteri(depth_map_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(depth_map_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(depth_map_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTextureParameteri(depth_map_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTextureParameterfv(depth_map_, GL_TEXTURE_BORDER_COLOR, border_color);

    glCreateFramebuffers(1, &fbo_);
	glNamedFramebufferTexture(fbo_, GL_DEPTH_ATTACHMENT, depth_map_, 0);
	glNamedFramebufferDrawBuffer(fbo_, GL_NONE);
	glNamedFramebufferReadBuffer(fbo_, GL_NONE);

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
			spdlog::error("Unknown glCheckFramebufferStatus");
			assert(false);
			break;
		}

		throw std::runtime_error(ss.str());
	}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

renderer::framebuffers::ShadowFrameBuffer::ShadowFrameBuffer(ShadowFrameBuffer &&rhs)
    : fbo_(rhs.fbo_)
	, depth_map_(rhs.depth_map_)
	, width_(rhs.width_)
	, height_(rhs.height_)
	, layers_(rhs.layers_)
{
	rhs.fbo_ = 0;
	rhs.depth_map_ = 0;
}

renderer::framebuffers::ShadowFrameBuffer &renderer::framebuffers::ShadowFrameBuffer::operator=(ShadowFrameBuffer &&rhs)
{
    if (this == &rhs) return *this;

	std::swap(fbo_, rhs.fbo_);
	std::swap(depth_map_, rhs.depth_map_);
	width_ = rhs.width_;
	height_ = rhs.height_;
	layers_ = rhs.layers_;
	return *this;
}

GLsizei renderer::framebuffers::ShadowFrameBuffer::get_depth_map()
{
    return depth_map_;
}

void renderer::framebuffers::ShadowFrameBuffer::set_layers_count(GLsizei layers)
{
    if (layers_ >= layers) return;
	layers_ = layers;

	glDeleteTextures(1, &depth_map_);
	
	glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &depth_map_);
	glTextureStorage3D(depth_map_, 1, GL_DEPTH_COMPONENT24, width_, height_, layers_);

    glTextureParameteri(depth_map_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(depth_map_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(depth_map_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTextureParameteri(depth_map_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTextureParameterfv(depth_map_, GL_TEXTURE_BORDER_COLOR, border_color);

	glNamedFramebufferTexture(fbo_, GL_DEPTH_ATTACHMENT, depth_map_, 0);
}

void renderer::framebuffers::ShadowFrameBuffer::bind(GLsizei layer) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	if (layer >= 0) glNamedFramebufferTextureLayer(fbo_, GL_DEPTH_ATTACHMENT, depth_map_, 0, layer);
}

void renderer::framebuffers::ShadowFrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLsizei renderer::framebuffers::ShadowFrameBuffer::get_width() const
{
	return width_;
}

GLsizei renderer::framebuffers::ShadowFrameBuffer::get_height() const 
{
    return height_;
}

size_t renderer::framebuffers::ShadowFrameBuffer::get_layers_count() const
{
    return layers_;
}

renderer::framebuffers::ShadowFrameBuffer::~ShadowFrameBuffer()
{
    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(1, &depth_map_);
}
