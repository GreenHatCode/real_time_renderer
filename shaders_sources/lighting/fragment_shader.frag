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

const uint MAX_LIGHTS = 10;

in vec3 v_normal;
in vec3 v_frag_pos;

out vec4 FragColor;

uniform vec3 u_object_color;

uniform DirectionalLight directional_lights[MAX_LIGHTS];
uniform PointLight point_lights[MAX_LIGHTS];
uniform SpotLight spot_lights[MAX_LIGHTS];

uniform int u_directional_lights_count;
uniform int u_point_lights_count;
uniform int u_spot_lights_count;

vec3 process_directional_lights();
vec3 process_point_lights();
vec3 process_spot_lights();

void main() {

    vec3 direction_lights_color = process_directional_lights();
    vec3 point_lights_color = process_point_lights();
    vec3 spot_lights_color = process_spot_lights();

    vec3 result = (direction_lights_color + point_lights_color + spot_lights_color) * u_object_color;
    FragColor = vec4(result, 1.0);
}

vec3 process_directional_lights() {
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 unit_normal = normalize(v_normal);
    vec3 view_dir = normalize(-v_frag_pos);

    for (int i = 0; i < u_directional_lights_count; ++i) {
        ambient += directional_lights[i].ambient * directional_lights[i].color;

        float diff = max(dot(unit_normal, -directional_lights[i].direction), 0.0);
        diffuse += diff * directional_lights[i].color;

        vec3 reflect_dir = reflect(-directional_lights[i].direction, unit_normal);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 64);
        specular += spec * directional_lights[i].color;
    }
    
    return ambient + diffuse + specular;
}

vec3 process_point_lights() {
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 unit_normal = normalize(v_normal);
    vec3 view_dir = normalize(-v_frag_pos);

    for (int i = 0; i < u_point_lights_count; ++i) {

        vec3 light_dir = normalize(point_lights[i].position - v_frag_pos);
        float diff = max(dot(unit_normal, light_dir), 0.0);

        vec3 reflect_dir = reflect(-light_dir, unit_normal);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 64);

        float distance = length(point_lights[i].position - v_frag_pos);
        float attenuation = 1.0 / (point_lights[i].constant  + point_lights[i].linear * distance + point_lights[i].quadratic * pow(distance, 2));

        ambient += point_lights[i].ambient * point_lights[i].color * attenuation;
        diffuse += point_lights[i].diffuse * point_lights[i].color * diff * attenuation;
        specular += point_lights[i].specular * point_lights[i].color * spec * attenuation;
    }

    return ambient + diffuse + specular;
}

vec3 process_spot_lights() {
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 unit_normal = normalize(v_normal);
    vec3 view_dir = normalize(-v_frag_pos);

    for (int i = 0; i < u_spot_lights_count; ++i) {

        vec3 light_dir = normalize(spot_lights[i].position - v_frag_pos); // direction of the light ray, not spot light itself
        float diff = max(dot(unit_normal, light_dir), 0.0); 

        vec3 reflect_dir = reflect(-light_dir, unit_normal);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 64);

        // problem
        float theta = dot(light_dir, normalize(-spot_lights[i].direction));
        float epsilon = spot_lights[i].cut_off - spot_lights[i].outer_cut_off;
        float intensity = clamp((theta - spot_lights[i].outer_cut_off) / epsilon, 0.0, 1.0);
        diff *= intensity;
        spec *= intensity;


        float distance = length(spot_lights[i].position - v_frag_pos);
        float attenuation = 1.0 / (spot_lights[i].constant  + spot_lights[i].linear * distance + spot_lights[i].quadratic * pow(distance, 2));

        ambient += spot_lights[i].ambient * spot_lights[i].color * attenuation;
        diffuse += spot_lights[i].diffuse * spot_lights[i].color * diff * attenuation;
        specular += spot_lights[i].specular *  spot_lights[i].color * spec * attenuation;
    }

    return ambient + diffuse + specular;
}