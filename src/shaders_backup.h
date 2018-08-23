#pragma once
//-----------------------------------------------------------------------------
/*  #version:49#
    machine generated, do not edit!
*/
#include "Gfx/GfxTypes.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat2x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"
#include "Resource/Id.h"
namespace CanvasShader {
    static const int tex = 0;
    extern Oryol::ShaderSetup Setup();
}
namespace NormalShader {
    static const int canvas = 0;
    extern Oryol::ShaderSetup Setup();
}
namespace ShapeShader {
    extern Oryol::ShaderSetup Setup();
}

