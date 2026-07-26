#pragma once
#include <shader/shader.h>
#include <vector>
#include <cstdint>
#include "assimp/material.h"
#include <memory>
#include "assimp_wrapper/textures/texture_2d.h"

namespace assimp_wrapper {

    class Material final {
        std::vector<std::shared_ptr<Texture2D>> textures_; // all textures, use by this material

    public:
        Material(const std::vector<std::shared_ptr<Texture2D>>& textures);
        void bind(Shader &shader) const;
    };
    
}

