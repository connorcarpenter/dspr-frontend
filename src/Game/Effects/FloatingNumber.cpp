//
// Created by connor on 10/29/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include "FloatingNumber.h"
#include "GraphicsManager.h"

namespace DsprFrontend {

    FloatingNumber::FloatingNumber(Ref<Point> position, Color color1, int number) : color(color1) {
        this->floatPosition = New<FloatPoint>(position->x, position->y);
        this->position = position;
        this->number = number;
        this->numberStr = String::getStringFromNumber(this->number);

        this->OnUpdate([&](float deltaFrameMs){ step(deltaFrameMs); });
    }

    void FloatingNumber::step(float deltaMs) {
        this->floatPosition->y += this->vspeed;
        this->position->set(this->floatPosition->x, this->floatPosition->y);
        this->ttl--;
        if (this->ttl < 0)this->Destroy();
    }

    void FloatingNumber::drawSelf(Ref<Camera> camera, int xoffset, int yoffset) {
        auto g = (Global*) InternalApp::getGlobal();
        g->graphicsManager->drawText(camera, this->position->x + xoffset, this->position->y + yoffset, this->numberStr,
                                     this->color, false);
    }
}