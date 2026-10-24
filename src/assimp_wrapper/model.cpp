#include "assimp_wrapper/model.h"
#include "assimp/Importer.hpp"
#include "spdlog/spdlog.h"
#include "stb_image.h"

using namespace assimp_wrapper;

assimp_wrapper::Model::Model(const std::filesystem::path &path, uint32_t postprocess_flags)
    : vertices_count_(0)
    , faces_count_(0)
    , transparent_(false)
    , file_path_(path)
{

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        file_path_.string(),
        postprocess_flags
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        spdlog::error("ASSIMP:{}", importer.GetErrorString());
        throw std::runtime_error(importer.GetErrorString());
    }
    spdlog::info("model:{} mRootNode->mNumChildren={} mNumMeshes[]={}, mNumMaterials[]={}", 
        file_path_.c_str(), scene->mRootNode->mNumChildren, scene->mNumMeshes, scene->mNumMaterials);

    materials_.reserve(scene->mNumMaterials);
    size_t opaque_meshes_count = std::count_if(  // how many transparent and opaque materials 
        materials_.cbegin(), 
        materials_.cend(), 
        [](const assimp_wrapper::Material& m) { 
            return m.get_opacity() >= assimp_wrapper::Material::OPAQUE_MESH_MATERIAL_OPACITY;
        }
    );
    opaque_meshes_.reserve(opaque_meshes_count);
    transparent_meshes_.reserve(scene->mNumMeshes - opaque_meshes_count);

    load_materials(scene); // load all meterials once
    process_node(scene->mRootNode, scene);

}

void assimp_wrapper::Model::process_node(aiNode *node, const aiScene *scene)
{
    // processing all meshes in selected node
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        bool transparent = materials_[mesh->mMaterialIndex].get_opacity() < 1.0f;
        
        if (transparent) {
            transparent_meshes_.push_back(process_mesh(mesh, scene));
        }
        else {
            opaque_meshes_.push_back(process_mesh(mesh, scene));
        }
    }

    // do the same for all child items
    for (size_t i = 0; i < node->mNumChildren; ++i) {
        process_node(node->mChildren[i], scene);
    }
}

Mesh assimp_wrapper::Model::process_mesh(aiMesh *mesh, const aiScene *scene)
{
    if (!mesh->HasNormals()) {
        throw std::runtime_error("Assimp returned a mesh without normals");
    }

    bool has_tangents = mesh->HasTangentsAndBitangents();
    if (!has_tangents) spdlog::warn("Assimp returned a mesh without tangents or bitangents normals. Continuing loading the model");

    std::vector<vertex> vertices(mesh->mNumVertices);
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

        // read and push all tangents and bitangents
        // in case tangent and bitangent are absent
        // we consider that the model will not use normal mapping
        // or textures at all (only base color is used)
        glm::vec3 tangent(ABSENT_TANGENT_VALUE), bitangent(ABSENT_BITANGENT_VALUE);
        if (has_tangents) {
            tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        }

        vertices[i] = vertex{
                .position = position,
                .normal = normal,
                .tex_coords = texture_coords,
                .tangent = tangent,
                .bitangent = bitangent
            };
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
    TexturesManager manager(file_path_.parent_path(), scene);

    for (size_t i = 0; i < scene->mNumMaterials; ++i) {
        aiMaterial *material_ptr = scene->mMaterials[i];

        aiColor4D color{1.0f};
        material_ptr->Get(AI_MATKEY_COLOR_DIFFUSE, color);

        float opacity = 1.0f;
        if (material_ptr->Get(AI_MATKEY_OPACITY, opacity) != AI_SUCCESS) {
            opacity = color.a; 
        } else {
            color.a = opacity;
        }

        aiString mat_name;
        material_ptr->Get(AI_MATKEY_NAME, mat_name);
        spdlog::info("Loaded Material: {} | Opacity: {}", mat_name.C_Str(), opacity);
        
        std::shared_ptr<Texture2D> ambient = manager.load_material_texture(material_ptr, aiTextureType::aiTextureType_AMBIENT);
        std::shared_ptr<Texture2D> diffuse = manager.load_material_texture(material_ptr, aiTextureType::aiTextureType_DIFFUSE);
        std::shared_ptr<Texture2D> specular = manager.load_material_texture(material_ptr, aiTextureType::aiTextureType_SPECULAR);
        std::shared_ptr<Texture2D> height = manager.load_material_texture(material_ptr, aiTextureType::aiTextureType_HEIGHT);
        std::shared_ptr<Texture2D> displacement = manager.load_material_texture(material_ptr, aiTextureType::aiTextureType_DISPLACEMENT);
        
        materials_.emplace_back(glm::vec4(color.r, color.g, color.b, color.a), opacity, ambient, diffuse, specular, height);
    }
}

void assimp_wrapper::Model::draw(GLenum draw_mode, bool draw_transparent, Shader &shader_program) const
{
    if (draw_transparent) {
        for (auto& mesh : transparent_meshes_) {
            materials_[mesh.get_material_index()].bind(shader_program);
            mesh.render(draw_mode);
        }
    }
    else {
        for (auto& mesh : opaque_meshes_) {
            materials_[mesh.get_material_index()].bind(shader_program);
            mesh.render(draw_mode);
        }
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
    return !transparent_meshes_.empty();
}
