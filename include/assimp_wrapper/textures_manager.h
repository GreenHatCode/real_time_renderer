#pragma once
#include <unordered_map>
#include <filesystem>
#include <vector>
#include "assimp_wrapper/material.h"


namespace assimp_wrapper {

    class TexturesManager final {
        std::unordered_map<std::filesystem::path, std::shared_ptr<texture>> textures_; // stores all textures, used in model
        std::filesystem::path model_base_dir_;
    
    public:
        using textures_vector = std::vector<std::shared_ptr<texture>>;
        TexturesManager(std::filesystem::path &model_base_dir);
        textures_vector load_material_textures(aiMaterial *material, aiTextureType type, const aiScene *scene);
        };

    private:
        textures_vector load_embedded_textures();
        textures_vector load_file_textures();

} // namespace assimp_wrapper