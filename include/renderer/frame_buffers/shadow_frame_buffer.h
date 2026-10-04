#pragma once
#include "renderer/frame_buffers/i_shadow_frame_buffer.h"
#include "glm/glm.hpp"

namespace renderer
{
    namespace framebuffers
    {
        class ShadowFrameBuffer final: public IShadowFrameBuffer {
            GLuint fbo_;
            GLuint depth_map_;

            GLsizei width_;
            GLsizei height_;
            size_t layers_; // number of 2D textures that will be stored

            static constexpr float border_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

        public:
            ShadowFrameBuffer(GLsizei width, GLsizei height, size_t layers = DEFAULT_SHADOW_MAP_LAYERS_NUMBER);
           
            ShadowFrameBuffer(const ShadowFrameBuffer& rhs) = delete;
            ShadowFrameBuffer(ShadowFrameBuffer&& rhs);
            ShadowFrameBuffer& operator=(const ShadowFrameBuffer& rhs) = delete;
            ShadowFrameBuffer& operator=(ShadowFrameBuffer&& rhs);

           
            GLsizei get_depth_map() override;
            void set_layers_count(GLsizei layers) override;
            void bind(GLsizei layer = -1) const override;
            void unbind() const override;
            GLsizei get_width() const override;
            GLsizei get_height() const override;
            size_t get_layers_count() const override;
            ~ShadowFrameBuffer();

        };
    }
} // namespace renderer
