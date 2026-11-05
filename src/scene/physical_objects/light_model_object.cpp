#include "scene/physical_objects/light_model_object.h"
#include "scene/lights/light_base.h"
#include "assimp_wrapper/model_loader.h"

using namespace scene;
using namespace scene::physical_objects;
using namespace physical_light_object_constants;

LightModelObject::LightModelObject(
    assimp_wrapper::model_t&& loaded_model, 
    const std::string& object_name, 
    glm::vec3 color, 
    const std::shared_ptr<Shader>& g_path_shader,
    const std::shared_ptr<Shader>& shader
)
    : PhysicalObject()
    , model_(std::move(loaded_model))
    , name_(object_name)
    , g_path_shader_(g_path_shader)
    , shader_(shader)
    , color_(1.0f, 1.0f, 1.0f)
{
    set_color(color);
}

uint32_t LightModelObject::get_vertices_count() const
{
    return model_.vertices_count;
}

uint32_t LightModelObject::get_faces_count() const
{
    return model_.faces_count;
}

void LightModelObject::set_shader(const std::shared_ptr<Shader> &shader)
{
    shader_ = shader;
}

std::shared_ptr<Shader> LightModelObject::get_shader() const
{
    return shader_;
}

std::shared_ptr<Shader> scene::physical_objects::LightModelObject::get_geometry_path_shader() const
{
    return g_path_shader_;
}

std::string LightModelObject::get_name() const
{
    return name_;
}

void LightModelObject::render(GLenum draw_mode, bool render_transparent_meshes, const std::shared_ptr<Shader> &specific_shader) const
{
    const std::shared_ptr<Shader>& sh = specific_shader == nullptr ? shader_ : specific_shader;
    
    sh->set_uniform("u_model_mat", model_matrix());
    sh->set_uniform("u_normal_mat", normal_matrix());
    sh->set_uniform("u_light_object_color", color_);
    
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

void scene::physical_objects::LightModelObject::render_geometry_path(GLenum draw_mode) const
{
    render(draw_mode, false, g_path_shader_);
}

assimp_wrapper::ModelType LightModelObject::model_type() const
{
    return model_.type;
}

void LightModelObject::set_color(glm::vec3 color)
{
    color_ = glm::clamp(color, scene::lights::lighting_constants::MIN_LIGHTING_COMPONENT_VALUE, scene::lights::lighting_constants::MAX_LIGHTING_COMPONENT_VALUE);
}
