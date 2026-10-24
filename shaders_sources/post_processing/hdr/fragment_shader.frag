#version 460 core

const float OFFSET = 1.0 / 300.0;

in vec2 o_tex_coords;

out vec4 FragColor;

uniform sampler2D u_screen_texture;
uniform float u_gamma;

void main() {

    vec3 hdr_color = texture(u_screen_texture, o_tex_coords).rgb;
    vec3 mapped = hdr_color / (hdr_color + vec3(1.0));
    mapped = pow(mapped, vec3(1.0 / u_gamma));

    FragColor = vec4(mapped, 1.0);
} 