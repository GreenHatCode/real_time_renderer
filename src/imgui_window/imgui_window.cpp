#include "imgui_window/imgui_window.h"
#include "misc/cpp/imgui_stdlib.h"

ImGuiWindow::ImGuiWindow(const std::shared_ptr<scene::Scene> &scene)
    : scene_(scene)
    , model_dialog_state_{.model_file_path="", .shader_name="", .name=""}
    , shader_dialog_state_{.vert_shader_file_path="", .frag_shader_file_path="", .name=""}
    , model_inspector_state_{.selected_model_ptr=nullptr}
{

}

void ImGuiWindow::render_window()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);

    // rendering options
    ImGui::Text("Read-Timer Renderer");
    ImGui::ColorEdit4("clear color", glm::value_ptr(clear_color_));
    ImGui::Separator();

    // loading models and shaders
    
    if (ImGui::Button("manage model")) {
        ImGui::OpenPopup("manage model");
    }
    render_model_dialog();
    ImGui::SameLine();
    
    if (ImGui::Button("manage shader")) {
        ImGui::OpenPopup("manage shader");
    }
    render_shader_dialog();
    ImGui::Separator();


    render_model_inspector();
    ImGui::Separator();


    // rendering statistics
    ImGui::Text("App average FPS: %.2f", calculate_fps());
    ImGui::Text("App frame time: %.2f", delta_time_);
    ImGui::Text("Total vertices count: %d", scene_->get_total_vertices_count());
    ImGui::Text("Total faces count: %d", scene_->get_total_faces_count());
}

glm::vec4 ImGuiWindow::get_clear_color() const
{
    return clear_color_;
}

float ImGuiWindow::get_delta_time() const
{
    return static_cast<float>(delta_time_);
}

double ImGuiWindow::calculate_fps()
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

void ImGuiWindow::render_model_dialog()
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
            scene_->delete_model(model_dialog_state_.name);
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

void ImGuiWindow::render_shader_dialog()
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

        if (ImGui::Button("Cancel")) {
            shader_dialog_state_ = {};
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
void ImGuiWindow::render_model_inspector()
{
    // list box of loaded models
    ImGui::Text("Scene Objects");
    ImVec2 list_box_size = ImGui::GetWindowSize();
    list_box_size.y = 60;
    if (ImGui::BeginListBox("##", list_box_size)) {
        for (const auto& m : scene_->get_models()) {
            bool selected = model_inspector_state_.selected_model_ptr == m;

            if (ImGui::Selectable(m->get_name().c_str(), selected)) {
                model_inspector_state_.selected_model_ptr = m;
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
void ImGuiWindow::load_model(const std::string &shader_name, const std::string &name, const std::filesystem::path &file_path)
{
    std::shared_ptr<Shader> model_shader = scene_->get_shader(shader_name);
    auto loaded_model = scene_->add_model(name, file_path);
    if (model_shader == nullptr) spdlog::error("Failed to load model. There isn't shader with name={}", shader_name.c_str());
    if (loaded_model == nullptr) spdlog::error("Failed to load model. Failed to load model object from file: {}", file_path.c_str());

    loaded_model->set_shader(model_shader);
    loaded_models_names_.push_back(name);
}

void ImGuiWindow::load_shader(const std::string &name, const std::filesystem::path &vertex_shader_file, const std::filesystem::path &fragment_shader_file)
{
    std::shared_ptr<Shader> new_shader = scene_->add_shader(name, vertex_shader_file, fragment_shader_file);
    if (new_shader == nullptr) {
        spdlog::error("Failed to load shader");
        return; 
    }
    loaded_shaders_names_.push_back(shader_dialog_state_.name);
}
