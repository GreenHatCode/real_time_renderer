#pragma once
#include "opengl.h"
#include "glm/glm.hpp"

namespace renderer
{
    class ShadowFrameBuffer {
        GLuint fbo_;
        GLuint depth_map_;
        GLuint rbo_;

        const GLsizei width_;
        const GLsizei height_;
        size_t size_; // number of 2D textures that will be stored

        static constexpr float border_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    public:

        inline static constexpr GLsizei DEFAULT_SHADOW_MAP_HEIGHT = 1024;
        inline static constexpr GLsizei DEFAULT_SHADOW_MAP_WIDTH = 1024;
        inline static constexpr GLsizei DEFAULT_SHADOW_MAP_LAYERS = 1;

        ShadowFrameBuffer(GLsizei width, GLsizei height, size_t size = DEFAULT_SHADOW_MAP_LAYERS);
        GLsizei get_depth_map();
        void resize(GLsizei size);
        void bind() const;
        void bind_layer(GLsizei layer) const;
        void unbind() const;
        GLsizei get_width() const;
        GLsizei get_height() const;
        size_t get_size() const;
        ~ShadowFrameBuffer();

    };
} // namespace renderer
