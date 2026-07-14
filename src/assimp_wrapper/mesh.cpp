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

Mesh::Mesh(const std::vector<vertex> &vertices, const std::vector<uint32_t> &vertex_indices, const std::vector<texture> &textures)
    : vertices_(vertices),
      vertex_indices_(vertex_indices),
      textures_(textures)
{
    mesh_init();
}

void assimp_wrapper::Mesh::Render(Shader shader_program)
{
    uint32_t diffuse_sampler_number = 1;
    uint32_t specular_sampler_number = 1;
    for (size_t i = 0; i < textures_.size(); ++i) {
        std::string texture_number;
        std::string name = textures_[i].type;
        if (name == "texture_diffuse") name += std::to_string(diffuse_sampler_number++);
        else if (name == "texture_specular") name += std::to_string(specular_sampler_number++);

        glActiveTexture(GL_TEXTURE0 + i);
        shader_program.set_uniform("material." + name, static_cast<int>(i));
        glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, vertex_indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}
