#pragma once

//
// Created by connor on 4/9/18.
//

#include <iostream>

#include <Sova/References/Refable.h>
#include <Sova/References/Ref.h>
#include <Modules/Core/Time/TimePoint.h>
#include <Modules/Core/Time/Clock.h>
#include "Sova/SovaMain.h"

#include "DsprShaderHandler.h"
#include "Game/Global.h"
#include "Game/Unit/UnitManager.h"
#include "Game/FogManager.h"
#include "Game/UI/UiManager.h"
#include "SpriteCatalog.h"
#include "Game/TileManager.h"
#include "Game/Minimap/Minimap.h"
#include "Network/NetworkManager.h"
#include "Game/UI/Cursor.h"
#include "Game/UI/ButtonCardCatalog.h"
#include "Circle/CircleCache.h"
#include "IsoBox/IsoBoxCache.h"
#include "Game/Unit/UnitTemplateCatalog.h"
#include "ResourceList.h"
#include "Game/EconomyManager.h"
#include "GraphicsManager.h"
#include "Game/Item/ItemManager.h"
#include "Game/Unit/ShadowManager.h"
#include "Game/Item/ItemTemplateCatalog.h"
#include "Game/UI/ChatManager.h"

using namespace Sova;

namespace DsprFrontend
{
    class Controller : public Sova::Refable
    {
    public:

        Controller();
        virtual const char* getClassName() { return "Controller"; }

        void onLoadFinish();
        void onGameStart();
        void onGameUpdate(float deltaFrameMs);

        Ref<Global> g = Null<Global>();
        Ref<Timer> tickTimer = Null<Timer>();
        const int tickRateMs = 100;

        void onGameTick();
    };

    Controller::Controller() = default;

    void Controller::onGameStart()
    {
        {
            g = New<Global>();
            g->app = New<App>(1280, 720, New<String>("Demo"), New<DsprShaderHandler>());
            g->app->setGlobal(g.obj);

            g->app->load(New<ResourceList>()->getResources())
                    ->onFinish(
                            [&]() {
                                onLoadFinish();
                            });
        }

        g->app->start(); // code continues processing within this function, which is why it should be outside the above scope
    }

    void Controller::onLoadFinish()
    {
        // after loading is done
        g->world = New<Container>();
        static const int pixelRatio = 5;
        g->camera = New<Camera>(20, 20, g->app->width / pixelRatio, g->app->height / pixelRatio, g->world);
        g->viewport = g->app->createViewport(0, 0, g->app->width, g->app->height, g->camera);

        g->spriteCatalog = New<SpriteCatalog>();
        g->circleCache = New<CircleCache>();
        g->isoBoxCache = New<IsoBoxCache>();

        g->graphicsManager = New<GraphicsManager>();
        g->economyManager = New<EconomyManager>();
        g->tribeManager = New<TribeManager>();

        g->tileManager = New<TileManager>();
        g->tileManager->SetDepth(9999);
        g->world->AddChild(g->tileManager);

        g->shadowManager = New<ShadowManager>();
        g->shadowManager->SetDepth(8888);
        g->world->AddChild(g->shadowManager);

        g->unitManager = New<UnitManager>();
        g->itemTemplateCatalog = New<ItemTemplateCatalog>();
        g->itemManager = New<ItemManager>();

        g->uiManager = New<UiManager>();
        g->uiManager->SetDepth(-9990);
        g->world->AddChild(g->uiManager);

        g->chatManager = New<ChatManager>();
        g->chatManager->SetDepth(-9991);
        g->world->AddChild(g->chatManager);

        g->buttonCardCatalog = New<ButtonCardCatalog>();
        g->unitTemplateCatalog = New<UnitTemplateCatalog>();

        g->fogManager = New<FogManager>();
        g->fogManager->SetDepth(-8888);
        g->world->AddChild(g->fogManager);

        g->app->onUpdate(
                [&](float deltaFrameMs) {
                    onGameUpdate(deltaFrameMs);
                });

        tickTimer = g->app->makeTimer([&]() {
            onGameTick();
        }, tickRateMs, tickRateMs);

        g->networkManager = New<NetworkManager>();

        g->minimap = New<Minimap>();
        g->world->AddChild(g->minimap);
    }

    int gcCount = 0;

    void Controller::onGameUpdate(float deltaFrameMs)
    {
        // this is the game loop
        g->world->UpdateChildren(deltaFrameMs);

        //when enough time has passed, do this
        if (gcCount > 1000) {
            Sova::GarbageCollector::getGC()->collect(this);
            gcCount = 0;
        }

        gcCount += 1;
    }

    void Controller::onGameTick()
    {
        //std::cout << "tick" << gcCount << std::endl;
    }
}
