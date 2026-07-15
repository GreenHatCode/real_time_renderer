#include "assimp_wrapper/material.h"

using namespace assimp_wrapper;

Material::Material() {

}

void Material::bind(Shader& shader) {
    // uint32_t diffuse_sampler_number = 1;
    // uint32_t specular_sampler_number = 1;
    // for (size_t i = 0; i < textures_.size(); ++i) {
    //     std::string texture_number;
    //     std::string name = textures_[i].type;
    //     if (name == "texture_diffuse") name += std::to_string(diffuse_sampler_number++);
    //     else if (name == "texture_specular") name += std::to_string(specular_sampler_number++);

    //     glActiveTexture(GL_TEXTURE0 + i);
    //     shader_program.set_uniform("material." + name, static_cast<int>(i));
    //     glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    // }
}






