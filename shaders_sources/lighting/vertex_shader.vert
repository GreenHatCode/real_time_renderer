#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

uniform mat3 u_normal_mat;
uniform mat4 u_model_mat;
uniform mat4 u_view_mat;
uniform mat4 u_projection_mat;

out vec3 o_normal;
out vec3 o_frag_pos;

void main() {
    gl_Position = u_projection_mat * u_view_mat * u_model_mat * vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);
    o_normal = u_normal_mat * a_normal;
    o_frag_pos = vec3(u_model_mat * vec4(a_pos, 1.0));
}