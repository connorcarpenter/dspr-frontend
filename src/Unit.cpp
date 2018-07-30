//
// Created by connor on 7/26/18.
//

#include "Global.h"
#include <Sova/Internal/InternalApp.h>
#include <Sova/Math/Math.h>
#include "Unit.h"

namespace DsprFrontend
{

    Unit::Unit() : AnimatedSprite() {
        this->spriteDownName = New<Sova::String>("images/workerDown.png");
        this->spriteUpName = New<Sova::String>("images/workerUp.png");
        this->setTexture(spriteDownName);

        this->moveTo = New<Point>();
        this->tilePosition = New<Point>();
        this->nextTilePosition = New<Point>();
        this->frameWidth = 15;
        this->frameHeight = 20;
        this->padding = 1;
        this->anchor->set(7, 18);

        this->OnUpdate([&](){ step(); });
    }

    void Unit::step() {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        bool cursorIsHovering = Math::PointInBox(g->cursor->position->x, g->cursor->position->y,
                                              this->position->x - 6, this->position->y - 10,
                                              this->position->x + 4, this->position->y + 1);
        if (cursorIsHovering != this->hovering)
        {
            g->cursor->changeState(cursorIsHovering ? 0 : 1);
            this->hovering = cursorIsHovering;
        }

        if (this->hovering)
        {
            if (InternalApp::mouseButtonPressed(MouseButton::Left))
                this->selected = true;
        }

        if (this->selected)
        {
            if (!this->hovering)
            {
                if (InternalApp::mouseButtonPressed(MouseButton::Left))
                    this->selected = false;
            }

            if (InternalApp::mouseButtonPressed(MouseButton::Right))
            {
                this->moveTo = g->cursor->getTilePosition();
            }
        }

        if (!this->tilePosition->Equals(nextTilePosition))
        {
            this->imageSpeed = 0.1f;
            this->walkAmount += this->walkSpeed;
            if (this->walkAmount >= 100)
            {
                walkAmount = 0;
                this->tilePosition->set(this->nextTilePosition->x, this->nextTilePosition->y);
                this->SetDepth(this->tilePosition->y * -1);
            }
        }
        else
        {
            this->imageSpeed = 0;
            this->imageIndex = 0;
        }

        if (this->walkAmount == 0 && !this->tilePosition->Equals(this->moveTo))
        {
            int difx = Math::SignOrZero(this->moveTo->x - this->tilePosition->x);
            int dify = Math::SignOrZero(this->moveTo->y - this->tilePosition->y);
            if (difx == 0 || dify == 0)
            {
                difx *= 2; dify *= 2;
                walkSpeed = 6;
            }
            else
            {
                walkSpeed = 10;
            }
            if (difx != 0)
            {
                this->scale->x = Math::Sign(difx);
            }
            if (dify >= 0 && !this->textureName->Equals(this->spriteDownName))
            {
                this->setTexture(this->spriteDownName);
            }
            if (dify < 0 && !this->textureName->Equals(this->spriteUpName))
            {
                this->setTexture(this->spriteUpName);
            }
            this->nextTilePosition->set(this->tilePosition->x + difx, this->tilePosition->y + dify);
        }

        this->position->x = (int) (((Math::Lerp(this->tilePosition->x, this->nextTilePosition->x, walkAmount)/2) + 0.5f) * g->tileManager->tileWidth);
        this->position->y = (int) (((Math::Lerp(this->tilePosition->y, this->nextTilePosition->y, walkAmount)/2) + 0.5f) * g->tileManager->tileHeight);
    }

    void Unit::drawSelf(Ref<Camera> camera, int xoffset, int yoffset) {
        auto g = (Global*) InternalApp::getGlobal();

        if (selected) {
            g->unitSelectCircle->position->set(this->position->x, this->position->y);
            g->unitSelectCircle->drawSelf(camera, xoffset, yoffset);

            if (!this->tilePosition->Equals(this->moveTo)){
                g->moveMarker->Update();
                int x = (int) ((((float) this->moveTo->x / 2) + 0.5f) * g->tileManager->tileWidth);
                int y = (int) ((((float) this->moveTo->y / 2) + 0.5f) * g->tileManager->tileHeight);
                g->moveMarker->position->set(x, y);
                g->moveMarker->drawSelf(camera, xoffset, yoffset);
            }
        }

        if (hovering) {
            g->unitHoverCircle->Update();
            g->unitHoverCircle->position->set(this->position->x, this->position->y);
            g->unitHoverCircle->drawSelf(camera, xoffset, yoffset);
        }

        AnimatedSprite::drawSelf(camera, xoffset + (this->scale->x == -1 ? -2 : 0), yoffset);
    }
}