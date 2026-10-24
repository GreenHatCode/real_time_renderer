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
            case aiTextureType_DISPLACEMENT: return "aiTextureType_DISPLACEMENT";
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
        const aiScene *scene_;
    
    public:
        TexturesManager(const std::filesystem::path &model_base_dir, const aiScene *scene);
        std::shared_ptr<Texture2D> load_material_texture(aiMaterial *material, const aiTextureType type);

    private:
        std::shared_ptr<Texture2D> load_embedded_texture(const aiTexture *texture_ptr, const aiTextureType type);
        std::shared_ptr<Texture2D> load_file_texture(const std::filesystem::path file, const aiTextureType type);

        bool starts_with(const std::string& str, const std::string& prefix);
        std::filesystem::path make_texture_path(const std::filesystem::path& directory, aiString const& texture_file);


    };

} // namespace assimp_wrapper