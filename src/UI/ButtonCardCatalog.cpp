//
// Created by connor on 9/10/18.
//

#include <Sova/Internal/InternalApp.h>
#include "ButtonCardCatalog.h"
#include "ButtonCard.h"
#include "ButtonActions.h"
#include "SpriteCatalog.h"
#include "Global.h"

namespace DsprFrontend
{

    ButtonCardCatalog::ButtonCardCatalog()
    {
        this->buttonActions = New<ButtonActions>();
        initWorkerButtonCard();
        initTempleButtonCard();
    }

    void ButtonCardCatalog::initWorkerButtonCard()
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        auto moveButton = New<Button>(g->spriteCatalog->sprCommandActions, 0, true, Key::M); moveButton->usesSubframes = true;
        auto stopButton = New<Button>(g->spriteCatalog->sprCommandActions, 1, false, Key::S); stopButton->usesSubframes = true;
        auto holdButton = New<Button>(g->spriteCatalog->sprCommandActions, 2, false, Key::H); holdButton->usesSubframes = true;
        this->attackButton = New<Button>(g->spriteCatalog->sprCommandActions, 3, true, Key::A); attackButton->usesSubframes = true;
        auto dashButton = New<Button>(g->spriteCatalog->sprCommandActions, 4, true, Key::D); dashButton->usesSubframes = true;

        this->workerCommandCard = New<ButtonCard>();
        this->workerCommandCard->AddButton(moveButton);
        this->workerCommandCard->AddButton(stopButton);
        this->workerCommandCard->AddButton(holdButton);
        this->workerCommandCard->AddButton(this->attackButton);
        this->workerCommandCard->AddButton(dashButton);

        moveButton->setAction([&](){
            this->buttonActions->moveAction();
        });
        stopButton->setAction([&](){
            this->buttonActions->stopAction();
        });
        holdButton->setAction([&](){
            this->buttonActions->holdAction();
        });
        this->attackButton->setAction([&](){
            this->buttonActions->attackAction();
        });
    }

    void ButtonCardCatalog::initTempleButtonCard()
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        this->templeCommandCard = New<ButtonCard>();

        auto villagerButton = New<Button>(g->spriteCatalog->workerUnitPortrait, false, Key::V); villagerButton->usesTeamColor = true;
        villagerButton->tcSpriteInfo = g->spriteCatalog->workerUnitPortraitTC;
        this->templeCommandCard->AddButton(villagerButton);
        villagerButton->setAction([&]{
           this->buttonActions->createVillagerAction();
        });
    }
}