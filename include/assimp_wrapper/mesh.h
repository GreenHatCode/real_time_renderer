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
    
    class Mesh final { // allocates all buffers (including tex coords, but doesn't work with textures and uniforms (like lighting))
    private:
        uint32_t vao_;
        uint32_t vbo_;
        uint32_t ebo_;
        uint32_t material_idx_;

        size_t indices_count_;
        void mesh_init(const std::vector<vertex>& vertices, const std::vector<uint32_t>& vertex_indices);

    public:
        Mesh(const std::vector<vertex>& vertices, const std::vector<uint32_t>& vertex_indices, uint32_t material_idx);
        Mesh(const Mesh& rhs) = delete;
        Mesh(Mesh&& rhs) noexcept;
        Mesh& operator=(const Mesh& rhs) = delete;
        Mesh& operator=(Mesh&& rhs) noexcept;
        ~Mesh();

        void render() const;
        uint32_t get_material_index() const;
    };

} // assimp_wrapper