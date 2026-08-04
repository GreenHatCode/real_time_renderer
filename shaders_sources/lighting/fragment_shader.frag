#version 460 core

struct DirectionalLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    vec3 direction;
};

const uint MAX_DIRECTION_LIGHTS = 10;

in vec3 v_normal;
in vec3 v_frag_pos;

out vec4 FragColor;

uniform vec3 u_object_color;

uniform DirectionalLight directional_lights[MAX_DIRECTION_LIGHTS];

uniform int u_directional_lights_count;



vec3 process_directional_lights();

void main() {

    vec3 direction_lights_color = process_directional_lights();

    vec3 result = (direction_lights_color) * u_object_color;
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