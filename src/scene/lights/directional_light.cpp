#include "scene/lights/directional_light.h"

scene::lights::DirectionalLight::DirectionalLight(
    glm::vec3 ambient, 
    glm::vec3 diffuse, 
    glm::vec3 specular, 
    glm::vec3 rgb_color, 
    glm::vec3 direction
)
    : LightBase(ambient, diffuse, specular, rgb_color)
    , direction_(glm::normalize(direction))
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
