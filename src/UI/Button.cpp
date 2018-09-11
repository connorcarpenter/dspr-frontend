//
// Created by connor on 9/10/18.
//

#include "Button.h"

namespace DsprFrontend {

    Button::Button(int imageIndex, bool requiresClickOnGameArea, Key::Code keyboardShortcut) {
        this->imageIndex = imageIndex;
        this->requiresClickOnGameArea = requiresClickOnGameArea;
        this->keyboardShortcut = keyboardShortcut;
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