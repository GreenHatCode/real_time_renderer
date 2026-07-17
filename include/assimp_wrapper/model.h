#pragma once
#include "assimp_wrapper/mesh.h"
#include "assimp/scene.h"
#include <filesystem>
#include "assimp/postprocess.h"
#include "assimp_wrapper/material.h"
#include <memory>


namespace assimp_wrapper
{
    inline bool starts_with(const std::string& str, const std::string& prefix) {
        if (str.size() < prefix.size()) return false;
        return str.compare(0, prefix.size(), prefix) == 0;
    }

    inline std::filesystem::path make_texture_path(
        const std::filesystem::path& directory,
        aiString const& texture_file)
    {
        std::string path = texture_file.C_Str();

        std::replace(path.begin(), path.end(), '\\', '/');

        if (starts_with(path, "./"))
            path.erase(0, 2);

        return (directory / path).lexically_normal();
    }

    class Model final {
        std::vector<Mesh> meshes_;
        std::vector<Material> materials_;
        
        

        void load_model(const std::filesystem::path &path, uint32_t postprocess_flags);
        void process_node(aiNode *node, const aiScene *scene);
        Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
        
        void load_materials(const aiScene *scene);
        std::vector<std::shared_ptr<texture>> load_material_textures(aiMaterial *material, aiTextureType type, const aiScene *scene);

    public:
        Model(const std::filesystem::path &path, uint32_t postprocess_flags);
        void draw(Shader &shader_program);
    };

} // namespace assimp_wrapper
