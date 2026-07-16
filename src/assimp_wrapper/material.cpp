#include "assimp_wrapper/material.h"
#include "spdlog/spdlog.h"

using namespace assimp_wrapper;

Material::Material(const std::vector<std::shared_ptr<texture>>& textures) 
    : textures_(textures)
{ }

void Material::bind(Shader& shader) {

    uint32_t diffuse_sampler_number = 1;
    uint32_t specular_sampler_number = 1;
    uint32_t normal_sampler_number = 1;
    uint32_t height_sampler_number = 1;


    for (size_t i = 0; i < textures_.size(); ++i) {
        std::string texture_number, texture_name;
        switch (textures_[i]->type)
        {
        case aiTextureType::aiTextureType_DIFFUSE:
            texture_name = "texture_diffuse";
            texture_number = std::to_string(++diffuse_sampler_number);
            break;
        case aiTextureType::aiTextureType_SPECULAR:
            texture_name = "texture_specular";
            texture_number = std::to_string(++specular_sampler_number);
            break;
        case aiTextureType::aiTextureType_HEIGHT:
            texture_name = "texture_height";
            texture_number = std::to_string(++normal_sampler_number);
            break;
        case aiTextureType::aiTextureType_AMBIENT:
            texture_name = "texture_ambient";
            texture_number = std::to_string(++height_sampler_number);
            break;
        default:
            spdlog::error("Unknown texture type");
            assert(false);
            break;        
        }

        glActiveTexture(GL_TEXTURE0 + i);
        shader.set_uniform(texture_name + texture_number, static_cast<int>(i));
        glBindTexture(GL_TEXTURE_2D, textures_[i]->id);
    }
}






