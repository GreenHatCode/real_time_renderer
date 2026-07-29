#pragma once
#include "glm/glm.hpp"
#include "shader/shader.h"
#include <memory>

namespace scene
{   
    class SceneObject {
        glm::vec3 move_vector_;
        float rotation_angle_cw_; // radians
        glm::vec3 rotation_axes_;
        glm::vec3 scaling_vec_;

        std::string name_;

    protected:
        std::shared_ptr<Shader> shader_;

    public:
        SceneObject(const std::string& name, std::shared_ptr<Shader> shader = nullptr);

        void move(glm::vec3 move_vector);
        void rotate(float angle_rad, glm::vec3 axes_vec); // CW direction
        void scale(glm::vec3 prescaling_vec);

        glm::vec3 get_move_vector() const;
        float get_rotation_angle_cw() const;
        glm::vec3 get_rotation_axes() const;
        glm::vec3 get_scaling_vector() const;

        glm::mat4 model_matrix() const;

        void set_shader(const std::shared_ptr<Shader>& shader);
        std::shared_ptr<Shader> get_shader() const;

        std::string get_name() const;

        virtual void render() const = 0;
        virtual uint32_t get_vertices_count() const = 0;
        virtual uint32_t get_faces_count() const = 0;

        virtual ~SceneObject();
    };
} // namespace scene

