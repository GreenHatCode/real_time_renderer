#pragma once
#include "shader/shader.h"
#include "scene/i_scene_object.h"

namespace scene::lights {

    enum class LightType
    {
        DIRECTIONAL_LIGHT,
        POINT_LIGHT,
        SPOT_LIGHT,
        COUNT
    };

    inline const char* const* get_light_type_names() 
    {
        static const char* names[] = {
            "Directional Light",
            "Point Light",
            "Spot Light"
        };
        return names;
    }

    class ILight: public ISceneObject {
    public:
        virtual void upload_components(Shader& obj_shader, size_t light_idx) const = 0;
        virtual LightType get_type() const = 0;
        virtual ~ILight() = default;
    };

} // namespace scene::lights