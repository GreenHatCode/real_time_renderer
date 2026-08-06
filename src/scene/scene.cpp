#include "scene/scene.h"
#include "spdlog/spdlog.h"

using namespace scene;

void scene::Scene::add_object(const std::string &name, const std::shared_ptr<scene::physical_objects::PhysicalObject> &ptr)
{
    physical_objects_map_.emplace(name, ptr);
    total_vertices_count += ptr->get_vertices_count();
    total_faces_count += ptr->get_faces_count();
}

void scene::Scene::add_object(const std::string &name, const std::shared_ptr<scene::physical_objects::PhysicalLightObject> &ptr)
{
    physical_light_objects_map_.emplace(name, ptr);
    total_vertices_count += ptr->get_vertices_count();
    total_faces_count += ptr->get_faces_count();
}

void scene::Scene::add_light(const std::string &name, const std::shared_ptr<scene::lights::LightBase> &ptr)
{
    lights_map_.emplace(name, ptr);
}

void scene::Scene::add_shader(const std::string &name, const std::shared_ptr<Shader> &ptr)
{
    shaders_map_.emplace(name, ptr);
}

bool scene::Scene::delete_scene_object(const std::string &name)
{
    // add decreasing faces and vertices count
    if (physical_objects_map_.find(name) != physical_objects_map_.end()) {
        physical_objects_map_.erase(name);
        return true;
    }
    
    if (physical_light_objects_map_.find(name) != physical_light_objects_map_.end()) {
        physical_light_objects_map_.erase(name);
        return true;
    }

    if (lights_map_.find(name) != lights_map_.end()) {
        lights_map_.erase(name);
        return true;
    }

    spdlog::error("Failed to delete non-existent scene object, name = {}", name);
    return false;
}

bool scene::Scene::delete_shader(const std::string &name)
{
    auto it = shaders_map_.find(name);
    if (it == shaders_map_.end()) {
        spdlog::error("Failed to delete shader that isn't loaded to scene. Shader name = {}", name);
        return false;
    }

    if (it->second.use_count() > 1) {
        spdlog::error("Failed to delete shader. Some objects still use it: shader_name={}, use_count={}", name, it->second.use_count());
        return false;
    }

    shaders_map_.erase(name);
    spdlog::info("Deleted shader: {}", name);
    return true;
}

std::vector<std::shared_ptr<scene::physical_objects::PhysicalObject>> scene::Scene::get_objects(bool lights_models = false) const
{
    std::vector<std::shared_ptr<scene::physical_objects::PhysicalObject>> objects;
    for (const auto& m : physical_objects_map_) {
        objects.push_back(m.second);
    }
    return objects;
}

std::vector<std::shared_ptr<scene::lights::LightBase>> scene::Scene::get_lights() const
{
    std::vector<std::shared_ptr<scene::lights::LightBase>> lights;
    for (const auto& l : lights_map_) {
        lights.push_back(l.second);
    }
    return lights;
}

std::vector<std::shared_ptr<Shader>> scene::Scene::get_shaders() const
{
    std::vector<std::shared_ptr<Shader>> shaders;
    for (const auto& s : shaders_map_) {
        shaders.push_back(s.second);
    }

    return shaders;
}

std::shared_ptr<scene::physical_objects::PhysicalObject> scene::Scene::get_object(const std::string &name) const
{
    auto it = physical_objects_map_.find(name);
    if (it != physical_objects_map_.end()) return it->second;
        
    return nullptr;
}

std::shared_ptr<scene::lights::LightBase> scene::Scene::get_light(const std::string &name) const
{
    auto it = lights_map_.find(name);
    if (it != lights_map_.end()) return it->second;
        
    return nullptr;
}

std::shared_ptr<Shader> scene::Scene::get_shader(const std::string &name) const
{
    auto it = shaders_map_.find(name);
    if (it != shaders_map_.end()) return it->second;
        
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
