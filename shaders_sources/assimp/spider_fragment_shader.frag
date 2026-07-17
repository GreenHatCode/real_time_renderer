#version 460 core

out vec4 FragColor;

in vec2 o_tex_coord;

uniform sampler2D texture_diffuse1;

void main() {
    // FragColor = vec4(0.2, 1.0, 0.5, 1.0);
    FragColor = texture(texture_diffuse1, o_tex_coord);
}