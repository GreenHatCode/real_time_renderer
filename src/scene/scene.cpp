#include "scene/scene.h"
#include "spdlog/spdlog.h"

using namespace scene;

std::shared_ptr<ModelObject> scene::Scene::add_model(const std::string &name, const std::filesystem::path &model_file_path)
{
    std::shared_ptr<ModelObject> model = nullptr;
    try
    {
        model = std::make_shared<ModelObject>(
        assimp_wrapper::Model(
            model_file_path,
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_ImproveCacheLocality |
            aiProcess_ValidateDataStructure
        ),
        name
        );

    }
    catch(const std::exception& e)
    {
        spdlog::error("Failed to load model: {}", e.what());
        return model;
    }
    
    models_map_.emplace(name, model);
    return model;
}

std::shared_ptr<Shader> scene::Scene::add_shader(const std::string &name, const std::filesystem::path &vert_shader_file_path, const std::filesystem::path &frag_shader_file_path)
{
    std::shared_ptr<Shader> shader = get_shader(name);
    if (shader != nullptr) return shader;

    try
    {
        shader = std::make_shared<Shader>(name, vert_shader_file_path.c_str(), frag_shader_file_path.c_str());
    }
    catch(const std::exception& e)
    {
        spdlog::error(e.what());
        return shader;
    }
    
    shaders_map_.emplace(name, shader);
    return shader;
}

bool scene::Scene::delete_model(const std::string &name)
{
    if (models_map_.find(name) == models_map_.end()) {
        spdlog::error("Attempt to delete model that isn't loaded to scene. Model name = {}", name);
        return false;
    }
    models_map_.erase(name);
    spdlog::info("Deleted model: {}", name);
    return true;
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

std::vector<std::shared_ptr<ModelObject>> scene::Scene::get_models() const
{
    std::vector<std::shared_ptr<ModelObject>> models;
    for (const auto& m : models_map_) {
        models.push_back(m.second);
    }

    return models;
}

std::vector<std::shared_ptr<Shader>> scene::Scene::get_shaders() const
{
    std::vector<std::shared_ptr<Shader>> shaders;
    for (const auto& s : shaders_map_) {
        shaders.push_back(s.second);
    }

    return shaders;
}

std::shared_ptr<Shader> scene::Scene::get_shader(const std::string &name) const
{
    auto it = shaders_map_.find(name);
    if (it != shaders_map_.end()) return it->second;
        
    return nullptr;
}

std::shared_ptr<ModelObject> scene::Scene::get_model(const std::string &name) const
{
    auto it = models_map_.find(name);
    if (it != models_map_.end()) return it->second;
        
    return nullptr;
}