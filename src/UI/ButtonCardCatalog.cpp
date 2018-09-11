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
        auto moveButton = New<Button>(0);
        auto stopButton = New<Button>(1);
        auto holdButton = New<Button>(2);
        auto attackButton = New<Button>(3);
        auto dashButton = New<Button>(4);

        this->basicCommandCard = New<ButtonCard>();
        this->basicCommandCard->AddButton(moveButton);
        this->basicCommandCard->AddButton(stopButton);
        this->basicCommandCard->AddButton(holdButton);
        this->basicCommandCard->AddButton(attackButton);
        this->basicCommandCard->AddButton(dashButton);

        this->buttonActions = New<ButtonActions>();
        moveButton->setAction([&](){
            this->buttonActions->moveAction();
        });
        stopButton->setAction([&](){
            this->buttonActions->stopAction();
        });
        holdButton->setAction([&](){
            this->buttonActions->holdAction();
        });
        attackButton->setAction([&](){
            this->buttonActions->attackAction();
        });
    }
}