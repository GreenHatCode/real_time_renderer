#include "assimp_wrapper/textures/texture_2d.h"
#include "glad/glad.h"

assimp_wrapper::Texture2D::Texture2D(aiTextureType type)
    : type_(type)
{
    glGenTextures(1, &id_);
}

assimp_wrapper::Texture2D::Texture2D(Texture2D &&rhs) noexcept
    :type_(rhs.type_)
{
    id_ = rhs.id_;
    rhs.id_ = 0;
}

assimp_wrapper::Texture2D &assimp_wrapper::Texture2D::operator=(Texture2D &&rhs) noexcept
{
    if (this == &rhs) return *this;

    glDeleteTextures(1, &id_);
    id_ = rhs.id_;
    rhs.id_ = 0;

    type_ = rhs.type_;

    return *this;
}

void assimp_wrapper::Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id_);
}

uint32_t assimp_wrapper::Texture2D::id() const
{
    return id_;
}

aiTextureType assimp_wrapper::Texture2D::type() const
{
    return type_;
}

assimp_wrapper::Texture2D::~Texture2D()
{
    glDeleteTextures(1, &id_);
}
