//
// Created by connor on 10/29/18.
//

#include <Sova/Math/Math.h>
#include <Sova/Internal/InternalApp.h>
#include "Manaball.h"
#include "Global.h"
#include "SpriteCatalog.h"

namespace DsprFrontend {

    Manaball::Manaball(Ref<Point> position, int size) : AnimatedSprite() {
        this->floatPosition = New<FloatPoint>(position->x, position->y);
        this->position = New<Point>(position->x, position->y);
        this->size = size;
        this->OnUpdate([&](float deltaFrameMs){ step(deltaFrameMs); });

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        this->useAnimatedSpriteInfo(g->spriteCatalog->manaball);
    }

    void Manaball::step(float deltaMs) {
        this->visible = (this->size > 0);
        if (!this->visible)return;
        this->imgIndex += this->imgSpeed;
        if (this->imgIndex >= 3.9f) this->imgIndex = 0;
        this->imageIndex = this->imgIndex + ((this->size-1) * 4);

        if (this->moveToPosition != nullptr && (this->floatPosition->x != this->moveToPosition->x || this->floatPosition->y != this->moveToPosition->y))
        {
            auto changeX = this->moveToPosition->x - this->floatPosition->x;
            auto changeY = this->moveToPosition->y - this->floatPosition->y;
            if (Math::Distance(0,0,changeX,changeY)>this->speed)
            {
                auto dir = Math::Direction(0,0,changeX,changeY);
                changeX = Math::Cos(dir) * this->speed;
                changeY = Math::Sin(dir) * this->speed;
            }
            else
            {
                this->position = this->moveToPosition;
                if (destroyAfterArrival)
                    this->Destroy();
            }

            this->floatPosition->x += changeX;
            this->floatPosition->y += changeY;

            this->position->x = this->floatPosition->x;
            this->position->y = this->floatPosition->y;
        }
    }
}