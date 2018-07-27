//
// Created by connor on 7/26/18.
//

#include "Global.h"
#include <Sova/Internal/OryolApp.h>
#include <Sova/Math/Math.h>
#include "Unit.h"

namespace DsprFrontend
{

    Unit::Unit() : AnimatedSprite() {
        auto str = New<Sova::String>("images/worker.png");
        this->setTexture(str);
        this->frameWidth = 15;
        this->frameHeight = 20;
        this->padding = 1;
        this->anchor->x = 7;
        this->anchor->y = 18;
        this->OnUpdate([&](){ step(); });
    }

    void Unit::step() {
        auto g = (Global*) OryolApp::getSovaApp()->getGlobal();
        if (Math::PointInBox(g->cursor->position->x, g->cursor->position->y,
                             this->position->x - 6, this->position->y - 10,
                             this->position->x + 4, this->position->y + 1))
        {
            g->cursor->changeState(0);
            this->hovering = true;

            if (OryolApp::getOryolApp()->mouseButtonPressed(MouseButton::Left))
                this->selected = true;
        }
        else
        {
            g->cursor->changeState(1);
            this->hovering = false;

            if (OryolApp::getOryolApp()->mouseButtonPressed(MouseButton::Left))
                this->selected = false;
        }
    }

    void Unit::drawSelf(Ref<Camera> camera, int xoffset, int yoffset) {
        auto g = (Global*) OryolApp::getSovaApp()->getGlobal();

        if (selected) {
            g->unitSelectCircle->position->set(this->position->x, this->position->y);
            g->unitSelectCircle->drawSelf(camera, xoffset, yoffset);
        }

        if (hovering) {
            g->unitHoverCircle->Update();
            g->unitHoverCircle->position->set(this->position->x, this->position->y);
            g->unitHoverCircle->drawSelf(camera, xoffset, yoffset);
        }

        AnimatedSprite::drawSelf(camera, xoffset, yoffset);
    }
}