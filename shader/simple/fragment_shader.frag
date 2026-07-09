#version 460 core

in vec3 o_color;
in vec2 o_tex_coord;

out vec4 FragColor;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

void main() {
    FragColor = mix(texture(u_texture1, o_tex_coord), texture(u_texture2, o_tex_coord), 0.2);
}