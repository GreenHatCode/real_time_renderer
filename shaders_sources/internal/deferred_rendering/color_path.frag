#version 460 core

struct DirectionalLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    vec3 direction;

    mat4 light_space_mat; // light's projection_mat * view_mat
};

struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    float far_plane;
    mat4 light_space_mat; // light's projection_mat * view_mat
};

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    vec3 position;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;

    float cut_off;
    float outer_cut_off;

    mat4 light_space_mat; // light's projection_mat * view_mat
};

const uint MAX_LIGHTS = 10;
const float DEFAULT_AMBIENT = 0.1;

in VSOut {
    vec2 tex_coords;
} vs_out;

out vec4 FragColor;


// shadow mapping
uniform sampler2DArray u_directional_lights_shadow_map_array; // layout (binding = 0)
uniform sampler2DArray u_spot_lights_shadow_map_array; // layout (binding = 1)
uniform samplerCubeArray u_point_lights_shadow_map_array; // layout (binding = 2)

// textures
layout (binding = 9) uniform sampler2D frag_coords_texture;
layout (binding = 10) uniform sampler2D frag_normal_texture;
layout (binding = 11) uniform sampler2D frag_color_texture;
layout (binding = 12) uniform sampler2D depth_buffer_texture;


// lighting
uniform vec3 u_camera_pos;
uniform DirectionalLight directional_lights[MAX_LIGHTS];
uniform PointLight point_lights[MAX_LIGHTS];
uniform SpotLight spot_lights[MAX_LIGHTS];

uniform uint u_directional_lights_count;
uniform uint u_point_lights_count;
uniform uint u_spot_lights_count;

vec3 process_directional_lights(vec3, vec3, vec3, vec3);
vec3 process_point_lights(vec3, vec3, vec3, vec3);
vec3 process_spot_lights(vec3, vec3, vec3, vec3);

float calculate_shadow(
    sampler2DArray depth_map, 
    vec4 frag_pos_light_space, 
    uint layer, 
    vec3 light_dir, 
    vec3 frag_normal
)
{
    vec3 ndc_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
    ndc_coords = ndc_coords * 0.5 + 0.5;
    
    if (ndc_coords.z > 1.0 || ndc_coords.x < 0.0 || ndc_coords.x > 1.0 || ndc_coords.y < 0.0 || ndc_coords.y > 1.0) {
        return 0.0;
    }

    float closest_depth = texture(depth_map, vec3(ndc_coords.xy, layer)).r;
    float curr_depth = ndc_coords.z;

    float bias = max(0.005 * (1.0 - dot(frag_normal, -light_dir)), 0.0005);
    bias *= max((1.0 - curr_depth) * 5.0, 0.001);
    
    return curr_depth - bias > closest_depth ? 1.0 : 0.0;
}

float calculate_point_light_shadow(
    samplerCubeArray depth_map, 
    vec3 frag_pos, 
    uint layer, 
    vec3 light_pos, 
    float far_plane,
    vec3 frag_normal
)
{
    vec3 frag_to_light = frag_pos - light_pos;
    float closest_depth = texture(depth_map, vec4(frag_to_light, layer)).r;
    closest_depth *= far_plane;

    float curr_depth = length(frag_to_light);

    float bias = max(0.051 * (1.0 - dot(frag_normal, normalize(frag_to_light))), 0.005);
    return curr_depth - bias > closest_depth ? 1.0 : 0.0;
}

void main() {

    if (texture(depth_buffer_texture, vs_out.tex_coords).r >= 1.0) discard;

    vec3 frag_pos = texture(frag_coords_texture, vs_out.tex_coords).rgb;
    vec3 unit_normal = texture(frag_normal_texture, vs_out.tex_coords).rgb; // normalized in g-buffer shader
    vec4 frag_color = texture(frag_color_texture, vs_out.tex_coords).rgba;
    vec3 view_dir = normalize(u_camera_pos - frag_pos);


    vec3 direction_lights_color = process_directional_lights(unit_normal, view_dir, frag_pos, frag_color.rgb);
    vec3 point_lights_color = process_point_lights(unit_normal, view_dir, frag_pos, frag_color.rgb);
    vec3 spot_lights_color = process_spot_lights(unit_normal, view_dir, frag_pos, frag_color.rgb);
    vec3 ambient_color = vec3(frag_color) * DEFAULT_AMBIENT;

    vec3 result = direction_lights_color + point_lights_color + spot_lights_color + ambient_color;  
    FragColor = vec4(result, frag_color.a);
}

