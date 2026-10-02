#include "scene/lights/spot_light.h"
#include "renderer/renderer.h"
#include <cassert>

using namespace scene::lights::lighting_constants;

scene::lights::SpotLight::SpotLight(
    const std::string &name,
    const std::shared_ptr<scene::physical_objects::LightModelObject> &cone_model,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    glm::vec3 rgb_color,
    glm::vec3 position,
    glm::vec3 direction_,
    float linear,
    float quadratic,
    float cut_off_rad,
    float outer_cut_off_rad)
    : LightBase(ambient, diffuse, specular, rgb_color)
    , position_(position)
    , direction_(direction_)
    , name_(name)
    , type_(LightType::SPOT_LIGHT)
    , light_cone_(cone_model)
    , linear_(linear)
    , quadratic_(quadratic)
    , cosine_cut_off_rad_(glm::cos(cut_off_rad))
    , cosine_outer_cut_off_rad_(glm::cos(outer_cut_off_rad))
    , proj_mat_(1.0f)
{
}

float scene::lights::SpotLight::calculate_far_plane(float threshold) const
{
    // solving quadratic equation
    float a = quadratic_;
    float b = linear_;
    float c = constant_ - (1.0f / threshold);

    if (std::abs(a) < 1e-6f) {
        if (std::abs(b) < 1e-6f) {
            return 100.0f; // fallback if no attenuation is defined
        }
        return -c / b;
    }

    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f) {
        return 100.0f;
    }

    float distance = (-b + std::sqrt(discriminant)) / (2.0f * a);
    
    return std::max(distance, 1.0f);
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

void scene::lights::SpotLight::set_linear(float value)
{
    linear_ = glm::clamp(value, MIN_LIGHT_LINEAR, MAX_LIGHT_LINEAR);
}

float scene::lights::SpotLight::get_linear() const
{
    return linear_;
}

void scene::lights::SpotLight::set_quadratic(float value)
{
    quadratic_ = glm::clamp(value, MIN_LIGHT_QUADRATIC, MAX_LIGHT_QUADRATIC);
}

float scene::lights::SpotLight::get_quadratic() const
{
    return quadratic_;
}

void scene::lights::SpotLight::set_cut_off(float degrees)
{
    cosine_cut_off_rad_ = glm::cos(glm::radians(std::clamp(degrees, MIN_SPOT_CUTOFF, MAX_SPOT_CUTOFF)));
}

float scene::lights::SpotLight::get_cut_off() const
{
    return glm::degrees(glm::acos(cosine_cut_off_rad_));
}

void scene::lights::SpotLight::set_outer_cut_off(float degrees)
{
    cosine_outer_cut_off_rad_ = glm::cos(glm::radians(std::clamp(degrees, MIN_SPOT_CUTOFF, MAX_SPOT_CUTOFF)));
}

float scene::lights::SpotLight::get_outer_cut_off() const
{
    return glm::degrees(glm::acos(cosine_outer_cut_off_rad_));
}

glm::mat4 scene::lights::SpotLight::view_matrix() const
{
    if (glm::abs(glm::dot(renderer::defaults::WORLD_UP, direction_)) > 0.999f) {
        // the dir and world_up vectors are collinear, fallback
        return glm::lookAt(position_, position_ + direction_, renderer::defaults::FALLBACK_WORLD_UP);
    }    
    
    return glm::lookAt(position_, position_+direction_, renderer::defaults::WORLD_UP);
}

glm::mat4 scene::lights::SpotLight::projection_matrix(float framebuffer_width, float framebuffer_height) const
{
    assert(framebuffer_height > 0.0f);
    assert(framebuffer_width > 0.0f);

    // bad idea, but in case we do not store last calculated mat
    // we need to pass framebuffer width and height during uploading light's components

    proj_mat_ = glm::perspective(2 * glm::acos(cosine_outer_cut_off_rad_), framebuffer_width / framebuffer_height, lighting_constants::SPOT_LIGHT_DEFAULT_NEAR_PLANE, calculate_far_plane());
    return proj_mat_;
}

void scene::lights::SpotLight::set_model_ptr(const std::shared_ptr<scene::physical_objects::LightModelObject> &value)
{
    light_cone_ = value;
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
    obj_shader.set_uniform(uniform_name + ".constant", LIGHT_CONSTANT);
    obj_shader.set_uniform(uniform_name + ".linear", linear_);
    obj_shader.set_uniform(uniform_name + ".quadratic", quadratic_);
    obj_shader.set_uniform(uniform_name + ".cut_off", cosine_cut_off_rad_);
    obj_shader.set_uniform(uniform_name + ".outer_cut_off", cosine_outer_cut_off_rad_);
    obj_shader.set_uniform(uniform_name + ".light_space_mat", proj_mat_ * view_matrix());
}

