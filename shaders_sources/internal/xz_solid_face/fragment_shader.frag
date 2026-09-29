#version 460 core

struct DirectionalLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    vec3 direction;
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
};

const float MAX_GRID_VISIBILITY_DISTANCE = 120.0;
const uint MAX_LIGHTS = 10;
const vec3 BASE_COLOR = vec3(1.0);
const float BIAS = 0.005;

in VSOut {
    vec3 frag_pos;
    vec3 normal;
    vec4 frag_pos_light_space;
} vs_out;

out vec4 FragColor;

uniform sampler2DArray u_directional_lights_shadow_map_array;
uniform vec3 u_camera_pos;
uniform DirectionalLight directional_lights[MAX_LIGHTS];
uniform PointLight point_lights[MAX_LIGHTS];
uniform SpotLight spot_lights[MAX_LIGHTS];

uniform int u_directional_lights_count;
uniform int u_point_lights_count;
uniform int u_spot_lights_count;

vec3 process_directional_lights(vec3 unit_normal, vec3 view_dir);
vec3 process_point_lights(vec3 unit_normal, vec3 view_dir);
vec3 process_spot_lights(vec3 unit_normal, vec3 view_dir);

float ShadowCalculation(vec4 fragPosLightSpace, uint layer, vec3 light_dir, vec3 frag_normal)
{
    vec3 ndc_coords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    ndc_coords = ndc_coords * 0.5 + 0.5;
    
    if (ndc_coords.z > 1.0 || ndc_coords.x < 0.0 || ndc_coords.x > 1.0 || ndc_coords.y < 0.0 || ndc_coords.y > 1.0) {
        return 0.0;
    }

    float closest_depth = texture(u_directional_lights_shadow_map_array, vec3(ndc_coords.xy, layer)).r;
    float curr_depth = ndc_coords.z;

    return curr_depth - BIAS > closest_depth ? 1.0 : 0.0;
}

void main() {

    float dist_to_camera = length(vs_out.frag_pos.xz - u_camera_pos.xz);
    float fade_alpha = 1.0 - smoothstep(20.0, MAX_GRID_VISIBILITY_DISTANCE, dist_to_camera);
    if (fade_alpha <= 0.0) discard;


    vec3 view_dir = normalize(u_camera_pos - vs_out.frag_pos);
    vec3 unit_normal = normalize(vs_out.normal);    

    vec3 direction_lights_color = process_directional_lights(unit_normal, view_dir);
    vec3 point_lights_color = process_point_lights(unit_normal, view_dir);
    vec3 spot_lights_color = process_spot_lights(unit_normal, view_dir);

    vec3 result = (direction_lights_color + point_lights_color + spot_lights_color) * BASE_COLOR + BASE_COLOR * 0.1;

    FragColor = vec4(result, fade_alpha);
}

vec3 process_directional_lights(vec3 unit_normal, vec3 view_dir) {
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    for (int i = 0; i < u_directional_lights_count; ++i) {
        float shadow = ShadowCalculation(vs_out.frag_pos_light_space, i, directional_lights[i].direction, unit_normal);

        ambient += directional_lights[i].ambient * directional_lights[i].color;

        float diff = max(dot(unit_normal, -directional_lights[i].direction), 0.0);

        vec3 reflect_dir = reflect(-directional_lights[i].direction, unit_normal);
        float spec = pow(max(dot(unit_normal, reflect_dir), 0.0), 64);


        diffuse += diff * directional_lights[i].color * (1.0 - shadow);
        specular += spec * directional_lights[i].color * (1.0 - shadow);
    }
    
    return ambient + diffuse + specular;
}

vec3 process_point_lights(vec3 unit_normal, vec3 view_dir) {
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    for (int i = 0; i < u_point_lights_count; ++i) {

        vec3 light_dir = normalize(point_lights[i].position - vs_out.frag_pos);
        float diff = max(dot(unit_normal, light_dir), 0.0);

        vec3 reflect_dir = reflect(-light_dir, unit_normal);
        float spec = pow(max(dot(unit_normal, reflect_dir), 0.0), 64);

        float distance = length(point_lights[i].position - vs_out.frag_pos);
        float attenuation = 1.0 / (point_lights[i].constant  + point_lights[i].linear * distance + point_lights[i].quadratic * pow(distance, 2));

        ambient += point_lights[i].ambient * point_lights[i].color * attenuation;
        diffuse += point_lights[i].diffuse * point_lights[i].color * diff * attenuation;
        specular += point_lights[i].specular * point_lights[i].color * spec * attenuation;
    }

    return ambient + diffuse + specular;
}

vec3 process_spot_lights(vec3 unit_normal, vec3 view_dir) {
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    

    for (int i = 0; i < u_spot_lights_count; ++i) {
        float shadow = ShadowCalculation(vs_out.frag_pos_light_space, i, spot_lights[i].direction, unit_normal);

        vec3 light_dir = normalize(spot_lights[i].position - vs_out.frag_pos); // direction of the light ray, not spot light itself
        float diff = max(dot(unit_normal, light_dir), 0.0); 

        vec3 reflect_dir = reflect(-light_dir, unit_normal);
        // vec3 halfway_dir = normalize(light_dir + view_dir);
        float spec = pow(max(dot(unit_normal, reflect_dir), 0.0), 64);

        // problem
        float theta = dot(light_dir, -spot_lights[i].direction);
        float epsilon = spot_lights[i].cut_off - spot_lights[i].outer_cut_off;
        float intensity = clamp((theta - spot_lights[i].outer_cut_off) / epsilon, 0.0, 1.0);
        diff *= intensity;
        spec *= intensity;

        float distance = length(spot_lights[i].position - vs_out.frag_pos);
        float attenuation = 1.0 / (spot_lights[i].constant  + spot_lights[i].linear * distance + spot_lights[i].quadratic * pow(distance, 2));

        ambient += spot_lights[i].ambient * spot_lights[i].color * attenuation;
        diffuse += spot_lights[i].diffuse * spot_lights[i].color * diff * attenuation * (1.0 - shadow);
        specular += spot_lights[i].specular *  spot_lights[i].color * spec * attenuation * (1.0 - shadow);
    }

    return ambient + diffuse + specular;
}