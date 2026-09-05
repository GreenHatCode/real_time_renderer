#version 460 core

layout (triangles_adjacency) in;
layout (line_strip, max_vertices = 6) out;

in VsData {
    vec3 position;
} v_vs_data[];

uniform mat4 u_projection_mat;
uniform vec3 u_camera_pos;

void main() {   

    // 1 ---- 2 ----- 3
    // \     / \     /
    //  \   e1  e2  /
    //   \ /     \ /
    //    0 -e3-- 4
    //     \     /
    //      \   /
    //       \ /
    //        5

    // triangle 0 2 4
    vec3 n_main = normalize(cross(v_vs_data[4].position - v_vs_data[2].position, v_vs_data[0].position - v_vs_data[2].position));

    // triangle 0 1 2
    vec3 n_adj1 = normalize(cross(v_vs_data[2].position - v_vs_data[1].position, v_vs_data[0].position - v_vs_data[1].position));

    // triangle 2 3 4
    vec3 n_adj2 = normalize(cross(v_vs_data[4].position - v_vs_data[3].position, v_vs_data[2].position - v_vs_data[3].position));

    // triangle 4 5 0
    vec3 n_adj3 = normalize(cross(v_vs_data[0].position - v_vs_data[5].position, v_vs_data[4].position - v_vs_data[5].position));

    vec3 view_dir = normalize(v_vs_data[0].position - u_camera_pos);

    bool main_visible = dot(n_main, view_dir) < 0.0;
    bool adj1_visible = dot(n_adj1, view_dir) < 0.0;
    bool adj2_visible = dot(n_adj2, view_dir) < 0.0;
    bool adj3_visible = dot(n_adj3, view_dir) < 0.0;

    if (!main_visible) return;

    if (!adj1_visible) {
        gl_Position = gl_in[0].gl_Position; 
        EmitVertex();
        gl_Position = gl_in[2].gl_Position;
        EmitVertex();
        EndPrimitive();
    }

    if (!adj2_visible) {
        gl_Position = gl_in[2].gl_Position; 
        EmitVertex();
        gl_Position = gl_in[4].gl_Position;
        EmitVertex();
        EndPrimitive();
    }

    if (!adj3_visible) {
        gl_Position = gl_in[4].gl_Position; 
        EmitVertex();
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
        EndPrimitive();
    }
}