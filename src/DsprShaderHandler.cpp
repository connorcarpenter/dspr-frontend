//
// Created by connor on 7/9/18.
//

#include "DsprShaderHandler.h"
#include <Modules/Gfx/Gfx.h>
#include "shaders.h"

using namespace Oryol;

namespace DsprFrontend
{
    Oryol::Id DsprShaderHandler::getNormalShader()
    {
        return Gfx::CreateResource(NormalShader::Setup());
    }
    Oryol::Id DsprShaderHandler::getCanvasShader()
    {
        return Gfx::CreateResource(CanvasShader::Setup());
    }
}