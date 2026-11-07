#include "scene/scene.h"
#include "spdlog/spdlog.h"
#include "renderer/renderer.h"
#include "assimp_wrapper/model_loader.h"
#include "renderer/constants.h"

using namespace scene;

scene::Scene::Scene() 
    : total_vertices_count_(0)
    , total_faces_count_(0)
{
    // add internal objects that will be used but not displayed in ui

    // specific shaders
    try {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(
            "shadow_map_empty_frag_shader", 
            "shaders_sources/internal/shadow_map_empty_frag_shader/vertex_shader.vert",
            "shaders_sources/internal/shadow_map_empty_frag_shader/fragment_shader.frag"
        );
        internal_shaders_.emplace(InternalShadersIDS::SHADOW_MAP_EMPTY_FRAG_SHADER, shader);

    } catch (const std::exception& e) {
        spdlog::error("Failed to load internal shader: {}, what: {}", "shadow_map_empty_frag_shader", e.what());
    }

    try {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(
            "shadow_map_point_light_shader", 
            "shaders_sources/internal/shadow_map_point_light_shader/vertex_shader.vert",
            "shaders_sources/internal/shadow_map_point_light_shader/fragment_shader.frag"
        );
        internal_shaders_.emplace(InternalShadersIDS::SHADOW_MAP_POINT_LIGHT_SHADER, shader);

    } catch (const std::exception& e) {
        spdlog::error("Failed to load internal shader: {}, what: {}", "shadow_map_point_light_shader", e.what());
    }

    try {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(
            "g_buffer_shader", 
            "shaders_sources/internal/deferred_rendering/geometry_path.vert",
            "shaders_sources/internal/deferred_rendering/geometry_path.frag"
        );
        internal_shaders_.emplace(InternalShadersIDS::GEOMETRY_PATH_SHADER, shader);

    } catch (const std::exception& e) {
        spdlog::error("Failed to load internal shader: {}, what: {}", "g_buffer_shader", e.what());
    }

    try {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(
            "g_buffer_height_map_shader", 
            "shaders_sources/internal/deferred_rendering/geometry_path.vert",
            "shaders_sources/internal/deferred_rendering/height_map_geometry_path.frag"
        );
        internal_shaders_.emplace(InternalShadersIDS::GEOMETRY_PATH_WITH_HEIGHT_MAP_SHADER, shader);

    } catch (const std::exception& e) {
        spdlog::error("Failed to load internal shader: {}, what: {}", "g_buffer_height_map_shader", e.what());
    }

    try {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(
            "color_path_shader", 
            "shaders_sources/internal/deferred_rendering/color_path.vert",
            "shaders_sources/internal/deferred_rendering/color_path.frag"
        );
        internal_shaders_.emplace(InternalShadersIDS::COLOR_PATH_SHADER, shader);

    } catch (const std::exception& e) {
        spdlog::error("Failed to load internal shader: {}, what: {}", "color_path_shader", e.what());
    }

    try {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(
            "stencil_outline_shader", 
            "shaders_sources/stencil_outline/stencil_outline.vs",
            "shaders_sources/stencil_outline/stencil_outline.fs"
        );
        internal_shaders_.emplace(InternalShadersIDS::OUTLINE_SHADER, shader);

    } catch (const std::exception& e) {
        spdlog::error("Failed to load internal shader: {}, what: {}", "stencil_outline_shader", e.what());
    }




    assimp_wrapper::ModelLoader loader;
    
    // physical object to render axes markers (three perpendicular lines)
    try {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(
            "axes_lines_shader", 
            "shaders_sources/internal/axes_lines/vertex_shader.vert",
            "shaders_sources/internal/axes_lines/fragment_shader.frag"
        );
        add_shader(shader);

        assimp_wrapper::model_t model = loader.load_model("resources/axes_lines.obj", assimp_wrapper::POST_PROCESSING_FLAGS);
        std::shared_ptr<Shader> g_shader = get_internal_shader(loader.has_height_map() ? InternalShadersIDS::GEOMETRY_PATH_WITH_HEIGHT_MAP_SHADER : InternalShadersIDS::GEOMETRY_PATH_SHADER);
        std::shared_ptr<scene::physical_objects::ModelObject> loaded_model = std::make_shared<scene::physical_objects::ModelObject>(
            std::move(model),
            "axes_lines_obj",
            g_shader,
            shader
        );


        loaded_model->set_shader(shader);
        internal_physical_objects_.emplace_back(std::move(loaded_model), renderer::defaults::AXES_LINES_ID, GL_LINES);
    } catch (const std::exception &e) {
        spdlog::error("Failed to load internal model {}, what: {}", "resources/internal_models/axes_lines/axes_lines.obj", e.what());
    }

    // physical object to render xz face grid
    try {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(
            "xz_face_grid_shader", 
            "shaders_sources/internal/xz_face_grid/vertex_shader.vert",
            "shaders_sources/internal/xz_face_grid/fragment_shader.frag"
        );
        add_shader(shader);

        assimp_wrapper::model_t model = loader.load_model("resources/xz_face_grid.obj", assimp_wrapper::POST_PROCESSING_FLAGS);
        std::shared_ptr<Shader> g_shader = get_internal_shader(loader.has_height_map() ? InternalShadersIDS::GEOMETRY_PATH_WITH_HEIGHT_MAP_SHADER : InternalShadersIDS::GEOMETRY_PATH_SHADER);
        std::shared_ptr<scene::physical_objects::ModelObject> loaded_model = std::make_shared<scene::physical_objects::ModelObject>(
            std::move(model),
            "xz_face_grid_obj_top",
            g_shader,
            shader
        );


        // due to Z-fighting between 
        // xz grid and xz solid face (they have equal Y axis value)
        // the Y coord for grid is changed 
        // 
        // due to using lighting calculations 
        // in xz solid face shader (see shader sources)
        // the second (mirrored) xz grid added to save good view of grid
        // from another side of the solid surface
        loaded_model->scale(glm::vec3(100.0f, 1.0f, 100.0f));
        loaded_model->move(glm::vec3(0.0f, 0.01f, 0.0f));
        internal_physical_objects_.emplace_back(std::move(loaded_model), renderer::defaults::XZ_FACE_GRID_ID, GL_TRIANGLES);

        loaded_model.reset();

        assimp_wrapper::model_t model2 = loader.load_model("resources/xz_face_grid.obj", assimp_wrapper::POST_PROCESSING_FLAGS);
        loaded_model = std::make_shared<scene::physical_objects::ModelObject>(
            std::move(model2),
            "xz_face_grid_obj_bottom",
            g_shader,
            shader
        );
        loaded_model->scale(glm::vec3(100.0f, 1.0f, 100.0f));
        loaded_model->move(glm::vec3(0.0f, -0.01f, 0.0f));
        internal_physical_objects_.emplace_back(std::move(loaded_model), renderer::defaults::XZ_FACE_GRID_ID, GL_TRIANGLES);
    } catch (const std::exception &e) {
        spdlog::error("Failed to load internal model {}, what: {}", "resources/internal_models/axes_lines/axes_lines.obj", e.what());
    }

    // physical object to render xz solid face
    try {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(
            "xz_solid_face_shader", 
            "shaders_sources/internal/xz_solid_face/vertex_shader.vert",
            "shaders_sources/internal/xz_solid_face/fragment_shader.frag"
        );
        add_shader(shader);

        assimp_wrapper::model_t model = loader.load_model("resources/xz_face_grid.obj", assimp_wrapper::POST_PROCESSING_FLAGS);
        std::shared_ptr<Shader> g_shader = get_internal_shader(loader.has_height_map() ? InternalShadersIDS::GEOMETRY_PATH_WITH_HEIGHT_MAP_SHADER : InternalShadersIDS::GEOMETRY_PATH_SHADER);
        std::shared_ptr<scene::physical_objects::ModelObject> loaded_model = std::make_shared<scene::physical_objects::ModelObject>(
            std::move(model),
            "xz_solid_face_obj",
            g_shader,
            shader
        );

        loaded_model->set_shader(shader);
        loaded_model->scale(glm::vec3(100.0f, 1.0f, 100.0f));
        internal_physical_objects_.emplace_back(std::move(loaded_model), renderer::defaults::XZ_FACE_SOLID_ID, GL_TRIANGLES);
    } catch (const std::exception &e) {
        spdlog::error("Failed to load internal model {}, what: {}", "resources/internal_models/axes_lines/axes_lines.obj", e.what());
    }

}

