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
    if (rgb_color.r < MIN_COLOR || rgb_color.r > MAX_COLOR ||
        rgb_color.g < MIN_COLOR || rgb_color.g > MAX_COLOR ||
        rgb_color.b < MIN_COLOR || rgb_color.b > MAX_COLOR
    ) {
        spdlog::warn("RGB color component out of range, clamping to fit range. Passed value: r={}, g={}, b={}", rgb_color.r, rgb_color.g, rgb_color.b);
        color_rgb_ = glm::clamp(rgb_color, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
        return;
    }

    color_rgb_ = rgb_color;
}

glm::vec3 scene::lights::LightBase::get_color() const
{
    return color_rgb_;
}

void scene::lights::LightBase::set_ambient(glm::vec3 value)
{
    if (value.r < MIN_LIGHTING_COMPONENT_VALUE || value.r > MAX_LIGHTING_COMPONENT_VALUE ||
        value.g < MIN_LIGHTING_COMPONENT_VALUE || value.g > MAX_LIGHTING_COMPONENT_VALUE ||
        value.b < MIN_LIGHTING_COMPONENT_VALUE || value.b > MAX_LIGHTING_COMPONENT_VALUE
    ) {
        spdlog::warn("Ambient component out of range, clamping to fit range. Passed value: r={}, g={}, b={}", value.r, value.g, value.b);
        ambient_ = glm::clamp(value, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
        return;
    }

    ambient_ = value;
}

glm::vec3 scene::lights::LightBase::get_ambient() const
{
    return ambient_;
}

void scene::lights::LightBase::set_diffuse(glm::vec3 value)
{
    if (value.r < MIN_LIGHTING_COMPONENT_VALUE || value.r > MAX_LIGHTING_COMPONENT_VALUE ||
        value.g < MIN_LIGHTING_COMPONENT_VALUE || value.g > MAX_LIGHTING_COMPONENT_VALUE ||
        value.b < MIN_LIGHTING_COMPONENT_VALUE || value.b > MAX_LIGHTING_COMPONENT_VALUE
    ) {
        spdlog::warn("Diffuse component out of range, clamping to fit range. Passed value: r={}, g={}, b={}", value.r, value.g, value.b);
        diffuse_ = glm::clamp(value, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
        return;
    }

    diffuse_ = value;
}

glm::vec3 scene::lights::LightBase::get_diffuse() const
{
    return diffuse_;
}

void scene::lights::LightBase::set_specular(glm::vec3 value)
{
    if (value.r < MIN_LIGHTING_COMPONENT_VALUE || value.r > MAX_LIGHTING_COMPONENT_VALUE ||
        value.g < MIN_LIGHTING_COMPONENT_VALUE || value.g > MAX_LIGHTING_COMPONENT_VALUE ||
        value.b < MIN_LIGHTING_COMPONENT_VALUE || value.b > MAX_LIGHTING_COMPONENT_VALUE
    ) {
        spdlog::warn("Specular component out of range, clamping to fit range. Passed value: r={}, g={}, b={}", value.r, value.g, value.b);
        specular_ = glm::clamp(value, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
        return;
    }

    specular_ = value;
}

glm::vec3 scene::lights::LightBase::get_specular() const
{
    return specular_;
}
