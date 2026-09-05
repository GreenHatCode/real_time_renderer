
Here listed all uniforms that renderer can pass to shaders

// --- Fragment Shader ---
// lighting
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

uniform DirectionalLight directional_lights[MAX_LIGHTS];
uniform PointLight point_lights[MAX_LIGHTS];
uniform SpotLight spot_lights[MAX_LIGHTS];

uniform int u_directional_lights_count;
uniform int u_point_lights_count;
uniform int u_spot_lights_count;

uniform vec3 u_light_pos;
uniform vec3 u_light_color;
uniform vec4 u_base_color;
uniform float u_opacity;
uniform vec3 u_camera_pos;


// textures
uniform sampler2D texture_diffuse[MAX_TEXTURES_NUMBER];
uniform sampler2D texture_specular[MAX_TEXTURES_NUMBER];
uniform sampler2D texture_height[MAX_TEXTURES_NUMBER];
uniform sampler2D texture_ambient[MAX_TEXTURES_NUMBER];

uniform int diffuse_count;
uniform int specular_count;
uniform int height_count;
uniform int ambient_count;






// --- Vertex Shader ---
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;

uniform mat4 u_model_mat;
uniform mat4 u_view_mat;
uniform mat4 u_projection_mat;
uniform mat3 u_normal_mat;