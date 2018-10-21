//
// Created by connor on 7/26/18.
//


#include <Sova/Internal/InternalApp.h>
#include <Sova/Math/Math.h>
#include "Global.h"
#include "FogManager.h"
#include "SpriteCatalog.h"
#include <Sova/Audio/Sound.h>
#include "Unit.h"
#include "TileManager.h"
#include "DsprColors.h"
#include "UnitTemplate.h"
#include "UnitTemplateCatalog.h"

namespace DsprFrontend
{

    Unit::Unit(int id, int x, int y, int tribeIndex, Ref<UnitTemplate> unitTemplate) : AnimatedSprite()
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        this->id = id;
        this->moveTarget = New<Point>(x, y);
        this->tilePosition = New<Point>(x,y);
        this->nextTilePosition = New<Point>(x,y);
        this->lastPosition = New<Point>(x,y);
        this->tribeIndex = tribeIndex;
        this->unitTemplate = unitTemplate;

        this->useAnimatedSpriteInfo(this->unitTemplate->sprWalkDown);

        if (this->unitTemplate->index == 1)
        {
            int i = 12;
        }

        this->tcSprite = New<AnimatedSprite>();
        this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprWalkDownTC);
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

        if (this->tribeIndex == g->playersTribeIndex)
            g->fogManager->revealFog(this->tilePosition->x, this->tilePosition->y, this->unitTemplate->sight);

        this->updatePosition();

        this->SetDepth(this->tilePosition->y * -1);
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
                if (this->tribeIndex == g->playersTribeIndex) {
                    g->fogManager->conceilFog(this->tilePosition->x, this->tilePosition->y, this->unitTemplate->sight);
                    g->fogManager->revealFog(this->nextTilePosition->x, this->nextTilePosition->y, this->unitTemplate->sight);
                }
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
            if (!this->isATemple()) {
                this->stillFrames += 1;
                //idling

                if (this->stillFrames > 30) {
                    this->stillFrames = 0;
                    if (Math::Random(0, 20) < 1) {

                        //this->position->x += Math::Random(-1,1);
                        //this->position->y += Math::Random(-1,1);

                        if (Math::Random(0, 2) < 1) {
                            this->scale->x = this->scale->x * -1;
                        } else {
                            if (!this->isATemple()) {
                                if (this->textureName->Equals(this->unitTemplate->sprWalkUp->filename)) {
                                    this->facingDown = true;
                                    this->useAnimatedSpriteInfo(this->unitTemplate->sprWalkDown);
                                    this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprWalkDownTC);
                                } else {
                                    this->facingDown = false;
                                    this->useAnimatedSpriteInfo(this->unitTemplate->sprWalkUp);
                                    this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprWalkUpTC);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    bool Unit::isATemple(){
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        return this->unitTemplate == g->unitTemplateCatalog->temple;
    }

    void Unit::attackingStep(float deltaFrameMs) {
        if (this->attackWaitIndex <= 0) {
            this->attackFrameIndex += attackAnimationSpeed * deltaFrameMs / gameServerTickMs;
            if (this->attackFrameIndex >= this->attackFramesNumber) {
                this->attackFrameIndex = 0;
                this->attackWaitIndex = this->attackWaitFrames;
            }
            this->imageIndex = this->attackFrameIndex;
            if (this->unitTemplate->hitSound != nullptr) {
                if (Math::Floor(this->imageIndex) == 3) {
                    this->unitTemplate->hitSound->PlayAndDisable();
                } else this->unitTemplate->hitSound->Enable();
            }
        }
        else {
            this->attackWaitIndex -= this->attackWaitSpeed * deltaFrameMs / gameServerTickMs;
            this->imageIndex = 0;
        }
    }

    void Unit::newNextTilePosition(int x, int y)
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        if (this->tribeIndex == g->playersTribeIndex) {
            g->fogManager->conceilFog(this->tilePosition->x, this->tilePosition->y, this->unitTemplate->sight);
            g->fogManager->revealFog(this->nextTilePosition->x, this->nextTilePosition->y, this->unitTemplate->sight);
        }

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

        if (this->isATemple())return;

        if (difx != 0)
        {
            this->scale->x = Math::Sign(difx);
        }
        if (dify >= 0 && !this->textureName->Equals(this->unitTemplate->sprWalkDown->filename))
        {
            this->facingDown = true;
            this->useAnimatedSpriteInfo(this->unitTemplate->sprWalkDown);
            this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprWalkDownTC);
        }
        if (dify < 0 && !this->textureName->Equals(this->unitTemplate->sprWalkUp->filename))
        {
            this->facingDown = false;
            this->useAnimatedSpriteInfo(this->unitTemplate->sprWalkUp);
            this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprWalkUpTC);
        }
    }

    void Unit::setAnimationState(UnitAnimationState newState, int heading) {
        auto g = (Global*) InternalApp::getGlobal();

        this->animationState = newState;

        switch (newState)
        {
            case Walking:
            {
                this->useAnimatedSpriteInfo(this->unitTemplate->sprWalkDown);
                this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprWalkDownTC);
            }
            break;
            case Attacking:
            {
                this->imageIndex = 0;
                this->imageSpeed = 0;

                if (this->tribeIndex == g->playersTribeIndex) {
                    g->fogManager->conceilFog(this->tilePosition->x, this->tilePosition->y, this->unitTemplate->sight);
                    g->fogManager->revealFog(this->nextTilePosition->x, this->nextTilePosition->y, this->unitTemplate->sight);
                }

                this->lastPosition->set(this->position);
                this->interpolation = interpolationMax-interpolationStep;
                this->tilePosition->set(this->nextTilePosition);
                this->SetDepth(this->tilePosition->y * -1);
                this->walkAmount = 0;

                switch(heading)
                {
                    case 0: {
                        this->facingDown = true;
                        this->scale->x = 1;
                        this->useAnimatedSpriteInfo(this->unitTemplate->sprAttackRight);
                        this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprAttackRightTC);
                        }
                        break;
                    case 1: {
                        this->facingDown = false;
                        this->scale->x = 1;
                        this->useAnimatedSpriteInfo(this->unitTemplate->sprAttackUpRight);
                        this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprAttackUpRightTC);
                    }
                        break;
                    case 2: {
                        this->facingDown = false;
                        this->useAnimatedSpriteInfo(this->unitTemplate->sprAttackUp);
                        this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprAttackUpTC);
                    }
                        break;
                    case 3: {
                        this->facingDown = false;
                        this->scale->x = -1;
                        this->useAnimatedSpriteInfo(this->unitTemplate->sprAttackUpRight);
                        this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprAttackUpRightTC);
                    }
                        break;
                    case 4: {
                        this->facingDown = true;
                        this->scale->x = -1;
                        this->useAnimatedSpriteInfo(this->unitTemplate->sprAttackRight);
                        this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprAttackRightTC);
                    }
                        break;
                    case 5: {
                        this->facingDown = true;
                        this->scale->x = -1;
                        this->useAnimatedSpriteInfo(this->unitTemplate->sprAttackDownRight);
                        this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprAttackDownRightTC);
                    }
                        break;
                    case 6: {
                        this->facingDown = true;
                        this->useAnimatedSpriteInfo(this->unitTemplate->sprAttackDown);
                        this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprAttackDownTC);
                    }
                        break;
                    case 7: {
                        this->facingDown = true;
                        this->scale->x = 1;
                        this->useAnimatedSpriteInfo(this->unitTemplate->sprAttackDownRight);
                        this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprAttackDownRightTC);
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


    void Unit::drawSelf(Ref<Camera> camera, int xoffset, int yoffset)
    {
        auto g = (Global*) InternalApp::getGlobal();

        if (this->selected)
        {
            this->unitTemplate->sprSelectCircle->tint = (this->tribeIndex == g->playersTribeIndex) ? DsprColors::LightGreen : DsprColors::LightRed;
            this->unitTemplate->sprSelectCircle->position->set(this->position->x, this->position->y);
            this->unitTemplate->sprSelectCircle->drawSelf(camera, xoffset, yoffset);

            if (!this->tilePosition->Equals(this->moveTarget))
            {
                int x = (int) ((((float) this->moveTarget->x / 2) + 0.5f) * g->tileManager->tileWidth);
                int y = (int) ((((float) this->moveTarget->y / 2) + 0.5f) * g->tileManager->tileHeight);

                g->moveMarker->tint = (this->currentOrder == AttackTarget || this->currentOrder == AttackMove) ? DsprColors::LightRed : DsprColors::LightGreen;
                g->moveMarker->position->set(x, y);
                g->moveMarker->drawSelf(camera, xoffset, yoffset);
            }
        }

        if (this->hovering)
        {
            this->unitTemplate->sprHoverCircle->tint = (this->tribeIndex == g->playersTribeIndex) ? DsprColors::LightGreen : DsprColors::LightRed;
            this->unitTemplate->sprHoverCircle->position->set(this->position->x, this->position->y);
            this->unitTemplate->sprHoverCircle->drawSelf(camera, xoffset, yoffset);
        }

        int newOffset = xoffset;
        if (!this->isATemple()) newOffset += (this->scale->x == -1 ? -2 : 0);
        AnimatedSprite::drawSelf(camera, newOffset, yoffset);

        //TC

        this->tcSprite->imageIndex = imageIndex;
        this->tcSprite->position->set(this->position);
        this->tcSprite->scale->x = this->scale->x;
        this->tcSprite->drawSelf(camera, newOffset, yoffset);
    }

    void Unit::playSelectedSound() {
        if (this->unitTemplate->selectedSound != nullptr){
            this->unitTemplate->selectedSound->Play();
        }
    }

    void Unit::playDeathSound() {
        if (this->unitTemplate->dieSound != nullptr){
            this->unitTemplate->dieSound->Play();
        }
    }
}