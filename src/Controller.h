#pragma once

//
// Created by connor on 4/9/18.
//

#include "Sova/Sova.h"

#include "DsprShaderHandler.h"

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
        Ref<Sprite> nina = NullRef<Sprite>();
    };

    Controller::Controller() = default;

    void Controller::onGameStart() {

        app = NewRef<App>(640, 360, NewRef<String>("Demo"), NewRef<DsprShaderHandler>());

        Ref<List<String>> resources = NewRef<List<String>>();
        resources->Add(NewRef<String>("images/myNinaSmall.png"));
        resources->Add(NewRef<String>("images/owl.png"));

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
        Log::Info("Loading progress: Loaded resource ");
        Log::Info(resourceString->AsCStr());
        Log::Info("\n");
    }

    void Controller::onLoadFinish()
    {
        // after loading is done
        world = NewRef<Container>();
        camera = NewRef<Camera>(app->width, app->height, world);
        viewport = NewRef<Viewport>(0, 0, app->width, app->height, camera);
        app->addViewport(viewport);

        nina = NewRef<Sprite>(NewRef<String>("images/myNinaSmall.png"));

        world->addChild(nina);

        nina->position->set(82, 82);
        nina->onUpdate(
           [&]() {
              nina->position->x += 1;
           });

        app->onUpdate(
           [&]() {
              onGameUpdate();
           });
    }

    int gameCount = 0;

    void Controller::onGameUpdate()
    {
        // this is the game loop
        world->updateChildren();

        //when enough time has passed, do this
        if (gameCount > 1000) {
            Sova::GarbageCollector::getGC()->collect(this);
            gameCount = 0;
        }

        gameCount += 1;
    }
}
