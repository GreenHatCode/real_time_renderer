#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;
layout (location = 3) in vec3 a_tangent;
layout (location = 4) in vec3 a_bitangent;

uniform mat4 u_model_mat;
uniform mat4 u_view_mat;
uniform mat4 u_projection_mat;
uniform mat3 u_normal_mat;

out VSOut {
    vec3 normal;
    vec3 frag_pos;
    vec2 tex_coords;
    mat3 tbn_mat;
} vs_out;


void main() {

    vec4 world_pos = u_model_mat * vec4(a_pos, 1.0);

    gl_Position = u_projection_mat * u_view_mat * world_pos;
    
    vs_out.normal = u_normal_mat * a_normal;
    vs_out.frag_pos = vec3(u_model_mat * world_pos);
    vs_out.tex_coords = a_tex_coords;

    vs_out.tbn_mat = mat3(
        normalize(vec3(u_model_mat * vec4(a_tangent, 0))),
        normalize(vec3(u_model_mat * vec4(a_bitangent, 0))),
        normalize(vs_out.normal)
    );
}