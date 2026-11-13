#pragma once

namespace renderer {
    namespace post_processing {

        inline constexpr float MIN_GAMMA = 0.1f;
        inline constexpr float MAX_GAMMA = 5.0f;

        enum class PostProcessingTypes {
            NO_POST_PROCESSING,
            INVERSION,
            GRAYSCALE, 
            BLUR, 
            EDGE_DETECTION,
            GAMMA_CORRECTION, 
            HDR_POST_PROCESSING,
            COUNT
        };

        inline const char* const* post_processing_types_names() 
        {
            static const char* names[] = {
                "No post processing",
                "Inversion",
                "Grayscale",
                "Blur",
                "Edge detection",
                "Gamma correction",
                "HDR post processing"
            };
            return names;
        }

        inline void render_zero_buffer_quad() 
        {
            static GLuint empty_vao = 0;
            if (empty_vao == 0) {
                glCreateVertexArrays(1, &empty_vao);
            }

            glBindVertexArray(empty_vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }


    }
}