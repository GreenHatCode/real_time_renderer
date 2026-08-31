#include "renderer/post_processing/geometry_processor.h"

renderer::GeometryProcessor::GeometryProcessor()
{
    geometry_processing_shaders_.try_emplace(
        GeometryProcessingTypes::NORMALS_VISUALIZATIONS,
        "normals_visualization_gs",
        "shaders_sources/geometry/vertex_shader.vs",
        "shaders_sources/geometry/fragment_shader.fs",
        "shaders_sources/geometry/display_faces_normals.gs"
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

void renderer::GeometryProcessor::use_geometry_shader(GeometryProcessingTypes type)
{
    geometry_processing_shaders_.at(type).use();
}
