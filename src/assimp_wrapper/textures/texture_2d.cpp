#include "assimp_wrapper/textures/texture_2d.h"
#include "opengl.h"

using namespace assimp_wrapper;

Texture2D::Texture2D(aiTextureType type)
    : type_(type)
{
    glGenTextures(1, &id_);
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

    glDeleteTextures(1, &id_);
    id_ = rhs.id_;
    rhs.id_ = 0;

    type_ = rhs.type_;

    return *this;
}

std::shared_ptr<Texture2D> Texture2D::create_solid_color(
    aiTextureType type, 
    uint8_t r, 
    uint8_t g, 
    uint8_t b, 
    uint8_t a
)
{
    std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(type);
    texture->bind();

    // 1x1 pixel data array
    uint8_t pixel_data[4] = { r, g, b, a };

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture->unbind();
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

aiTextureType Texture2D::type() const
{
    return type_;
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &id_);
}
