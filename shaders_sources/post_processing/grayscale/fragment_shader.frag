#version 460 core

in vec2 o_tex_coords;

out vec4 FragColor;

uniform sampler2D screen_texture;

void main() {
    vec4 color = texture(screen_texture, o_tex_coords);
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    FragColor = vec4(average, average, average, 1.0);
}