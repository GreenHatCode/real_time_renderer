#pragma once
#include <shader/shader.h>
#include <vector>
#include <cstdint>
#include "assimp/material.h"
#include <memory>

namespace assimp_wrapper {

    enum class TextureType
    {
        TEXTURE_DIFFUSE,
        TEXTURE_SPECULAR,
        TEXTURE_NORMAL,
        TEXTURE_HEIGHT
    };

    struct texture {
        int32_t id;
        TextureType type;
    };

    class Material final {
        std::vector<std::shared_ptr<texture>> textures_; // all textures, use by this material

    public:
        Material(const std::vector<std::shared_ptr<texture>>& textures);
        void bind(Shader &shader);
    };
    
}

