//
// Created by connor on 7/26/18.
//


#include <Sova/Internal/InternalApp.h>
#include <Sova/Math/Math.h>
#include "Global.h"
#include "FogManager.h"
#include "SpriteCatalog.h"
#include <Sova/Audio/Sound.h>

namespace DsprFrontend
{

    Unit::Unit(int id, int x, int y, int tribeIndex) : AnimatedSprite()
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        this->id = id;
        this->moveTarget = New<Point>(x, y);
        this->tilePosition = New<Point>(x,y);
        this->nextTilePosition = New<Point>(x,y);
        this->lastPosition = New<Point>(x,y);
        this->tribeIndex = tribeIndex;

        this->useAnimatedSpriteInfo(g->spriteCatalog->workerWalkDown);

        this->tcSprite = New<AnimatedSprite>();
        this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerWalkDownTC);
        switch (tribeIndex)
        {
            case 0:
                this->tcSprite->tint = Color::Red;
                break;
            case 1:
                this->tcSprite->tint = Color::Blue;
                break;
        }

        this->OnUpdate([&](float deltaFrameMs){ step(deltaFrameMs); });

        g->fogManager->revealFog(this->tilePosition->x, this->tilePosition->y, this->sight, true);

        this->updatePosition();

        this->hitSound = New<Sound>(New<Sova::String>("sounds/hit.wav"));
    }

    void Unit::step(float deltaFrameMs)
    {
        switch(this->animationState)
        {
            case Walking:
                walkingStep(deltaFrameMs);
                break;
            case Attacking:
                attackingStep(deltaFrameMs);
                break;
        }
    }

    void Unit::walkingStep(float deltaFrameMs) {
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

            this->updatePosition();

            this->stillFrames = 0;
        }
        else
        {
            this->imageSpeed = 0;
            this->imageIndex = 0;
            this->stillFrames += 1;
            //idling

            if(this->stillFrames > 30) {
                this->stillFrames = 0;
                if (Math::Random(0, 20) < 1) {

                    //this->position->x += Math::Random(-1,1);
                    //this->position->y += Math::Random(-1,1);

                    if (Math::Random(0, 2) < 1) {
                        this->scale->x = this->scale->x * -1;
                    } else {
                        if (this->textureName->Equals(g->spriteCatalog->workerWalkUp->filename)) {
                            this->useAnimatedSpriteInfo(g->spriteCatalog->workerWalkDown);
                            this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerWalkDownTC);
                        } else {
                            this->useAnimatedSpriteInfo(g->spriteCatalog->workerWalkUp);
                            this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerWalkUpTC);
                        }
                    }
                }
            }
        }
    }

    void Unit::attackingStep(float deltaFrameMs) {
        this->imageSpeed = attackImageSpeed;
//        if (Math::Floor(this->imageIndex) == 3){
//            this->hitSound->PlayAndDisable();
//        } else this->hitSound->Enable();
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
        if (dify >= 0 && !this->textureName->Equals(g->spriteCatalog->workerWalkDown->filename))
        {
            this->useAnimatedSpriteInfo(g->spriteCatalog->workerWalkDown);
            this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerWalkDownTC);
        }
        if (dify < 0 && !this->textureName->Equals(g->spriteCatalog->workerWalkUp->filename))
        {
            this->useAnimatedSpriteInfo(g->spriteCatalog->workerWalkUp);
            this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerWalkUpTC);
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
        this->tcSprite->scale->x = this->scale->x;
        this->tcSprite->drawSelf(camera, xoffset + (this->scale->x == -1 ? -2 : 0), yoffset);
    }

    void Unit::setAnimationState(AnimationState newState, int heading) {
        auto g = (Global*) InternalApp::getGlobal();

        this->animationState = newState;

        switch (newState)
        {
            case Walking:
            {
                this->useAnimatedSpriteInfo(g->spriteCatalog->workerWalkDown);
                this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerWalkDownTC);
            }
            break;
            case Attacking:
            {
                this->imageIndex = 0;

                g->fogManager->revealFog(this->tilePosition->x, this->tilePosition->y, this->sight, false);
                g->fogManager->revealFog(this->nextTilePosition->x, this->nextTilePosition->y, this->sight, true);

                this->lastPosition->set(this->position);
                this->interpolation = interpolationMax-interpolationStep;
                this->tilePosition->set(this->nextTilePosition);
                this->SetDepth(this->tilePosition->y * -1);
                this->walkAmount = 0;

                switch(heading)
                {
                    case 0: {
                        this->scale->x = 1;
                        this->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackRight);
                        this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackRightTC);
                        }
                        break;
                    case 1: {
                        this->scale->x = 1;
                        this->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackUpRight);
                        this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackUpRightTC);
                    }
                        break;
                    case 2: {
                        this->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackUp);
                        this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackUpTC);
                    }
                        break;
                    case 3: {
                        this->scale->x = -1;
                        this->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackUpRight);
                        this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackUpRightTC);
                    }
                        break;
                    case 4: {
                        this->scale->x = -1;
                        this->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackRight);
                        this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackRightTC);
                    }
                        break;
                    case 5: {
                        this->scale->x = -1;
                        this->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackDownRight);
                        this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackDownRightTC);
                    }
                        break;
                    case 6: {
                        this->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackDown);
                        this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackDownTC);
                    }
                        break;
                    case 7: {
                        this->scale->x = 1;
                        this->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackDownRight);
                        this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerAttackDownRightTC);
                    }
                        break;

                }
            }
                break;
        }
    }

    void Unit::updatePosition() {
        auto g = (Global*) InternalApp::getGlobal();

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
}