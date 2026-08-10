#include "scene/physical_objects/physical_object.h"
#include "glm/gtc/matrix_transform.hpp"

scene::physical_objects::PhysicalObject::PhysicalObject()
    : move_vector_(0.0f, 0.0f, 0.0f)
    , rotation_quat_(1.0f, 0.0f, 0.0f, 0.0f)
    , scaling_vec_(1.0f, 1.0f, 1.0f)
    , selected_(false)
{
}

void scene::physical_objects::PhysicalObject::move(glm::vec3 move_vector)
{
    move_vector_ = move_vector;
}

void scene::physical_objects::PhysicalObject::set_rotation(const glm::quat& rotation_quat)
{
    rotation_quat_ = glm::normalize(rotation_quat);
}

void scene::physical_objects::PhysicalObject::scale(glm::vec3 prescaling_vec)
{
    scaling_vec_ = prescaling_vec;
}

glm::vec3 scene::physical_objects::PhysicalObject::get_move_vector() const
{
    return move_vector_;
}

glm::quat scene::physical_objects::PhysicalObject::get_rotation() const
{
    return rotation_quat_;
}

glm::vec3 scene::physical_objects::PhysicalObject::get_scaling_vector() const
{
    return scaling_vec_;
}

glm::mat4 scene::physical_objects::PhysicalObject::model_matrix() const
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, move_vector_);
    model = model * glm::mat4_cast(rotation_quat_);
    model = glm::scale(model, scaling_vec_);

    return model;
}

glm::mat4 scene::physical_objects::PhysicalObject::normal_matrix() const
{
    return glm::transpose(glm::inverse(model_matrix()));
}

bool scene::physical_objects::PhysicalObject::is_selected() const
{
    return selected_;
}

void scene::physical_objects::PhysicalObject::set_selected(bool selected)
{
    selected_ = selected;
}
