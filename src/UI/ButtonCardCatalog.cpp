//
// Created by connor on 9/10/18.
//

#include "ButtonCardCatalog.h"
#include "ButtonCard.h"
#include "ButtonActions.h"

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
        auto moveButton = New<Button>(0, true, Key::M);
        auto stopButton = New<Button>(1, false, Key::S);
        auto holdButton = New<Button>(2, false, Key::H);
        this->attackButton = New<Button>(3, true, Key::A);
        auto dashButton = New<Button>(4, true, Key::D);

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

    void ButtonCardCatalog::initTempleButtonCard() {

    }
}