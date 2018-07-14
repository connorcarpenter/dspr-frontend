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
        void onGameStart();
        void onGameUpdate();

        void onLoadProgress(Ref<String> resourceString);
        void onLoadFinish();

        Ref<App> app = NullRef<App>();
        Ref<Viewport> viewport = NullRef<Viewport>();
        Ref<Camera> camera = NullRef<Camera>();
        Ref<Container> world = NullRef<Container>();
    };

    Controller::Controller() = default;

    void Controller::onGameStart() {

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

        app->start();
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

        for (int i = 0; i<2500; i++) {
            Ref<MoveSprite> newSprite = NewRef<MoveSprite>(app);
            world->AddChild(newSprite);
        }
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

        //when enough time has passed, do this
        if (gameCount > 500) {
            Sova::GarbageCollector::getGC()->collect(this);
            gameCount = 0;
        }

        gameCount += 1;
    }
}
