//
// Created by connor on 7/9/18.
//

#include "DsprShaderHandler.h"
#include <Modules/Gfx/Gfx.h>
#include "shaders.h"

using namespace Oryol;

namespace DsprFrontend
{
    Oryol::Id DsprShaderHandler::getScreenShader()
    {
        return Gfx::CreateResource(ScreenShader::Setup());
    }
    Oryol::Id DsprShaderHandler::getBackScreenShader()
    {
        return Gfx::CreateResource(BackScreenShader::Setup());
    }
    Oryol::Id DsprShaderHandler::getCanvasShader()
    {
        return Gfx::CreateResource(CanvasShader::Setup());
    }
    Oryol::Id DsprShaderHandler::getShapeShader()
    {
        return Gfx::CreateResource(ShapeShader::Setup());
    }
}