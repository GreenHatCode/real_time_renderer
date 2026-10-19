#pragma once
#include "opengl.h"
#include <cstdio>
#include "imgui.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "scene/scene.h"
#include "scene/lights/directional_light.h"
#include "scene/lights/point_light.h"
#include "scene/lights/spot_light.h"
#include "renderer/post_processing/post_processor.h"
#include "renderer/post_processing/geometry_processor.h"
#include <filesystem>

class UIWindow final {
    
    struct AddModelDialog_t {
        std::string model_file_path;
        std::string shader_name;
        std::string name;

        bool flip_normals;
    };

    struct AddShaderDialog_t {
        std::string vert_shader_file_path;
        std::string frag_shader_file_path;
        std::string geom_shader_file_path;
        std::string name;
    };

    struct AddLightDialog_t {
        std::string light_name;
        int32_t type;
    };

    struct ModelInspectorState_t
    {
        std::shared_ptr<scene::physical_objects::PhysicalObject> selected_model_ptr;
    };

    struct LightInspectorState_t {
        std::shared_ptr<scene::lights::LightBase> selected_light_ptr;
    };

private: // do not expose outside
    std::shared_ptr<scene::Scene> scene_;

    double prev_s_ = 0.0;
    double title_countdown_s_ = 0.1;
    double fps_ = 0.0;
    AddModelDialog_t model_dialog_state_;  
    AddShaderDialog_t shader_dialog_state_;
    AddLightDialog_t light_dialog_state_;
    ModelInspectorState_t model_inspector_state_;
    LightInspectorState_t light_inspector_state_;

    double gpu_time_to_display_;
    double cpu_time_to_display_;

private: // should be accessible 
    glm::vec4 clear_color_;
    double delta_time_;

    int32_t selected_post_processing_type_;
    int32_t selected_geometry_processing_type_;

    bool render_axes_;
    bool render_xz_grid_;
    bool render_xz_surface_;

    float gamma_;

public:
    UIWindow(const std::shared_ptr<scene::Scene>& scene);
    void render_window();
    glm::vec4 get_clear_color() const;
    renderer::PostProcessingTypes get_post_processing_type() const;
    renderer::GeometryProcessingTypes get_geometry_processing_type() const;
    float get_delta_time() const;
    uint8_t get_selected_internal_objects_id_mask() const;

    void set_gpu_time_to_display(double value);
    void set_cpu_time_to_display(double value);

    float get_gamma() const;

private:
    double calculate_fps();
    void render_model_dialog();
    void render_shader_dialog();
    void render_light_dialog();
    void render_model_inspector();

    void render_lights_inspector();
    void render_common_lights_editor(scene::lights::LightBase& light);
    void render_directional_light_editor(scene::lights::DirectionalLight& light);
    void render_point_light_editor(scene::lights::PointLight& light);
    void render_spot_light_editor(scene::lights::SpotLight& light);

    void set_model_selected(const std::shared_ptr<scene::physical_objects::PhysicalObject> &model_ptr, bool selected);
    void set_light_model_selected(const std::shared_ptr<scene::lights::LightBase>& light_ptr, bool selected);

public: // dev methods to add models and shader by hand from code
    void load_model(
        const std::string& shader_name, 
        const std::string& name, 
        const std::filesystem::path& file_path, 
        uint32_t additional_post_processing_flags,
        const glm::vec3& scale_vec = glm::vec3(1.0f, 1.0f, 1.0f)
    );
    void load_shader(
        const std::string &name, 
        const std::filesystem::path &vertex_shader_file, 
        const std::filesystem::path &fragment_shader_file, 
        const std::filesystem::path &geometry_shader_file = ""
    );
    void add_light(const std::string &name, scene::lights::LightType type);
};