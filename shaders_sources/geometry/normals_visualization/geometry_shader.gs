#version 460 core

const float NORMAL_VEC_MAGNITUDE = 5;

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VsData {
    vec3 view_position;
} v_vs_data[];

uniform mat4 u_projection_mat;

void main() {
    vec3 face_normal = normalize(cross(v_vs_data[2].view_position - v_vs_data[1].view_position, v_vs_data[0].view_position - v_vs_data[1].view_position));
    vec3 center = (v_vs_data[0].view_position + v_vs_data[1].view_position + v_vs_data[2].view_position) / 3.0;
    
    vec4 begin = u_projection_mat * vec4(center, 1.0);
    vec4 end = begin + u_projection_mat * vec4(face_normal, 0.0) * NORMAL_VEC_MAGNITUDE;
    
    gl_Position = begin;
    EmitVertex();

    gl_Position = end;
    EmitVertex();
    EndPrimitive();
    
}