#version 460 core

in vec2 o_tex_coords;

out vec4 FragColor;

uniform sampler2D screen_texture;

void main() {
    vec3 color = texture(screen_texture, o_tex_coords).rgb;
    FragColor = vec4(color, 1.0);
}