#pragma once

//
// Created by connor on 7/9/18.
//

#include <Sova/Graphics/Internal/ShaderHandler.h>
#include <Sova/References/RefTypes.h>

using namespace Sova;
//using namespace Oryol;

namespace DsprFrontend
{
    class DsprShaderHandler : public ShaderHandler
    {
    public:
        SovaTypeDecl(DsprShaderHandler, ShaderHandler);
        virtual Oryol::Id getScreenShader();
        virtual Oryol::Id getBackScreenShader();
        virtual Oryol::Id getCanvasShader();
        virtual Oryol::Id getShapeShader();
    };
}