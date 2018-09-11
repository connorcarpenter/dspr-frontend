//
// Created by connor on 9/10/18.
//

#include "Button.h"

namespace DsprFrontend {

    Button::Button(int imageIndex, bool requiresClickOnGameArea) {
        this->imageIndex = imageIndex;
        this->requiresClickOnGameArea = requiresClickOnGameArea;
    }

    void Button::setAction(std::function<void()> actionFunction) {
        this->action = actionFunction;
    }

    void Button::executeAction() {
        if (this->action != nullptr){
            this->action();
        }
    }
}