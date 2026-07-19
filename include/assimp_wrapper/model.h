#pragma once
#include "assimp_wrapper/mesh.h"
#include "assimp/scene.h"
#include <filesystem>
#include "assimp/postprocess.h"
#include "assimp_wrapper/material.h"
#include "assimp_wrapper/textures/textures_manager.h"
#include <memory>


namespace assimp_wrapper
{

    class Model final {
        std::vector<Mesh> meshes_;
        std::vector<Material> materials_;
        TexturesManager textures_manager_;
        
    public:
        Model(const std::filesystem::path &path, uint32_t postprocess_flags);
        void draw(Shader &shader_program);

    private:
        void load_model(const std::filesystem::path &path, uint32_t postprocess_flags);
        void process_node(aiNode *node, const aiScene *scene);
        Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
        
        void load_materials(const aiScene *scene);

    };

} // namespace assimp_wrapper
