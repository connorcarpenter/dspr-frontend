#pragma once

//
// Created by connor on 4/9/18.
//

#include "Sova/Sova.h"

#include "DsprShaderHandler.h"

using namespace Sova;

namespace DsprFrontend
{
    class Controller : public virtual Refable
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

        Ref<List<Ref<String>>> resources;
        resources->Add(NewRef<String>("images/myNinaSmall.png"));
        resources->Add(NewRef<String>("images/owl.png"));

        app->load(resources)
           ->onProgress(
              [&](String resource) {
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
        // load progress here
    }

    void Controller::onLoadFinish()
    {
        // after loading is done
        camera = NewRef<Camera>(app->width, app->height, world);
        viewport = NewRef<Viewport>(0, 0, app->width, app->height, camera);
        app->addViewport(viewport);

        nina = NewRef<Sprite>(app, "images/myNinaSmall.png");

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

    void Controller::onGameUpdate()
    {
        // this is the game loop
        world->updateChildren();
    }
}
