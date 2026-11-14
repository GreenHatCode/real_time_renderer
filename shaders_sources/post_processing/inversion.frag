#version 460 core

in vec2 v_tex_coords;

out vec4 FragColor;

uniform sampler2D u_screen_texture;

void main() {
    vec3 color = texture(u_screen_texture, v_tex_coords).rgb;
    FragColor = vec4(vec3(1.0 - texture(u_screen_texture, v_tex_coords)), 1.0);
}