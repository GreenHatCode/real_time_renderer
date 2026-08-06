#include "scene/physical_objects/light_sphere.h"
#include "scene/lights/light_base.h"

namespace
{
    constexpr std::string_view kLightSphereModel = "resources/sphere.obj";
    constexpr std::string_view kLightVertexShader = "shaders_sources/light_object/light.vert";
    constexpr std::string_view kLightFragmentShader = "shaders_sources/light_object/light.frag";
}

scene::physical_objects::LightModelObject::LightModelObject(const std::string &object_name)
    : model_(assimp_wrapper::Model(kLightSphereModel,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_ValidateDataStructure))
    , name_(object_name)
    , shader_(std::make_shared<Shader>("light_object_shader", kLightVertexShader.data(), kLightFragmentShader.data()))
{
}

uint32_t scene::physical_objects::LightModelObject::get_vertices_count() const
{
    return model_.get_vertices_count();
}

uint32_t scene::physical_objects::LightModelObject::get_faces_count() const
{
    return model_.get_faces_count();
}

void scene::physical_objects::LightModelObject::set_shader(const std::shared_ptr<Shader> &shader)
{
    shader_ = shader;
}

std::shared_ptr<Shader> scene::physical_objects::LightModelObject::get_shader() const
{
    return shader_;
}

std::string scene::physical_objects::LightModelObject::get_name() const
{
    return name_;
}

void scene::physical_objects::LightModelObject::render() const
{
    glm::mat4 model = model_matrix();
    shader_->set_uniform("u_model_mat", model);

    model = glm::inverse(model);
    model = glm::transpose(model);
    shader_->set_uniform("u_normal_mat", glm::mat3(model));

    shader_->set_uniform("u_light_object_color", color_);
    model_.draw(*shader_);
}

void scene::physical_objects::LightModelObject::set_color(glm::vec3 color)
{
    color_ = glm::clamp(color, scene::lights::lighting_constants::MIN_LIGHTING_COMPONENT_VALUE, scene::lights::lighting_constants::MAX_LIGHTING_COMPONENT_VALUE);
}

void scene::physical_objects::LightModelObject::move_to(glm::vec3 target)
{
    glm::vec3 move_vec = target - get_move_vector();
    move(move_vec);
}
