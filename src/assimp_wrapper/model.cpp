#include "assimp_wrapper/model.h"
#include "assimp/Importer.hpp"
#include "spdlog/spdlog.h"
#include "stb_image.h"

using namespace assimp_wrapper;

void assimp_wrapper::Model::load_model(const std::filesystem::path &path, uint32_t postprocess_flags)
{
    Assimp::Importer importer;
    
    const aiScene *scene = importer.ReadFile(
        path.string(),
        postprocess_flags
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        spdlog::error("ASSIMP:{}", importer.GetErrorString());
        return;
    }

    directory_ = path.parent_path();

    spdlog::info("scene: mmRootNode->mNumChildren={} mNumMeshes[]={}, mNumMaterials[]={}", scene->mRootNode->mNumChildren, scene->mNumMeshes, scene->mNumMaterials);
    load_materials(scene); // load all meterial once
    process_node(scene->mRootNode, scene);
}

void assimp_wrapper::Model::process_node(aiNode *node, const aiScene *scene)
{
    // processing all meshes in selected node
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(process_mesh(mesh, scene));
    }

    // do the same for all child items
    for (size_t i = 0; i < node->mNumChildren; ++i) {
        process_node(node->mChildren[i], scene);
    }
}

Mesh assimp_wrapper::Model::process_mesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<vertex> vertices;
    std::vector<uint32_t> indices;

    // read and push all vertices
    for (size_t i = 0; i < mesh->mNumVertices; ++i) {

        glm::vec3 position(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        glm::vec3 normal(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );

        glm::vec2 texture_coords(0.0f, 0.0f);
        if (mesh->mTextureCoords[0])
        {
            texture_coords.x = mesh->mTextureCoords[0][i].x;
            texture_coords.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex{position, normal, texture_coords});
    }

    // read and push all indices
    for (size_t i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];

        for (size_t j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return Mesh(vertices, indices, mesh->mMaterialIndex);
}

void assimp_wrapper::Model::load_materials(const aiScene *scene)
{
    for (size_t i = 0; i < scene->mNumMaterials; ++i) {
        aiMaterial *material_ptr = scene->mMaterials[i];

        std::vector<std::shared_ptr<texture>> material_textures, tmp; // TODO: allocate memory for all material textures at the beginning
        
        tmp = load_material_textures(material_ptr, aiTextureType_DIFFUSE, scene);
        material_textures.insert(material_textures.end(), tmp.begin(), tmp.end());

        tmp = load_material_textures(material_ptr, aiTextureType_SPECULAR, scene);
        material_textures.insert(material_textures.end(), tmp.begin(), tmp.end());

        tmp = load_material_textures(material_ptr, aiTextureType_HEIGHT, scene);
        material_textures.insert(material_textures.end(), tmp.begin(), tmp.end());

        tmp = load_material_textures(material_ptr, aiTextureType_AMBIENT, scene);
        material_textures.insert(material_textures.end(), tmp.begin(), tmp.end());

        Material material(material_textures);
        materials_.push_back(material);
    }
}

std::vector<std::shared_ptr<texture>> assimp_wrapper::Model::load_material_textures(aiMaterial *material, aiTextureType type, const aiScene *scene)
{
    // allocate memory for all material textures
    // push every new textures to model 
    // return verctor of textures, used by this material

    std::vector<std::shared_ptr<texture>> loaded_textures;
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
            std::filesystem::path texture_file_path = make_texture_path(directory_, texture_file);

            int width, height, components_number;
            uint8_t *data = stbi_load(texture_file_path.c_str(), &width, &height, &components_number, 0);
            if (!data) {
                spdlog::error("Failed to load texture form file: texture_file_path={}, texture_file={}", texture_file_path.c_str(), texture_file.C_Str());
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

            GLuint texture_id;
            glGenTextures(1, &texture_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);

            auto ptr = std::make_shared<texture>(texture_id, type);
            textures_.emplace(texture_file_path, ptr);
            loaded_textures.push_back(ptr);
            spdlog::info("Loaded texture: type={}, texture_file_path={}", static_cast<int>(type), texture_file_path.c_str());
        }
    }

        return loaded_textures;
}

assimp_wrapper::Model::Model(const std::filesystem::path &path, uint32_t postprocess_flags)
{
    load_model(path, postprocess_flags);
}

void assimp_wrapper::Model::draw(Shader &shader_program)
{
    for (auto& mesh : meshes_) {
        materials_[mesh.get_material_index()].bind(shader_program);
        mesh.render();
    }
}
