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
    enum class ModelType { // defines which meshes model contains
        OPAQUE,
        TRANSPARENT,
        OPAQUE_AND_TRANSPARENT // general
    };

    inline const char* model_type_to_str(ModelType type) 
    {
        switch (type)
        {
        case ModelType::OPAQUE:
            return "OPAQUE";
        case ModelType::TRANSPARENT:
            return "TRANSPARENT";
        case ModelType::OPAQUE_AND_TRANSPARENT:
            return "OPAQUE_AND_TRANSPARENT";
        default:
            assert(false && "Unknown ModelType to get string name");
            return "";
            break;
        }
    }

    struct model_t {
        std::vector<Mesh> opaque_meshes;
        std::vector<Mesh> transparent_meshes;
        std::vector<Material> materials;
        uint32_t vertices_count;
        uint32_t faces_count;
        ModelType type;
    };

} // namespace assimp_wrapper
