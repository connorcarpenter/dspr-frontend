//
// Created by connor on 9/12/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include <Game/Unit/SpecificUnit/Manafount.h>
#include <DsprColors.h>
#include <Game/Unit/SpecificUnit/TempleBuilding.h>
#include <Game/Unit/SpecificUnit/TempleFlying.h>
#include "UnitTemplateCatalog.h"
#include "SpriteCatalog.h"
#include "../UI/ButtonCardCatalog.h"

namespace DsprFrontend
{
    UnitTemplateCatalog::UnitTemplateCatalog()
    {
        this->templateList = New<List<UnitTemplate>>();

        initWorker();
        initTempleBuilding();
        initTempleFlying();
        initManafount();
        initAshwalker();
        initRift();
    }

    Ref<UnitTemplate> UnitTemplateCatalog::findTemplateByIndex(int index)
    {
        return this->templateList->Find([&](Ref<UnitTemplate> unitTemplate){
            return unitTemplate->index == index;
        });
    }

    void UnitTemplateCatalog::initWorker()
    {
        auto g = (Global*) InternalApp::getGlobal();

        this->worker = New<UnitTemplate>(0);
        this->worker->sight = 6;
        this->worker->maxHealth = 420;
        this->worker->maxStamina = 100;
        this->worker->buildTime = 300;
        this->worker->setWalkSpeed(2,3);

        this->worker->hitSound = New<Sound>(New<Sova::String>("sounds/hit.wav"));
        this->worker->selectedSound = New<Sound>(New<Sova::String>("sounds/hello.wav"));
        this->worker->dieSound = New<Sound>(New<Sova::String>("sounds/die.wav"));
        this->worker->readySound = New<Sound>(New<Sova::String>("sounds/ready.wav"));
        this->worker->sprSelectCircle = g->unitSelectCircle_1x1;
        this->worker->sprHoverCircle = g->unitHoverCircle_1x1;
        this->worker->hasIdleTurnBehavior = true;
        this->worker->spriteFaceLeftXoffset = -2;
        this->worker->bleeds = true;
        this->worker->bloodColor = Color::Red;
        this->worker->hasDeathAnimation = true;
        this->worker->hasInventory = true;
        this->worker->commandCard = g->buttonCardCatalog->workerCommandCard;
        this->worker->canGather = true;
        this->worker->hasShadow = true;

        this->worker->sprBase = g->spriteCatalog->workerBase;
        this->worker->sprTC = g->spriteCatalog->workerTC;
        this->worker->sprSkin = g->spriteCatalog->workerSkin;
        this->worker->sprHairLong = g->spriteCatalog->workerHairLong;
        this->worker->sprHairShort = g->spriteCatalog->workerHairShort;

        this->worker->sprWalkDown = g->spriteCatalog->workerWalkDown;
        this->worker->sprWalkUp = g->spriteCatalog->workerWalkUp;
        this->worker->sprAttackUp = g->spriteCatalog->workerAttackUp1HS;
        this->worker->sprAttackUpRight = g->spriteCatalog->workerAttackUpRight1HS;
        this->worker->sprAttackRight = g->spriteCatalog->workerAttackRight1HS;
        this->worker->sprAttackDownRight = g->spriteCatalog->workerAttackDownRight1HS;
        this->worker->sprAttackDown = g->spriteCatalog->workerAttackDown1HS;
        this->worker->sprDieDown = g->spriteCatalog->workerDieDown;
        this->worker->sprDieUp = g->spriteCatalog->workerDieUp;
        this->worker->sprSummonFront = g->spriteCatalog->workerSummonFront;
        this->worker->sprSummonBack = g->spriteCatalog->workerSummonBack;
        this->worker->sprYieldFront = g->spriteCatalog->workerYieldFront;
        this->worker->sprYieldBack = g->spriteCatalog->workerYieldBack;

        this->worker->sprUnitPortrait = g->spriteCatalog->workerUnitPortrait;
        this->worker->sprUnitPortraitTC = g->spriteCatalog->workerUnitPortraitTC;
        this->worker->sprBigPortrait = g->spriteCatalog->workerBigPortrait;
        this->worker->sprBigPortraitTC = g->spriteCatalog->workerBigPortraitTC;

        this->templateList->Add(this->worker);
    }

