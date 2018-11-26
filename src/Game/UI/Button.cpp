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

    void Button::setFinalAction(std::function<void()> actionFunction)
    {
        this->finalAction = actionFunction;
    }

    void Button::executeFinalAction()
    {
        if (this->finalAction != nullptr){
            this->finalAction();
        }
    }

    void Button::setBeginAction(std::function<void()> actionFunction)
    {
        this->beginAction = actionFunction;
    }

    void Button::executeBeginAction()
    {
        if (this->beginAction != nullptr){
            this->beginAction();
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