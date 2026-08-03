#include "scene/lights/directional_light.h"

scene::lights::DirectionalLight::DirectionalLight(
    glm::vec3 ambient, 
    glm::vec3 diffuse, 
    glm::vec3 specular, 
    glm::vec4 rgba_color, 
    glm::vec3 direction
)
    : ISceneObject()
    , LightBase(ambient, diffuse, specular, rgba_color)
    , direction_(glm::normalize(direction))
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
