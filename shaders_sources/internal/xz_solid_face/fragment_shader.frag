#version 460 core

const float GRID_INTERVAL = 5.0;
const float MAX_GRID_VISIBILITY_DISTANCE = 120.0;

in vec3 v_frag_pos;
out vec4 FragColor;

uniform vec3 u_camera_pos;

void main() {

    float dist_to_camera = length(v_frag_pos.xz - u_camera_pos.xz);
    float fade_alpha = 1.0 - smoothstep(20.0, MAX_GRID_VISIBILITY_DISTANCE, dist_to_camera);
    if (fade_alpha <= 0.0) discard;
    
    FragColor = vec4(1.0, 1.0, 1.0, fade_alpha);
}