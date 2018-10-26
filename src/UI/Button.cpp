//
// Created by connor on 9/10/18.
//

#include "Button.h"

namespace DsprFrontend {

    Button::Button(Ref<AnimatedSpriteInfo> spriteInfo, int imageIndex, bool requiresClickOnGameArea,
                       Key::Code keyboardShortcut)
    {
        this->spriteInfo = spriteInfo;
        this->usesSubframes = true;
        this->imageIndex = imageIndex;
        this->requiresClickOnGameArea = requiresClickOnGameArea;
        this->keyboardShortcut = keyboardShortcut;
    }

    void Button::setAction(std::function<void()> actionFunction)
    {
        this->action = actionFunction;
    }

    void Button::executeAction()
    {
        if (this->action != nullptr){
            this->action();
        }
    }

    Button::Button(Ref<AnimatedSpriteInfo> spriteInfo, bool requiresClickOnGameArea, Key::Code keyboardShortcut)
    {
        this->usesSubframes = false;
        this->spriteInfo = spriteInfo;
        this->requiresClickOnGameArea = requiresClickOnGameArea;
        this->keyboardShortcut = keyboardShortcut;
    }
}