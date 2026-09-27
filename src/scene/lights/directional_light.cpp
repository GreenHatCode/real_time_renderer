#include "scene/lights/directional_light.h"
#include "renderer/renderer.h"

scene::lights::DirectionalLight::DirectionalLight(
    const std::string& name, 
    glm::vec3 ambient, 
    glm::vec3 diffuse, 
    glm::vec3 specular, 
    glm::vec3 rgb_color, 
    glm::vec3 direction
)
    : LightBase(ambient, diffuse, specular, rgb_color)
    , direction_(glm::normalize(direction))
    , name_(name)
    , type_(LightType::DIRECTIONAL_LIGHT)
{
}

void scene::lights::DirectionalLight::set_direction(glm::vec3 direction)
{
    direction_ = glm::normalize(direction);
}

glm::vec3 scene::lights::DirectionalLight::get_direction() const
{
    return direction_;
}

glm::mat4 scene::lights::DirectionalLight::view_matrix() const
{
    glm::vec3 position = -direction_ * DISTANCE_FROM_DIRECTION_PRESCALER;

    if (glm::abs(glm::dot(renderer::defaults::WORLD_UP, direction_)) > 0.999f) {
        // the dir and world_up vectors are collinear, fallback
        return glm::lookAt(position, LOOK_POINT, renderer::defaults::FALLBACK_WORLD_UP);
    }    
    
    return glm::lookAt(position, LOOK_POINT, renderer::defaults::WORLD_UP);
}

glm::mat4 scene::lights::DirectionalLight::projection_matrix(float framebuffer_width, float framebuffer_height) const
{
    return glm::ortho(
        -lighting_constants::LIGHTS_ORTHO_SIDE_SIZE, 
        lighting_constants::LIGHTS_ORTHO_SIDE_SIZE, 
        -lighting_constants::LIGHTS_ORTHO_SIDE_SIZE, 
        lighting_constants::LIGHTS_ORTHO_SIDE_SIZE,
        lighting_constants::SPOT_LIGHT_DEFAULT_NEAR_PLANE, 
        lighting_constants::DIRECTIONAL_LIGHT_DEFAULT_FAR_PLANE
    );
}

std::string scene::lights::DirectionalLight::get_name() const
{
    return name_;
}

scene::lights::LightType scene::lights::DirectionalLight::get_type() const
{
    return type_;
}

void scene::lights::DirectionalLight::upload_components(Shader& obj_shader, size_t light_idx) const
{
    std::string uniform_name = "directional_lights[" + std::to_string(light_idx) + "]";
    obj_shader.set_uniform(uniform_name + ".ambient", get_ambient());
    obj_shader.set_uniform(uniform_name + ".diffuse", get_diffuse());
    obj_shader.set_uniform(uniform_name + ".specular", get_specular());
    obj_shader.set_uniform(uniform_name + ".color", get_color());
    obj_shader.set_uniform(uniform_name + ".direction", direction_);
}
