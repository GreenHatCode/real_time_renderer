#pragma once
#include "assimp_wrapper/model.h"

namespace assimp_wrapper
{

    class ModelLoader {
        std::vector<Mesh> opaque_meshes_;
        std::vector<Mesh> transparent_meshes_;
        std::vector<Material> materials_;
        uint32_t vertices_count_;
        uint32_t faces_count_;

        bool height_map_flag_;

        void process_node(aiNode *node, const aiScene *scene);
        Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
        void load_materials(const std::filesystem::path &path, const aiScene *scene);
        void clear();

    public:
        model_t load_model(const std::filesystem::path &path, uint32_t postprocess_flags);
        bool has_height_map() const;
    };

} // namespace assimp_wrapper