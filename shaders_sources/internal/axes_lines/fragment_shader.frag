#version 460 core

out vec4 FragColor;

in vec3 v_frag_color;

void main() {
    FragColor = vec4(normalize(v_frag_color), 1.0);
}