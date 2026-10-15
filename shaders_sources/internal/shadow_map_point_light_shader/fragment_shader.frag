#version 460 core

struct PointLight {
    vec3 position;
    float far_plane;
};

uniform PointLight point_lights[1];

in vec3 v_world_frag_pos;

out vec4 FragColor;

void main() {

    float light_dist = length(v_world_frag_pos - point_lights[0].position);
    light_dist = light_dist / point_lights[0].far_plane;
    gl_FragDepth = light_dist;
}