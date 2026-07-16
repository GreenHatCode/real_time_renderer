#include "assimp_wrapper/model.h"
#include "assimp/Importer.hpp"
#include "spdlog/spdlog.h"
#include "model.h"
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
    load_materials(scene);
    process_node(scene->mRootNode, scene);
}

void assimp_wrapper::Model::process_node(aiNode *node, const aiScene *scene)
{
    // processing all meshes in selected node
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(process_mesh(mesh, scene));
        
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        materials_.emplace(mesh->mMaterialIndex, process_material(material, scene));
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

        // std::vector<std::shared_ptr<texture>> material_textures;
        // load_material_textures(material_ptr, aiTextureType_AMBIENT, scene);

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

        auto texture_it = textures_.find(texture_file.C_Str());
        if (texture_it != textures_.end()) { // new texture, load it to memory
            std::filesystem::path texture_file_path = directory_ / std::filesystem::path(texture_file.C_Str());

            GLuint texture_id;
            glGenTextures(1, texture_id);

            int width, height, components_number;
            uint8_t *data = stbi_load(texture_file_path.c_str(), &width, &height, &components_number, 0);
            if (!data) {
                spdlog::error("Failed to load texture form file: {}", texture_file_path);
                stbi_image_free(data);
                throw std::runtime_error("Failed to load texture form file");
            }

            // allocate memory




        }
        else { // get ptr to already loaded texture
            loaded_textures.push_back(texture_it->second);
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
