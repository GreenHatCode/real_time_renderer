#pragma once 
#include "shader/shader.h"
#include <map>
#include <memory>

namespace renderer {

    // for now every geometry shader requires 2 steps in redering
    // 1) render normal model
    // 2) render model using geometry shader
    
    enum class GeometryProcessingTypes {
        NO_GEOMETRY_PROCESSING,
        NORMALS_VISUALIZATIONS,
        WIREFRAME_VISUALIZATION, 
        COUNT
    };

    inline const char* const* geometry_processing_types_names() 
    {
        static const char* names[] = {
            "No geometry processing",
            "Visualize normals",
            "Visualize wireframe"
        };
        return names;
    }

    class GeometryProcessor final {
        std::map<GeometryProcessingTypes, std::shared_ptr<Shader>> geometry_processing_shaders_;
   
    public:
        GeometryProcessor();
        std::shared_ptr<Shader> get_geometry_shader(GeometryProcessingTypes type);

    };
} // namespace renderer