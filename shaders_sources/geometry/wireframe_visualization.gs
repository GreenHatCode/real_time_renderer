#version 460 core

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
    
    // first side
    gl_Position = u_projection_mat * u_view_mat * vec4(gs_in[0].frag_pos, 1.0);
    gs_out.normal = gs_in[0].normal;
    gs_out.frag_pos = gs_in[0].frag_pos;
    gs_out.tex_coords = gs_in[0].tex_coords;
    gs_out.tbn_mat = gs_in[0].tbn_mat;
    EmitVertex();

    gl_Position = u_projection_mat * u_view_mat * vec4(gs_in[1].frag_pos, 1.0);
    gs_out.normal = gs_in[1].normal;
    gs_out.frag_pos = gs_in[1].frag_pos;
    gs_out.tex_coords = gs_in[1].tex_coords;
    gs_out.tbn_mat = gs_in[1].tbn_mat;
    EmitVertex();

    // second side
    gl_Position = u_projection_mat * u_view_mat * vec4(gs_in[1].frag_pos, 1.0);
    gs_out.normal = gs_in[1].normal;
    gs_out.frag_pos = gs_in[1].frag_pos;
    gs_out.tex_coords = gs_in[1].tex_coords;
    gs_out.tbn_mat = gs_in[1].tbn_mat;
    EmitVertex();

    gl_Position = u_projection_mat * u_view_mat * vec4(gs_in[2].frag_pos, 1.0);
    gs_out.normal = gs_in[2].normal;
    gs_out.frag_pos = gs_in[2].frag_pos;
    gs_out.tex_coords = gs_in[2].tex_coords;
    gs_out.tbn_mat = gs_in[2].tbn_mat;
    EmitVertex();

    // third side
    gl_Position = u_projection_mat * u_view_mat * vec4(gs_in[2].frag_pos, 1.0);
    gs_out.normal = gs_in[2].normal;
    gs_out.frag_pos = gs_in[2].frag_pos;
    gs_out.tex_coords = gs_in[2].tex_coords;
    gs_out.tbn_mat = gs_in[2].tbn_mat;
    EmitVertex();

    gl_Position = u_projection_mat * u_view_mat * vec4(gs_in[0].frag_pos, 1.0);
    gs_out.normal = gs_in[0].normal;
    gs_out.frag_pos = gs_in[0].frag_pos;
    gs_out.tex_coords = gs_in[0].tex_coords;
    gs_out.tbn_mat = gs_in[0].tbn_mat;
    EmitVertex();

    EndPrimitive();
}