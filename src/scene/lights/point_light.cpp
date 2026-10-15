#include "scene/lights/point_light.h"
#include "spdlog/spdlog.h"

using namespace scene::lights::lighting_constants;

float scene::lights::PointLight::calculate_far_plane(float threshold) const
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

scene::lights::PointLight::PointLight(
    const std::string &name,
    const std::weak_ptr<scene::physical_objects::LightModelObject> &sphere_model,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    glm::vec3 rgb_color,
    glm::vec3 position,
    float linear,
    float quadratic)
    : LightBase(ambient, diffuse, specular, rgb_color), position_(position), name_(name), type_(LightType::POINT_LIGHT), light_sphere_(sphere_model), linear_(linear), quadratic_(quadratic)
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

size_t scene::lights::PointLight::get_view_count() const
{
    return VIEW_COUNT;
}

glm::mat4 scene::lights::PointLight::view_matrix(size_t face_idx) const
{
    switch (face_idx) {
        case 0: // +X
            return glm::lookAt(position_, position_ + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
        case 1: // -X
            return glm::lookAt(position_, position_ + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
        case 2: // +Y
            return glm::lookAt(position_, position_ + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f));
        case 3: // -Y
            return glm::lookAt(position_, position_ + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f));
        case 4: // +Z
            return glm::lookAt(position_, position_ + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
        case 5: // -Z
            return glm::lookAt(position_, position_ + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
        default:
            assert(false && "Invalid cubemap face index");
            return glm::mat4(1.0f);
    }

}

glm::mat4 scene::lights::PointLight::projection_matrix(float framebuffer_width, float framebuffer_height) const
{
    assert(framebuffer_height > 0.0f);
    assert(framebuffer_width > 0.0f);

    // bad idea, but in case we do not store last calculated mat
    // we need to pass framebuffer width and height during uploading light's components

    proj_mat_ = glm::perspective(glm::radians(90.0f), framebuffer_width / framebuffer_height, lighting_constants::SPOT_LIGHT_DEFAULT_NEAR_PLANE, calculate_far_plane());
    return proj_mat_;
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
    obj_shader.set_uniform(uniform_name + ".far_plane", calculate_far_plane());
    obj_shader.set_uniform(uniform_name + ".light_space_mat", proj_mat_ * view_matrix());

}
