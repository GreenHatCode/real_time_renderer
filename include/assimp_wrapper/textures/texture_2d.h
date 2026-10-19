#pragma once
#include <cstdint>
#include "assimp/material.h"
#include <memory>

namespace assimp_wrapper{
    class Texture2D {
        uint32_t id_;
        aiTextureType type_;

    public:
        Texture2D(aiTextureType type);
        Texture2D(const Texture2D& rhs) = delete;
        Texture2D(Texture2D&& rhs) noexcept;
        Texture2D& operator=(const Texture2D& rhs) = delete;
        Texture2D& operator=(Texture2D&& rhs) noexcept;

        static std::shared_ptr<Texture2D> create_solid_color(
            aiTextureType type, 
            uint8_t r, 
            uint8_t g, 
            uint8_t b, 
            uint8_t a = 255
        );

        void bind() const;
        void unbind() const;
        uint32_t id() const;
        aiTextureType type() const;
        ~Texture2D();
    };
} // namespace assimp_wrapper