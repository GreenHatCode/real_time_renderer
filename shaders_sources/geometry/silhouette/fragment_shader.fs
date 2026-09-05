#version 460 core

out vec4 FragColor;

uniform vec4 u_base_color;

void main() {
    FragColor = vec4(1.0 - u_base_color);
}