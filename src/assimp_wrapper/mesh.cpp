#include "assimp_wrapper/mesh.h"
#include <iostream>

using namespace assimp_wrapper;

assimp_wrapper::Mesh::Mesh(
    const std::vector<vertex> &vertices, 
    const std::vector<uint32_t> &vertex_indices, 
    uint32_t material_idx
)
    : indices_count_(vertex_indices.size())
    , material_idx_(material_idx)
{

    glCreateVertexArrays(1, &vao_);
    glCreateBuffers(1, &vbo_);
    glCreateBuffers(1, &ebo_);


    glNamedBufferData(vbo_, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);
    glNamedBufferData(ebo_, vertex_indices.size() * sizeof(uint32_t), vertex_indices.data(), GL_STATIC_DRAW);

    glVertexArrayElementBuffer(vao_, ebo_);
    glVertexArrayVertexBuffer(vao_, 0, vbo_, 0, sizeof(vertex));

    glEnableVertexArrayAttrib(vao_, 0);
    glVertexArrayAttribFormat(vao_, 0, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, position));
    glVertexArrayAttribBinding(vao_, 0, 0);

    glEnableVertexArrayAttrib(vao_, 1);
    glVertexArrayAttribFormat(vao_, 1, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, normal));
    glVertexArrayAttribBinding(vao_, 1, 0);

    glEnableVertexArrayAttrib(vao_, 2);
    glVertexArrayAttribFormat(vao_, 2, 2, GL_FLOAT, GL_FALSE, offsetof(vertex, tex_coords));
    glVertexArrayAttribBinding(vao_, 2, 0);

    glEnableVertexArrayAttrib(vao_, 3);
    glVertexArrayAttribFormat(vao_, 3, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, tangent));
    glVertexArrayAttribBinding(vao_, 3, 0);

    glEnableVertexArrayAttrib(vao_, 4);
    glVertexArrayAttribFormat(vao_, 4, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, bitangent));
    glVertexArrayAttribBinding(vao_, 4, 0);

}

assimp_wrapper::Mesh::Mesh(Mesh &&rhs) noexcept
    : vao_(rhs.vao_), vbo_(rhs.vbo_), ebo_(rhs.ebo_), material_idx_(rhs.material_idx_), indices_count_(rhs.indices_count_)
{
    rhs.vao_ = 0;
    rhs.vbo_ = 0;
    rhs.ebo_ = 0;
}

assimp_wrapper::Mesh &assimp_wrapper::Mesh::operator=(Mesh &&rhs) noexcept
{
    if (this == &rhs) return *this;

    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);

    vao_ = rhs.vao_;
    vbo_ = rhs.vbo_;
    ebo_ = rhs.ebo_;
    material_idx_ = rhs.material_idx_;
    indices_count_ = rhs.indices_count_;

    rhs.vao_ = 0;
    rhs.vbo_ = 0;
    rhs.ebo_ = 0;    

    return *this;
}

uint32_t assimp_wrapper::Mesh::get_material_index() const
{
    return material_idx_;
}
void assimp_wrapper::Mesh::render(GLenum mode) const
{
    glBindVertexArray(vao_);
    glDrawElements(mode, static_cast<GLsizei>(indices_count_), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

assimp_wrapper::Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
}