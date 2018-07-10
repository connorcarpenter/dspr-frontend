#pragma once

//
// Created by connor on 7/9/18.
//

#include <Sova/Internal/ShaderHandler.h>

using namespace Sova;

namespace DsprFrontend
{
    class Id;
    class DsprShaderHandler : public ShaderHandler
    {
    public:
        virtual Id getNormalShader();
        virtual Id getCanvasShader();
    };
}