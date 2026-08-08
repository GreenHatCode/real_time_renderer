#include "scene/lights/light_base.h"
#include "spdlog/spdlog.h"

using namespace scene::lights::lighting_constants;

scene::lights::LightBase::LightBase(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 rgb_color)
{
    set_ambient(ambient);
    set_diffuse(diffuse);
    set_specular(specular);
    set_color(rgb_color);
}

void scene::lights::LightBase::set_color(glm::vec3 rgb_color)
{
    color_rgb_ = glm::clamp(rgb_color, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
}

glm::vec3 scene::lights::LightBase::get_color() const
{
    return color_rgb_;
}

void scene::lights::LightBase::set_ambient(glm::vec3 value)
{
    ambient_ = glm::clamp(value, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
}

glm::vec3 scene::lights::LightBase::get_ambient() const
{
    return ambient_;
}

void scene::lights::LightBase::set_diffuse(glm::vec3 value)
{
    diffuse_ = glm::clamp(value, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
}

glm::vec3 scene::lights::LightBase::get_diffuse() const
{
    return diffuse_;
}

void scene::lights::LightBase::set_specular(glm::vec3 value)
{
    specular_ = glm::clamp(value, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
}

glm::vec3 scene::lights::LightBase::get_specular() const
{
    return specular_;
}
