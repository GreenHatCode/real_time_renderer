#pragma once
#include <shader/shader.h>
#include <vector>
#include <cstdint>
#include "assimp/material.h"
#include <memory>
#include "assimp_wrapper/textures/texture_2d.h"

namespace assimp_wrapper {

    class Material final {
        glm::vec4 color_;
        float opacity_;
        std::vector<std::shared_ptr<Texture2D>> textures_; // all textures, use by this material

    public:
        Material(const glm::vec4& color, float opacity, const std::vector<std::shared_ptr<Texture2D>>& textures);
        float get_opacity() const;
        void bind(Shader &shader) const;
    };
    
}

