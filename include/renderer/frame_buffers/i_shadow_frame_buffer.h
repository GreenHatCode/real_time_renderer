#pragma once
#include "opengl.h"

namespace renderer
{
    namespace framebuffers
    {
        inline static constexpr GLsizei DEFAULT_SHADOW_MAP_HEIGHT = 1024;
        inline static constexpr GLsizei DEFAULT_SHADOW_MAP_WIDTH = 1024;
        inline static constexpr GLsizei DEFAULT_SHADOW_MAP_LAYERS_NUMBER = 1;

        class IShadowFrameBuffer {
        public:
            virtual GLsizei get_depth_map() = 0;
            virtual void set_layers_count(GLsizei size) = 0;
            virtual void bind(GLsizei layer = -1) const = 0;
            virtual void unbind() const = 0;
            virtual GLsizei get_width() const = 0;
            virtual GLsizei get_height() const = 0;
            virtual size_t get_layers_count() const = 0;
            virtual ~IShadowFrameBuffer() = default;
        };
    } // namespace framebuffers
}
