#pragma once
#include <cstdint>
#include "assimp/material.h"
#include <memory>

namespace assimp_wrapper{
    class Texture2D {
        uint32_t id_;
        aiTextureType type_;
        bool fallback_used_;

    public:
        Texture2D(aiTextureType type, bool fallback_used = false);
        Texture2D(const Texture2D& rhs) = delete;
        Texture2D(Texture2D&& rhs) noexcept;
        Texture2D& operator=(const Texture2D& rhs) = delete;
        Texture2D& operator=(Texture2D&& rhs) noexcept;

        static std::shared_ptr<Texture2D> create_solid_color(
            aiTextureType type, 
            uint8_t r, 
            uint8_t g, 
            uint8_t b, 
            uint8_t a = 255,
            bool fallback_used = false
        );

        void bind() const;
        void unbind() const;
        uint32_t id() const;
        bool is_fallback_used() const;
        aiTextureType type() const;
        ~Texture2D();
    };
} // namespace assimp_wrapper