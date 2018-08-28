//
// Created by connor on 7/26/18.
//


#include <Sova/Internal/InternalApp.h>
#include <Sova/Math/Math.h>
#include "Global.h"
#include "FogManager.h"

namespace DsprFrontend
{

    Unit::Unit(int id, int x, int y, int tribeIndex) : AnimatedSprite()
    {
        this->id = id;
        this->moveTarget = New<Point>(x, y);
        this->tilePosition = New<Point>(x,y);
        this->nextTilePosition = New<Point>(x,y);
        this->lastPosition = New<Point>(x,y);

        this->spriteDownName = New<Sova::String>("images/worker/workerDown.png");
        this->spriteUpName = New<Sova::String>("images/worker/workerUp.png");
        this->spriteDownTCName = New<Sova::String>("images/worker/TC/workerDown_TC.png");
        this->spriteUpTCName = New<Sova::String>("images/worker/TC/workerUp_TC.png");
        this->setTexture(spriteDownName);
        this->frameWidth = 15;
        this->frameHeight = 20;
        this->padding = 1;
        this->anchor->set(7, 18);
        this->tribeIndex = tribeIndex;
        this->tcSprite = New<AnimatedSprite>();
        switch (tribeIndex)
        {
            case 0:
                this->tcSprite->tint = Color::Red;
                break;
            case 1:
                this->tcSprite->tint = Color::Blue;
                break;
        }
        this->tcSprite->padding = 1;
        this->tcSprite->frameWidth = 14;
        this->tcSprite->frameHeight = 11;
        this->tcSprite->setTexture(spriteDownTCName);
        this->tcSprite->anchor->set(7, 13);

        this->OnUpdate([&](float deltaFrameMs){ step(deltaFrameMs); });

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        g->fogManager->revealFog(this->tilePosition->x, this->tilePosition->y, this->sight, true);
    }

    void Unit::step(float deltaFrameMs)
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        if (!this->tilePosition->Equals(this->nextTilePosition))
        {
            this->imageSpeed = walkImageSpeed;
            this->walkAmount += this->walkSpeed * (deltaFrameMs / gameServerTickMs);
            if (this->walkAmount >= maxWalkAmount)
            {
                walkAmount = 0;
                g->fogManager->revealFog(this->tilePosition->x, this->tilePosition->y, this->sight, false);
                g->fogManager->revealFog(this->nextTilePosition->x, this->nextTilePosition->y, this->sight, true);
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
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        g->fogManager->revealFog(this->tilePosition->x, this->tilePosition->y, this->sight, false);
        g->fogManager->revealFog(this->nextTilePosition->x, this->nextTilePosition->y, this->sight, true);

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
            this->tcSprite->setTexture(this->spriteDownTCName);
        }
        if (dify < 0 && !this->textureName->Equals(this->spriteUpName))
        {
            this->setTexture(this->spriteUpName);
            this->tcSprite->setTexture(this->spriteUpTCName);
        }
    }

    void Unit::drawSelf(Ref<Camera> camera, int xoffset, int yoffset)
    {
        auto g = (Global*) InternalApp::getGlobal();

        if (this->selected)
        {
            g->unitSelectCircle->position->set(this->position->x, this->position->y);
            g->unitSelectCircle->drawSelf(camera, xoffset, yoffset);

            if (!this->tilePosition->Equals(this->moveTarget))
            {
                int x = (int) ((((float) this->moveTarget->x / 2) + 0.5f) * g->tileManager->tileWidth);
                int y = (int) ((((float) this->moveTarget->y / 2) + 0.5f) * g->tileManager->tileHeight);
                g->moveMarker->position->set(x, y);
                g->moveMarker->drawSelf(camera, xoffset, yoffset);
            }
        }

        if (this->hovering)
        {
            g->unitHoverCircle->position->set(this->position->x, this->position->y);
            g->unitHoverCircle->drawSelf(camera, xoffset, yoffset);
        }

        AnimatedSprite::drawSelf(camera, xoffset + (this->scale->x == -1 ? -2 : 0), yoffset);

        //TC

        this->tcSprite->imageIndex = imageIndex;
        this->tcSprite->position->set(this->position);
        this->tcSprite->drawSelf(camera, xoffset, yoffset);
    }
}