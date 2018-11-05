//
// Created by connor on 7/26/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Sova/Math/Math.h>
#include "Global.h"
#include "FogManager.h"
#include "SpriteCatalog.h"
#include <Sova/Audio/Sound.h>
#include <Effects/Manaball.h>
#include <Effects/FloatingNumber.h>
#include <Unit/SpecificUnit/Manafount.h>
#include "Unit.h"
#include "TileManager.h"
#include "DsprColors.h"
#include "UnitTemplate.h"
#include "UnitTemplateCatalog.h"
#include "UnitManager.h"
#include "EconomyManager.h"

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

        this->health = this->unitTemplate->maxHealth;

        this->useAnimatedSpriteInfo(this->unitTemplate->sprBase);

        this->tcSprite = New<AnimatedSprite>();
        this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprTC);
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

        if (this->unitTemplate->hasConstructionQueue)
        {
            this->constructionQueue = New<ConstructionQueue>();
        }

        if (this->unitTemplate->createSpecificUnitFunction != nullptr)
        {
            this->specificUnit = this->unitTemplate->createSpecificUnitFunction(this);
        }
    }

    Unit::~Unit() {}

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
            case Gathering:
                gatheringStep(deltaFrameMs);
                break;
        }

        //construction queue
        if (this->constructionQueue != nullptr)
        {
            this->constructionQueue->update(deltaFrameMs / gameServerTickMs);
        }

        //call into specific unit
        if (this->specificUnit != nullptr)
        {
            this->specificUnit->stepFunction();
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
                //update fog
                if (this->tribeIndex == g->playersTribeIndex) {
                    g->fogManager->conceilFog(this->tilePosition->x, this->tilePosition->y, this->unitTemplate->sight);
                    g->fogManager->revealFog(this->nextTilePosition->x, this->nextTilePosition->y, this->unitTemplate->sight);
                }

                this->updateTilePosition(this->nextTilePosition);
            }

            this->updatePosition();

            this->stillFrames = 0;
        }
        else
        {
            this->imageSpeed = 0;
            this->imageIndex = 0;
            if (this->unitTemplate->hasIdleTurnBehavior) {
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
                                if (this->textureName->Equals(this->unitTemplate->sprWalkUp->spriteInfo->filename)) {
                                    this->facingDown = true;
                                    this->useAnimatedSequenceInfo(this->unitTemplate->sprWalkDown);
                                } else {
                                    this->facingDown = false;
                                    this->useAnimatedSequenceInfo(this->unitTemplate->sprWalkUp);
                                }
                        }
                    }
                }
            }
        }
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

    void Unit::gatheringStep(float deltaFrameMs) {
        this->gatherFrameIndex += (deltaFrameMs / gameServerTickMs);
        if (this->myManaball == nullptr)
        {
            auto manapartPoint = New<Point>(this->position->x + (this->scale->x * 4) + (this->scale->x==-1 ? this->unitTemplate->spriteFaceLeftXoffset : 0),
                                            this->position->y - 6);
            this->myManaball = New<Manaball>(manapartPoint, 0);
            this->myManaball->depth = this->depth + (this->facingDown ? 0 : 1);
            this->myManaball->scale->x = this->scale->x;

            auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
            g->world->AddChild(this->myManaball);
        }
        else
        {
            if ((int) this->gatherFrameIndex % 10 == 0)
            {
                auto g = (Global *) InternalApp::getSovaApp()->getGlobal();
                auto targetUnit = g->unitManager->getUnitWithId(this->targetUnitId);
                if (targetUnit != nullptr && Math::Random(0,5)<1)
                {
                    auto manapartPoint = New<Point>(targetUnit->position->x - targetUnit->unitTemplate->sprCenterAdjust->x,
                                                    targetUnit->position->y - targetUnit->unitTemplate->sprCenterAdjust->y - 6);
                    manapartPoint->x += Math::Random(-14, 14);
                    manapartPoint->y += Math::Random(-7, 7);
                    auto manaparticle = New<Manaball>(manapartPoint, 1);
                    manaparticle->destroyAfterArrival = true;
                    manaparticle->moveToPosition = this->myManaball->position;
                    manaparticle->moveToManaball = this->myManaball;
                    manaparticle->speed = 1;
                    manaparticle->SetDepth(this->myManaball->depth);
                    g->world->AddChild(manaparticle);
                }
            }

            this->myManaball->size = (int) ((this->gatherFrameIndex/this->gatherFramesToYield) * 9);
        }
        if (this->gatherYielding)
        {
            if (this->gatherFrameIndex >= 4)
            {
                this->gatherFrameIndex = 0;
                this->gatherYielding = false;

                this->useAnimatedSequenceInfo(this->facingDown ? this->unitTemplate->sprSummonFront : this->unitTemplate->sprSummonBack);
            }
        }

        this->imageIndex = ((int) this->gatherFrameIndex % 4);
    }

    void Unit::gatherYield(int yieldAmount)
    {
        if (this->animationState != Gathering) return;
        if (this->gatherYielding) return;

        this->gatherFrameIndex = 0;
        this->gatherYielding = true;

        this->useAnimatedSequenceInfo(this->facingDown ? this->unitTemplate->sprYieldFront : this->unitTemplate->sprYieldBack);

        if (this->myManaball != nullptr)
        {
            auto floatingNumber = New<FloatingNumber>(
                    New<Point>(this->myManaball->position->x, this->myManaball->position->y - 10),
                    DsprColors::ManaLightBlue, yieldAmount);
            floatingNumber->ttl = 60;
            floatingNumber->vspeed = -0.3f;
            floatingNumber->depth = this->myManaball->depth - 20;

            auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
            g->world->AddChild(floatingNumber);

            this->myManaball->moveToPosition = New<Point>(this->myManaball->position->x,
                                                          this->myManaball->position->y - 48);
            this->myManaball->destroyAfterArrival = true;
            this->myManaball->speed = 1;
            this->myManaball->size = 9;
            this->myManaball = Null<Manaball>();
        }
    }

    void Unit::newNextTilePosition(int x, int y)
    {
        if (!this->unitTemplate->canMove) return;

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        if (this->tribeIndex == g->playersTribeIndex) {
            g->fogManager->conceilFog(this->tilePosition->x, this->tilePosition->y, this->unitTemplate->sight);
            g->fogManager->revealFog(this->nextTilePosition->x, this->nextTilePosition->y, this->unitTemplate->sight);
        }

        this->lastPosition->set(this->position);
        this->interpolation = interpolationMax-interpolationStep;

        this->updateTilePosition(this->nextTilePosition);

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
        if (dify >= 0 && !this->textureName->Equals(this->unitTemplate->sprWalkDown->spriteInfo->filename))
        {
            this->facingDown = true;
            this->useAnimatedSequenceInfo(this->unitTemplate->sprWalkDown);
        }
        if (dify < 0 && !this->textureName->Equals(this->unitTemplate->sprWalkUp->spriteInfo->filename))
        {
            this->facingDown = false;
            this->useAnimatedSequenceInfo(this->unitTemplate->sprWalkUp);
        }
    }

    void Unit::updateTilePosition(Ref<Point> newPosition)
    {
        this->tilePosition->set(newPosition);
        this->SetDepth(this->tilePosition->y * -1);
        this->walkAmount = 0;
    }

    void Unit::setAnimationState(UnitAnimationState newState, int heading) {
        auto g = (Global*) InternalApp::getGlobal();

        switch (this->animationState)
        {
            case Gathering:
            {
                this->myManaball->Destroy();
                this->myManaball = nullptr;
                this->gatherFrameIndex = 0;
                this->gatherYielding = false;
            }
                break;
        }

        this->animationState = newState;

        switch (newState)
        {
            case Walking:
            {
                this->useAnimatedSequenceInfo(this->unitTemplate->sprWalkDown);
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
                this->updateTilePosition(this->nextTilePosition);

                this->handleFacing(heading);

                switch(heading)
                {
                    case 0: {
                        this->useAnimatedSequenceInfo(this->unitTemplate->sprAttackRight);
                        }
                        break;
                    case 1: {
                        this->useAnimatedSequenceInfo(this->unitTemplate->sprAttackUpRight);
                    }
                        break;
                    case 2: {
                        this->useAnimatedSequenceInfo(this->unitTemplate->sprAttackUp);
                    }
                        break;
                    case 3: {
                        this->useAnimatedSequenceInfo(this->unitTemplate->sprAttackUpRight);
                    }
                        break;
                    case 4: {
                        this->useAnimatedSequenceInfo(this->unitTemplate->sprAttackRight);
                    }
                        break;
                    case 5: {
                        this->useAnimatedSequenceInfo(this->unitTemplate->sprAttackDownRight);
                    }
                        break;
                    case 6: {
                        this->useAnimatedSequenceInfo(this->unitTemplate->sprAttackDown);
                    }
                        break;
                    case 7: {
                        this->useAnimatedSequenceInfo(this->unitTemplate->sprAttackDownRight);
                    }
                        break;

                }
            }
                break;
            case Gathering:
            {
                this->imageIndex = 0;
                this->imageSpeed = 0;

                if (this->tribeIndex == g->playersTribeIndex) {
                    g->fogManager->conceilFog(this->tilePosition->x, this->tilePosition->y, this->unitTemplate->sight);
                    g->fogManager->revealFog(this->nextTilePosition->x, this->nextTilePosition->y, this->unitTemplate->sight);
                }

                this->lastPosition->set(this->position);
                this->interpolation = interpolationMax-interpolationStep;
                this->updateTilePosition(this->nextTilePosition);

                this->handleFacing(heading);

                switch(heading)
                {
                    case 0: case 4: case 5: case 6: case 7: {
                        this->useAnimatedSequenceInfo(this->unitTemplate->sprSummonFront);
                    }
                        break;
                    case 1: case 2: case 3: {
                        this->useAnimatedSequenceInfo(this->unitTemplate->sprSummonBack);
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
            this->unitTemplate->sprSelectCircle->tint = (this->tribeIndex==-1) ?
                                                        DsprColors::LightYellow :
                                                        (this->tribeIndex == g->playersTribeIndex) ?
                                                        DsprColors::LightGreen :
                                                        DsprColors::LightRed;

            this->unitTemplate->sprSelectCircle->position->set(this->position->x - this->unitTemplate->sprCenterAdjust->x, this->position->y - this->unitTemplate->sprCenterAdjust->y);

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
            if (this->unitTemplate->sprHoverCircle != nullptr) {
                this->unitTemplate->sprHoverCircle->tint = (this->tribeIndex==-1) ?
                                                           DsprColors::LightYellow :
                                                           (this->tribeIndex == g->playersTribeIndex) ?
                                                           DsprColors::LightGreen :
                                                           DsprColors::LightRed;

                this->unitTemplate->sprHoverCircle->position->set(this->position->x - this->unitTemplate->sprCenterAdjust->x, this->position->y - this->unitTemplate->sprCenterAdjust->y);

                this->unitTemplate->sprHoverCircle->drawSelf(camera, xoffset, yoffset);
            }
        }

        int newOffset = (this->scale->x == -1) ? (xoffset + this->unitTemplate->spriteFaceLeftXoffset) : xoffset;

        AnimatedSprite::drawSelf(camera, newOffset - this->unitTemplate->sprCenterAdjust->x, yoffset - this->unitTemplate->sprCenterAdjust->y);

        //TC

        this->tcSprite->imageIndex = this->frameStartIndex + this->imageIndex;
        this->tcSprite->position->set(this->position->x - this->unitTemplate->sprCenterAdjust->x, this->position->y - this->unitTemplate->sprCenterAdjust->y);
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

    void Unit::trainUnit(Ref<UnitTemplate> unitTemplate) {
        if (!this->unitTemplate->hasConstructionQueue) return;
        if (this->constructionQueue->atMaxQueue()) return;

        //this->constructionQueue->enqueue(unitTemplate);
    }

    void Unit::handleFacing(int heading) {
        switch(heading)
        {
            case 0: {
                this->facingDown = true;
                this->scale->x = 1;
            }
                break;
            case 1: {
                this->facingDown = false;
                this->scale->x = 1;
            }
                break;
            case 2: {
                this->facingDown = false;
            }
                break;
            case 3: {
                this->facingDown = false;
                this->scale->x = -1;
            }
                break;
            case 4: {
                this->facingDown = true;
                this->scale->x = -1;
            }
                break;
            case 5: {
                this->facingDown = true;
                this->scale->x = -1;
            }
                break;
            case 6: {
                this->facingDown = true;
            }
                break;
            case 7: {
                this->facingDown = true;
                this->scale->x = 1;
            }
                break;

        }
    }
}