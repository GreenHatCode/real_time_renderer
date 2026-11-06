#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in VSOut {
    vec3 normal;
    vec3 frag_pos;
    vec2 tex_coords;
    mat3 tbn_mat;
} vs_out;

layout (binding = 5) uniform sampler2D texture_ambient;
layout (binding = 6) uniform sampler2D texture_diffuse;
layout (binding = 7) uniform sampler2D texture_specular;
layout (binding = 8) uniform sampler2D texture_height;  

void main() {    

    gNormal = vs_out.normal;
    gPosition = vs_out.frag_pos;
    gAlbedoSpec = vec4(1.0); // normals color - white
}