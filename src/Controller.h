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

        Ref<App> app = NullRef<App>();
        Ref<Viewport> viewport = NullRef<Viewport>();
        Ref<Camera> camera = NullRef<Camera>();
        Ref<Container> world = NullRef<Container>();
        Ref<Websocket> server = NullRef<Websocket>();
    };

    Controller::Controller() = default;

    void Controller::onGameStart()
    {
        {
            app = NewRef<App>(1280, 720, NewRef<String>("Demo"), NewRef<DsprShaderHandler>());

            Ref<List<String>> resources = NewRef<List<String>>();
            resources->Add(NewRef<String>("images/owl.png"));
            resources->Add(NewRef<String>("images/cat.png"));
            resources->Add(NewRef<String>("images/mouse.png"));

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
        world = NewRef<Container>();
        camera = NewRef<Camera>(0, 0, app->width / 2, app->height / 2, world);
        viewport = NewRef<Viewport>(0, 0, app->width, app->height, camera);
        app->addViewport(viewport);

        app->onUpdate(
           [&]() {
              onGameUpdate();
           });

        for (int i = 0; i<1; i++) {
            Ref<MoveSprite> newSprite = NewRef<MoveSprite>(app);
            world->AddChild(newSprite);
        }

        server = app->openWebsocket(NewRef<String>("ws://localhost:3001"));

        server->onOpen([&](){
            auto i = 10;
            server->send(NewRef<String>("Hey from app!"));
        });

        server->onError([&](Ref<String> error){
            auto i = 11;
            server->close();
        });

        server->onMessage([&](Ref<String> message){
            auto i = 12;
        });
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

        //if (Math::Random() < 0.3f)
//        {
//            auto newSprite = NewRef<MoveSprite>(app);
//            world->AddChild(newSprite);
//        }

        //when enough time has passed, do this
        if (gameCount > 1000) {
            Sova::GarbageCollector::getGC()->collect(this);
            gameCount = 0;
        }

        gameCount += 1;
    }
}
