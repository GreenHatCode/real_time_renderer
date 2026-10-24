#include "renderer/post_processing/post_processor.h"

using namespace renderer::framebuffers;

renderer::PostProcessor::PostProcessor(GLsizei width, GLsizei height)
    : framebuffer_(width, height)
{
    // fill map with shaders
    post_processing_shaders_.try_emplace(
        PostProcessingTypes::INVERSION, 
        "inversion_shader", 
        "shaders_sources/post_processing/inversion/vertex_shader.vert", 
        "shaders_sources/post_processing/inversion/fragment_shader.frag"
    );

    post_processing_shaders_.try_emplace(
        PostProcessingTypes::GRAYSCALE, 
        "grayscale_shader", 
        "shaders_sources/post_processing/grayscale/vertex_shader.vert", 
        "shaders_sources/post_processing/grayscale/fragment_shader.frag"
    );

    post_processing_shaders_.try_emplace(
        PostProcessingTypes::BLUR, 
        "blur_shader", 
        "shaders_sources/post_processing/blur/vertex_shader.vert", 
        "shaders_sources/post_processing/blur/fragment_shader.frag"
    );

    post_processing_shaders_.try_emplace(
        PostProcessingTypes::EDGE_DETECTION, 
        "edge_detection_shader", 
        "shaders_sources/post_processing/edge_detection/vertex_shader.vert", 
        "shaders_sources/post_processing/edge_detection/fragment_shader.frag"
    );

    post_processing_shaders_.try_emplace(
        PostProcessingTypes::GAMMA_CORRECTION, 
        "gamma_correction_shader", 
        "shaders_sources/post_processing/gamma_correction/vertex_shader.vert", 
        "shaders_sources/post_processing/gamma_correction/fragment_shader.frag"
    );

    post_processing_shaders_.try_emplace(
        PostProcessingTypes::HDR_POST_PROCESSING, 
        "hdr_shader", 
        "shaders_sources/post_processing/hdr/vertex_shader.vert", 
        "shaders_sources/post_processing/hdr/fragment_shader.frag"
    );

    // allocate memory for quad that will be used
    // to render framebuffer on the final window

    float quat_vertices[] = { // vertex attributes for a quad that fills the entire screen in NDC
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glCreateVertexArrays(1, &vao_);
    glCreateBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quat_vertices), quat_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
}

renderer::PostProcessor::~PostProcessor()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

void renderer::PostProcessor::enable_off_sceen_rendering(GLsizei scr_width, GLsizei scr_height)
{
    framebuffer_.rescale(scr_width, scr_height);
    framebuffer_.bind();
    glEnable(GL_DEPTH_TEST);
}

void renderer::PostProcessor::do_post_process(PostProcessingTypes type)
{
    framebuffer_.unbind();
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    
    post_processing_shaders_.at(type).use();

    glActiveTexture(GL_TEXTURE0);
    post_processing_shaders_.at(type).set_uniform("u_screen_texture", 0);
    post_processing_shaders_.at(type).set_uniform("u_gamma", gamma_);
    glBindTexture(GL_TEXTURE_2D, framebuffer_.get_frame_texture());
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 6);


    // restore to defaults
    glEnable(GL_DEPTH_TEST);
}

void renderer::PostProcessor::set_gamma(float value)
{
    gamma_ = value;
}
