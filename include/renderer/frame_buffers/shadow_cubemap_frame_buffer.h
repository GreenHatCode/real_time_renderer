#pragma once
#include "renderer/frame_buffers/i_shadow_frame_buffer.h"
#include "glm/glm.hpp"

namespace renderer
{
    namespace framebuffers
    {
        class ShadowCubemapFrameBuffer final: public IShadowFrameBuffer {
            GLuint fbo_;
            GLuint cube_maps_array_;

            GLsizei width_;
            GLsizei height_;
            GLsizei mip_levels_;
            size_t layers_; // number of cubemaps that will be stored

        public:
            inline static constexpr GLsizei CUBEMAP_LAYERS_MULTIPLICATOR = 6;

            ShadowCubemapFrameBuffer(GLsizei width, GLsizei height, size_t layers = DEFAULT_SHADOW_MAP_LAYERS_NUMBER);
            ShadowCubemapFrameBuffer(const ShadowCubemapFrameBuffer& rhs) = delete;
            ShadowCubemapFrameBuffer(ShadowCubemapFrameBuffer&& rhs);
            ShadowCubemapFrameBuffer& operator=(const ShadowCubemapFrameBuffer& rhs) = delete;
            ShadowCubemapFrameBuffer& operator=(ShadowCubemapFrameBuffer&& rhs);

           
           
            GLsizei get_depth_map() override;
            void set_layers_count(GLsizei layers) override;
            void bind(GLsizei layer = -1) const override;
            void unbind() const override;
            GLsizei get_width() const override;
            GLsizei get_height() const override;
            size_t get_layers_count() const override;
            ~ShadowCubemapFrameBuffer();

        };
    }
} // namespace renderer
