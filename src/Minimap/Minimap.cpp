//
// Created by connor on 8/20/18.
//

#include <Sova/Internal/InternalApp.h>
#include "MinimapFog.h"
#include "MinimapUnits.h"
#include "Global.h"

#include "Sova/Graphics/Internal/InternalCamera.h"

namespace DsprFrontend {

    Minimap::Minimap()
    {
        this->g = (Global*) InternalApp::getSovaApp()->getGlobal();

        this->terrainCamera = New<Camera>(0,0,64,64,Null<Container>(),false);
        auto terrainViewport = New<Viewport>(0,0,64,64,this->terrainCamera);
        g->app->addViewport(terrainViewport);

        auto minimapUnits = New<MinimapUnits>();
        auto unitsCamera = New<Camera>(0,0,64,64,minimapUnits);
        auto unitsViewport = New<Viewport>(0,0,64,64,unitsCamera);
        g->app->addViewport(unitsViewport);

        auto minimapFog = New<MinimapFog>();
        auto fogCamera = New<Camera>(0,0,64,64,minimapFog);
        auto fogViewport = New<Viewport>(0,0,64,64,fogCamera);
        g->app->addViewport(fogViewport);
    }

    void Minimap::DrawTile(int x, int y, Color color)
    {
        Oryol::Gfx::BeginPass(this->terrainCamera->getInternalCamera()->getCanvasPass());
        this->pixel->setLineStyle(1, color);
        this->pixel->drawSelf(this->terrainCamera, x, y);
        Oryol::Gfx::EndPass();
    }
}