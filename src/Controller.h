#pragma once

//
// Created by connor on 4/9/18.
//

#include <iostream>
#include "Sova/SovaMain.h"

#include "DsprShaderHandler.h"
#include "MoveSprite.h"

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

        Ref<App> app = Null<App>();
        Ref<Viewport> viewport = Null<Viewport>();
        Ref<Camera> camera = Null<Camera>();
        Ref<Container> world = Null<Container>();
        Ref<Websocket> bffServer = Null<Websocket>();
        Ref<Websocket> gameServer = Null<Websocket>();
    };

    Controller::Controller() = default;

    void Controller::onGameStart()
    {
        {
            app = New<App>(1280, 720, New<String>("Demo"), New<DsprShaderHandler>());

            Ref<List<String>> resources = New<List<String>>();
            resources->Add(New<String>("images/owl.png"));
            resources->Add(New<String>("images/cat.png"));
            resources->Add(New<String>("images/mouse.png"));

            app->load(resources)
                    ->onFinish(
                            [&]() {
                                onLoadFinish();
                            });
        }

        app->start(); // code continues processing within this function, which is why it should be outside the above scope
    }

    void Controller::onLoadFinish()
    {
        // after loading is done
        world = New<Container>();
        camera = New<Camera>(0, 0, app->width / 2, app->height / 2, world);
        viewport = New<Viewport>(0, 0, app->width, app->height, camera);
        app->addViewport(viewport);

        app->onUpdate(
           [&]() {
              onGameUpdate();
           });

        for (int i = 0; i<1; i++) {
            Ref<MoveSprite> newSprite = New<MoveSprite>(app);
            world->AddChild(newSprite);
        }

        Ref<HttpRequest> bffReq = app->makeHttpRequest(New<String>("GET"), New<String>("http://www.deuspora.com:3170/orchestrator/bff"));
        bffReq->onResponse(
                [&](Ref<HttpResponse> response)
                {
                    if (response->status == 200 && response->responseText->Length() > 0)
                    {
                        bffServer = app->openWebsocket(response->responseText);
                        bffServer->onOpen(
                                [&](){
                                    this->onBffServerStart();
                                });
                        bffServer->onMessage(
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
        if (app->keyPressed(Key::Left)) camera->position->x -= 2;
        if (app->keyPressed(Key::Right)) camera->position->x += 2;
        if (app->keyPressed(Key::Up)) camera->position->y -= 2;
        if (app->keyPressed(Key::Down)) camera->position->y += 2;

        // this is the game loop
        world->UpdateChildren();

        //when enough time has passed, do this
        if (gcCount > 1000) {
            Sova::GarbageCollector::getGC()->collect(this);
            gcCount = 0;
        }

        gcCount += 1;
    }

    void Controller::onBffServerStart()
    {
        bffServer->send(New<String>("gameservers/1.0/join"));
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

            gameServer = app->openWebsocket(fullAddress);
            gameServer->onOpen(
                    [&](){
                        this->onGameServerStart();
                    });
            gameServer->onMessage(
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
        std::cout << "From Gameserver: " << message->AsCStr() << "" << std::endl;
    }
}
