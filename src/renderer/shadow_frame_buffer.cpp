#include "renderer/shadow_frame_buffer.h"
#include <exception>
#include "spdlog/spdlog.h"
#include <sstream>

renderer::ShadowFrameBuffer::ShadowFrameBuffer(GLsizei width, GLsizei height)
    : width_(width), height_(height)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &depth_map_);
	glTextureStorage2D(depth_map_, 1, GL_DEPTH_COMPONENT24, width_, height_);

    glTexParameteri(depth_map_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(depth_map_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
	glTexParameteri(depth_map_, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(depth_map_, GL_TEXTURE_WRAP_T, GL_REPEAT); 




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

void renderer::ShadowFrameBuffer::rescale(GLsizei width, GLsizei height)
{
    if (width_ == width && height_ == height) return;

	width_ = width;
	height_ = height;

	glDeleteTextures(1, &depth_map_);
	
	glCreateTextures(GL_TEXTURE_2D, 1, &depth_map_);
	glTextureStorage2D(depth_map_, 1, GL_DEPTH_COMPONENT24, width_, height_);

    glTexParameteri(depth_map_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(depth_map_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
	glTexParameteri(depth_map_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(depth_map_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); 
	constexpr std::array<float, 4> border_color = {1.0f, 1.0f, 1.0f, 1.0f};
    glTextureParameterfv(depth_map_, GL_TEXTURE_BORDER_COLOR, border_color.data());

	glNamedFramebufferTexture(fbo_, GL_DEPTH_ATTACHMENT, depth_map_, 0);
}

void renderer::ShadowFrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void renderer::ShadowFrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

renderer::ShadowFrameBuffer::~ShadowFrameBuffer()
{
    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(1, &depth_map_);
}
