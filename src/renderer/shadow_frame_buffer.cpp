#include "renderer/shadow_frame_buffer.h"
#include <exception>
#include "spdlog/spdlog.h"
#include <sstream>

renderer::ShadowFrameBuffer::ShadowFrameBuffer(GLsizei width, GLsizei height, size_t size)
    : width_(width), height_(height), size_(size)
{
    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &depth_map_);
	glTextureStorage3D(depth_map_, 1, GL_DEPTH_COMPONENT24, width_, height_, size_);

    glTexParameteri(depth_map_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(depth_map_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(depth_map_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(depth_map_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
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

GLsizei renderer::ShadowFrameBuffer::get_depth_map()
{
    return depth_map_;
}

void renderer::ShadowFrameBuffer::resize(GLsizei size)
{
    if (size_ == size) return;
	size_ = size;

	glDeleteTextures(1, &depth_map_);
	
	glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &depth_map_);
	glTextureStorage3D(depth_map_, 1, GL_DEPTH_COMPONENT24, width_, height_, size_);

    glTexParameteri(depth_map_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(depth_map_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(depth_map_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(depth_map_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTextureParameterfv(depth_map_, GL_TEXTURE_BORDER_COLOR, border_color);

	glNamedFramebufferTexture(fbo_, GL_DEPTH_ATTACHMENT, depth_map_, 0);
}

void renderer::ShadowFrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void renderer::ShadowFrameBuffer::bind_layer(GLsizei layer) const
{
	glNamedFramebufferTextureLayer(fbo_, GL_DEPTH_ATTACHMENT, depth_map_, 0, layer);
}

void renderer::ShadowFrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLsizei renderer::ShadowFrameBuffer::get_width() const
{
	return width_;
}

GLsizei renderer::ShadowFrameBuffer::get_height() const 
{
    return height_;
}

size_t renderer::ShadowFrameBuffer::get_size() const
{
    return size_;
}

renderer::ShadowFrameBuffer::~ShadowFrameBuffer()
{
    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(1, &depth_map_);
}
