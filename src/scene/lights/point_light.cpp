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
    float constant, 
    float linear, 
    float quadratic
)
    : LightBase(ambient, diffuse, specular, rgb_color)
    , position_(position)
    , name_(name)
    , type_(LightType::POINT_LIGHT)
    , light_sphere_(sphere_model)
    , constant_(constant)
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

void scene::lights::PointLight::set_constant(float value)
{
    if (value < POINT_LIGHT_MIN_CONSTANT) {
        spdlog::warn("Constant component out of range, clamping to fit range. Passed value: {}", value);
        constant_ = std::min(value, POINT_LIGHT_MIN_CONSTANT);
        return;
    }

    constant_ = value;
}

float scene::lights::PointLight::get_constant() const
{
    return constant_;
}

void scene::lights::PointLight::set_linear(float value)
{
    if (value < POINT_LIGHT_MIN_LINEAR) {
        spdlog::warn("Linear component out of range, clamping to fit range. Passed value: {}", value);
        linear_ = std::min(value, POINT_LIGHT_MIN_LINEAR);
        return;
    }

    linear_ = value;
}

float scene::lights::PointLight::get_linear() const
{
    return linear_;
}

void scene::lights::PointLight::set_quadratic(float value)
{
    if (value < POINT_LIGHT_MIN_QUADRATIC) {
        spdlog::warn("Quadratic component out of range, clamping to fit range. Passed value: {}", value);
        quadratic_ = std::min(value, POINT_LIGHT_MIN_QUADRATIC);
        return;
    }

    quadratic_ = value;
}

float scene::lights::PointLight::get_quadratic() const
{
    return quadratic_;
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
    obj_shader.set_uniform(uniform_name + ".constant", constant_);
    obj_shader.set_uniform(uniform_name + ".linear", linear_);
    obj_shader.set_uniform(uniform_name + ".quadratic", quadratic_);
}
