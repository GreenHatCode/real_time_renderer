#pragma once
#include <cstdint>
#include "assimp/material.h"


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

        void bind() const;
        uint32_t id() const;
        aiTextureType type() const;
        ~Texture2D();
    };
} // namespace assimp_wrapper