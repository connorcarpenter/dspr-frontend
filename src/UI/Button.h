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

        void executeAction();
        void setAction(std::function<void()> actionFunction);

        int imageIndex;
        bool requiresClickOnGameArea = false;
        Key::Code keyboardShortcut;
        bool needKeyUp = false;
        bool usesSubframes = false;
        bool usesTeamColor = false;
        Ref<AnimatedSpriteInfo> spriteInfo = Null<AnimatedSpriteInfo>();
        Ref<AnimatedSpriteInfo> tcSpriteInfo = Null<AnimatedSpriteInfo>();
    private:
        std::function<void()> action = nullptr;
    };
}