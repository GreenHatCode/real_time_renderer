#include "scene/physical_objects/physical_object.h"
#include "glm/gtc/matrix_transform.hpp"

scene::physical_objects::PhysicalObject::PhysicalObject()
    : move_vector_(0.0f, 0.0f, 0.0f)
    , rotation_angle_cw_(0.0f)
    , rotation_axes_(1.0f, 1.0f, 1.0f)
    , scaling_vec_(1.0f, 1.0f, 1.0f)
{
}

void scene::physical_objects::PhysicalObject::move(glm::vec3 move_vector)
{
    move_vector_ = move_vector;
}

void scene::physical_objects::PhysicalObject::rotate(float angle_rad, glm::vec3 axes_vec)
{
    rotation_angle_cw_ = angle_rad;
    rotation_axes_ = glm::normalize(axes_vec);
}

void scene::physical_objects::PhysicalObject::scale(glm::vec3 prescaling_vec)
{
    scaling_vec_ = prescaling_vec;
}

glm::vec3 scene::physical_objects::PhysicalObject::get_move_vector() const
{
    return move_vector_;
}

float scene::physical_objects::PhysicalObject::get_rotation_angle_cw() const
{
    return rotation_angle_cw_;
}

glm::vec3 scene::physical_objects::PhysicalObject::get_rotation_axes() const
{
    return rotation_axes_;
}

glm::vec3 scene::physical_objects::PhysicalObject::get_scaling_vector() const
{
    return scaling_vec_;
}

glm::mat4 scene::physical_objects::PhysicalObject::model_matrix() const
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, move_vector_);
    model = glm::rotate(model, rotation_angle_cw_, rotation_axes_);
    model = glm::scale(model, scaling_vec_);

    return model;
}
