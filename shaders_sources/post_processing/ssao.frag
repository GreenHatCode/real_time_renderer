#version 450

in vec2 v_tex_coords;

out float FragColor;

layout (binding = 5) uniform sampler2D noise_texture;
layout (binding = 6) uniform sampler2D frag_coords_texture;
layout (binding = 7) uniform sampler2D frag_normal_texture;
layout (binding = 9) uniform sampler2D depth_buffer_texture;

uniform vec3 samples[64]; // size = SAMPLES_COUNT
uniform vec2 noise_scale;
uniform mat4 u_projection_mat;

// TODO: possibly make uniforms and configure on cpu side
int kernel_size = 64;
float radius = 0.5;
float bias = 0.025;

void main() {

    // discard fragments that contain only backgound data
    if (texture(depth_buffer_texture, v_tex_coords).r >= 1.0) discard;

    vec3 frag_pos = texture(frag_coords_texture, v_tex_coords).rgb;
    vec3 unit_normal = texture(frag_normal_texture, v_tex_coords).rgb; // normalized in g-buffer shader
    vec3 random_vec = texture(noise_texture, v_tex_coords * noise_scale).xyz; // normalized on CPU side

    // TBN mat creation
    vec3 tangent = normalize(random_vec - unit_normal * dot(random_vec, unit_normal));
    vec3 bitangent = cross(unit_normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, unit_normal);

    float occlusion = 0.0;
    for(int i = 0; i < kernel_size; ++i)
    {
        vec3 sample_pos = TBN * samples[i]; // to view-space
        sample_pos = frag_pos + sample_pos * radius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(sample_pos, 1.0);
        offset = u_projection_mat * offset; //to clip-space
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range [0.0; 1.0]
        
        // get sample depth 
        float sample_depth = texture(frag_coords_texture, offset.xy).z; // get depth value of kernel sample
        
        // range check & accumulate
        float range_check = smoothstep(0.0, 1.0, radius / abs(frag_pos.z - sample_depth));
        occlusion += (sample_depth >= sample_pos.z + bias ? 1.0 : 0.0) * range_check;           
    }
    occlusion = 1.0 - (occlusion / kernel_size);
    
    FragColor = occlusion;
}
