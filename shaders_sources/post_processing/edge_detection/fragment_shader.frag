#version 460 core

const float OFFSET = 1.0 / 300.0;

in vec2 o_tex_coords;

out vec4 FragColor;

uniform sampler2D u_screen_texture;

void main() {

    vec2 offsets[9] = vec2[](
        vec2(-OFFSET,  OFFSET), // top-left
        vec2( 0.0f,    OFFSET), // top-center
        vec2( OFFSET,  OFFSET), // top-right
        vec2(-OFFSET,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( OFFSET,  0.0f),   // center-right
        vec2(-OFFSET, -OFFSET), // bottom-left
        vec2( 0.0f,   -OFFSET), // bottom-center
        vec2( OFFSET, -OFFSET)  // bottom-right    
    );

    float kernel[9] = float[](
        1.0, 1.0, 1.0,
        1.0, -8.0, 1.0,
        1.0, 1.0, 1.0  
    );

    vec3 sampled_tex[9];
    for (int i = 0; i < 9; i++) {
        sampled_tex[i] = texture(u_screen_texture, o_tex_coords + offsets[i]).rgb;
    }

    vec3 color = vec3(0.0);
    for (int i = 0; i < 9; i++) {
        color += sampled_tex[i] * kernel[i];
    }

    FragColor = vec4(color, 1.0);
}