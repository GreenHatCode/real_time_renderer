#include "renderer/post_processing/geometry_processor.h"

renderer::GeometryProcessor::GeometryProcessor()
{
    geometry_processing_shaders_.try_emplace(
        GeometryProcessingTypes::NORMALS_VISUALIZATIONS,
        std::make_shared<Shader>(
            "normals_visualization_gs",
            "shaders_sources/geometry/vertex_shader.vs",
            "shaders_sources/geometry/fragment_shader.fs",
            "shaders_sources/geometry/display_faces_normals.gs"
        )
    );

    // geometry_processing_shaders_.try_emplace(
    //     GeometryProcessingTypes::WIREFRAME_VISUALIZATION,
    //     "wireframe_visualization_gs",
    //     "shaders_sources/post_processing/inversion/vertex_shader.vs",
    //     "shaders_sources/post_processing/inversion/vertex_shader.fs",
    //     "shaders_sources/post_processing/inversion/vertex_shader.gs"
    // );

    // geometry_processing_shaders_.try_emplace(
    //     GeometryProcessingTypes::SILHOUETTE,
    //     "silhouette_gs",
    //     "shaders_sources/post_processing/inversion/vertex_shader.vs",
    //     "shaders_sources/post_processing/inversion/vertex_shader.fs",
    //     "shaders_sources/post_processing/inversion/vertex_shader.gs"
    // );

}

std::shared_ptr<Shader> renderer::GeometryProcessor::get_geometry_shader(GeometryProcessingTypes type)
{
    auto it = geometry_processing_shaders_.find(type);
    assert(it != geometry_processing_shaders_.end());
    return it->second;
}
