#include "assimp_wrapper/model.h"
#include "assimp/Importer.hpp"
#include "spdlog/spdlog.h"
#include "stb_image.h"

using namespace assimp_wrapper;

assimp_wrapper::Model::Model(const std::filesystem::path &path, uint32_t postprocess_flags)
    : textures_manager_(path.parent_path()), 
    vertices_count_(0), 
    faces_count_(0),
    transparent_(false)
{
    load_model(path, postprocess_flags);
}

void assimp_wrapper::Model::load_model(const std::filesystem::path &path, uint32_t postprocess_flags)
{
    Assimp::Importer importer;
    
    const aiScene *scene = importer.ReadFile(
        path.string(),
        postprocess_flags
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        spdlog::error("ASSIMP:{}", importer.GetErrorString());
        throw std::runtime_error(importer.GetErrorString());
    }
    spdlog::info("model:{} mRootNode->mNumChildren={} mNumMeshes[]={}, mNumMaterials[]={}", path.c_str(), scene->mRootNode->mNumChildren, scene->mNumMeshes, scene->mNumMaterials);

    materials_.reserve(scene->mNumMaterials);
    meshes_.reserve(scene->mNumMeshes);

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

    vertices_count_ += mesh->mNumVertices;
    faces_count_ += mesh->mNumFaces;

    return Mesh(vertices, indices, mesh->mMaterialIndex);
}

void assimp_wrapper::Model::load_materials(const aiScene *scene)
{
    for (size_t i = 0; i < scene->mNumMaterials; ++i) {
        aiMaterial *material_ptr = scene->mMaterials[i];

        aiColor4D color{1.0f};
        material_ptr->Get(AI_MATKEY_COLOR_DIFFUSE, color);

        if (color.a < 1.0f) transparent_ = true;

        Material material(glm::vec4(color.r, color.g, color.g, color.a), color.a, textures_manager_.load_material_textures(material_ptr, scene, kMaterialTextureTypes));
        materials_.push_back(material);
    }
}

void assimp_wrapper::Model::draw(Shader &shader_program) const
{
    for (auto& mesh : meshes_) {
        materials_[mesh.get_material_index()].bind(shader_program);
        mesh.render();
    }
}

uint32_t assimp_wrapper::Model::get_vertices_count() const
{
    return vertices_count_;
}

uint32_t assimp_wrapper::Model::get_faces_count() const
{
    return faces_count_;
}

bool assimp_wrapper::Model::is_transparent() const
{
    return transparent_;
}
