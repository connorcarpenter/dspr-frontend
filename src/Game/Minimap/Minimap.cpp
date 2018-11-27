//
// Created by connor on 8/20/18.
//

#include <Sova/Internal/InternalApp.h>
#include "Sova/Graphics/Internal/InternalCamera.h"

#include "MinimapFog.h"
#include "MinimapUnits.h"
#include "Game/Global.h"
#include "Minimap.h"
#include "Game/UI/Cursor.h"
#include "Game/TileManager.h"

namespace DsprFrontend {

    Minimap::Minimap()
    {
        this->g = (Global*) InternalApp::getGlobal();
        g->cursor = New<Cursor>();

        this->pixel = New<Pixel>();

        this->OnUpdate([&](float deltaFrameMs){ Step(deltaFrameMs); });
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
        g->minimap->DrawTile(x, y, Color::Brown);
    }

    void Minimap::initMinimap(int width, int height) {
        this->receivedGrid = true;
        this->terrainCamera = New<Camera>(0,0,width,height,Null<Container>(), Color::Black, 1.0f, false, false);
        auto terrainViewport = g->app->createViewport(5*5,103*5,38*5,38*5, this->terrainCamera);

        auto minimapFog = New<MinimapFog>();
        auto fogCamera = New<Camera>(0,0,width,height,minimapFog, Color::Black, 0.0f, true, false);
        fogCamera->SkipFramesToDrawFramesRatio = 5;
        auto fogViewport = g->app->createViewport(5*5,103*5,38*5,38*5,fogCamera);

        auto minimapUnits = New<MinimapUnits>();
        auto unitsCamera = New<Camera>(0,0,width,height, minimapUnits, Color::Black, 0.0f, true, true);
        unitsCamera->SkipFramesToDrawFramesRatio = 5;
        auto unitsViewport = g->app->createViewport(5*5,103*5,38*5,38*5, unitsCamera);

        auto cursorCamera = New<Camera>(0,0,g->app->width/5,g->app->height/5, g->cursor, Color::Black, 0.0f, true, true);
        auto cursorViewport = g->app->createViewport(0,0,g->app->width,g->app->height, cursorCamera);

        fogCamera->clearCamera();
        this->terrainCamera->clearCamera();
    }

    void Minimap::ReceiveGrid(int width, int height) {
        initMinimap(width*2, height*2);
    }
}