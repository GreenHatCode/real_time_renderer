#version 460 core

const float OFFSET = 1.0 / 300.0;

in vec2 o_tex_coords;

out vec4 FragColor;

uniform sampler2D u_screen_texture;
uniform float near_plane;
uniform float far_plane;


// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{             
    float depthValue = texture(u_screen_texture, o_tex_coords).r;
    FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    // FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}