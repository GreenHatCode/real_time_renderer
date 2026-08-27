#include "renderer/post_processing/post_processor.h"

renderer::PostProcessor::PostProcessor(GLsizei width, GLsizei height)
    : framebuffer_(width, height)
{
    // fill map with shaders
    post_processing_shaders_.try_emplace(PostProcessingTypes::INVERSION, "inversion_shader", INVERSION_VERTEX_SHADER.data(), INVERSION_FRAGMENT_SHADER.data());
    post_processing_shaders_.try_emplace(PostProcessingTypes::GRAYSCALE, "grayscale_shader", GRAYSCALE_VERTEX_SHADER.data(), GRAYSCALE_FRAGMENT_SHADER.data());
    post_processing_shaders_.try_emplace(PostProcessingTypes::BLUR, "blur_shader", BLUR_VERTEX_SHADER.data(), BLUR_FRAGMENT_SHADER.data());
    post_processing_shaders_.try_emplace(PostProcessingTypes::EDGE_DETECTION, "edge_detection_shader", EDGE_DETECTION_VERTEX_SHADER.data(), EDGE_DETECTION_FRAGMENT_SHADER.data());

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
    post_processing_shaders_.at(type).set_uniform("screen_texture", 0);
    glBindTexture(GL_TEXTURE_2D, framebuffer_.get_frame_texture());
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 6);


    // restore to defaults
    glEnable(GL_DEPTH_TEST);
}
