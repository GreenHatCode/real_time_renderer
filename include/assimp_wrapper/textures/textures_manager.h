#pragma once
#include <unordered_map>
#include <filesystem>
#include <vector>
#include <span>
#include "assimp_wrapper/material.h"
#include "spdlog/spdlog.h"
#include "assimp/scene.h"
#include "assimp/material.h"
#include "assimp_wrapper/textures/texture_2d.h"

namespace assimp_wrapper {

    namespace helpers {
        inline std::string_view texture_type_name(aiTextureType type) {
            switch (type)
            {
            case aiTextureType_DIFFUSE: return "aiTextureType_DIFFUSE";
            case aiTextureType_SPECULAR: return "aiTextureType_SPECULAR";
            case aiTextureType_HEIGHT: return "aiTextureType_HEIGHT";
            case aiTextureType_AMBIENT: return "aiTextureType_AMBIENT";
            default:
                spdlog::error("Failed to get texture type name. No such a type.");
                assert(false);
                return "Unknown";
            }
        }
    }
    
    constexpr std::array kMaterialTextureTypes{
        aiTextureType_DIFFUSE,
        aiTextureType_SPECULAR,
        aiTextureType_HEIGHT,
        aiTextureType_AMBIENT
    };

    class TexturesManager final {
        std::unordered_map<std::filesystem::path, std::shared_ptr<Texture2D>> textures_; // stores all textures, used in model
        std::filesystem::path model_base_dir_;
    
    public:
        using textures_vector = std::vector<std::shared_ptr<Texture2D>>;
        TexturesManager(const std::filesystem::path &model_base_dir);
        textures_vector load_material_textures(aiMaterial *material, const aiScene *scene, std::span<const aiTextureType> textures_types);

    private:
        textures_vector load_embedded_textures(const aiScene *scene);
        textures_vector load_file_textures(aiMaterial *material, const aiTextureType type, const aiScene *scene);

        bool starts_with(const std::string& str, const std::string& prefix);
        std::filesystem::path make_texture_path(const std::filesystem::path& directory, aiString const& texture_file);


    };

} // namespace assimp_wrapper