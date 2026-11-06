#version 460 core

const float NORMAL_VEC_MAGNITUDE = 5;

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VSOut {
    vec3 normal;
    vec3 frag_pos;
    vec2 tex_coords;
    mat3 tbn_mat;
} gs_in[];

out VSOut {
    vec3 normal;
    vec3 frag_pos;
    vec2 tex_coords;
    mat3 tbn_mat;
} gs_out;

uniform mat4 u_view_mat;
uniform mat4 u_projection_mat;

void main() {
    vec3 face_normal = normalize(cross(gs_in[2].frag_pos - gs_in[1].frag_pos, gs_in[0].frag_pos - gs_in[1].frag_pos));
    vec3 center = (gs_in[0].frag_pos + gs_in[1].frag_pos + gs_in[2].frag_pos) / 3.0;
    
    vec4 begin =  vec4(center, 1.0);
    vec4 end = begin + vec4(face_normal, 0.0) * NORMAL_VEC_MAGNITUDE;
    
    gl_Position = u_projection_mat * u_view_mat * begin;
    gs_out.normal = face_normal;
    gs_out.frag_pos = vec3(begin);
    gs_out.tex_coords = vec2(0.0);
    gs_out.tbn_mat = mat3(0.0);
    EmitVertex();

    gl_Position = u_projection_mat * u_view_mat * end;
    gs_out.normal = face_normal;
    gs_out.frag_pos = vec3(end);
    gs_out.tex_coords = vec2(0.0);
    gs_out.tbn_mat = mat3(0.0);
    EmitVertex();
    
    EndPrimitive();
    
}