#include "renderer/frame_buffers/shadow_cubemap_frame_buffer.h"
#include <sstream>
#include "spdlog/spdlog.h"
// todo: rewrite as shadow_frame_buffer
renderer::framebuffers::ShadowCubemapFrameBuffer::ShadowCubemapFrameBuffer(GLsizei width, GLsizei height, size_t layers)
    : width_(width), height_(height), layers_(layers), mip_levels_(1 + std::floor(std::log2(std::max(width, height))))
{
    glCreateTextures(GL_TEXTURE_CUBE_MAP_ARRAY, 1, &cube_maps_array_);
    glTextureStorage3D(cube_maps_array_, 1, GL_DEPTH_COMPONENT24, width_, height_, layers_ * CUBEMAP_LAYERS_MULTIPLICATOR);

    glTextureParameteri(cube_maps_array_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(cube_maps_array_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(cube_maps_array_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(cube_maps_array_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(cube_maps_array_, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 

    glCreateFramebuffers(1, &fbo_);
	glNamedFramebufferTexture(fbo_, GL_DEPTH_ATTACHMENT, cube_maps_array_, 0);
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
			spdlog::error("Unknown glCheckNamedFramebufferStatus");
			assert(false);
			break;
		}

		throw std::runtime_error(ss.str());
	}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

GLsizei renderer::framebuffers::ShadowCubemapFrameBuffer::get_depth_map()
{
    return cube_maps_array_;
}

void renderer::framebuffers::ShadowCubemapFrameBuffer::set_layers_count(GLsizei layers)
{
    if (layers_ >= layers) return;
    layers_ = layers;

    glDeleteTextures(1, &cube_maps_array_);

    glCreateTextures(GL_TEXTURE_CUBE_MAP_ARRAY, 1, &cube_maps_array_);
    glTextureStorage3D(cube_maps_array_, 1, GL_DEPTH_COMPONENT24, width_, height_, layers_ * CUBEMAP_LAYERS_MULTIPLICATOR);

    glTextureParameteri(cube_maps_array_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(cube_maps_array_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(cube_maps_array_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(cube_maps_array_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(cube_maps_array_, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 

	glNamedFramebufferTexture(fbo_, GL_DEPTH_ATTACHMENT, cube_maps_array_, 0);
}

void renderer::framebuffers::ShadowCubemapFrameBuffer::bind(GLsizei layer) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    if (layer >= 0) glNamedFramebufferTextureLayer(fbo_, GL_DEPTH_ATTACHMENT, cube_maps_array_, 0, layer * CUBEMAP_LAYERS_MULTIPLICATOR);
}

void renderer::framebuffers::ShadowCubemapFrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLsizei renderer::framebuffers::ShadowCubemapFrameBuffer::get_width() const
{
    return width_;
}

GLsizei renderer::framebuffers::ShadowCubemapFrameBuffer::get_height() const
{
    return height_;
}

size_t renderer::framebuffers::ShadowCubemapFrameBuffer::get_layers_count() const
{
    return layers_;
}

renderer::framebuffers::ShadowCubemapFrameBuffer::~ShadowCubemapFrameBuffer()
{
    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(1, &cube_maps_array_);
}
