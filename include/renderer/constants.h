#pragma once
#include <string_view>
#include "glm/glm.hpp"

namespace renderer {

    namespace defaults {        
        inline constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
        inline constexpr glm::vec3 FALLBACK_WORLD_UP = glm::vec3(0.0f, 0.0f, 1.0f);

        inline constexpr uint8_t AXES_LINES_ID = 0x01; // 0b00000001
        inline constexpr uint8_t XZ_FACE_GRID_ID = 0x02; // 0b000010
        inline constexpr uint8_t XZ_FACE_SOLID_ID = 0x04; // 0b00100
    } // namespace defaults

    namespace bind_texture_units {

        // if texture units have the same values,
        // it means that they should not be used
        // in the same shader

        inline constexpr int32_t DIRECTIONAL_LIGHTS_SHADOW_MAP_TEXTURE_UNIT = 0;
        inline constexpr int32_t SPOT_LIGHTS_SHADOW_MAP_TEXTURE_UNIT = 1;
        inline constexpr int32_t POINT_LIGHTS_SHADOW_MAP_TEXTURE_UNIT = 2;

        // 2 units reserved for PBR

        inline constexpr int32_t AMBIENT_TEXTURE_TEXTURE_UNIT = 5;
        inline constexpr int32_t DIFFUSE_TEXTURE_TEXTURE_UNIT = 6;
        inline constexpr int32_t SPECULAR_TEXTURE_TEXTURE_UNIT = 7;
        inline constexpr int32_t HEIGHT_TEXTURE_TEXTURE_UNIT = 8;

        // deferred render with ssao
        inline constexpr int32_t NOISE_TEXTURE_TEXTURE_UNIT = 5;
        inline constexpr int32_t FRAG_COORDS_TEXTURE_UNIT = 6;
        inline constexpr int32_t FRAG_NORMAL_TEXTURE_UNIT = 7;
        inline constexpr int32_t FRAG_COLOR_TEXTURE_UNIT = 8;
        inline constexpr int32_t DEPTH_BUFFER_TEXTURE_UNIT = 9;


    } // namespace bind_texture_units



};