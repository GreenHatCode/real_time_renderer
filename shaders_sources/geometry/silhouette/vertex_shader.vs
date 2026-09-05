#version 460 core

layout (location = 0) in vec3 a_pos;

out VsData {
    vec3 position;
} v_vs_data;

uniform mat4 u_model_mat;
uniform mat4 u_view_mat;
uniform mat4 u_projection_mat;
uniform mat3 u_normal_mat;

void main() {
    vec4 position = u_model_mat * vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);
    v_vs_data.position = vec3(position);
    gl_Position = u_projection_mat * u_view_mat * position;
}