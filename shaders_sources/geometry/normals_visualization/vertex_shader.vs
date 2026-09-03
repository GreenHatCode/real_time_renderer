#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

out VsData {
    vec3 view_position;
} v_vs_data;

uniform mat4 u_model_mat;
uniform mat4 u_view_mat;
uniform mat4 u_projection_mat;
uniform mat3 u_normal_mat;

void main() {

    vec4 view_position = u_view_mat * u_model_mat * vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);
    v_vs_data.view_position = vec3(view_position);
    gl_Position = u_projection_mat * view_position;
}