#include "renderer/post_processing/ssao_processor.h"
#include <random>
#include "spdlog/spdlog.h"
#include "renderer/constants.h"
#include "renderer/post_processing/common.h"

using namespace renderer;

float renderer::SSAOProcessor::lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

SSAOProcessor::SSAOProcessor()
    : ssao_framebuffer_(framebuffers::FrameBuffer::create_ssao())
    , ssao_blur_framebuffer_(framebuffers::FrameBuffer::create_ssao_blur())
    , ssao_shader_(nullptr)
    , ssao_blur_shader_(nullptr)
{
    std::uniform_real_distribution<GLfloat> rand_floats(0.0f, 1.0f);
    std::default_random_engine generator;
    for (size_t i = 0; i < SAMPLES_COUNT; ++i) {
        glm::vec3 sample(rand_floats(generator) * 2.0f - 1.0f, rand_floats(generator) * 2.0f - 1.0f, rand_floats(generator));
        sample = glm::normalize(sample);
        sample *= rand_floats(generator);

        // scale samples to make them more aligned to center of kernel
        float scale = static_cast<float>(i) / static_cast<float>(SAMPLES_COUNT);
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        kernels_.push_back(sample);
    }


    // generating noise texture 4x4 size
    std::vector<glm::vec3> noise_data;
    for (size_t i = 0; i < 16; ++i) {
        // rotation around z-axis in tangent space
        glm::vec3 noise (rand_floats(generator) * 2.0f - 1.0f, rand_floats(generator) * 2.0f - 1.0f, 0.0f); 
        noise_data.push_back(glm::normalize(noise)); // originally normalized in ssao_shader
    }

    noise_texture_ = assimp_wrapper::Texture2D::create_noise_texture(aiTextureType::aiTextureType_AMBIENT, 4, 4, noise_data.data());


    // load ssao shaders
    try {
        ssao_shader_ = std::make_unique<Shader>(
            "ssao_shader", 
            "shaders_sources/post_processing/vertex_shader.vert",
            "shaders_sources/post_processing/ssao.frag"
        );
    } catch (const std::exception& e) {
        spdlog::error("Failed to load ssao shader: {}, what: {}", "ssao_shader", e.what());
        throw;
    }
    



}

void renderer::SSAOProcessor::do_ssao_render(const framebuffers::GFrameBuffer &g_buffer, const glm::mat4& camera_projection_mat)
{
    // do ssao path
    ssao_framebuffer_.rescale(g_buffer.get_width(), g_buffer.get_height());
    ssao_framebuffer_.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);



    ssao_shader_->use();

    // set uniforms
    ssao_shader_->set_uniform("noise_texture", bind_texture_units::NOISE_TEXTURE_TEXTURE_UNIT);
    glBindTextureUnit(bind_texture_units::NOISE_TEXTURE_TEXTURE_UNIT, noise_texture_.id());
    ssao_shader_->set_uniform("frag_coords_texture", bind_texture_units::FRAG_COORDS_TEXTURE_UNIT);
    glBindTextureUnit(bind_texture_units::FRAG_COORDS_TEXTURE_UNIT, g_buffer.get_position_buffer_texture());
    ssao_shader_->set_uniform("frag_normal_texture", bind_texture_units::FRAG_NORMAL_TEXTURE_UNIT);
    glBindTextureUnit(bind_texture_units::FRAG_NORMAL_TEXTURE_UNIT, g_buffer.get_normal_buffer_texture());
    ssao_shader_->set_uniform("depth_buffer_texture", bind_texture_units::DEPTH_BUFFER_TEXTURE_UNIT);
    glBindTextureUnit(bind_texture_units::DEPTH_BUFFER_TEXTURE_UNIT, g_buffer.get_depth_map_texture());

    for (size_t i = 0; i < SAMPLES_COUNT; ++i) {
        std::string name = "samples[" + std::to_string(i) + "]";
        ssao_shader_->set_uniform(name, kernels_[i]);
    }

    glm::vec2 noise_scale = glm::vec2(g_buffer.get_width() / 4.0, g_buffer.get_height() / 4.0); // screen size divided by noise size 
    ssao_shader_->set_uniform("noise_scale", noise_scale);

    ssao_shader_->set_uniform("u_projection_mat", camera_projection_mat);

    post_processing::render_zero_buffer_quad();
    ssao_framebuffer_.unbind();

    // do ssao blur

}

GLuint renderer::SSAOProcessor::get_ssao_oclusion_texture()
{
    return ssao_blur_framebuffer_.get_frame_texture();
}
