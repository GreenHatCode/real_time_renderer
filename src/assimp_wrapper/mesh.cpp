#include "assimp_wrapper/mesh.h"

using namespace assimp_wrapper;

void assimp_wrapper::Mesh::mesh_init()
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vertex), vertices_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_indices_.size() * sizeof(uint32_t), vertex_indices_.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, tex_coords));

    glBindVertexArray(0);
}

Mesh::Mesh(const std::vector<vertex> &vertices, const std::vector<uint32_t> &vertex_indices, uint32_t material_idx)
    : vertices_(vertices)
    ,   vertex_indices_(vertex_indices)
    ,   material_idx(material_idx)
{
    mesh_init();
}

uint32_t assimp_wrapper::Mesh::get_material_index() const
{
    return material_idx;
}
void assimp_wrapper::Mesh::render() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, vertex_indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}
