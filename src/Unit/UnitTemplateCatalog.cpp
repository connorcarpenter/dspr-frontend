//
// Created by connor on 9/12/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Global.h>
#include "UnitTemplateCatalog.h"
#include "SpriteCatalog.h"
#include "../UI/ButtonCardCatalog.h"

namespace DsprFrontend
{
    UnitTemplateCatalog::UnitTemplateCatalog()
    {
        initWorker();
        initTemple();

        this->templateList = New<List<UnitTemplate>>();
        this->templateList->Add(this->worker);
        this->templateList->Add(this->temple);
    }

    Ref<UnitTemplate> UnitTemplateCatalog::findTemplateByIndex(int index)
    {
        return this->templateList->Find([&](Ref<UnitTemplate> unitTemplate){
            return unitTemplate->index == index;
        });
    }

    void UnitTemplateCatalog::initWorker()
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        this->worker = New<UnitTemplate>(0);
        this->worker->sight = 6;
        this->worker->maxHealth = 420;
        this->worker->buildTime = 100;

        this->worker->hitSound = New<Sound>(New<Sova::String>("sounds/hit.wav"));
        this->worker->selectedSound = New<Sound>(New<Sova::String>("sounds/hello.wav"));
        this->worker->dieSound = New<Sound>(New<Sova::String>("sounds/die.wav"));
        this->worker->readySound = New<Sound>(New<Sova::String>("sounds/ready.wav"));
        this->worker->sprSelectCircle = g->unitSelectCircle_1x1;
        this->worker->sprHoverCircle = g->unitHoverCircle_1x1;
        this->worker->hasIdleTurnBehavior = true;
        this->worker->spriteFaceLeftXoffset = -2;
        this->worker->bleeds = true;
        this->worker->hasDeathAnimation = true;
        this->worker->hasInventory = true;
        this->worker->commandCard = g->buttonCardCatalog->workerCommandCard;

        this->worker->sprWalkDown = g->spriteCatalog->workerWalkDown;
        this->worker->sprWalkUp = g->spriteCatalog->workerWalkUp;
        this->worker->sprWalkDownTC = g->spriteCatalog->workerWalkDownTC;
        this->worker->sprWalkUpTC = g->spriteCatalog->workerWalkUpTC;
        this->worker->sprAttackUp = g->spriteCatalog->workerAttackUp;
        this->worker->sprAttackUpRight = g->spriteCatalog->workerAttackUpRight;
        this->worker->sprAttackRight = g->spriteCatalog->workerAttackRight;
        this->worker->sprAttackDownRight = g->spriteCatalog->workerAttackDownRight;
        this->worker->sprAttackDown = g->spriteCatalog->workerAttackDown;
        this->worker->sprAttackUpTC = g->spriteCatalog->workerAttackUpTC;
        this->worker->sprAttackUpRightTC = g->spriteCatalog->workerAttackUpRightTC;
        this->worker->sprAttackRightTC = g->spriteCatalog->workerAttackRightTC;
        this->worker->sprAttackDownRightTC = g->spriteCatalog->workerAttackDownRightTC;
        this->worker->sprAttackDownTC = g->spriteCatalog->workerAttackDownTC;
        this->worker->sprDieDown = g->spriteCatalog->workerDieDown;
        this->worker->sprDieUp = g->spriteCatalog->workerDieUp;
        this->worker->sprDieDownTC = g->spriteCatalog->workerDieDownTC;
        this->worker->sprDieUpTC = g->spriteCatalog->workerDieUpTC;
        this->worker->sprUnitPortrait = g->spriteCatalog->workerUnitPortrait;
        this->worker->sprUnitPortraitTC = g->spriteCatalog->workerUnitPortraitTC;
        this->worker->sprBigPortrait = g->spriteCatalog->workerBigPortrait;
        this->worker->sprBigPortraitTC = g->spriteCatalog->workerBigPortraitTC;
    }

    void UnitTemplateCatalog::initTemple()
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        this->temple = New<UnitTemplate>(1);
        this->temple->sight = 12;
        this->temple->maxHealth = 1500;
        this->temple->buildTime = 1000;
        this->temple->sprSelectCircle = g->unitSelectCircle_5x5;
        this->temple->sprHoverCircle = g->unitHoverCircle_5x5;
        this->temple->canMove = false;
        this->temple->tileWidth = 5;
        this->temple->tileHeight = 5;
        this->temple->bleeds = false;
        this->temple->hasDeathAnimation = false;
        this->temple->commandCard = g->buttonCardCatalog->templeCommandCard;
        this->temple->hasConstructionQueue = true;

        this->temple->sprWalkDown = g->spriteCatalog->temple;
        this->temple->sprWalkDownTC = g->spriteCatalog->templeTC;
        this->temple->sprUnitPortrait = g->spriteCatalog->templeUnitPortrait;
        this->temple->sprUnitPortraitTC = g->spriteCatalog->templeUnitPortraitTC;
        this->temple->sprBigPortrait = g->spriteCatalog->templeBigPortrait;
        this->temple->sprBigPortraitTC = g->spriteCatalog->templeBigPortraitTC;
    }
}