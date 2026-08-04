#include "scene/lights/light_base.h"
#include <stdexcept>
#include <sstream>

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
        std::stringstream ss;
        ss << "rgb_color out of range. Passed value:" 
        << " r=" << rgb_color.r 
        << " g=" << rgb_color.g
        << " b=" << rgb_color.b;
        throw std::out_of_range(ss.str());
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
