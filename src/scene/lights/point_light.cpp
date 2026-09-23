#include "scene/lights/point_light.h"
#include "spdlog/spdlog.h"

using namespace scene::lights::lighting_constants;

scene::lights::PointLight::PointLight(
    const std::string &name, 
    const std::weak_ptr<scene::physical_objects::LightModelObject>& sphere_model,
    glm::vec3 ambient, 
    glm::vec3 diffuse, 
    glm::vec3 specular, 
    glm::vec3 rgb_color, 
    glm::vec3 position, 
    float linear, 
    float quadratic
)
    : LightBase(ambient, diffuse, specular, rgb_color)
    , position_(position)
    , name_(name)
    , type_(LightType::POINT_LIGHT)
    , light_sphere_(sphere_model)
    , linear_(linear)
    , quadratic_(quadratic)
{
}

void scene::lights::PointLight::set_position(glm::vec3 position)
{
    position_ = position;
}

glm::vec3 scene::lights::PointLight::get_position() const
{
    return position_;
}

void scene::lights::PointLight::set_linear(float value)
{
    linear_ = glm::clamp(value, MIN_LIGHT_LINEAR, MAX_LIGHT_LINEAR);
}

float scene::lights::PointLight::get_linear() const
{
    return linear_;
}

void scene::lights::PointLight::set_quadratic(float value)
{
    quadratic_ = glm::clamp(value, MIN_LIGHT_QUADRATIC, MAX_LIGHT_QUADRATIC);
}

float scene::lights::PointLight::get_quadratic() const
{
    return quadratic_;
}

void scene::lights::PointLight::set_model_ptr(const std::weak_ptr<scene::physical_objects::LightModelObject> &value)
{
    light_sphere_ = value;
}

std::weak_ptr<scene::physical_objects::LightModelObject> scene::lights::PointLight::get_model_ptr() const
{
    return light_sphere_;
}
std::string scene::lights::PointLight::get_name() const
{
    return name_;
}

scene::lights::LightType scene::lights::PointLight::get_type() const
{
    return type_;
}

void scene::lights::PointLight::upload_components(Shader &obj_shader, size_t light_idx) const
{
    std::string uniform_name = "point_lights[" + std::to_string(light_idx) + "]";
    obj_shader.set_uniform(uniform_name + ".ambient", get_ambient());
    obj_shader.set_uniform(uniform_name + ".diffuse", get_diffuse());
    obj_shader.set_uniform(uniform_name + ".specular", get_specular());
    obj_shader.set_uniform(uniform_name + ".color", get_color());
    obj_shader.set_uniform(uniform_name + ".position", position_);
    obj_shader.set_uniform(uniform_name + ".constant", LIGHT_CONSTANT);
    obj_shader.set_uniform(uniform_name + ".linear", linear_);
    obj_shader.set_uniform(uniform_name + ".quadratic", quadratic_);
}
