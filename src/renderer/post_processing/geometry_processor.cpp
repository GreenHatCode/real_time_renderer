#include "renderer/post_processing/geometry_processor.h"

renderer::GeometryProcessor::GeometryProcessor()
{
    geometry_processing_shaders_.try_emplace(
        GeometryProcessingTypes::NORMALS_VISUALIZATIONS,
        std::make_shared<Shader>(
            "normals_visualization_gs",
            "shaders_sources/internal/deferred_rendering/geometry_path.vert",
            "shaders_sources/geometry/normals_visualization.fs",
            "shaders_sources/geometry/normals_visualization.gs"
        )
    );

    geometry_processing_shaders_.try_emplace(
        GeometryProcessingTypes::WIREFRAME_VISUALIZATION,
        std::make_shared<Shader>(
            "wireframe_visualization_gs",
            "shaders_sources/internal/deferred_rendering/geometry_path.vert",
            "shaders_sources/geometry/wireframe_visualization.fs",
            "shaders_sources/geometry/wireframe_visualization.gs"
        )
    );

}

std::shared_ptr<Shader> renderer::GeometryProcessor::get_geometry_shader(GeometryProcessingTypes type)
{
    auto it = geometry_processing_shaders_.find(type);
    assert(it != geometry_processing_shaders_.end());
    return it->second;
}
