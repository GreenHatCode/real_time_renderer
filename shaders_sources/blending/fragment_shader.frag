#version 460 core

out vec4 FragColor;

uniform vec4 u_base_color;
uniform float u_opacity;

void main() {
    FragColor = vec4(u_base_color);
}