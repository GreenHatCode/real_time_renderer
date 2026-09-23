#include "scene/scene.h"
#include "spdlog/spdlog.h"
#include "renderer/renderer.h"

using namespace scene;

scene::Scene::Scene() 
    : total_vertices_count_(0)
    , total_faces_count_(0)
{
    // add internal objects that will be used but not displayed in ui

    // physical objects to render axes markers (three perpendicular lines)
    try {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(
            "axes_lines_shader", 
            "shaders_sources/internal/axes_lines/vertex_shader.vert",
            "shaders_sources/internal/axes_lines/fragment_shader.frag"
        );
        add_shader(shader);

        std::shared_ptr<scene::physical_objects::ModelObject> loaded_model = std::make_shared<scene::physical_objects::ModelObject>(
            assimp_wrapper::Model("resources/axes_lines.obj", assimp_wrapper::POST_PROCESSING_FLAGS),
            "axes_lines_obj"
        );

        loaded_model->set_shader(shader);
        internal_physical_objects_.emplace_back(std::move(loaded_model), renderer::defaults::AXES_LINES_ID, GL_LINES);
    } catch (const std::exception &e) {
        spdlog::error("Failed to load internal model {}, what: {}", "resources/internal_models/axes_lines/axes_lines.obj", e.what());
    }

    // physical objects to render xz face grid
    try {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(
            "xz_face_grid_shader", 
            "shaders_sources/internal/xz_face_grid/vertex_shader.vert",
            "shaders_sources/internal/xz_face_grid/fragment_shader.frag"
        );
        add_shader(shader);

        std::shared_ptr<scene::physical_objects::ModelObject> loaded_model = std::make_shared<scene::physical_objects::ModelObject>(
            assimp_wrapper::Model("resources/xz_face_grid.obj", assimp_wrapper::POST_PROCESSING_FLAGS),
            "xz_face_grid_obj"
        );

        loaded_model->set_shader(shader);
        loaded_model->scale(glm::vec3(100.0f, 1.0f, 100.0f));
        internal_physical_objects_.emplace_back(std::move(loaded_model), renderer::defaults::XZ_FACE_GRID_ID, GL_TRIANGLES);
    } catch (const std::exception &e) {
        spdlog::error("Failed to load internal model {}, what: {}", "resources/internal_models/axes_lines/axes_lines.obj", e.what());
    }

    try {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(
            "xz_solid_face_shader", 
            "shaders_sources/internal/xz_solid_face/vertex_shader.vert",
            "shaders_sources/internal/xz_solid_face/fragment_shader.frag"
        );
        add_shader(shader);

        std::shared_ptr<scene::physical_objects::ModelObject> loaded_model = std::make_shared<scene::physical_objects::ModelObject>(
            assimp_wrapper::Model("resources/xz_face_grid.obj", assimp_wrapper::POST_PROCESSING_FLAGS),
            "xz_solid_face_obj"
        );

        loaded_model->set_shader(shader);
        loaded_model->scale(glm::vec3(100.0f, 1.0f, 100.0f));
        internal_physical_objects_.emplace_back(std::move(loaded_model), renderer::defaults::XZ_FACE_SOLID_ID, GL_TRIANGLES);
    } catch (const std::exception &e) {
        spdlog::error("Failed to load internal model {}, what: {}", "resources/internal_models/axes_lines/axes_lines.obj", e.what());
    }


}

void scene::Scene::add_object(const std::shared_ptr<scene::physical_objects::PhysicalObject> &ptr)
{
    physical_objects_.push_back(ptr);
    total_vertices_count_ += ptr->get_vertices_count();
    total_faces_count_ += ptr->get_faces_count();
}

void scene::Scene::add_light(const std::shared_ptr<scene::lights::LightBase> &ptr)
{
    lights_.push_back(ptr);
}

void scene::Scene::add_shader(const std::shared_ptr<Shader> &ptr)
{
    shaders_.push_back(ptr);
}

bool scene::Scene::delete_scene_object(const std::string &name)
{
    auto physical_obj_it = std::find_if(
        physical_objects_.begin(), 
        physical_objects_.end(), 
        [name](const std::shared_ptr<scene::physical_objects::PhysicalObject>& obj)
        {
            return obj->get_name() == name;
        }
    );

    if (physical_obj_it != physical_objects_.end()) {

        total_vertices_count_ -= (*physical_obj_it)->get_vertices_count();
        total_faces_count_ -= (*physical_obj_it)->get_faces_count();

        physical_objects_.erase(physical_obj_it);
        return true;
    }


    auto light_it = std::find_if(
        lights_.begin(), 
        lights_.end(), 
        [name](const std::shared_ptr<scene::lights::LightBase>& obj)
        {
            return obj->get_name() == name;
        }
    );

    if (light_it != lights_.end()) {
        lights_.erase(light_it);
        return true;
    }

    spdlog::error("Failed to delete non-existent scene object, name = {}", name);
    return false;
}

bool scene::Scene::delete_shader(const std::string &name)
{
    auto it = std::find_if(
        shaders_.begin(),
        shaders_.end(),
        [name](const std::shared_ptr<Shader>& s) 
        {
            return s->get_name() == name;
        }
    );

    if (it == shaders_.end()) {
        spdlog::error("Failed to delete shader that isn't loaded to scene. Shader name = {}", name);
        return false;
    }

    if ((*it).use_count() > 1) {
        spdlog::error("Failed to delete shader. Some objects still use it: shader_name={}, use_count={}", name, (*it).use_count());
        return false;
    }

    shaders_.erase(it);
    spdlog::info("Deleted shader: {}", name);
    return true;
}

std::vector<std::shared_ptr<scene::physical_objects::PhysicalObject>> scene::Scene::get_objects() const
{
    return physical_objects_;
}

std::vector<scene::Scene::internal_physical_object> scene::Scene::get_internal_objects() const
{
    return internal_physical_objects_;
}

std::vector<std::shared_ptr<scene::lights::LightBase>> scene::Scene::get_lights() const
{
    return lights_;
}

std::vector<std::shared_ptr<Shader>> scene::Scene::get_shaders() const
{
    return shaders_;
}

std::shared_ptr<scene::physical_objects::PhysicalObject> scene::Scene::get_object(const std::string &name) const
{
    auto it = std::find_if(
        physical_objects_.begin(), 
        physical_objects_.end(), 
        [name](const std::shared_ptr<scene::physical_objects::PhysicalObject>& obj)
        {
            return obj->get_name() == name;
        }
    );

    if (it != physical_objects_.end()) return *it;
        
    return nullptr;
}

std::shared_ptr<scene::lights::LightBase> scene::Scene::get_light(const std::string &name) const
{
    auto it = std::find_if(
        lights_.begin(), 
        lights_.end(), 
        [name](const std::shared_ptr<scene::lights::LightBase>& obj)
        {
            return obj->get_name() == name;
        }
    );

    if (it != lights_.end()) return *it;
        
    return nullptr;
}

std::shared_ptr<Shader> scene::Scene::get_shader(const std::string &name) const
{
    auto it = std::find_if(
        shaders_.begin(),
        shaders_.end(),
        [name](const std::shared_ptr<Shader>& s) 
        {
            return s->get_name() == name;
        }
    );
    if (it != shaders_.end()) return *it;
        
    return nullptr;
}

uint32_t scene::Scene::get_total_vertices_count() const
{
    return total_vertices_count_;
}

uint32_t scene::Scene::get_total_faces_count() const
{
    return total_faces_count_;
}
