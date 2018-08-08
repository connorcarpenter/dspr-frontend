//
// Created by connor on 7/26/18.
//


#include <Sova/Internal/InternalApp.h>
#include <Sova/Math/Math.h>
#include "Global.h"

namespace DsprFrontend
{

    Unit::Unit(int id, int x, int y) : AnimatedSprite()
    {
        this->id = id;
        this->moveTarget = New<Point>(x, y);
        this->tilePosition = New<Point>(x,y);
        this->nextTilePosition = New<Point>(x,y);
        this->lastPosition = New<Point>(x,y);

        this->spriteDownName = New<Sova::String>("images/workerDown.png");
        this->spriteUpName = New<Sova::String>("images/workerUp.png");
        this->setTexture(spriteDownName);
        this->frameWidth = 15;
        this->frameHeight = 20;
        this->padding = 1;
        this->anchor->set(7, 18);

        this->OnUpdate([&](float deltaFrameMs){ step(deltaFrameMs); });
    }

    void Unit::step(float deltaFrameMs)
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        if (g->cursor->leftButtonDragging)
        {
            bool boxIsHovering = Math::BoxesOverlap(g->cursor->leftButtonDragPoint->x,
                                                    g->cursor->leftButtonDragPoint->y,
                                                    g->cursor->position->x, g->cursor->position->y,
                                                    this->position->x - 6, this->position->y - 10,
                                                    this->position->x + 4, this->position->y + 1);
            //boxIsHovering = true;
            if (boxIsHovering != this->hovering)
            {
                g->cursor->changeState(boxIsHovering ? 0 : 1);
                this->hovering = boxIsHovering;
                this->checkReleaseSelectionBox = this->hovering;
            }
        }
        else
        {
            if (this->checkReleaseSelectionBox)
            {
                this->checkReleaseSelectionBox = false;
                this->selected = true;
                g->unitManager->addToSelectionList(id);
            }

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
                {
                    this->selected = true;
                    g->unitManager->clearSelectionList();
                    g->unitManager->addToSelectionList(id);
                }
            }

            if (this->selected)
            {
                if (!this->hovering)
                {
                    if (InternalApp::mouseButtonPressed(MouseButton::Left))
                    {
                        g->unitManager->removeFromSelectionList(id);
                        this->selected = false;
                    }
                }
            }
        }

        if (!this->tilePosition->Equals(this->moveTarget))
        {
            this->imageSpeed = walkImageSpeed;
            this->walkAmount += this->walkSpeed * (deltaFrameMs / gameServerTickMs);
            if (this->walkAmount >= maxWalkAmount)
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

        float extrapolatedPositionX = ((Math::Lerp(this->tilePosition->x, this->nextTilePosition->x, walkAmount/maxWalkAmount)/2) + 0.5f) * g->tileManager->tileWidth;
        float extrapolatedPositionY = ((Math::Lerp(this->tilePosition->y, this->nextTilePosition->y, walkAmount/maxWalkAmount)/2) + 0.5f) * g->tileManager->tileHeight;
        if (interpolation > 0)
        {
            float interpolatedPositionX = Math::Lerp(this->lastPosition->x, extrapolatedPositionX, 1-(interpolation/interpolationMax));
            float interpolatedPositionY = Math::Lerp(this->lastPosition->y, extrapolatedPositionY, 1-(interpolation/interpolationMax));

            this->position->x = interpolatedPositionX;
            this->position->y = interpolatedPositionY;

            interpolation-=interpolationStep;
        }
        else
        {
            this->position->x = extrapolatedPositionX;
            this->position->y = extrapolatedPositionY;
        }
    }

    void Unit::newNextTilePosition(int x, int y)
    {
        this->lastPosition->set(this->position);
        this->interpolation = interpolationMax-interpolationStep;

        this->tilePosition->set(this->nextTilePosition);
        this->SetDepth(this->tilePosition->y * -1);

        this->walkAmount = 0;
        this->nextTilePosition->set(x, y);

        int difx = Math::SignOrZero(this->nextTilePosition->x - this->tilePosition->x);
        int dify = Math::SignOrZero(this->nextTilePosition->y - this->tilePosition->y);
        if (difx == 0 || dify == 0)
        {
            difx *= 2; dify *= 2;
            walkSpeed = walkSpeedDiagonal;
        }
        else
        {
            walkSpeed = walkSpeedStraight;
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
    }

    void Unit::drawSelf(Ref<Camera> camera, int xoffset, int yoffset)
    {
        auto g = (Global*) InternalApp::getGlobal();

        if (selected) {
            g->unitSelectCircle->position->set(this->position->x, this->position->y);
            g->unitSelectCircle->drawSelf(camera, xoffset, yoffset);

            if (!this->tilePosition->Equals(this->moveTarget)){
                g->moveMarker->Update(0);
                int x = (int) ((((float) this->moveTarget->x / 2) + 0.5f) * g->tileManager->tileWidth);
                int y = (int) ((((float) this->moveTarget->y / 2) + 0.5f) * g->tileManager->tileHeight);
                g->moveMarker->position->set(x, y);
                g->moveMarker->drawSelf(camera, xoffset, yoffset);
            }
        }

        if (hovering)
        {
            g->unitHoverCircle->Update(0);
            g->unitHoverCircle->position->set(this->position->x, this->position->y);
            g->unitHoverCircle->drawSelf(camera, xoffset, yoffset);
        }

        AnimatedSprite::drawSelf(camera, xoffset + (this->scale->x == -1 ? -2 : 0), yoffset);
    }
}