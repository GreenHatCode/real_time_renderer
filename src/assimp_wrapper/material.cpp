#include "assimp_wrapper/material.h"

using namespace assimp_wrapper;

assimp_wrapper::Material::Material(
    const glm::vec4 &color
    , float opacity
    , std::shared_ptr<Texture2D> ambient_texture
    , std::shared_ptr<Texture2D> diffuse_texture
    , std::shared_ptr<Texture2D> specular_texture
    , std::shared_ptr<Texture2D> height_texture
)
    : color_(color)
    , opacity_(opacity)
    , ambient_texture_(ambient_texture)
    , diffuse_texture_(diffuse_texture)
    , specular_texture_(specular_texture)
    , height_texture_(height_texture)
{
}

float assimp_wrapper::Material::get_opacity() const
{
    return opacity_;
}

void Material::bind(Shader& shader) const {

    shader.set_uniform("u_base_color", color_);
    shader.set_uniform("u_opacity", opacity_);

    shader.set_uniform("texture_ambient", 5);
    shader.set_uniform("texture_diffuse", 6);
    shader.set_uniform("texture_specular", 7);
    shader.set_uniform("texture_height", 8);

    if (ambient_texture_) glBindTextureUnit(5, ambient_texture_->id());
    if (diffuse_texture_) glBindTextureUnit(6, diffuse_texture_->id());
    if (specular_texture_) glBindTextureUnit(7, specular_texture_->id());
    if (height_texture_) glBindTextureUnit(8, height_texture_->id());

}