    void UnitTemplateCatalog::initAshwalker()
    {
        auto g = (Global*) InternalApp::getGlobal();

        this->ashwalker = New<UnitTemplate>(3);
        this->ashwalker->sight = 6;
        this->ashwalker->maxHealth = 340;
        this->ashwalker->setWalkSpeed(4,6);

        this->ashwalker->hitSound = New<Sound>(New<Sova::String>("sounds/hit.wav"));
        this->ashwalker->dieSound = New<Sound>(New<Sova::String>("sounds/die.wav"));
        this->ashwalker->sprSelectCircle = g->unitSelectCircle_1x1;
        this->ashwalker->sprHoverCircle = g->unitHoverCircle_1x1;
        this->ashwalker->hasIdleTurnBehavior = true;
        this->ashwalker->spriteFaceLeftXoffset = -2;
        this->ashwalker->bleeds = true;
        this->ashwalker->bloodColor = DsprColors::AshwalkerBlood;
        this->ashwalker->hasDeathAnimation = true;
        this->ashwalker->hasShadow = true;

        this->ashwalker->sprBase = g->spriteCatalog->ashwalkerBase;

        this->ashwalker->sprWalkDown = g->spriteCatalog->ashwalkerWalkDown;
        this->ashwalker->sprWalkUp = g->spriteCatalog->ashwalkerWalkUp;
        this->ashwalker->sprAttackUp = g->spriteCatalog->ashwalkerAttackUp1HS;
        this->ashwalker->sprAttackUpRight = g->spriteCatalog->ashwalkerAttackUpRight1HS;
        this->ashwalker->sprAttackRight = g->spriteCatalog->ashwalkerAttackRight1HS;
        this->ashwalker->sprAttackDownRight = g->spriteCatalog->ashwalkerAttackDownRight1HS;
        this->ashwalker->sprAttackDown = g->spriteCatalog->ashwalkerAttackDown1HS;
        this->ashwalker->sprDieDown = g->spriteCatalog->ashwalkerDieDown;
        this->ashwalker->sprDieUp = g->spriteCatalog->ashwalkerDieUp;

        this->templateList->Add(this->ashwalker);
    }

    void UnitTemplateCatalog::initTempleBuilding()
    {
        auto g = (Global*) InternalApp::getGlobal();

        this->templeBuilding = New<UnitTemplate>(1);
        this->templeBuilding->sight = 12;
        this->templeBuilding->maxHealth = 1500;
        this->templeBuilding->buildTime = 1000;
        this->templeBuilding->sprSelectCircle = g->unitSelectCircle_5x5;
        this->templeBuilding->sprHoverCircle = g->unitHoverCircle_5x5;
        this->templeBuilding->canMove = false;
        this->templeBuilding->tileWidth = 5;
        this->templeBuilding->tileHeight = 5;
        this->templeBuilding->bleeds = false;
        this->templeBuilding->hasDeathAnimation = false;
        this->templeBuilding->commandCard = g->buttonCardCatalog->templeBuildingCommandCard;
        this->templeBuilding->hasConstructionQueue = true;
        this->templeBuilding->sprCenterAdjust = New<Point>(0,1);
        this->templeBuilding->hasRallyPoint = true;
        this->templeBuilding->hasShadow = true;

        this->templeBuilding->sprBase = g->spriteCatalog->templeBase;
        this->templeBuilding->sprWalkDown = g->spriteCatalog->templeBuilding;
        this->templeBuilding->sprTC = g->spriteCatalog->templeTC;
        this->templeBuilding->sprUnitPortrait = g->spriteCatalog->templeUnitPortrait;
        this->templeBuilding->sprUnitPortraitTC = g->spriteCatalog->templeUnitPortraitTC;
        this->templeBuilding->sprBigPortrait = g->spriteCatalog->templeBigPortrait;
        this->templeBuilding->sprBigPortraitTC = g->spriteCatalog->templeBigPortraitTC;
        this->templeBuilding->createSpecificUnitFunction = [&](Unit* unit) {
            return New<DsprFrontend::TempleBuilding>(unit);
        };

        this->templateList->Add(this->templeBuilding);
    }

