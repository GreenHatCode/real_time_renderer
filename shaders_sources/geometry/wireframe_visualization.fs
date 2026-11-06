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


// textures
uniform vec4 u_base_color;
layout (binding = 5) uniform sampler2D texture_ambient;
layout (binding = 6) uniform sampler2D texture_diffuse;
layout (binding = 7) uniform sampler2D texture_specular;
layout (binding = 8) uniform sampler2D texture_height;  

void main() {    

    gNormal = vs_out.normal;
    gPosition = vs_out.frag_pos;

    vec3 diffuse = texture(texture_diffuse, vs_out.tex_coords).rgb;
    float specular = texture(texture_specular, vs_out.tex_coords).r;
    
    gAlbedoSpec.rgb = u_base_color.rgb * diffuse + u_base_color.rgb * specular;
    gAlbedoSpec.a = u_base_color.a;
}