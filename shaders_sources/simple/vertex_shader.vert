#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;

uniform mat4 u_model_mat;
uniform mat4 u_view_mat;
uniform mat4 u_projection_mat;

out VSOut {
    vec2 tex_coords;
} vs_out;

void main() {
    gl_Position = u_projection_mat * u_view_mat * u_model_mat * vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);
    vs_out.tex_coords = a_tex_coords;
}