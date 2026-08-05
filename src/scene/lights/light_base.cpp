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
        spdlog::error("RGB color component out of range, clamping to fit range. Passed value: r={}, g={}, b={}", rgb_color.r, rgb_color.g, rgb_color.b);
        std::clamp(rgb_color.r, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
        std::clamp(rgb_color.g, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
        std::clamp(rgb_color.b, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);

        color_rgb_ = rgb_color;
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
        spdlog::error("Ambient component out of range, clamping to fit range. Passed value: r={}, g={}, b={}", value.r, value.g, value.b);
        std::clamp(value.r, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
        std::clamp(value.g, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
        std::clamp(value.b, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);

        ambient_ = value;
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
        spdlog::error("Diffuse component out of range, clamping to fit range. Passed value: r={}, g={}, b={}", value.r, value.g, value.b);
        std::clamp(value.r, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
        std::clamp(value.g, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
        std::clamp(value.b, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);

        diffuse_ = value;
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
        spdlog::error("Specular component out of range, clamping to fit range. Passed value: r={}, g={}, b={}", value.r, value.g, value.b);
        std::clamp(value.r, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
        std::clamp(value.g, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);
        std::clamp(value.b, MIN_LIGHTING_COMPONENT_VALUE, MAX_LIGHTING_COMPONENT_VALUE);

        specular_ = value;
        return;
    }

    specular_ = value;
}

glm::vec3 scene::lights::LightBase::get_specular() const
{
    return specular_;
}
