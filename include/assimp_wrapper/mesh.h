#pragma once
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "shader/shader.h"
#include "assimp/postprocess.h"

namespace assimp_wrapper {

    inline static constexpr uint32_t POST_PROCESSING_FLAGS = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace;
    inline static constexpr GLenum DEFAULT_DRAW_MODE = GL_TRIANGLES;
    inline static constexpr glm::vec3 ABSENT_TANGENT_VALUE = glm::vec3(-1.0f);
    inline static constexpr glm::vec3 ABSENT_BITANGENT_VALUE = glm::vec3(-1.0f);

    struct vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coords;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };
    
    class Mesh final { // allocates all buffers (including tex coords, but doesn't work with textures and uniforms (like lighting))
    private:
        GLuint vao_;
        GLuint vbo_;
        GLuint ebo_;
        GLuint material_idx_;
        size_t indices_count_;

    public:
        Mesh(const std::vector<vertex>& vertices, 
            const std::vector<uint32_t>& vertex_indices, 
            uint32_t material_idx);
        Mesh(const Mesh& rhs) = delete;
        Mesh(Mesh&& rhs) noexcept;
        Mesh& operator=(const Mesh& rhs) = delete;
        Mesh& operator=(Mesh&& rhs) noexcept;
        ~Mesh();

        void render(GLenum mode = GL_TRIANGLES) const;
        uint32_t get_material_index() const;
    };

} // assimp_wrapper