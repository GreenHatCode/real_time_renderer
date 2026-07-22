#include "scene/scene.h"

using namespace scene;

std::shared_ptr<ModelObject> scene::Scene::add_model(const std::filesystem::path &model_file_path)
{
    std::shared_ptr<ModelObject> model = std::make_shared<ModelObject>(assimp_wrapper::Model(
        model_file_path,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_ValidateDataStructure
    ));
    models_.push_back(model);
    return model;
}

std::shared_ptr<Shader> scene::Scene::add_shader(const std::filesystem::path &vert_shader_file_path, const std::filesystem::path &frag_shader_file_path)
{
    std::shared_ptr<Shader> shader = std::make_shared<Shader>(vert_shader_file_path.c_str(), frag_shader_file_path.c_str());
    objects_shader_.push_back(shader);
    return shader;
}

std::vector<std::shared_ptr<ModelObject>> scene::Scene::get_models() const
{
    return models_;
}
