#pragma once

//
// Created by connor on 9/10/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/Color.h>
#include <functional>
#include <Sova/Input.h>
#include <Sova/Graphics/AnimatedSpriteInfo.h>

using namespace Sova;

namespace DsprFrontend {
    class Button : public Sova::Refable {
    public:
        Button(Ref<AnimatedSpriteInfo> spriteInfo, int imageIndex, bool requiresClickOnGameArea,
                       Key::Code keyboardShortcut);

        Button(Ref<AnimatedSpriteInfo> spriteInfo, bool requiresClickOnGameArea,
               Key::Code keyboardShortcut);

        void executeFinalAction();
        void setFinalAction(std::function<void()> actionFunction);

        void executeBeginAction();
        void setBeginAction(std::function<void()> actionFunction);

        bool evalConditionalFunc();
        void setConditionalFunc(std::function<bool()> actionFunction);

        int imageIndex;
        bool requiresClickOnGameArea = false;
        Key::Code keyboardShortcut;
        bool needKeyUp = false;
        bool usesSubframes = false;
        bool usesTeamColor = false;
        Ref<AnimatedSpriteInfo> spriteInfo = Null<AnimatedSpriteInfo>();
        Ref<AnimatedSpriteInfo> tcSpriteInfo = Null<AnimatedSpriteInfo>();
    private:
        std::function<void()> finalAction = nullptr;
        std::function<void()> beginAction = nullptr;
        std::function<bool()> conditionalFunc = nullptr;
    };
}