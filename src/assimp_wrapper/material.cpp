#include "assimp_wrapper/material.h"
#include "renderer/constants.h"

using namespace assimp_wrapper;
using namespace renderer;

assimp_wrapper::Material::Material(
    const glm::vec4 &color
    , std::shared_ptr<Texture2D> ambient_texture
    , std::shared_ptr<Texture2D> diffuse_texture
    , std::shared_ptr<Texture2D> specular_texture
    , std::shared_ptr<Texture2D> height_texture
)
    : color_(color)
    , ambient_texture_(ambient_texture)
    , diffuse_texture_(diffuse_texture)
    , specular_texture_(specular_texture)
    , height_texture_(height_texture)
{
}

float assimp_wrapper::Material::get_opacity() const
{
    return color_.a;
}

void Material::bind(Shader& shader) const {

    shader.set_uniform("u_base_color", color_);

    shader.set_uniform("texture_ambient", bind_texture_units::AMBIENT_TEXTURE_TEXTURE_UNIT);
    shader.set_uniform("texture_diffuse", bind_texture_units::DIFFUSE_TEXTURE_TEXTURE_UNIT);
    shader.set_uniform("texture_specular", bind_texture_units::SPECULAR_TEXTURE_TEXTURE_UNIT);
    shader.set_uniform("texture_height", bind_texture_units::HEIGHT_TEXTURE_TEXTURE_UNIT);

    if (ambient_texture_) glBindTextureUnit(bind_texture_units::AMBIENT_TEXTURE_TEXTURE_UNIT, ambient_texture_->id());
    if (diffuse_texture_) glBindTextureUnit(bind_texture_units::DIFFUSE_TEXTURE_TEXTURE_UNIT, diffuse_texture_->id());
    if (specular_texture_) glBindTextureUnit(bind_texture_units::SPECULAR_TEXTURE_TEXTURE_UNIT, specular_texture_->id());
    if (height_texture_) glBindTextureUnit(bind_texture_units::HEIGHT_TEXTURE_TEXTURE_UNIT, height_texture_->id());

}






