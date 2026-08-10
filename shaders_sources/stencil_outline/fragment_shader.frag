#version 460 core

out vec4 FragColor;

uniform vec3 u_outline_color;

void main() {

    FragColor = vec4(u_outline_color, 1.0);
}
