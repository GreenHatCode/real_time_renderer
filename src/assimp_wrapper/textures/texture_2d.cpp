#include "assimp_wrapper/textures/texture_2d.h"
#include "opengl.h"

using namespace assimp_wrapper;

Texture2D::Texture2D(aiTextureType type, bool fallback_used)
    : type_(type), fallback_used_(fallback_used)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &id_);
}

Texture2D::Texture2D(Texture2D &&rhs) noexcept
    :type_(rhs.type_)
{
    id_ = rhs.id_;
    rhs.id_ = 0;
}

Texture2D &Texture2D::operator=(Texture2D &&rhs) noexcept
{
    if (this == &rhs) return *this;

    if (id_) glDeleteTextures(1, &id_);
    id_ = rhs.id_;
    rhs.id_ = 0;

    type_ = rhs.type_;

    return *this;
}

Texture2D Texture2D::create_solid_color(
    aiTextureType type, 
    uint8_t r, 
    uint8_t g, 
    uint8_t b, 
    uint8_t a,
    bool fallback_used
)
{
    Texture2D texture(type, fallback_used);
    uint8_t pixel_data[4] = { r, g, b, a }; // 1x1 pixel data array

    glTextureStorage2D(texture.id(), 1, GL_RGBA8, 1, 1);
    glTextureSubImage2D(texture.id(), 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data);
    
    glTextureParameteri(texture.id(), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture.id(), GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture.id(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture.id(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

Texture2D assimp_wrapper::Texture2D::create_noise_texture(aiTextureType type, GLsizei width, GLsizei height, void *data, bool fallback_used)
{
    Texture2D texture(type, fallback_used);

    glTextureStorage2D(texture.id(), 1, GL_RGBA32F, width, height);
    if (data != nullptr) {
        glTextureSubImage2D(texture.id(), 0, 0, 0, width, height, GL_RGB, GL_FLOAT, data);
    }
    
    glTextureParameteri(texture.id(), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture.id(), GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture.id(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture.id(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texture;
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture2D::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t Texture2D::id() const
{
    return id_;
}

bool assimp_wrapper::Texture2D::is_fallback_used() const
{
    return fallback_used_;
}

aiTextureType Texture2D::type() const
{
    return type_;
}

Texture2D::~Texture2D()
{
    if (id_) glDeleteTextures(1, &id_);
}
