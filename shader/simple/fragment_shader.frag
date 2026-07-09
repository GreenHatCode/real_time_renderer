#version 460 core

in vec3 o_color;
in vec2 o_tex_coord;

out vec4 FragColor;

uniform sampler2D u_texture;

void main() {
    FragColor = texture(u_texture, o_tex_coord);
}