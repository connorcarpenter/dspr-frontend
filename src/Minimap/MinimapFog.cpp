//
// Created by connor on 8/20/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Global.h>
#include "MinimapFog.h"
#include "FogManager.h"

namespace DsprFrontend {

    MinimapFog::MinimapFog()
    {
        this->g = (Global*) InternalApp::getSovaApp()->getGlobal();
    }

    void MinimapFog::drawSelf(Sova::Ref<Camera> camera, int xoffset, int yoffset)
    {
        this->g->fogManager->minimapDrawFog(camera, xoffset, yoffset);
    }
}