vec3 process_directional_lights(vec3 unit_normal, vec3 view_dir, vec3 frag_pos, vec3 diffuse_color) {
    vec3 diffuse = vec3(0.0);

    for (uint i = 0; i < u_directional_lights_count; ++i) {
        vec4 frag_pos_light_space = directional_lights[i].light_space_mat * vec4(frag_pos, 1.0);
        float shadow = calculate_shadow(
            u_directional_lights_shadow_map_array, frag_pos_light_space, i, -directional_lights[i].direction, unit_normal
        );

        float diff = max(dot(unit_normal, -directional_lights[i].direction), 0.0);

        diffuse += diff * directional_lights[i].color * (1.0 - shadow) * diffuse_color;
    }
    
    return diffuse;
}

vec3 process_point_lights(vec3 unit_normal, vec3 view_dir, vec3 frag_pos, vec3 diffuse_color) {
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    for (uint i = 0; i < u_point_lights_count; ++i) {

        vec3 light_dir = normalize(point_lights[i].position - frag_pos);

        float shadow = calculate_point_light_shadow(
            u_point_lights_shadow_map_array, frag_pos, i, point_lights[i].position, point_lights[i].far_plane, unit_normal
        );

        
        float diff = max(dot(unit_normal, light_dir), 0.0);

        vec3 reflect_dir = reflect(-light_dir, unit_normal);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 64);

        float distance = length(point_lights[i].position - frag_pos);
        float attenuation = 1.0 / (point_lights[i].constant  + point_lights[i].linear * distance + point_lights[i].quadratic * pow(distance, 2));

        diffuse += point_lights[i].diffuse * point_lights[i].color * diff * attenuation * (1.0 - shadow) * diffuse_color;
        specular += point_lights[i].specular * point_lights[i].color * spec * attenuation * (1.0 - shadow) * diffuse_color; // todo: maybe use specular map if possible
    }

    return diffuse + specular;
}

vec3 process_spot_lights(vec3 unit_normal, vec3 view_dir, vec3 frag_pos, vec3 diffuse_color) {
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    for (uint i = 0; i < u_spot_lights_count; ++i) {
        vec4 frag_pos_light_space = spot_lights[i].light_space_mat * vec4(frag_pos, 1.0);

        vec3 light_dir = normalize(spot_lights[i].position - frag_pos); // direction of the light ray, not spot light itself
        float shadow = calculate_shadow(
            u_spot_lights_shadow_map_array, frag_pos_light_space, i, light_dir, unit_normal
        );

        float diff = max(dot(unit_normal, light_dir), 0.0); 

        vec3 reflect_dir = reflect(-light_dir, unit_normal);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 64);

        float theta = dot(light_dir, -spot_lights[i].direction);
        float epsilon = spot_lights[i].cut_off - spot_lights[i].outer_cut_off;
        float intensity = clamp((theta - spot_lights[i].outer_cut_off) / epsilon, 0.0, 1.0);
        diff *= intensity;
        spec *= intensity;

        float distance = length(spot_lights[i].position - frag_pos);
        float attenuation = 1.0 / (spot_lights[i].constant  + spot_lights[i].linear * distance + spot_lights[i].quadratic * pow(distance, 2));

        diffuse += spot_lights[i].diffuse * spot_lights[i].color * diff * attenuation * (1.0 - shadow) * diffuse_color;
        specular += spot_lights[i].specular *  spot_lights[i].color * spec * attenuation * (1.0 - shadow) * diffuse_color;
    }

    return diffuse + specular;
}
