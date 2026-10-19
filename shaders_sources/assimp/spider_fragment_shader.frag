#version 460 core

out vec4 FragColor;

in vec3 o_normal;
in vec3 o_frag_pos;
in vec2 o_tex_coords;

uniform vec4 u_base_color; 

layout (binding = 5) uniform sampler2D texture_ambient;
layout (binding = 6) uniform sampler2D texture_diffuse;
layout (binding = 7) uniform sampler2D texture_specular;
layout (binding = 8) uniform sampler2D texture_height;

void main() {

    FragColor = vec4(texture(texture_diffuse, vec2(o_tex_coords.x, o_tex_coords.y)).rgb, u_base_color.a);
}