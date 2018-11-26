//
// Created by connor on 9/10/18.
//

#include <Sova/Internal/InternalApp.h>
#include "ButtonCardCatalog.h"
#include "ButtonCard.h"
#include "ButtonActions.h"
#include "SpriteCatalog.h"
#include "Game/Global.h"
#include "Game/Unit/UnitTemplateCatalog.h"
#include "Cursor.h"
#include "Game/TileManager.h"

namespace DsprFrontend
{

    ButtonCardCatalog::ButtonCardCatalog()
    {
        auto g = (Global*) InternalApp::getGlobal();
        this->buttonActions = New<ButtonActions>();

        //create standard actions
        this->moveButton = New<Button>(g->spriteCatalog->sprCommandActions, 0, true, Key::M); moveButton->usesSubframes = true;
        moveButton->setFinalAction([&]() {
            this->buttonActions->moveAction();
        });
        this->stopButton = New<Button>(g->spriteCatalog->sprCommandActions, 1, false, Key::S); stopButton->usesSubframes = true;
        stopButton->setFinalAction([&]() {
            this->buttonActions->stopAction();
        });
        this->attackButton = New<Button>(g->spriteCatalog->sprCommandActions, 3, true, Key::A); attackButton->usesSubframes = true;
        this->attackButton->setFinalAction([&]() {
            this->buttonActions->attackAction();
        });

        //init specific command cards
        initWorkerButtonCard();
        initTempleBuildingButtonCard();
        initTempleFlyingButtonCard();
    }

    void ButtonCardCatalog::initWorkerButtonCard()
    {
        auto g = (Global*) InternalApp::getGlobal();


        auto holdButton = New<Button>(g->spriteCatalog->sprCommandActions, 2, false, Key::H); holdButton->usesSubframes = true;
        auto dashButton = New<Button>(g->spriteCatalog->sprCommandActions, 4, true, Key::D); dashButton->usesSubframes = true;
        auto gatherButton = New<Button>(g->spriteCatalog->sprCommandActions, 5, true, Key::G); gatherButton->usesSubframes = true;

        this->workerCommandCard = New<ButtonCard>();
        this->workerCommandCard->AddButton(this->moveButton);
        this->workerCommandCard->AddButton(this->stopButton);
        this->workerCommandCard->AddButton(holdButton);
        this->workerCommandCard->AddButton(this->attackButton);
        this->workerCommandCard->AddButton(dashButton);
        this->workerCommandCard->AddButton(gatherButton);

        holdButton->setFinalAction([&]() {
            this->buttonActions->holdAction();
        });
        gatherButton->setFinalAction([&]() {
            this->buttonActions->gatherAction();
        });
    }

    void ButtonCardCatalog::initTempleBuildingButtonCard()
    {
        auto g = (Global*) InternalApp::getGlobal();

        this->templeBuildingCommandCard = New<ButtonCard>();

        auto villagerButton = New<Button>(g->spriteCatalog->workerUnitPortrait, false, Key::V); villagerButton->usesTeamColor = true;
        villagerButton->tcSpriteInfo = g->spriteCatalog->workerUnitPortraitTC;
        this->templeBuildingCommandCard->AddButton(villagerButton);
        villagerButton->setFinalAction([&] {
            this->buttonActions->createVillagerAction();
        });

        for(int i=0;i<6;i++)
            this->templeBuildingCommandCard->AddButton(Null<Button>());
        
        auto rallyButton = New<Button>(g->spriteCatalog->sprCommandActions, 6, true, Key::Y); rallyButton->usesSubframes = true;
        this->templeBuildingCommandCard->AddButton(rallyButton);
        rallyButton->setFinalAction([&] {
            this->buttonActions->rallyAction();
        });

        for(int i=0;i<2;i++)
            this->templeBuildingCommandCard->AddButton(Null<Button>());

        auto liftOffButton = New<Button>(g->spriteCatalog->sprCommandActions, 8, false, Key::L); liftOffButton->usesSubframes = true;
        this->templeBuildingCommandCard->AddButton(liftOffButton);
        liftOffButton->setFinalAction([&] {
            this->buttonActions->specialAction(0);
        });

        auto cancelButton = New<Button>(g->spriteCatalog->sprCommandActions, 7, false, Key::C); cancelButton->usesSubframes = true;
        this->templeBuildingCommandCard->AddButton(cancelButton);
        cancelButton->setFinalAction([&] {
            this->buttonActions->cancelTrainAction();
        });
    }

    void ButtonCardCatalog::initTempleFlyingButtonCard()
    {
        auto g = (Global*) InternalApp::getGlobal();

        this->templeFlyingCommandCard = New<ButtonCard>();

        this->templeFlyingCommandCard->AddButton(this->moveButton);
        this->templeFlyingCommandCard->AddButton(this->stopButton);

        for(int i=0;i<9;i++)
            this->templeFlyingCommandCard->AddButton(Null<Button>());

        auto landButton = New<Button>(g->spriteCatalog->sprCommandActions, 9, true, Key::L); landButton->usesSubframes = true;
        this->templeFlyingCommandCard->AddButton(landButton);
        landButton->setBeginAction([&] {
            auto g = (Global*) InternalApp::getGlobal();
            g->cursor->beginBuildingState(g->unitTemplateCatalog->templeBuilding);
        });
        landButton->setFinalAction([&] {
            auto g = (Global*) InternalApp::getGlobal();
            auto tilePosition = g->tileManager->getTilePosition(g->cursor->worldPosition->x, g->cursor->worldPosition->y);
            this->buttonActions->specialAction(0, tilePosition->x, tilePosition->y);
        });
    }
}