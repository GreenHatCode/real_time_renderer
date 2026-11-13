#version 460 core

out VSOut {
    vec2 tex_coords;
} vs_out;

void main() {

    vec2 vertices[3] = vec2[](vec2(-1,-1), vec2(3,-1), vec2(-1, 3));

    gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);
    vs_out.tex_coords = gl_Position.xy * 0.5 + vec2(0.5);
}