void scene::Scene::add_object(const std::shared_ptr<scene::physical_objects::ModelObject> &ptr)
{
    model_objects_.push_back(ptr);
    total_vertices_count_ += ptr->get_vertices_count();
    total_faces_count_ += ptr->get_faces_count();
}

void scene::Scene::add_object(const std::shared_ptr<scene::physical_objects::LightModelObject> &ptr)
{
    light_model_objects_.push_back(ptr);
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
    auto model_obj_it = std::find_if(
        model_objects_.begin(), 
        model_objects_.end(), 
        [name](const std::shared_ptr<scene::physical_objects::PhysicalObject>& obj)
        {
            return obj->get_name() == name;
        }
    );

    if (model_obj_it != model_objects_.end()) {
        total_vertices_count_ -= (*model_obj_it)->get_vertices_count();
        total_faces_count_ -= (*model_obj_it)->get_faces_count();

        model_objects_.erase(model_obj_it);
        return true;
    }


    auto light_model_obj_it = std::find_if(
        light_model_objects_.begin(), 
        light_model_objects_.end(), 
        [name](const std::shared_ptr<scene::physical_objects::PhysicalObject>& obj)
        {
            return obj->get_name() == name;
        }
    );

    if (light_model_obj_it != light_model_objects_.end()) {
        total_vertices_count_ -= (*light_model_obj_it)->get_vertices_count();
        total_faces_count_ -= (*light_model_obj_it)->get_faces_count();

        light_model_objects_.erase(light_model_obj_it);
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

std::vector<std::shared_ptr<scene::physical_objects::ModelObject>> scene::Scene::get_model_objects() const
{
    return model_objects_;
}

std::vector<std::shared_ptr<scene::physical_objects::LightModelObject>> scene::Scene::get_light_objects() const
{
    return light_model_objects_;
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

std::shared_ptr<Shader> scene::Scene::get_internal_shader(InternalShadersIDS id) const
{
    try
    {
        return internal_shaders_.at(id);
    }
    catch(const std::exception& e)
    {
        spdlog::error("There is no shader with id={}", static_cast<int>(id));
        return nullptr;
    }
}

uint32_t scene::Scene::get_total_vertices_count() const
{
    return total_vertices_count_;
}

uint32_t scene::Scene::get_total_faces_count() const
{
    return total_faces_count_;
}
