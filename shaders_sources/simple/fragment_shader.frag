#version 460 core

in VSOut {
    vec2 tex_coords;
} vs_out;

out vec4 FragColor;

// textures
uniform vec4 u_base_color;
layout (binding = 5) uniform sampler2D texture_ambient;
layout (binding = 6) uniform sampler2D texture_diffuse;
layout (binding = 7) uniform sampler2D texture_specular;
layout (binding = 8) uniform sampler2D texture_height;  

void main() {    
    vec3 diffuse = texture(texture_diffuse, vs_out.tex_coords).rgb;
    float specular = texture(texture_specular, vs_out.tex_coords).r;
    
    FragColor.rgb = u_base_color.rgb * diffuse + u_base_color.rgb * specular;
    FragColor.a = u_base_color.a;
}