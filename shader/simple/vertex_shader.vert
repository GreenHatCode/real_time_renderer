#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_tex_coord;

out vec3 o_color;
out vec2 o_tex_coord;


void main() {
    gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);
    o_color = a_color;
    o_tex_coord = vec2(a_tex_coord.x, a_tex_coord.y);
}