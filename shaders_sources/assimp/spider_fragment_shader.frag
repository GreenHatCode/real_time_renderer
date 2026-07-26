#version 460 core

out vec4 FragColor;

in vec2 o_tex_coord;

uniform sampler2D texture_diffuse1;

void main() {
    FragColor = texture(texture_diffuse1, o_tex_coord);
}