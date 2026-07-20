#include "scene/scene_objects/model_object.h"

scene::ModelObject::ModelObject(const assimp_wrapper::Model &loaded_model)
    : SceneObject()
    , model_(loaded_model)
{
}

void scene::ModelObject::render(Shader &shader_program, const glm::mat4& view_mat, const glm::mat4& projection_mat)
{
    shader_program.set_uniform("model_mat", model_matrix());
    shader_program.set_uniform("view_mat", view_mat);
    shader_program.set_uniform("projection_mat", projection_mat);

    model_.draw(shader_program);
}
