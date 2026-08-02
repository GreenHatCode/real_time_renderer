#include "assimp_wrapper/textures/textures_manager.h"
#include <assimp/scene.h>
#include <stb_image.h>

using namespace assimp_wrapper;
using textures_vector = std::vector<std::shared_ptr<Texture2D>>;

assimp_wrapper::TexturesManager::TexturesManager(const std::filesystem::path &model_base_dir)
    : model_base_dir_(model_base_dir)
{
}

textures_vector assimp_wrapper::TexturesManager::load_material_textures(aiMaterial *material, const aiScene *scene, std::span<const aiTextureType> textures_types)
{
    textures_vector material_textures;
    if (scene->HasTextures()) {
        spdlog::info("Loading embedded textures");
        material_textures = load_embedded_textures(scene);
    } 
    else {
        spdlog::info("Loading file textures");
        textures_vector tmp;
        for (auto type: textures_types) {
            tmp = load_file_textures(material, type, scene);
            material_textures.insert(material_textures.end(), tmp.begin(), tmp.end());
        }
    }

    return material_textures;
}

textures_vector assimp_wrapper::TexturesManager::load_embedded_textures(const aiScene *scene)
{
    std::vector<std::shared_ptr<Texture2D>> loaded_textures;
    for (size_t i = 0; i < scene->mNumTextures; ++i) 
    {
        aiTexture* texture_ptr = scene->mTextures[i];
        auto texture_it = textures_.find(texture_ptr->mFilename.C_Str());
        if (texture_it != textures_.end()) { // texture is already loaded, get ptr to it
            loaded_textures.push_back(texture_it->second);
            spdlog::info("Used previously loaded texture: id={}, type={}", texture_it->second->id(), static_cast<int>(texture_it->second->type()));
        } else { // new texture, load it to memory
            auto loaded_texture_ptr = std::make_shared<Texture2D>(aiTextureType::aiTextureType_DIFFUSE);
            loaded_texture_ptr->bind();

            if (texture_ptr->mHeight == 0) {
                // load with stbimage and decode texture format
                spdlog::info("Received uncompressed texture");

                int width, height, components_number;
                uint8_t* data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(texture_ptr->pcData), texture_ptr->mWidth, &width, &height, &components_number, 0);

                GLenum format;
                switch (components_number)
                {
                case 1:
                    format = GL_RED;
                    break;
                case 3:
                    format = GL_RGB;
                break;
                case 4:
                    format = GL_RGBA;
                break;
                default:
                    spdlog::error("Failed to select texture format, read components_number={}", components_number);
                    assert(false);
                    break;
                }

                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                spdlog::info("Loaded uncompressed textures: format={}", static_cast<int>(format));

            } else {
                // raw pixel data
                spdlog::info("Received compressed texture");
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_ptr->mWidth, texture_ptr->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_ptr->pcData);
            }
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (texture_ptr->achFormatHint[0] & 0x01) ? GL_REPEAT : GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (texture_ptr->achFormatHint[0] & 0x01) ? GL_REPEAT : GL_CLAMP);
        
            textures_.emplace(texture_ptr->mFilename.C_Str(), loaded_texture_ptr);
            loaded_textures.push_back(loaded_texture_ptr);
            spdlog::info("Loaded embedded texture: texture_file_path={}",  texture_ptr->mFilename.C_Str());
        }
        
    }   
    glBindTexture(GL_TEXTURE_2D, 0); // return to default state

    return loaded_textures;
}

textures_vector assimp_wrapper::TexturesManager::load_file_textures(aiMaterial *material, const aiTextureType type, const aiScene *scene)
{
    // allocate memory for all material textures
    // push every new textures to model 
    // return verctor of textures, used by this material

    std::vector<std::shared_ptr<Texture2D>> loaded_textures;
    for (size_t i = 0; i < material->GetTextureCount(type); ++i) {
        aiString texture_file;
        material->GetTexture(type, i, &texture_file);
        if (texture_file.Empty()) {
            spdlog::info("For material {} there is no texture with type={}", material->GetName().C_Str(), static_cast<int>(type));
            continue;
        }

        auto texture_it = textures_.find(texture_file.C_Str());
        if (texture_it != textures_.end()) { // texture is already loaded, get ptr to it
            loaded_textures.push_back(texture_it->second);
            spdlog::info("Used previously loaded texture");
        } else { // new texture, load it to memory
            std::filesystem::path texture_file_path = make_texture_path(model_base_dir_, texture_file);

            int width, height, components_number;
            uint8_t *data = stbi_load(texture_file_path.c_str(), &width, &height, &components_number, 0);
            if (!data) {
                spdlog::error("Failed to load texture form file: texture_file_path={}", texture_file_path.c_str());
                stbi_image_free(data);
                throw std::runtime_error("Failed to load texture form file");
            }

            // allocate memory
            GLenum format;
            switch (components_number)
            {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
            break;
            case 4:
                format = GL_RGBA;
            break;
            default:
                spdlog::error("Failed to select texture format, read components_number={}", components_number);
                assert(false);
                break;
            }

            auto loaded_texture_ptr = std::make_shared<Texture2D>(type);
            loaded_texture_ptr->bind();

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);

            textures_.emplace(texture_file_path, loaded_texture_ptr);
            loaded_textures.push_back(loaded_texture_ptr);
            spdlog::info("Loaded texture from file: type={}, texture_file_path={}", helpers::texture_type_name(type).data(), texture_file_path.c_str());
        }
    }

        return loaded_textures;
}

bool assimp_wrapper::TexturesManager::starts_with(const std::string &str, const std::string &prefix)
{
    if (str.size() < prefix.size()) return false;
    return str.compare(0, prefix.size(), prefix) == 0;
}

std::filesystem::path assimp_wrapper::TexturesManager::make_texture_path(const std::filesystem::path &directory, aiString const &texture_file)
{
    std::string path = texture_file.C_Str();
    std::replace(path.begin(), path.end(), '\\', '/');
    if (starts_with(path, "./"))
        path.erase(0, 2);

    return (directory / path).lexically_normal();
}
