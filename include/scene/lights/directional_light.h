#pragma once
#include "scene/lights/light_base.h"
#include "scene/i_scene_object.h"
#include <string>

namespace scene::lights
{
    class DirectionalLight final: public ISceneObject, public LightBase {
        glm::vec3 direction_;
        std::string name_;

    public:
        DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, 
            glm::vec3 specular, glm::vec4 rgba_color, glm::vec3 direction);
        void set_direction(glm::vec3 direction);
        glm::vec3 get_direction() const;
        std::string get_name() const override;
    };
    
} // namespace scene::lights
