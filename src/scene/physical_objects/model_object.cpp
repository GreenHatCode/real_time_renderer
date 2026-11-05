#include "scene/physical_objects/model_object.h"

using namespace scene::physical_objects;

ModelObject::ModelObject(
    assimp_wrapper::model_t&& loaded_model, 
    const std::string& object_name, 
    const std::shared_ptr<Shader>& g_path_shader,
    const std::shared_ptr<Shader>& shader
)
    : PhysicalObject()
    , model_(std::move(loaded_model))
    , g_path_shader_(g_path_shader)
    , shader_(shader)
    , name_(object_name)
{
}

uint32_t ModelObject::get_vertices_count() const
{
    return model_.vertices_count;
}

uint32_t ModelObject::get_faces_count() const
{
    return model_.faces_count;
}

void ModelObject::set_shader(const std::shared_ptr<Shader> &shader)
{
    shader_ = shader;
}

std::shared_ptr<Shader> ModelObject::get_shader() const
{
    return shader_;
}

std::shared_ptr<Shader> scene::physical_objects::ModelObject::get_geometry_path_shader() const
{
    return g_path_shader_;
}

std::string ModelObject::get_name() const
{
    return name_;
}

void ModelObject::render(GLenum draw_mode, bool render_transparent_meshes, const std::shared_ptr<Shader> &specific_shader) const
{
    const std::shared_ptr<Shader>& sh = specific_shader == nullptr ? shader_ : specific_shader;

    sh->set_uniform("u_model_mat", model_matrix());
    sh->set_uniform("u_normal_mat", normal_matrix());

    if (render_transparent_meshes) {
        for (auto& mesh : model_.transparent_meshes) {
            model_.materials[mesh.get_material_index()].bind(*sh);
            mesh.render(draw_mode);
        }
    }
    else {
        for (auto& mesh : model_.opaque_meshes) {
            model_.materials[mesh.get_material_index()].bind(*sh);
            mesh.render(draw_mode);
        }
    }

}

void scene::physical_objects::ModelObject::render_geometry_path(GLenum draw_mode) const
{
    render(draw_mode, false, g_path_shader_);
}

assimp_wrapper::ModelType ModelObject::model_type() const
{
    return model_.type;
}
