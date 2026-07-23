#include "scene/scene_objects/scene_object.h"
#include "glm/gtc/matrix_transform.hpp"
#include "scene/scene_objects/scene_object.h"

scene::SceneObject::SceneObject(std::shared_ptr<Shader> shader)
    : move_vector_(0.0f, 0.0f, 0.0f)
    , rotation_angle_cw_(0.0f)
    , rotation_axes_(1.0f, 1.0f, 1.0f)
    , scaling_vec_(1.0f, 1.0f, 1.0f)
    , shader_(shader)
{
}

void scene::SceneObject::move(glm::vec3 move_vector)
{
    move_vector_ = move_vector;
}

void scene::SceneObject::rotate(float angle_rad, glm::vec3 axes_vec)
{
    rotation_angle_cw_ = angle_rad;
    rotation_axes_ = glm::normalize(axes_vec);
}

void scene::SceneObject::scale(glm::vec3 prescaling_vec)
{
    scaling_vec_ = prescaling_vec;
}

glm::vec3 scene::SceneObject::get_move_vector() const
{
    return move_vector_;
}

float scene::SceneObject::get_rotation_angle_cw() const
{
    return rotation_angle_cw_;
}

glm::vec3 scene::SceneObject::get_rotation_axes() const
{
    return rotation_axes_;
}

glm::vec3 scene::SceneObject::get_scaling_vector() const
{
    return scaling_vec_;
}

glm::mat4 scene::SceneObject::model_matrix() const
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, move_vector_);
    model = glm::rotate(model, rotation_angle_cw_, rotation_axes_);
    model = glm::scale(model, scaling_vec_);

    return model;
}

void scene::SceneObject::set_shader(const std::shared_ptr<Shader> &shader)
{
    shader_ = shader;
}

std::shared_ptr<Shader> scene::SceneObject::get_shader() const
{
    return shader_;
}

scene::SceneObject::~SceneObject()
{
}
