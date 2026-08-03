#include "scene/lights/light_base.h"
#include <stdexcept>
#include <sstream>

using namespace scene::lights::lighting_constants;

scene::lights::LightBase::LightBase(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec4 rgba_color)
{
    set_ambient(ambient);
    set_diffuse(diffuse);
    set_specular(specular);
    set_color(rgba_color);
}

void scene::lights::LightBase::set_color(glm::vec4 rgba_color)
{
    if (rgba_color.r < MIN_COLOR || rgba_color.r > MAX_COLOR ||
        rgba_color.g < MIN_COLOR || rgba_color.g > MAX_COLOR ||
        rgba_color.b < MIN_COLOR || rgba_color.b > MAX_COLOR ||
        rgba_color.a < MIN_COLOR || rgba_color.a > MAX_COLOR
    ) {
        std::stringstream ss;
        ss << "rgba_color out of range. Passed value:" 
        << " r=" << rgba_color.r 
        << " g=" << rgba_color.g
        << " b=" << rgba_color.b
        << " a=" << rgba_color.a;
        throw std::out_of_range(ss.str());
    }

    color_rgba_ = rgba_color;
}

glm::vec4 scene::lights::LightBase::get_color() const
{
    return color_rgba_;
}

void scene::lights::LightBase::set_ambient(glm::vec3 value)
{
    if (value.r < MIN_LIGHTING_COMPONENT_VALUE || value.r > MAX_LIGHTING_COMPONENT_VALUE ||
        value.g < MIN_LIGHTING_COMPONENT_VALUE || value.g > MAX_LIGHTING_COMPONENT_VALUE ||
        value.b < MIN_LIGHTING_COMPONENT_VALUE || value.b > MAX_LIGHTING_COMPONENT_VALUE
    ) {
        std::stringstream ss;
        ss << "Ambient component out of range. Passed value:" 
        << " r=" << value.r 
        << " g=" << value.g
        << " b=" << value.b;
        throw std::out_of_range(ss.str());
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
        std::stringstream ss;
        ss << "Diffuse component out of range. Passed value:" 
        << " r=" << value.r 
        << " g=" << value.g
        << " b=" << value.b;
        throw std::out_of_range(ss.str());
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
        std::stringstream ss;
        ss << "Specular component out of range. Passed value:" 
        << " r=" << value.r 
        << " g=" << value.g
        << " b=" << value.b;
        throw std::out_of_range(ss.str());
    }

    specular_ = value;
}

glm::vec3 scene::lights::LightBase::get_specular() const
{
    return specular_;
}
