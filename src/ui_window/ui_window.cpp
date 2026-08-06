#include "ui_window/ui_window.h"
#include "misc/cpp/imgui_stdlib.h"
#include "scene/physical_objects/physical_light_object.h"

UIWindow::UIWindow(const std::shared_ptr<scene::Scene> &scene)
    : scene_(scene)
    , model_dialog_state_{.model_file_path="", .shader_name="", .name=""}
    , shader_dialog_state_{.vert_shader_file_path="", .frag_shader_file_path="", .name=""}
    , light_dialog_state_{.light_name="", .type=static_cast<int32_t>(scene::lights::LightType::DIRECTIONAL_LIGHT)}
    , model_inspector_state_{.selected_model_ptr=nullptr}
    , light_inspector_state_{.selected_light_ptr=nullptr}
    , gpu_time_to_display_(0.0)
    , cpu_time_to_display_(0.0)
    , clear_color_(0.0f, 0.0f, 0.0f, 1.0f)
    , delta_time_(0.0)
{

}

void UIWindow::render_window()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);

    // rendering options
    ImGui::Text("Read-Timer Renderer");
    ImGui::ColorEdit4("clear color", glm::value_ptr(clear_color_));
    ImGui::Separator();

    
    if (ImGui::Button("manage model")) {
        ImGui::OpenPopup("manage model");
    }
    render_model_dialog();
    ImGui::SameLine();
    
    if (ImGui::Button("manage shader")) {
        ImGui::OpenPopup("manage shader");
    }
    render_shader_dialog();
    ImGui::SameLine();

    if (ImGui::Button("manage light")) {
        ImGui::OpenPopup("manage light");
    }
    render_light_dialog();
    ImGui::Separator();

    render_model_inspector();
    ImGui::Separator();

    render_lights_inspector();
    ImGui::Separator();

    // rendering statistics
    ImGui::Text("App average FPS: %.2f", calculate_fps());
    ImGui::Text("App frame time: %.2f", delta_time_);
    ImGui::Text("Total vertices count: %d", scene_->get_total_vertices_count());
    ImGui::Text("Total faces count: %d", scene_->get_total_faces_count());
    ImGui::Text("GPU time: %.2f", gpu_time_to_display_);
    ImGui::Text("CPU time: %.2f", cpu_time_to_display_);
}

glm::vec4 UIWindow::get_clear_color() const
{
    return clear_color_;
}

float UIWindow::get_delta_time() const
{
    return static_cast<float>(delta_time_);
}

void UIWindow::set_gpu_time_to_display(double value)
{
    gpu_time_to_display_ = value;
}

void UIWindow::set_cpu_time_to_display(double value)
{
    cpu_time_to_display_ = value;
}

double UIWindow::calculate_fps()
{
    double curr_s = glfwGetTime();
    delta_time_ = curr_s - prev_s_;
    prev_s_ = curr_s;

    title_countdown_s_ -= delta_time_;
    if (title_countdown_s_ <= 0.0 && delta_time_ > 0.0) {
        fps_ = 1.0 / delta_time_;
        title_countdown_s_ = 0.1;
        return fps_;
    }
    return fps_;
}

