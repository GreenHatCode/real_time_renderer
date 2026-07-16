#pragma once
#include "assimp_wrapper/mesh.h"
#include "assimp/scene.h"
#include <filesystem>
#include "assimp/postprocess.h"
#include "assimp_wrapper/material.h"
#include <memory>


namespace assimp_wrapper
{
    class Model final {
        std::vector<Mesh> meshes_;
        std::vector<Material> materials_;
        std::unordered_map<std::filesystem::path, std::shared_ptr<texture>> textures_; // stores all textures, used in model
        std::filesystem::path directory_;

        void load_model(const std::filesystem::path &path, uint32_t postprocess_flags);
        void process_node(aiNode *node, const aiScene *scene);
        Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
        Material process_material(aiMaterial *material, const aiScene *scene);
        void load_materials(const aiScene *scene);
        std::vector<std::shared_ptr<texture>> load_material_textures(aiMaterial *material, aiTextureType type, const aiScene *scene);

    public:
        Model(const std::filesystem::path &path, uint32_t postprocess_flags);
        void draw(Shader &shader_program);
    };

} // namespace assimp_wrapper
