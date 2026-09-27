#include "assimp_wrapper/mesh.h"
#include <iostream>

using namespace assimp_wrapper;

Mesh::Mesh(const std::vector<vertex> &vertices, const std::vector<uint32_t> &vertex_indices, uint32_t material_idx)
    :   indices_count_(vertex_indices.size())
    ,   material_idx_(material_idx)
{
    mesh_init(vertices, vertex_indices);
}

void assimp_wrapper::Mesh::mesh_init(const std::vector<vertex>& vertices, const std::vector<uint32_t>& vertex_indices)
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_indices.size() * sizeof(uint32_t), vertex_indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, tex_coords));

    glBindVertexArray(0);
}

assimp_wrapper::Mesh::Mesh(Mesh &&rhs) noexcept
    :   vao_(rhs.vao_)
    ,   vbo_(rhs.vbo_)
    ,   ebo_(rhs.ebo_)
    ,   material_idx_(rhs.material_idx_)
    ,   indices_count_(rhs.indices_count_)
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
    glDrawElements(mode, indices_count_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

assimp_wrapper::Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
}