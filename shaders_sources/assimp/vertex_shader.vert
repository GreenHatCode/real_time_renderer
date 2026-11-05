#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex_coords;

out VSOut {
    vec2 tex_coords;
} vs_out;

uniform mat4 u_model_mat;
uniform mat4 u_view_mat;
uniform mat4 u_projection_mat;

void main() {
    gl_Position = vec4(a_pos.x, a_pos.y, 0.0, 1.0);
    vs_out.tex_coords = a_tex_coords;
}