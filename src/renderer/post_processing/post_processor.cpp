#include "renderer/post_processing/post_processor.h"

using namespace renderer;
using namespace framebuffers;
using namespace post_processing;

PostProcessor::PostProcessor()
    : framebuffer_(FrameBuffer::create_post_processing())
{
    // fill map with shaders
    post_processing_shaders_.try_emplace(
        PostProcessingTypes::INVERSION, 
        "inversion_shader", 
        "shaders_sources/post_processing/vertex_shader.vert", 
        "shaders_sources/post_processing/inversion.frag"
    );

    post_processing_shaders_.try_emplace(
        PostProcessingTypes::GRAYSCALE, 
        "grayscale_shader", 
        "shaders_sources/post_processing/vertex_shader.vert", 
        "shaders_sources/post_processing/grayscale.frag"
    );

    post_processing_shaders_.try_emplace(
        PostProcessingTypes::BLUR, 
        "blur_shader", 
        "shaders_sources/post_processing/vertex_shader.vert", 
        "shaders_sources/post_processing/blur.frag"
    );

    post_processing_shaders_.try_emplace(
        PostProcessingTypes::EDGE_DETECTION, 
        "edge_detection_shader", 
        "shaders_sources/post_processing/vertex_shader.vert", 
        "shaders_sources/post_processing/edge_detection.frag"
    );

    post_processing_shaders_.try_emplace(
        PostProcessingTypes::GAMMA_CORRECTION, 
        "gamma_correction_shader", 
        "shaders_sources/post_processing/vertex_shader.vert", 
        "shaders_sources/post_processing/gamma_correction.frag"
    );

    post_processing_shaders_.try_emplace(
        PostProcessingTypes::HDR_POST_PROCESSING, 
        "hdr_shader", 
        "shaders_sources/post_processing/vertex_shader.vert", 
        "shaders_sources/post_processing/hdr_post_process.frag"
    );
}

void PostProcessor::enable_off_sceen_rendering(GLsizei scr_width, GLsizei scr_height)
{
    framebuffer_.rescale(scr_width, scr_height);
    framebuffer_.bind();
    glEnable(GL_DEPTH_TEST);
}

void PostProcessor::do_post_process(PostProcessingTypes type)
{
    framebuffer_.unbind();
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    
    post_processing_shaders_.at(type).use();

    glActiveTexture(GL_TEXTURE0);
    post_processing_shaders_.at(type).set_uniform("u_screen_texture", 0);
    post_processing_shaders_.at(type).set_uniform("u_gamma", gamma_);
    glBindTexture(GL_TEXTURE_2D, framebuffer_.get_frame_texture());
    
    post_processing::render_zero_buffer_quad();


    // restore to defaults
    glEnable(GL_DEPTH_TEST);
}

void PostProcessor::set_gamma(float value)
{
    gamma_ = value;
}
