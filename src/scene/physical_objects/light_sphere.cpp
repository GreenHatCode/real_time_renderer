#include "scene/physical_objects/light_sphere.h"
#include "scene/lights/light_base.h"

using namespace scene::physical_objects::physical_light_object_constants;

scene::physical_objects::LightModelObject::LightModelObject(const std::string &object_name, const std::filesystem::path& file_path, glm::vec3 color, const std::shared_ptr<Shader>& shader)
    : model_(assimp_wrapper::Model(file_path,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_ValidateDataStructure))
    , name_(object_name)
    , shader_(shader)
    , color_(1.0f, 1.0f, 1.0f)
{
    set_color(color);
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
