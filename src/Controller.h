#pragma once

//
// Created by connor on 4/9/18.
//

#include "Sova/SovaMain.h"

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
        Ref<Sprite> nina2 = NullRef<Sprite>();
        Ref<Point> ninaSpeed = NullRef<Point>();
        Ref<Point> ninaSpeed2 = NullRef<Point>();
    };

    Controller::Controller() = default;

    void Controller::onGameStart() {

        app = NewRef<App>(1280, 720, NewRef<String>("Demo"), NewRef<DsprShaderHandler>());

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

        nina = NewRef<Sprite>(NewRef<String>("images/myNinaSmall.png"));
        nina2 = NewRef<Sprite>(NewRef<String>("images/owl.png"));

        world->addChild(nina);
        world->addChild(nina2);

        nina->position->set(132, 82);
        ninaSpeed = NewRef<Point>(4, 4);

        nina->onUpdate(
           [&]() {
               nina->position->x += ninaSpeed->x;
               nina->position->y += ninaSpeed->y;

               if (nina->position->x > camera->width - nina->getWidth()) ninaSpeed->x = -4;
               if (nina->position->y > camera->height - nina->getHeight()) ninaSpeed->y = -4;
               if (nina->position->x < 0) ninaSpeed->x = 4;
               if (nina->position->y < 0) ninaSpeed->y = 4;
           });

        nina2->position->set(42, 253);
        ninaSpeed2 = NewRef<Point>(-4, -4);

        nina2->onUpdate(
                [&]() {
                    nina2->position->x += ninaSpeed2->x;
                    nina2->position->y += ninaSpeed2->y;

                    if (nina2->position->x > camera->width - nina2->getWidth()) ninaSpeed2->x = -4;
                    if (nina2->position->y > camera->height - nina2->getHeight()) ninaSpeed2->y = -4;
                    if (nina2->position->x < 0) ninaSpeed2->x = 4;
                    if (nina2->position->y < 0) ninaSpeed2->y = 4;
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
