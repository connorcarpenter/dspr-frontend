//
// Created by connor on 7/26/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Sova/Math/Math.h>
#include "Game/Global.h"
#include "Game/FogManager.h"
#include "SpriteCatalog.h"
#include <Sova/Audio/Sound.h>
#include <Game/Effects/Manaball.h>
#include <Game/Effects/FloatingNumber.h>
#include <Game/Unit/SpecificUnit/Manafount.h>
#include "Unit.h"
#include "Game/TileManager.h"
#include "DsprColors.h"
#include "UnitTemplate.h"
#include "UnitTemplateCatalog.h"
#include "UnitManager.h"
#include "Game/EconomyManager.h"

namespace DsprFrontend
{
    Unit::Unit(int id, int x, int y, int tribeIndex, Ref<UnitTemplate> unitTemplate) : AnimatedSprite()
    {
        auto g = (Global*) InternalApp::getGlobal();

        this->id = id;
        this->moveTarget = New<Point>(x, y);
        this->tilePosition = New<Point>(x,y);
        this->nextTilePosition = New<Point>(x,y);
        this->lastPosition = New<Point>(x,y);
        this->tribeIndex = tribeIndex;
        this->unitTemplate = unitTemplate;

        this->health = this->unitTemplate->maxHealth;
        this->stamina = this->unitTemplate->maxStamina;

        this->useAnimatedSpriteInfo(this->unitTemplate->sprBase);

        this->tcSprite = New<AnimatedSprite>();
        this->tcSprite->useAnimatedSpriteInfo(this->unitTemplate->sprTC);
        this->tcSprite->tint = g->unitManager->getColorFromTribeIndex(tribeIndex);

        //skin
        this->skinSprite = New<AnimatedSprite>();
        this->skinSprite->useAnimatedSpriteInfo(this->unitTemplate->sprSkin);
        int skinIndex = (int) Math::Random(0, 3);

        switch (skinIndex)
        {
            case 0:
                this->skinSprite->tint = DsprColors::DarkSkin;
                this->hairColor = DsprColors::BlackHair;
                break;
            case 1:
                this->skinSprite->tint = DsprColors::MediumSkin;
                if (Math::Random(0, 2) < 1)
                    this->hairColor = DsprColors::BlackHair;
                else
                    this->hairColor = DsprColors::BrownHair;
                break;
            case 2:
                this->skinSprite->tint = DsprColors::LightSkin;
                {
                    switch ((int) Math::Random(0, 4)) {
                        case 0:
                            this->hairColor = DsprColors::BlackHair;
                            break;
                        case 1:
                            this->hairColor = DsprColors::BrownHair;
                            break;
                        case 2:
                            this->hairColor = DsprColors::BlondHair;
                            break;
                        case 3:
                            this->hairColor = DsprColors::RedHair;
                            break;
                    }
                }
                break;
        }

        this->hasShortHair = Math::Random(0, 2) < 1;
        this->updateHeadSprite(Ref<ItemTemplate>());

        this->OnUpdate([&](float deltaFrameMs){ step(deltaFrameMs); });

        if (this->tribeIndex == g->playersTribeIndex)
            g->fogManager->revealFog(this->tilePosition->x, this->tilePosition->y, this->unitTemplate->sight);

        this->updatePosition();

        this->SetDepth(this->tilePosition->y * -1);

        if (this->unitTemplate->hasConstructionQueue)
        {
            this->constructionQueue = New<ConstructionQueue>();
        }

        if (this->unitTemplate->hasRallyPoint)
        {
            this->rallyPoint = New<Point>();
        }

        if (this->unitTemplate->hasInventory)
        {
            this->inventory = New<Inventory>(this);
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
        auto g = (Global*) InternalApp::getGlobal();

        if (!this->tilePosition->Equals(this->nextTilePosition))
        {
            this->imageSpeed = walkImageSpeed;
            this->walkAmount += this->walkSpeed * (deltaFrameMs / gameServerTickMs);
            if (this->walkAmount >= this->unitTemplate->walkMax)
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
                                if (Math::Random(0, 2) < 1) {
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

            auto g = (Global*) InternalApp::getGlobal();
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

            this->myManaball->size = Math::Min((this->gatherFrameIndex/this->gatherFramesToYield) * 9, 9);
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

            auto g = (Global*) InternalApp::getGlobal();
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

        auto g = (Global*) InternalApp::getGlobal();
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
            walkSpeed = this->unitTemplate->walkSpeedDiagonal;
        }
        else
        {
            walkSpeed = this->unitTemplate->walkSpeedStraight;
        }

        if (difx != 0)
        {
            this->scale->x = Math::Sign(difx);
        }
        if (dify >= 0)
        {
            this->facingDown = true;
            this->useAnimatedSequenceInfo(this->unitTemplate->sprWalkDown);
        }
        if (dify < 0)
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

        float extrapolatedPositionX = ((Math::Lerp(this->tilePosition->x, this->nextTilePosition->x, walkAmount/this->unitTemplate->walkMax)/2) + 0.5f) * g->tileManager->tileWidth;
        float extrapolatedPositionY = ((Math::Lerp(this->tilePosition->y, this->nextTilePosition->y, walkAmount/this->unitTemplate->walkMax)/2) + 0.5f) * g->tileManager->tileHeight;
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

    void Unit::updateHeadSprite(Ref<ItemTemplate> itemTemplate)
    {
        if (this->headSprite == nullptr) this->headSprite = New<AnimatedSprite>();
        if (itemTemplate == nullptr) {
            if (this->hasShortHair)
                this->headSprite->useAnimatedSpriteInfo(this->unitTemplate->sprHairShort);
            else
                this->headSprite->useAnimatedSpriteInfo(this->unitTemplate->sprHairLong);
            this->headSprite->tint = this->hairColor;
        }
        else
        {
            this->headSprite->tint = Color::White;
            this->headSprite->useAnimatedSpriteInfo(itemTemplate->wornSpriteInfo);
        }
    }

    void Unit::updateBodySprite(Ref<ItemTemplate> itemTemplate) {
        if (itemTemplate == nullptr)
        {
            this->armorSprite = Null<AnimatedSprite>();
        }
        else
        {
            if (this->armorSprite == nullptr) this->armorSprite = New<AnimatedSprite>();
            this->armorSprite->useAnimatedSpriteInfo(itemTemplate->wornSpriteInfo);
        }
    }

    void Unit::updateLeftHandSprite(Ref<ItemTemplate> itemTemplate)
    {
        if (itemTemplate == nullptr)
        {
            this->leftHandSprite = Null<AnimatedSprite>();
        }
        else
        {
            if (this->leftHandSprite == nullptr) this->leftHandSprite = New<AnimatedSprite>();
            this->leftHandSprite->useAnimatedSpriteInfo(itemTemplate->wornSpriteInfo);
        }
    }

    void Unit::updateRightHandSprite(Ref<ItemTemplate> itemTemplate)
    {
        if (itemTemplate == nullptr)
        {
            this->rightHandSprite = Null<AnimatedSprite>();
        }
        else
        {
            if (this->rightHandSprite == nullptr) this->rightHandSprite = New<AnimatedSprite>();
            this->rightHandSprite->useAnimatedSpriteInfo(itemTemplate->wornSpriteInfo);
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

        //Weapon
        if (!this->facingDown) {
            if (this->leftHandSprite != nullptr) {
                this->leftHandSprite->imageIndex = this->frameStartIndex + this->imageIndex;
                this->leftHandSprite->position->set(this->position->x - this->unitTemplate->sprCenterAdjust->x,
                                                    this->position->y - this->unitTemplate->sprCenterAdjust->y);
                this->leftHandSprite->scale->x = this->scale->x;
                this->leftHandSprite->drawSelf(camera, newOffset, yoffset);
            }

            if (this->rightHandSprite != nullptr) {
                this->rightHandSprite->imageIndex = this->frameStartIndex + this->imageIndex;
                this->rightHandSprite->position->set(this->position->x - this->unitTemplate->sprCenterAdjust->x,
                                                     this->position->y - this->unitTemplate->sprCenterAdjust->y);
                this->rightHandSprite->scale->x = this->scale->x;
                this->rightHandSprite->drawSelf(camera, newOffset, yoffset);
            }
        }

        AnimatedSprite::drawSelf(camera, newOffset - this->unitTemplate->sprCenterAdjust->x, yoffset - this->unitTemplate->sprCenterAdjust->y);

        //TC
        this->tcSprite->imageIndex = this->frameStartIndex + this->imageIndex;
        this->tcSprite->position->set(this->position->x - this->unitTemplate->sprCenterAdjust->x, this->position->y - this->unitTemplate->sprCenterAdjust->y);
        this->tcSprite->scale->x = this->scale->x;
        this->tcSprite->drawSelf(camera, newOffset, yoffset);

        //Skin
        this->skinSprite->imageIndex = this->frameStartIndex + this->imageIndex;
        this->skinSprite->position->set(this->position->x - this->unitTemplate->sprCenterAdjust->x, this->position->y - this->unitTemplate->sprCenterAdjust->y);
        this->skinSprite->scale->x = this->scale->x;
        this->skinSprite->drawSelf(camera, newOffset, yoffset);

        //Head
        this->headSprite->imageIndex = this->frameStartIndex + this->imageIndex;
        this->headSprite->position->set(this->position->x - this->unitTemplate->sprCenterAdjust->x, this->position->y - this->unitTemplate->sprCenterAdjust->y);
        this->headSprite->scale->x = this->scale->x;
        this->headSprite->drawSelf(camera, newOffset, yoffset);

        //Armor
        if (this->armorSprite != nullptr) {
            this->armorSprite->imageIndex = this->frameStartIndex + this->imageIndex;
            this->armorSprite->position->set(this->position->x - this->unitTemplate->sprCenterAdjust->x,
                                             this->position->y - this->unitTemplate->sprCenterAdjust->y);
            this->armorSprite->scale->x = this->scale->x;
            this->armorSprite->drawSelf(camera, newOffset, yoffset);
        }

        //Weapon
        if (this->facingDown) {
            if (this->leftHandSprite != nullptr) {
                this->leftHandSprite->imageIndex = this->frameStartIndex + this->imageIndex;
                this->leftHandSprite->position->set(this->position->x - this->unitTemplate->sprCenterAdjust->x,
                                                    this->position->y - this->unitTemplate->sprCenterAdjust->y);
                this->leftHandSprite->scale->x = this->scale->x;
                this->leftHandSprite->drawSelf(camera, newOffset, yoffset);
            }

            if (this->rightHandSprite != nullptr) {
                this->rightHandSprite->imageIndex = this->frameStartIndex + this->imageIndex;
                this->rightHandSprite->position->set(this->position->x - this->unitTemplate->sprCenterAdjust->x,
                                                     this->position->y - this->unitTemplate->sprCenterAdjust->y);
                this->rightHandSprite->scale->x = this->scale->x;
                this->rightHandSprite->drawSelf(camera, newOffset, yoffset);
            }
        }
    }

    void Unit::drawShadow(Ref<Camera> camera, int xoffset, int yoffset)
    {
        int newOffset = (this->scale->x == -1) ? (xoffset + this->unitTemplate->spriteFaceLeftXoffset) : xoffset;
        const int theSkewX = -6;
        const float theAlpha = 0.25f;
        this->skew->x = theSkewX;
        this->scale->y = 0.5f;
        this->tint = Color::Black;
        this->alpha = theAlpha;

        AnimatedSprite::drawSelf(camera, newOffset - this->unitTemplate->sprCenterAdjust->x, yoffset - this->unitTemplate->sprCenterAdjust->y);

        this->skew->x = 0;
        this->scale->y = 1.0f;
        this->tint = Color::White;
        this->alpha = 1.0f;

        //Weapon
        if (this->leftHandSprite != nullptr)
        {
            this->leftHandSprite->skew->x = theSkewX;
            this->leftHandSprite->scale->y = 0.5f;
            auto lastTint = this->leftHandSprite->tint;
            this->leftHandSprite->tint = Color::Black;
            this->leftHandSprite->alpha = theAlpha;

            this->leftHandSprite->imageIndex = this->frameStartIndex + this->imageIndex;
            this->leftHandSprite->position->set(this->position->x - this->unitTemplate->sprCenterAdjust->x,
                                                this->position->y - this->unitTemplate->sprCenterAdjust->y);
            this->leftHandSprite->scale->x = this->scale->x;
            this->leftHandSprite->drawSelf(camera, newOffset, yoffset);

            this->leftHandSprite->skew->x = 0;
            this->leftHandSprite->scale->y = 1.0f;
            this->leftHandSprite->tint = lastTint;
            this->leftHandSprite->alpha = 1.0f;
        }

        if (this->rightHandSprite != nullptr)
        {
            this->rightHandSprite->skew->x = theSkewX;
            this->rightHandSprite->scale->y = 0.5f;
            auto lastTint = this->rightHandSprite->tint;
            this->rightHandSprite->tint = Color::Black;
            this->rightHandSprite->alpha = theAlpha;

            this->rightHandSprite->imageIndex = this->frameStartIndex + this->imageIndex;
            this->rightHandSprite->position->set(this->position->x - this->unitTemplate->sprCenterAdjust->x,
                                                 this->position->y - this->unitTemplate->sprCenterAdjust->y);
            this->rightHandSprite->scale->x = this->scale->x;
            this->rightHandSprite->drawSelf(camera, newOffset, yoffset);

            this->rightHandSprite->skew->x = 0;
            this->rightHandSprite->scale->y = 1.0f;
            this->rightHandSprite->tint = lastTint;
            this->rightHandSprite->alpha = 1.0f;
        }
    }
}