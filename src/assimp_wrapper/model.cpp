#include "assimp_wrapper/model.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "spdlog/spdlog.h"

using namespace assimp_wrapper;

void assimp_wrapper::Model::load_model(const std::filesystem::path &path)
{
    Assimp::Importer importer;
    
    const aiScene *scene = importer.ReadFile(
        path.string(),
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_ValidateDataStructure
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        spdlog::error("ASSIMP:{}", importer.GetErrorString());
        return;
    }

    directory_ = path.parent_path();

    process_node(scene->mRootNode, scene);
}

void assimp_wrapper::Model::process_node(aiNode *node, const aiScene *scene)
{
    // processing all meshes in selected node
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
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
    std::vector<texture> textures;

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

    for (size_t i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];

        for (size_t j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return Mesh(vertices, indices, textures);
}

assimp_wrapper::Model::Model(const std::filesystem::path &path)
{
    load_model(path);
}

void assimp_wrapper::Model::draw(const Shader &shader_program)
{
    for (auto& mesh : meshes_) {
        mesh.Render(shader_program);
    }
}