    void UnitTemplateCatalog::initTempleFlying()
    {
        auto g = (Global*) InternalApp::getGlobal();

        this->templeFlying = New<UnitTemplate>(4);
        this->templeFlying->sight = 12;
        this->templeFlying->maxHealth = 1500;
        this->templeFlying->buildTime = 1000;
        this->templeFlying->sprSelectCircle = g->unitSelectCircle_5x5;
        this->templeFlying->sprHoverCircle = g->unitHoverCircle_5x5;
        this->templeFlying->canMove = true;
        this->templeFlying->tileWidth = 1;
        this->templeFlying->tileHeight = 1;
        this->templeFlying->bleeds = false;
        this->templeFlying->hasDeathAnimation = false;
        this->templeFlying->commandCard = g->buttonCardCatalog->templeFlyingCommandCard;
        this->templeFlying->setWalkSpeed(20,30);
        this->templeFlying->hasShadow = true;
        this->templeFlying->canTurnLeftAndRight = false;
        this->templeFlying->zBase = -6;
        this->templeFlying->sprCenterAdjust = New<Point>(0,1);

        this->templeFlying->sprBase = g->spriteCatalog->templeBase;
        this->templeFlying->sprWalkDown = g->spriteCatalog->templeFlying;
        this->templeFlying->sprWalkUp = g->spriteCatalog->templeFlying;

        this->templeFlying->sprTC = g->spriteCatalog->templeTC;
        this->templeFlying->sprUnitPortrait = g->spriteCatalog->templeUnitPortrait;
        this->templeFlying->sprUnitPortraitTC = g->spriteCatalog->templeUnitPortraitTC;
        this->templeFlying->sprBigPortrait = g->spriteCatalog->templeBigPortrait;
        this->templeFlying->sprBigPortraitTC = g->spriteCatalog->templeBigPortraitTC;
        this->templeFlying->createSpecificUnitFunction = [&](Unit* unit) {
            return New<DsprFrontend::TempleFlying>(unit);
        };

        this->templateList->Add(this->templeFlying);
    }

    void UnitTemplateCatalog::initManafount() {
        auto g = (Global*) InternalApp::getGlobal();

        this->manafount = New<UnitTemplate>(2);
        this->manafount->canMove = false;
        this->manafount->tileWidth = 2;
        this->manafount->tileHeight = 2;
        this->manafount->bleeds = false;
        this->manafount->hasDeathAnimation = false;
        this->manafount->sprSelectCircle = g->unitSelectCircle_2x2;
        this->manafount->sprHoverCircle = g->unitHoverCircle_2x2;
        this->manafount->sprCenterAdjust = New<Point>(10,-3+5);
        this->manafount->isGatherable = true;

        this->manafount->sprBase = g->spriteCatalog->manafount;
        this->manafount->createSpecificUnitFunction = [&](Unit* unit) {
            return New<DsprFrontend::Manafount>(unit);
        };

        this->templateList->Add(this->manafount);
    }

    void UnitTemplateCatalog::initRift() {
        auto g = (Global*) InternalApp::getGlobal();

        this->rift = New<UnitTemplate>(5);
        this->rift->sight = 0;
        this->rift->sprSelectCircle = g->unitSelectCircle_5x5;
        this->rift->sprHoverCircle = g->unitHoverCircle_5x5;
        this->rift->canMove = false;
        this->rift->tileWidth = 5;
        this->rift->tileHeight = 5;
        this->rift->bleeds = false;
        this->rift->hasDeathAnimation = false;
        this->rift->hasShadow = true;

        this->rift->sprBase = g->spriteCatalog->rift;

        this->templateList->Add(this->rift);
    }
}