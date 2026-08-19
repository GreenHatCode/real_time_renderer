#include "scene/scene.h"
#include "spdlog/spdlog.h"

using namespace scene;

void scene::Scene::add_object(const std::shared_ptr<scene::physical_objects::PhysicalObject> &ptr)
{
    physical_objects_.push_back(ptr);
    total_vertices_count += ptr->get_vertices_count();
    total_faces_count += ptr->get_faces_count();
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

        total_vertices_count -= (*physical_obj_it)->get_vertices_count();
        total_faces_count -= (*physical_obj_it)->get_faces_count();

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
    return total_vertices_count;
}

uint32_t scene::Scene::get_total_faces_count() const
{
    return total_faces_count;
}
