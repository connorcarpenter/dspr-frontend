//
// Created by connor on 9/10/18.
//

#include "ButtonCard.h"

namespace DsprFrontend
{
    ButtonCard::ButtonCard()
    {
        this->buttonList = New<List<Button>>();
    }

    void ButtonCard::AddButton(Ref<Button> button)
    {
        this->buttonList->Add(button);
    }
}