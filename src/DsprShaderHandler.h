#pragma once

//
// Created by connor on 7/9/18.
//

#include <Modules/Gfx/Gfx.h>
#include <Sova/Internal/ShaderHandler.h>
#include "shaders.h"

using namespace Sova;

namespace DsprFrontend
{
    class DsprShaderHandler : public ShaderHandler
    {
    public:
        virtual Oryol::Id getNormalShader()
        {
            return Gfx::CreateResource(NormalShader::Setup());
        }
        virtual Oryol::Id getCanvasShader()
        {
            return Gfx::CreateResource(CanvasShader::Setup());
        }
    };
}