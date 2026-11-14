#version 450

in vec2 v_tex_coords;

out float FragColor;

layout (binding = 5) uniform sampler2D ssao_unblured_texture;
layout (binding = 9) uniform sampler2D depth_buffer_texture;
uniform int u_blur_size = 4;


void main() {

    // discard fragments that contain only backgound data
    if (texture(depth_buffer_texture, v_tex_coords).r >= 1.0) discard;

    vec2 texel_size = 1.0 / vec2(textureSize(ssao_unblured_texture, 0));
    float result = 0.0;

    vec2 hlim = vec2(-u_blur_size * 0.5 + 0.5);
    for (int i = 0; i < u_blur_size; ++i) {
        for (int j = 0; j < u_blur_size; ++j) {
            vec2 offset = (hlim + vec2(float(i), float(j))) * texel_size;
            vec4 res = texture(ssao_unblured_texture, v_tex_coords + offset);
            result += res.r;
        }
    }
    FragColor = result / float(u_blur_size * u_blur_size);
}