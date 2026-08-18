#include "assimp_wrapper/material.h"
#include "spdlog/spdlog.h"

using namespace assimp_wrapper;

Material::Material(const glm::vec4& color, float opacity, const std::vector<std::shared_ptr<Texture2D>>& textures) 
    : color_(color), opacity_(opacity), textures_(textures)
{ }

void Material::bind(Shader& shader) const{

    uint32_t diffuse_sampler_idx = 0;
    uint32_t specular_sampler_idx = 0;
    uint32_t normal_sampler_idx = 0;
    uint32_t height_sampler_idx = 0;

    // how to load and consume arbitrary amount of textures of different types

    for (size_t i = 0; i < textures_.size(); ++i) {
        std::string uniform_name;
        switch (textures_[i]->type())
        {
        case aiTextureType::aiTextureType_DIFFUSE:
            uniform_name = "texture_diffuse[" + std::to_string(diffuse_sampler_idx++) + "]";
            break;
        case aiTextureType::aiTextureType_SPECULAR:
            uniform_name = "texture_specular[" + std::to_string(specular_sampler_idx++) + "]";
            break;
        case aiTextureType::aiTextureType_HEIGHT:
            uniform_name = "texture_height[" + std::to_string(normal_sampler_idx++) + "]";
            break;
        case aiTextureType::aiTextureType_AMBIENT:
            uniform_name = "texture_ambient[" + std::to_string(height_sampler_idx++) + "]";
            break;
        default:
            spdlog::error("Unknown texture type");
            assert(false);
            break;        
        }

        glActiveTexture(GL_TEXTURE0 + i);
        shader.set_uniform(uniform_name, static_cast<int>(i));
        glBindTexture(GL_TEXTURE_2D, textures_[i]->id());
    }

    shader.set_uniform("diffuse_count", diffuse_sampler_idx+1);
    shader.set_uniform("specular_count", specular_sampler_idx+1);
    shader.set_uniform("height_count", normal_sampler_idx+1);
    shader.set_uniform("ambient_count", height_sampler_idx+1);

    shader.set_uniform("u_base_color", color_);
    shader.set_uniform("u_opacity", opacity_);
}






