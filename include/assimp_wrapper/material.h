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
        
        // textures
        std::shared_ptr<Texture2D> ambient_texture_;
        std::shared_ptr<Texture2D> diffuse_texture_;
        std::shared_ptr<Texture2D> specular_texture_;
        std::shared_ptr<Texture2D> height_texture_;

    public:
        inline static constexpr float OPAQUE_MESH_MATERIAL_OPACITY = 1.0f;

        Material(
            const glm::vec4& color, 
            float opacity,
            std::shared_ptr<Texture2D> ambient_texture,
            std::shared_ptr<Texture2D> diffuse_texture,
            std::shared_ptr<Texture2D> specular_texture,
            std::shared_ptr<Texture2D> height_texture
        );
        float get_opacity() const;
        void bind(Shader &shader) const;
    };
    
}

