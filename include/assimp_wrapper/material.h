#pragma once
#include <shader/shader.h>
#include <vector>
#include <cstdint>
#include "assimp/material.h"
#include <memory>

namespace assimp_wrapper {

    struct texture {
        int32_t id;
        aiTextureType type;
    };

    class Material final {
        std::vector<std::shared_ptr<texture>> textures_; // all textures, use by this material

    public:
        Material(const std::vector<std::shared_ptr<texture>>& textures);
        void bind(Shader &shader);
    };
    
}

