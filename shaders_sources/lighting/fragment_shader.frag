#version 460 core

in vec3 o_normal;
in vec3 o_frag_pos;

out vec4 FragColor;

uniform vec3 u_object_color;
uniform vec3 u_light_pos;
uniform vec3 u_light_color;

void main() {
    
    float ambient_strength = 0.1;
    float specular_intensity = 0.5;

    // ambient color
    vec3 ambient_color = 0.1 * u_light_color;

    // diffuse color
    vec3 unit_normal = normalize(o_normal);
    vec3 light_direction = normalize(u_light_pos - o_frag_pos);

    float diff = max(dot(unit_normal, light_direction), 0.0);
    vec3 diffuse_color = diff * u_light_color;

    // specular color
    vec3 view_direction = normalize(-o_frag_pos);
    vec3 reflection_direction = reflect(-light_direction, unit_normal);
    float spec = pow(max(dot(view_direction, reflection_direction), 0.0), 32);
    vec3 specular = specular_intensity * spec * u_light_color;


    vec3 result = (ambient_color + diffuse_color + specular) * u_object_color;
    FragColor = vec4(result, 1.0);
}