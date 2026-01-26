#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include "SOGLR/Rendering/Texture.hpp"

namespace SOUI
{
    struct Character
    {
        std::shared_ptr<SOGLR::Texture> texture;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        uint32_t Advance;
    };
}