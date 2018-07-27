#pragma once

//
// Created by connor on 4/9/18.
//

#include <iostream>

#include <Sova/References/Refable.h>
#include <Sova/References/Ref.h>
#include "Sova/SovaMain.h"

#include "DsprShaderHandler.h"
#include "Global.h"
#include "Unit.h"

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
        void onGameUpdate();

        void onBffServerStart();
        void onBffServerUpdate(Ref<String> message);

        void onGameServerStart();
        void onGameServerUpdate(Ref<String> message);

        Ref<Global> g = Null<Global>();
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
            resources->Add(New<String>("images/worker.png"));

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
        g->camera = New<Camera>(0, 0, g->app->width / pixelRatio, g->app->height / pixelRatio, g->world);
        g->viewport = New<Viewport>(0, 0, g->app->width, g->app->height, g->camera);
        g->app->addViewport(g->viewport);

        g->tileManager = New<TileManager>();
        g->world->AddChild(g->tileManager);

        g->worker = New<Unit>();
        g->worker->position->set(40, 40);
        g->world->AddChild(g->worker);

        ///UI stuff
        g->unitHoverCircle = New<AnimatedSprite>(New<String>("images/unitHover.png"), 16, 12, 1);
        g->unitHoverCircle->imageSpeed = 0.1f;
        g->unitHoverCircle->anchor->set(8, 6);

        g->unitSelectCircle = New<Sprite>(New<String>("images/unitSelection.png"));
        g->unitSelectCircle->anchor->set(8, 5);

        g->moveMarker = New<AnimatedSprite>(New<String>("images/moveMarker.png"), 11, 9, 1);
        g->moveMarker->imageSpeed = 0.05f;

        g->cursor = New<Cursor>();
        g->world->AddChild(g->cursor);
        /////////////////////////

        g->app->onUpdate(
                [&]() {
                    onGameUpdate();
                });

        Ref<HttpRequest> bffReq = g->app->makeHttpRequest(New<String>("GET"), New<String>("http://www.deuspora.com:3170/orchestrator/bff"));
        bffReq->onResponse(
                [&](Ref<HttpResponse> response)
                {
                    if (response->status == 200 && response->responseText->Length() > 0)
                    {
                        g->bffServer = g->app->openWebsocket(response->responseText);
                        g->bffServer->onOpen(
                                [&](){
                                    this->onBffServerStart();
                                });
                        g->bffServer->onMessage(
                                [&](Ref<String> message){
                                    this->onBffServerUpdate(message);
                                });
                    }
                });
        bffReq->send();
    }

    int gcCount = 0;

    void Controller::onGameUpdate()
    {
        //move camera
        if (g->app->keyPressed(Key::Left)) g->camera->position->x -= 2;
        if (g->app->keyPressed(Key::Right)) g->camera->position->x += 2;
        if (g->app->keyPressed(Key::Up)) g->camera->position->y -= 2;
        if (g->app->keyPressed(Key::Down)) g->camera->position->y += 2;

        // this is the game loop
        g->world->UpdateChildren();

        //when enough time has passed, do this
        if (gcCount > 1000) {
            Sova::GarbageCollector::getGC()->collect(this);
            gcCount = 0;
        }

        gcCount += 1;
    }

    void Controller::onBffServerStart()
    {
        g->bffServer->send(New<String>("gameservers/1.0/join"));
    }

    void Controller::onBffServerUpdate(Ref<String> message)
    {
        message = message->TrimEnd("\r\n");

        Ref<List<String>> splitString = message->Split('|');

        if (splitString->Size() != 2) return;

        Ref<String> command = splitString->At(0);
        if (command->Equals("gameserver/connect"))
        {
            auto addressBody = splitString->At(1);

            Ref<StringBuilder> sb = New<StringBuilder>();
            sb->Append(New<String>("ws://"));
            sb->Append(addressBody);
            Ref<String> fullAddress = sb->ToString();

            g->gameServer = g->app->openWebsocket(fullAddress);
            g->gameServer->onOpen(
                    [&](){
                        this->onGameServerStart();
                    });
            g->gameServer->onMessage(
                    [&](Ref<String> message){
                        this->onGameServerUpdate(message);
                    });
        }
    }

    void Controller::onGameServerStart()
    {
        std::cout << "Gameserver Start" << std::endl;
    }

    void Controller::onGameServerUpdate(Ref<String> message)
    {
        message = message->TrimEnd("\r\n")->TrimStart("\n");

        Ref<List<String>> splitString = message->Split('|');

        if (splitString->Size() != 2) return;

        Ref<String> command = splitString->At(0);

        if (command->Equals("auth/1.0/gametoken"))
        {
            g->gameServer->send(New<String>("auth/1.0/gametoken|game1"));
            return;
        }
        else if (command->Equals("grid/1.0/give")) {
            Ref<List<String>> gridString = splitString->At(1)->Split(',');
            g->tileManager->receiveGrid(gridString->At(0), gridString->At(1));
            return;
        }
        else if (command->Equals("tile/1.0/give")) {
            Ref<List<String>> tileString = splitString->At(1)->Split(',');
            g->tileManager->receiveTile(tileString->At(0), tileString->At(1), tileString->At(2));
            return;
        }
    }
}
