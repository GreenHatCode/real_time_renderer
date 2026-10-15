#version 460 core

const float GRID_INTERVAL = 5.0;
const float MAX_GRID_VISIBILITY_DISTANCE = 120.0;

in vec3 v_frag_pos;
out vec4 FragColor;

uniform vec3 u_camera_pos;

void main() {

    float u_grid_interval = 10;

    vec2 coord = v_frag_pos.xz / u_grid_interval;
    
    // 2. Compute screen-space derivatives to keep lines exactly 1 pixel wide
    // fwidth() tells us how much 'coord' changes from one pixel to the next
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
    
    // 3. Find the closest line (X or Z)
    float line_dist = min(grid.x, grid.y);
    
    // 4. Anti-alias the line (1.0 at exactly the center, fading to 0.0 one pixel away)
    float line_alpha = 1.0 - min(line_dist, 1.0);
    
    // Drop the pixel early if it's completely inside the cell
    if (line_alpha <= 0.0) discard;
    
    // 5. Smoothly fade the grid out into the distance (e.g., fades out between 20 and 60 units)
    float dist_to_camera = length(v_frag_pos.xz - u_camera_pos.xz);
    float fade_alpha = 1.0 - smoothstep(20.0, MAX_GRID_VISIBILITY_DISTANCE, dist_to_camera);
    
    if (fade_alpha <= 0.0) discard;
    
    FragColor = vec4(1.0, 1.0, 1.0, line_alpha * fade_alpha);
}