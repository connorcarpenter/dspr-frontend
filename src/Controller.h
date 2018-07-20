#pragma once

//
// Created by connor on 4/9/18.
//

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
        void onGameStart();
        void onGameUpdate();

        void onLoadProgress(Ref<String> resourceString);
        void onLoadFinish();

        Ref<App> app = Null<App>();
        Ref<Viewport> viewport = Null<Viewport>();
        Ref<Camera> camera = Null<Camera>();
        Ref<Container> world = Null<Container>();
        Ref<Websocket> server = Null<Websocket>();

        void onServerUpdate(Ref<String> message);
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
                    ->onProgress(
                            [&](Ref<String> resource) {
                                onLoadProgress(resource);
                            })
                    ->onFinish(
                            [&]() {
                                onLoadFinish();
                            });
        }

        app->start(); // code continues processing within this function, which is why it should be outside the above scope
    }

    void Controller::onLoadProgress(Ref<String> resourceString)
    {
        //Log::Info("Loading progress: Loaded resource ");
        //Log::Info(resourceString->AsCStr());
        //Log::Info("\n");
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

        Ref<HttpRequest> req = New<HttpRequest>(New<String>("GET"), New<String>("http://localhost:3171/orchestrator/bff"));
        req->setRequestHeader(New<String>("Content-Type"), New<String>("application/json"));
        req->onResponse(
                [&](Ref<HttpResponse> response)
                {
                    if (response->status == 200 && response->responseText->Length() > 0)
                    {
                        server = app->openWebsocket(response->responseText);
                        server->onMessage(
                                [&](Ref<String> message){
                                    this->onServerUpdate(message);
                                });
                    }
                });
        req->send();
    }

    int gameCount = 0;

    void Controller::onGameUpdate()
    {
        //move camera
        if (app->keyPressed(Key::Left)) camera->position->x -= 2;
        if (app->keyPressed(Key::Right)) camera->position->x += 2;
        if (app->keyPressed(Key::Up)) camera->position->y -= 2;
        if (app->keyPressed(Key::Down)) camera->position->y += 2;

        // this is the game loop
        world->UpdateChildren();

//        if (Math::Random() < 0.01f)
//        {
//            server->send(New<String>("hey server!"));
//        }

        //when enough time has passed, do this
        if (gameCount > 1000) {
            Sova::GarbageCollector::getGC()->collect(this);
            gameCount = 0;
        }

        gameCount += 1;
    }

    void Controller::onServerUpdate(Ref<String> message) {
        auto i = 10;
    }
}
