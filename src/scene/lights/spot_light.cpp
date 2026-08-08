#include "scene/lights/spot_light.h"

using namespace scene::lights::lighting_constants;

scene::lights::SpotLight::SpotLight(
    const std::string &name, 
    const std::weak_ptr<scene::physical_objects::LightModelObject> &cone_model, 
    glm::vec3 ambient, 
    glm::vec3 diffuse, 
    glm::vec3 specular, 
    glm::vec3 rgb_color, 
    glm::vec3 position, 
    glm::vec3 direction_, 
    float constant, 
    float linear, 
    float quadratic, 
    float cut_off_rad, 
    float outer_cut_off_rad
)
    : LightBase(ambient, diffuse, specular, rgb_color)
    , position_(position)
    , direction_(direction_)
    , name_(name)
    , type_(LightType::SPOT_LIGHT)
    , light_cone_(cone_model)
    , constant_(constant)
    , linear_(linear)
    , quadratic_(quadratic)
    , cosine_cut_off_rad_(glm::cos(cut_off_rad))
    , cosine_outer_cut_off_rad_(glm::cos(outer_cut_off_rad))
{
}

void scene::lights::SpotLight::set_position(glm::vec3 position)
{
    position_ = position;
}

glm::vec3 scene::lights::SpotLight::get_position() const
{
    return position_;
}

void scene::lights::SpotLight::set_direction(glm::vec3 direction)
{
    direction_ = glm::normalize(direction);
}

glm::vec3 scene::lights::SpotLight::get_direction() const
{
    return direction_;
}

void scene::lights::SpotLight::set_constant(float value)
{
    constant_ = std::min(value, POINT_LIGHT_MIN_CONSTANT);
}

float scene::lights::SpotLight::get_constant() const
{
    return constant_;
}

void scene::lights::SpotLight::set_linear(float value)
{
    linear_ = std::min(value, POINT_LIGHT_MIN_LINEAR);
}

float scene::lights::SpotLight::get_linear() const
{
    return linear_;
}

void scene::lights::SpotLight::set_quadratic(float value)
{
    quadratic_ = std::min(value, POINT_LIGHT_MIN_QUADRATIC);
}

float scene::lights::SpotLight::get_quadratic() const
{
    return quadratic_;
}

void scene::lights::SpotLight::set_cut_off(float degrees)
{
    cosine_cut_off_rad_ = glm::cos(glm::radians(std::clamp(degrees, SPOT_MIN_CUTOFF, SPOT_MAX_CUTOFF)));
}

float scene::lights::SpotLight::get_cut_off() const
{
    return glm::degrees(glm::acos(cosine_cut_off_rad_));
}

void scene::lights::SpotLight::set_outer_cut_off(float degrees)
{
    cosine_outer_cut_off_rad_ = glm::cos(glm::radians(std::clamp(degrees, SPOT_MIN_CUTOFF, SPOT_MAX_CUTOFF)));
}

float scene::lights::SpotLight::get_outer_cut_off() const
{
    return glm::degrees(glm::acos(cosine_outer_cut_off_rad_));
}

std::weak_ptr<scene::physical_objects::LightModelObject> scene::lights::SpotLight::get_model_ptr() const
{
    return light_cone_;
}
std::string scene::lights::SpotLight::get_name() const
{
    return name_;
}

scene::lights::LightType scene::lights::SpotLight::get_type() const
{
    return type_;
}

void scene::lights::SpotLight::upload_components(Shader &obj_shader, size_t light_idx) const
{
    std::string uniform_name = "spot_lights[" + std::to_string(light_idx) + "]";
    obj_shader.set_uniform(uniform_name + ".ambient", get_ambient());
    obj_shader.set_uniform(uniform_name + ".diffuse", get_diffuse());
    obj_shader.set_uniform(uniform_name + ".specular", get_specular());
    obj_shader.set_uniform(uniform_name + ".color", get_color());
    obj_shader.set_uniform(uniform_name + ".position", position_);
    obj_shader.set_uniform(uniform_name + ".direction", direction_);
    obj_shader.set_uniform(uniform_name + ".constant", constant_);
    obj_shader.set_uniform(uniform_name + ".linear", linear_);
    obj_shader.set_uniform(uniform_name + ".quadratic", quadratic_);
    obj_shader.set_uniform(uniform_name + ".cut_off", cosine_cut_off_rad_);
    obj_shader.set_uniform(uniform_name + ".outer_cut_off", cosine_outer_cut_off_rad_);
}

