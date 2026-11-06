#include "renderer/frame_buffers/g_framebuffer.h"
#include <sstream>
#include "spdlog/spdlog.h"

using namespace renderer::framebuffers;

renderer::framebuffers::GFrameBuffer::GFrameBuffer(GLsizei width, GLsizei height)
    : width_(width), height_(height)
{
    glCreateFramebuffers(1, &fbo_);
    create_buffers();

    unbind();
}

renderer::framebuffers::GFrameBuffer::~GFrameBuffer()
{
    glDeleteFramebuffers(1, &fbo_);
    delete_buffers();
}

void renderer::framebuffers::GFrameBuffer::delete_buffers()
{
    glDeleteTextures(1, &texture_position_);
    glDeleteTextures(1, &texture_normal_);
    glDeleteTextures(1, &texture_albedospec_);
    glDeleteRenderbuffers(1, &rbo_);
}

void renderer::framebuffers::GFrameBuffer::create_buffers()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_position_);
    glTextureStorage2D(texture_position_, 1, GL_RGBA32F, width_, height_);
    glTextureParameteri(texture_position_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture_position_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glCreateTextures(GL_TEXTURE_2D, 1, &texture_normal_);
    glTextureStorage2D(texture_normal_, 1, GL_RGBA32F, width_, height_);
    glTextureParameteri(texture_normal_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture_normal_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glCreateTextures(GL_TEXTURE_2D, 1, &texture_albedospec_);
    glTextureStorage2D(texture_albedospec_, 1, GL_RGBA8, width_, height_);
    glTextureParameteri(texture_albedospec_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture_albedospec_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glCreateRenderbuffers(1, &rbo_);
    glNamedRenderbufferStorage(rbo_, GL_DEPTH24_STENCIL8, width_, height_);


    // attach buffer to framebuffer
    glNamedFramebufferTexture(fbo_, GL_COLOR_ATTACHMENT0, texture_position_, 0);
    glNamedFramebufferTexture(fbo_, GL_COLOR_ATTACHMENT1, texture_normal_, 0);
    glNamedFramebufferTexture(fbo_, GL_COLOR_ATTACHMENT2, texture_albedospec_, 0);

    constexpr GLenum draw_buffers[] = { 
        GL_COLOR_ATTACHMENT0, 
        GL_COLOR_ATTACHMENT1, 
        GL_COLOR_ATTACHMENT2 
    };
    glNamedFramebufferDrawBuffers(fbo_, 3, draw_buffers);

    glNamedFramebufferRenderbuffer(fbo_, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

    check_fbo_status();
}

void renderer::framebuffers::GFrameBuffer::check_fbo_status()
{
	GLenum status = glCheckNamedFramebufferStatus(fbo_, GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::stringstream ss;
		ss << "GFrameBuffer: Framebuffer is not complete! Status: ";

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

void renderer::framebuffers::GFrameBuffer::blit_depth_to_framebuffer(GLsizei width, GLsizei height, GLint framebuffer_id)
{
    if (width_ != width || height_ != height) 
        spdlog::warn("GFrameBuffer: incoming width or height for blitting depth are not equal to internal: width_={}, width={}, height_={}, height={}", width_, width, height_, height);

    glBlitNamedFramebuffer(
        fbo_, framebuffer_id, 
        0, 0, width_, height_, 
        0, 0, width, height, 
        GL_DEPTH_BUFFER_BIT, GL_NEAREST
    );
}

GLuint renderer::framebuffers::GFrameBuffer::get_position_buffer_texture()
{
    return texture_position_;
}

GLuint renderer::framebuffers::GFrameBuffer::get_normal_buffer_texture()
{
    return texture_normal_;
}

GLuint renderer::framebuffers::GFrameBuffer::get_albedospec_buffer_texture()
{
    return texture_albedospec_;
}

void renderer::framebuffers::GFrameBuffer::rescale(GLsizei width, GLsizei height)
{
    if(width_ == width && height_ == height) return;

    width_ = width;
    height_ = height;
    delete_buffers();
    create_buffers();
}

void renderer::framebuffers::GFrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void renderer::framebuffers::GFrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
