#pragma once

//
// Created by connor on 9/10/18.
//

#include <Sova/References/Refable.h>
#include <Sova/References/Ref.h>

using namespace Sova;

namespace DsprFrontend {
    class Button;
    class ButtonCard;
    class ButtonActions;

    class ButtonCardCatalog : public Sova::Refable {
    public:
        ButtonCardCatalog();
        Ref<ButtonCard> workerCommandCard = Null<ButtonCard>();
        Ref<Button> attackButton = Null<Button>();
    private:
        Ref<ButtonActions> buttonActions = Null<ButtonActions>();

        void initWorkerButtonCard();

        void initTempleButtonCard();
    };
}