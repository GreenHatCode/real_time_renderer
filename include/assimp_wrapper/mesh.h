#pragma once
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "shader/shader.h"

namespace assimp_wrapper {

    struct vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coords;
    };

    struct texture 
    {
        uint32_t id;
        std::string type;
    };
    
    class Mesh final {
    private:
        uint32_t vao_;
        uint32_t vbo_;
        uint32_t ebo_;

        std::vector<vertex> vertices_;
        std::vector<uint32_t> vertex_indices_;
        std::vector<texture> textures_;
        void mesh_init();

    public:
        Mesh(const std::vector<vertex>& vertices, const std::vector<uint32_t>& vertex_indices, const std::vector<texture>& textures);
        void Render(Shader shader_program);




    };

} // assimp_wrapper