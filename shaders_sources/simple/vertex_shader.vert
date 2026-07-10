#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex_coord;

out vec2 o_tex_coord;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

void main() {
    gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);
    o_tex_coord = vec2(a_tex_coord.x, a_tex_coord.y);
}