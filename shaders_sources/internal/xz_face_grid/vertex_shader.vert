#version 460 core

layout (location = 0) in vec3 a_pos;

uniform mat4 u_model_mat;
uniform mat4 u_view_mat;
uniform mat4 u_projection_mat;

out vec3 v_frag_pos;

void main() {
    vec4 world_pos = u_model_mat * vec4(a_pos, 1.0);
    v_frag_pos = world_pos.xyz;
    gl_Position = u_projection_mat * u_view_mat * world_pos;
}