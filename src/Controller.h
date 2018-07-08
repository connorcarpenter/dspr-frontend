#pragma once

//
// Created by connor on 4/9/18.
//

#include <Modules/Gfx/Gfx.h>
#include "../../sova/src/Sova/Sova.h"
#include "../../oryol/code/Modules/Core/String/String.h"
#include "../../oryol/code/Modules/Core/Containers/Array.h"
#include "shaders.h"

using namespace Sova;

namespace DsprFrontend
{
    class Controller {
    public:
        Controller() = default;;
        void onGameStart();
        void onGameUpdate();

        void onLoadProgress();
        void onLoadFinish();

        Oryol::Id getNormalShader();
        Oryol::Id getCanvasShader();

        App app;
        Viewport viewport;
        Camera camera;
        Container world;
        Sprite nina;
    };

    void Controller::onGameStart() {

        app = App(640, 360, "Demo",
                  [&]() {
                      return getNormalShader();
                  },
                  [&]() {
                      return getCanvasShader();
                  });

        Array<String> resources;
        resources.Add("images/myNinaSmall.png");
        resources.Add("images/owl.png");

        app.load(resources)
           .onProgress(
              [&]() {
                 onLoadProgress();
              })
           .onFinish(
              [&]() {
                 onLoadFinish();
              });

        app.start();
    }

    void Controller::onLoadProgress() {
        // load progress here
    }

    void Controller::onLoadFinish() {
        // after loading is done
        camera = Camera(app.width, app.height, world);
        viewport = Viewport(0, 0, app.width, app.height, camera);
        app.addViewport(viewport);

        nina = Sprite(app, "images/myNinaSmall.png");

        world.addChild(nina);

        nina.position.set(82, 82);
        nina.onUpdate(
           [&]() {
              nina.position.x += 1;
           });

        app.onUpdate(
           [&]() {
              onGameUpdate();
           });
    }

    void Controller::onGameUpdate() {
        // this is the game loop
        world.updateChildren();
    }

    Oryol::Id Controller::getNormalShader() {
        return Gfx::CreateResource(NormalShader::Setup());
    }

    Oryol::Id Controller::getCanvasShader() {
        return Gfx::CreateResource(CanvasShader::Setup());
    }
}
