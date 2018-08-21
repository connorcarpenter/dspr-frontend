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
#include "Global.h"
#include "UnitManager.h"
#include "FogManager.h"
#include "UiManager.h"

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

            Ref<List<String>> resources = New<List<String>>();
            resources->Add(New<String>("images/cursor.png"));
            resources->Add(New<String>("images/unitHover.png"));
            resources->Add(New<String>("images/unitSelection.png"));
            resources->Add(New<String>("images/moveMarker.png"));
            resources->Add(New<String>("images/tiles.png"));
            resources->Add(New<String>("images/trees.png"));
            resources->Add(New<String>("images/block.png"));
            resources->Add(New<String>("images/fogTile.png"));
            resources->Add(New<String>("images/workerDown.png"));
            resources->Add(New<String>("images/workerUp.png"));
            resources->Add(New<String>("images/armybar.png"));
            resources->Add(New<String>("images/commandcard.png"));
            resources->Add(New<String>("images/minimap.png"));
            resources->Add(New<String>("images/unitPortrait.png"));
            resources->Add(New<String>("images/commandActionsHover.png"));
            resources->Add(New<String>("images/commandActionsUp.png"));

            g->app->load(resources)
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
        g->viewport = New<Viewport>(0, 0, g->app->width, g->app->height, g->camera);
        g->app->addViewport(g->viewport);

        g->tileManager = New<TileManager>();
        g->tileManager->SetDepth(9999);
        g->world->AddChild(g->tileManager);

        g->unitManager = New<UnitManager>();

        g->uiManager = New<UiManager>();
        g->uiManager->SetDepth(-9990);
        g->world->AddChild(g->uiManager);

        g->minimap = New<Minimap>();

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
    }

    int gcCount = 0;

    void Controller::onGameUpdate(float deltaFrameMs)
    {
        //move camera
        if (g->app->keyPressed(Key::Left)) g->camera->position->x -= 2;
        if (g->app->keyPressed(Key::Right)) g->camera->position->x += 2;
        if (g->app->keyPressed(Key::Up)) g->camera->position->y -= 2;
        if (g->app->keyPressed(Key::Down)) g->camera->position->y += 2;

        // this is the game loop
        g->unitManager->uiUpdate();
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
