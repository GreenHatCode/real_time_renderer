#pragma once
#include "opengl.h"
#include <vector>
#include "glm/glm.hpp"
#include "renderer/frame_buffers/g_framebuffer.h"
#include "renderer/frame_buffers/frame_buffer.h"
#include "assimp_wrapper/textures/texture_2d.h"
#include "shader/shader.h"

namespace renderer {

    class SSAOProcessor {

        framebuffers::FrameBuffer ssao_framebuffer_;
        framebuffers::FrameBuffer ssao_blur_framebuffer_;
        assimp_wrapper::Texture2D noise_texture_;
        std::vector<glm::vec3> kernels_;

        std::unique_ptr<Shader> ssao_shader_;
        std::unique_ptr<Shader> ssao_blur_shader_;

        static float lerp(float a, float b, float f);

    public:
        static inline int32_t SAMPLES_COUNT = 64;
        static inline int32_t SSAO_BLUR_SIZE = 4;
        
        SSAOProcessor();
        void do_ssao_render(const framebuffers::GFrameBuffer &g_buffer, const glm::mat4& camera_view_mat, const glm::mat4& camera_projection_mat);
        GLuint get_ssao_oclusion_texture();

    };
}

