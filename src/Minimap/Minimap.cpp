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

        this->terrainCamera = New<Camera>(0,0,128,128,Null<Container>(), Color::Black, 1.0f, false, false);
        auto terrainViewport = New<Viewport>(5*5,103*5,38*5,38*5, this->terrainCamera);
        g->app->addViewport(terrainViewport);

        auto minimapFog = New<MinimapFog>();
        auto fogCamera = New<Camera>(0,0,128,128,minimapFog, Color::Black, 0.0f, true, false);
        fogCamera->SkipFramesToDrawFramesRatio = 1;
        auto fogViewport = New<Viewport>(5*5,103*5,38*5,38*5,fogCamera);
        g->app->addViewport(fogViewport);

        auto minimapUnits = New<MinimapUnits>();
        auto unitsCamera = New<Camera>(0,0,128,128, minimapUnits, Color::Black, 0.0f);
        unitsCamera->SkipFramesToDrawFramesRatio = 5;
        auto unitsViewport = New<Viewport>(5*5,103*5,38*5,38*5, unitsCamera);
        g->app->addViewport(unitsViewport);

        g->cursor = New<Cursor>();
        auto cursorCamera = New<Camera>(0,0,g->app->width/5,g->app->height/5, g->cursor, Color::Black, 0.0f);
        auto cursorViewport = New<Viewport>(0,0,g->app->width,g->app->height, cursorCamera);
        g->app->addViewport(cursorViewport);

        this->pixel = New<Pixel>();

        this->OnUpdate([&](float deltaFrameMs){ Step(deltaFrameMs); });

        g->world->AddChild(ThisRef<Minimap>());
    }

    void Minimap::Step(float deltaFrameMs) {
        g->cursor->Update(deltaFrameMs);
    }

    void Minimap::DrawTile(int x, int y, Color color)
    {
        Oryol::Gfx::BeginPass(this->terrainCamera->getInternalCamera()->getCanvasPass());
        this->pixel->setLineStyle(1, color);
        this->pixel->position->set(x, y);
        this->pixel->drawSelf(this->terrainCamera, 0,0);
        this->pixel->position->set(x+1, y);
        this->pixel->drawSelf(this->terrainCamera, 0,0);
        Oryol::Gfx::EndPass();
    }

    void Minimap::DrawTile(int x, int y) {
        auto frame = this->g->tileManager->getTileFrame(x,y);
        if (frame == -1) return;
        if (frame >= 0 && frame <=2) g->minimap->DrawTile(x, y, Color::Green);
        if (frame >= 3 && frame <=5) g->minimap->DrawTile(x, y, Color::Blue);
        if (frame >= 6 && frame <=8) g->minimap->DrawTile(x, y, Color::Brown);
    }
}