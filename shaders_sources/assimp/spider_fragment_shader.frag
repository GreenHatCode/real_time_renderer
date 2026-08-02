#version 460 core

const uint MAX_TEXTURES_NUMBER = 7;

out vec4 FragColor;

in vec3 o_normal;
in vec3 o_frag_pos;
in vec2 o_tex_coords;

uniform vec3 u_light_pos;
uniform vec3 u_light_color;

uniform sampler2D texture_diffuse[MAX_TEXTURES_NUMBER];
uniform sampler2D texture_specular[MAX_TEXTURES_NUMBER];
uniform sampler2D texture_height[MAX_TEXTURES_NUMBER];
uniform sampler2D texture_ambient[MAX_TEXTURES_NUMBER];

uniform int diffuse_count;
uniform int specular_count;
uniform int height_count;
uniform int ambient_count;

vec3 ambient_color_from_texture();
vec3 diffuse_color_from_texture();
vec3 specualr_color_from_texture();

void main() {
    vec3 ambient = ambient_color_from_texture();

    // diffuse color
    vec3 unit_normal = normalize(o_normal);
    vec3 light_dir = normalize(u_light_pos - o_frag_pos);
    float diff = max(dot(unit_normal, light_dir), 0.0);
    vec3 diffuse = diff * diffuse_color_from_texture();

    // specular color
    vec3 view_dir = normalize(-o_frag_pos);
    vec3 reflect_dir = reflect(-light_dir, unit_normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 64);
    vec3 specular = 0.5 * spec * u_light_color * specualr_color_from_texture();

    vec3 result_color = ambient + diffuse + specular;
    FragColor = vec4(result_color, 1.0);
}

vec3 ambient_color_from_texture() {
    vec3 result_color;
    for (int i = 0; i < ambient_count; ++i) {
        result_color += texture(texture_ambient[i], o_tex_coords).rgb;
    }
    return result_color;
}

vec3 diffuse_color_from_texture() {
    vec3 result_color;
    for (int i = 0; i < diffuse_count; ++i) {
        result_color += texture(texture_diffuse[i], o_tex_coords).rgb;
    }
    return result_color;
}

vec3 specualr_color_from_texture() {
    vec3 result_color;
    for (int i = 0; i < specular_count; ++i) {
        result_color += texture(texture_specular[i], o_tex_coords).rgb;
    }
    return result_color;
}