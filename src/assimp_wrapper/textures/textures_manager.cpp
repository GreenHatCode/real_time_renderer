#include "assimp_wrapper/textures/textures_manager.h"
#include <assimp/scene.h>
#include <stb_image.h>

using namespace assimp_wrapper;
using textures_vector = std::vector<std::shared_ptr<Texture2D>>;

assimp_wrapper::TexturesManager::TexturesManager(const std::filesystem::path &model_base_dir, const aiScene *scene)
    : model_base_dir_(model_base_dir), scene_(scene)
{
}

std::shared_ptr<Texture2D> assimp_wrapper::TexturesManager::load_material_texture(aiMaterial *material, const aiTextureType type)
{
    aiString file;
    material->GetTexture(type, 0, &file);

    if (file.Empty()) {
        spdlog::debug("For material {} there is no texture with type={}. Using fallback texture.", 
            material->GetName().C_Str(), helpers::texture_type_name(type).data());
        
        switch (type)
        {
        case aiTextureType::aiTextureType_DIFFUSE:
            return Texture2D::create_solid_color(aiTextureType_DIFFUSE, 255, 255, 255); // white
            
        case aiTextureType::aiTextureType_SPECULAR:
            return Texture2D::create_solid_color(aiTextureType_SPECULAR, 0, 0, 0); // black (no specular highlights)
            
        case aiTextureType::aiTextureType_NORMALS:
            return Texture2D::create_solid_color(aiTextureType_NORMALS, 128, 128, 255);  // flat normal (0,0,1)
            
        case aiTextureType::aiTextureType_AMBIENT:
            return Texture2D::create_solid_color(aiTextureType_AMBIENT, 255, 255, 255);  // white (full ambient light)
            
        case aiTextureType::aiTextureType_HEIGHT:
            return Texture2D::create_solid_color(aiTextureType_HEIGHT, 128, 128, 128);   // mid-gray (neutral height)

        case aiTextureType::aiTextureType_DISPLACEMENT:
            return Texture2D::create_solid_color(aiTextureType_DISPLACEMENT, 0, 0, 0);
            
        default:
            spdlog::error("Absent fallback texture for passed type={}", helpers::texture_type_name(type).data());
            assert(false && "Unhandled texture type fallback");
            return nullptr; 
        }
    }

    std::string str(file.C_Str());
    if (str[0] == '*') { // load embedded texture
        int texture_idx = stoi(str.substr(1));
        aiTexture* tex = scene_->mTextures[texture_idx];
        return load_embedded_texture(tex, type);

    }
    else { // load file texture
        return load_file_texture(make_texture_path(model_base_dir_, file), type);
    }    
}

std::shared_ptr<Texture2D> assimp_wrapper::TexturesManager::load_embedded_texture(const aiTexture *texture_ptr, const aiTextureType type)
{
    auto texture_it = textures_.find(texture_ptr->mFilename.C_Str());

    if (texture_it != textures_.end()) { // texture is already loaded, get ptr to it
        spdlog::info("Used previously loaded texture: id={}, type={}", 
            texture_it->second->id(), helpers::texture_type_name(texture_it->second->type()));
        return texture_it->second;

    } else { // new texture, load it to memory
        auto loaded_texture_ptr = std::make_shared<Texture2D>(type);
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
        spdlog::info("Loaded embedded texture: texture_file_path={}",  texture_ptr->mFilename.C_Str());
        loaded_texture_ptr->unbind();
        return loaded_texture_ptr;
    }
}

std::shared_ptr<Texture2D> assimp_wrapper::TexturesManager::load_file_texture(const std::filesystem::path file, const aiTextureType type)
{
    auto texture_it = textures_.find(file);
    if (texture_it != textures_.end()) { // texture is already loaded, get ptr to it
        spdlog::info("Used previously loaded file texture: id={}, type={}", 
            texture_it->second->id(), helpers::texture_type_name(texture_it->second->type()).data());
        return texture_it->second;
    
    } else { // new texture, load it to memory

        int width, height, components_number;
        uint8_t *data = stbi_load(file.c_str(), &width, &height, &components_number, 0);
        if (!data) {
            spdlog::error("stb_image failed to load texture form file: texture_file_path={}. Reason: {}", file.c_str(), stbi_failure_reason());
            throw std::runtime_error("stb_image failed to load texture form file");
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
            spdlog::error("Failed to select file texture format, read components_number={}", components_number);
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

        textures_.emplace(file, loaded_texture_ptr);
        spdlog::info("Loaded texture from file: type={}, texture_file_path={}", helpers::texture_type_name(type).data(), file.c_str());
        
        return loaded_texture_ptr;
    }
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