void UIWindow::render_model_dialog()
{
    if (ImGui::BeginPopupModal("manage model", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Manage model on the scene");
        ImGui::Separator();
        ImGui::InputText("File", &model_dialog_state_.model_file_path);
        ImGui::InputText("Shader name", &model_dialog_state_.shader_name);
        ImGui::InputText("Object name", &model_dialog_state_.name);
        ImGui::Spacing();

        if (ImGui::Button("Load"))
        {
            load_model(model_dialog_state_.shader_name, model_dialog_state_.name, model_dialog_state_.model_file_path);
            model_dialog_state_ = {};
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();

        if (ImGui::Button("Delete")) {
            model_inspector_state_.selected_model_ptr = nullptr;
            scene_->delete_scene_object(model_dialog_state_.name);
        }
        ImGui::SameLine();     

        if (ImGui::Button("Exit"))
        {
            model_dialog_state_ = {};
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

}

void UIWindow::render_shader_dialog()
{
    if (ImGui::BeginPopupModal("manage shader", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Manage shader");
        ImGui::Separator();
        ImGui::InputText("Vertex shader", &shader_dialog_state_.vert_shader_file_path);
        ImGui::InputText("Fragmet shader", &shader_dialog_state_.frag_shader_file_path);
        ImGui::InputText("Shader name", &shader_dialog_state_.name);
        ImGui::Spacing();

        if (ImGui::Button("Load")) {
            load_shader(shader_dialog_state_.name, shader_dialog_state_.vert_shader_file_path, shader_dialog_state_.frag_shader_file_path);
            shader_dialog_state_ = {};
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete")) {
            scene_->delete_shader(shader_dialog_state_.name);
        }

        ImGui::SameLine();

        if (ImGui::Button("Exit")) {
            shader_dialog_state_ = {};
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void UIWindow::render_light_dialog()
{
    if (ImGui::BeginPopupModal("manage light", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Manage light");
        ImGui::Separator();
        ImGui::InputText("Name", &light_dialog_state_.light_name);

        ImGui::Combo("Light type", &light_dialog_state_.type, scene::lights::get_light_type_names(), static_cast<int>(scene::lights::LightType::COUNT));

        if (ImGui::Button("Add")) {
            add_light(light_dialog_state_.light_name, static_cast<scene::lights::LightType>(light_dialog_state_.type));
            light_dialog_state_ = {};
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete")) {
            scene_->delete_scene_object(light_dialog_state_.light_name);
        }

        ImGui::SameLine();

        if (ImGui::Button("Exit")) {
            light_dialog_state_ = {};
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void UIWindow::render_model_inspector()
{
    // list box of loaded models
    ImVec2 list_box_size = ImGui::GetWindowSize();
    list_box_size.y = 60;
    ImGui::Text("Shaders List");
    if (ImGui::BeginListBox("##ShadersList", list_box_size))
    {
        for (const auto& s : scene_->get_shaders()) {
            bool selected = false;
            ImGui::Selectable(s->get_name().c_str(), selected);
        }
        ImGui::EndListBox();
    }

    ImGui::Text("Models List");
    if (ImGui::BeginListBox("##ModelsList", list_box_size))
    {
        for (const auto& m : scene_->get_objects()) {
            bool selected = model_inspector_state_.selected_model_ptr == m;

            if (ImGui::Selectable(m->get_name().c_str(), selected)) {
                if (selected) {
                    model_inspector_state_.selected_model_ptr = nullptr;
                } else {
                    model_inspector_state_.selected_model_ptr = m;
                }
            }

            if (selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    if (model_inspector_state_.selected_model_ptr)
    {
        ImGui::Separator();
        ImGui::Text("Selected Object");

        glm::vec3 pos = model_inspector_state_.selected_model_ptr->get_move_vector();
        if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.05f))
            model_inspector_state_.selected_model_ptr->move(pos);

        float angle = model_inspector_state_.selected_model_ptr->get_rotation_angle_cw();
        if (ImGui::DragFloat("Rotation", &angle, 0.01f))
            model_inspector_state_.selected_model_ptr->rotate(angle, model_inspector_state_.selected_model_ptr->get_rotation_axes());

        glm::vec3 axis = model_inspector_state_.selected_model_ptr->get_rotation_axes();
        if (ImGui::DragFloat3("Axis", glm::value_ptr(axis), 0.01f))
            model_inspector_state_.selected_model_ptr->rotate(model_inspector_state_.selected_model_ptr->get_rotation_angle_cw(), axis);

        glm::vec3 scale = model_inspector_state_.selected_model_ptr->get_scaling_vector();
        if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.01f))
            model_inspector_state_.selected_model_ptr->scale(scale);
   
        auto shader_ptr = model_inspector_state_.selected_model_ptr->get_shader();
        if (ImGui::BeginCombo("Shader", shader_ptr->get_name().c_str()))
        {
            for (const auto& shader : scene_->get_shaders())
            {
                bool selected = shader == shader_ptr;

                if (ImGui::Selectable(shader->get_name().c_str(), selected))
                    model_inspector_state_.selected_model_ptr->set_shader(shader);

                if (selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }
    
    }
}

void UIWindow::render_lights_inspector()
{
    ImVec2 list_box_size = ImGui::GetWindowSize();
    list_box_size.y = 60;
    ImGui::Text("LightsList");
    if (ImGui::BeginListBox("##LightsList", list_box_size))
    {
        for (const auto& m : scene_->get_lights()) {
            bool selected = (light_inspector_state_.selected_light_ptr == m);

            if (ImGui::Selectable(m->get_name().c_str(), selected)) {
                if (selected) {
                    light_inspector_state_.selected_light_ptr = nullptr;
                } else {
                    light_inspector_state_.selected_light_ptr = m;
                }
            }

            if (selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    if (light_inspector_state_.selected_light_ptr)
    {
        ImGui::Separator();
        ImGui::Text("Selected Light");

        switch (light_inspector_state_.selected_light_ptr->get_type())
        {
        case scene::lights::LightType::DIRECTIONAL_LIGHT:
            render_directional_light_editor(static_cast<scene::lights::DirectionalLight &>(*light_inspector_state_.selected_light_ptr));
            break;
        case scene::lights::LightType::POINT_LIGHT:
            render_point_light_editor(static_cast<scene::lights::PointLight &>(*light_inspector_state_.selected_light_ptr));
            break;

        default:
            spdlog::error("Unknow light type to render imgui editor, type={}", static_cast<int>(light_inspector_state_.selected_light_ptr->get_type()));
            assert(false);
            break;
        }
    }

}

void UIWindow::render_common_lights_editor(scene::lights::LightBase &light)
{
    auto ambient = light.get_ambient();
    if (ImGui::DragFloat3("Ambient", glm::value_ptr(ambient)))
        light.set_ambient(ambient);

    auto diffuse = light.get_diffuse();
    if (ImGui::DragFloat3("Diffuse", glm::value_ptr(diffuse)))
        light.set_diffuse(diffuse);

    auto specular = light.get_specular();
    if (ImGui::DragFloat3("Specular", glm::value_ptr(specular)))
        light.set_specular(specular);
}

void UIWindow::render_directional_light_editor(scene::lights::DirectionalLight &light)
{
    render_common_lights_editor(light);
    
    auto color = light.get_color();
    if (ImGui::ColorEdit3("Color", glm::value_ptr(color)))
        light.set_color(color);

    auto dir = light.get_direction();
    if (ImGui::DragFloat3("Direction", glm::value_ptr(dir), 0.05f))
        light.set_direction(dir);
}

void UIWindow::render_point_light_editor(scene::lights::PointLight &light)
{
    render_common_lights_editor(light);

    auto color = light.get_color();
    if (ImGui::ColorEdit3("Color", glm::value_ptr(color))) {
        light.set_color(color);
        if (auto model_ptr = light.get_model_ptr().lock())
            model_ptr->set_color(color);
        else 
            spdlog::warn("Failed to edit point light model color, there isn't such a model on the scene. Light name={}", light.get_name().c_str());
    }
        
    auto pos = light.get_position();
    if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.05f)) {
        light.set_position(pos);
        if (auto model_ptr = light.get_model_ptr().lock())
            model_ptr->move(pos);
        else 
            spdlog::warn("Failed to edit point light model position, there isn't such a model on the scene. Light name={}", light.get_name().c_str());
    }

    auto constant = light.get_constant();
    if (ImGui::DragFloat("Constant", &constant, 0.05f))
        light.set_constant(constant);

    auto linear = light.get_linear();
    if (ImGui::DragFloat("Linear", &linear, 0.05f))
        light.set_constant(linear);

    auto quadratic = light.get_quadratic();
    if (ImGui::DragFloat("Quadratic", &quadratic, 0.05f))
        light.set_constant(quadratic);
}

void UIWindow::load_model(const std::string &shader_name, const std::string &name, const std::filesystem::path &file_path)
{
    std::shared_ptr<Shader> model_shader = scene_->get_shader(shader_name);
    if (model_shader == nullptr) {
        spdlog::error("Failed to load model {}. There isn't shader with name={}", file_path.c_str(), shader_name.c_str());
        return;
    }

    try {
        std::shared_ptr<scene::physical_objects::ModelObject> loaded_model = std::make_shared<scene::physical_objects::ModelObject>(
            assimp_wrapper::Model(
                file_path,
                aiProcess_Triangulate |
                aiProcess_JoinIdenticalVertices |
                aiProcess_ImproveCacheLocality |
                aiProcess_ValidateDataStructure
            ),
            name
        );

        loaded_model->set_shader(model_shader);
        scene_->add_object(name, loaded_model);
    } catch (const std::exception &e) {
        spdlog::error("Caught exception during loading model {}, what: {}", file_path.c_str(), e.what());
    }
}

void UIWindow::load_shader(const std::string &name, const std::filesystem::path &vertex_shader_file, const std::filesystem::path &fragment_shader_file)
{
    try {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(name, vertex_shader_file.c_str(), fragment_shader_file.c_str());
        scene_->add_shader(name, shader);

    } catch (const std::exception& e) {
        spdlog::error("Failed to load shader: {}", e.what());
    }
    
}

void UIWindow::add_light(const std::string &name, scene::lights::LightType type)
{
    switch (type)
    {
    case scene::lights::LightType::DIRECTIONAL_LIGHT:
        try {
            std::shared_ptr<scene::lights::DirectionalLight> dl = std::make_shared<scene::lights::DirectionalLight>(
                name,
                glm::vec3(0.2f, 0.2f, 0.2f),  // Ambient
                glm::vec3(0.8f, 0.8f, 0.8f),  // Diffuse
                glm::vec3(1.0f, 1.0f, 1.0f),  // Specular
                glm::vec3(1.0f, 0.98f, 0.9f), // RGB Color
                glm::vec3(1.0f, 0.0f, 0.0f)   // Direction
            );
            scene_->add_light(name, dl);

        } catch (const std::exception& e) {
            spdlog::error("Failed to add DirectionalLight: {}", e.what());
        }
        break;
    case scene::lights::LightType::POINT_LIGHT:
        try {
            std::shared_ptr<Shader> model_shader = scene_->get_shader(std::string(scene::physical_objects::physical_light_object_constants::ShaderName));
            if (model_shader == nullptr) {
                spdlog::error("Failed to load point light model {}. Required shader with name={}", 
                    scene::physical_objects::physical_light_object_constants::LightSphereModel.data(), 
                    scene::physical_objects::physical_light_object_constants::ShaderName.data());
                return;
            }

            std::shared_ptr<scene::physical_objects::LightModelObject> light_model =
                std::make_shared<scene::physical_objects::LightModelObject>(
                    name + "_obj",
                    scene::physical_objects::physical_light_object_constants::LightSphereModel,
                    glm::vec3(1.0f, 1.0f, 1.0f),
                    model_shader
            );
            
            std::shared_ptr<scene::lights::PointLight> pl = std::make_shared<scene::lights::PointLight>(
                name,
                light_model,
                glm::vec3(0.05f, 0.05f, 0.05f), // Ambient
                glm::vec3(0.8f, 0.8f, 0.8f),    // Diffuse
                glm::vec3(1.0f, 1.0f, 1.0f),    // Specular
                glm::vec3(1.0f, 1.0f, 1.0f),    // RGB Color
                glm::vec3(1.2f, 1.0f, 2.0f),    // Position
                1.0f,                           // Constant (Kc)
                0.09f,                          // Linear (Kl)
                0.032f                          // Quadratic (Kq) for a ~50 distance radius
            );

            light_model->set_color(pl->get_color());
            
            scene_->add_light(name, pl);
            scene_->add_object(light_model->get_name(), light_model);
        } catch (const std::exception& e) {
            spdlog::error("Failed to add PointLight: {}", e.what());
        }
        break;
        
    default:
        spdlog::error("Can't add light to scene, unknow light type: {}", static_cast<int>(type));
        assert(false);
        break;
    }
}
