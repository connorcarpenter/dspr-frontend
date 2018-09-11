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
        auto moveButton = New<Button>(0, true);
        auto stopButton = New<Button>(1, false);
        auto holdButton = New<Button>(2, false);
        this->attackButton = New<Button>(3, true);
        auto dashButton = New<Button>(4, true);

        this->basicCommandCard = New<ButtonCard>();
        this->basicCommandCard->AddButton(moveButton);
        this->basicCommandCard->AddButton(stopButton);
        this->basicCommandCard->AddButton(holdButton);
        this->basicCommandCard->AddButton(this->attackButton);
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
        this->attackButton->setAction([&](){
            this->buttonActions->attackAction();
        });
    }
}