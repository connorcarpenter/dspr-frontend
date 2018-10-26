#pragma once

//
// Created by connor on 8/16/18.
//

#include <Sova/Graphics/Container.h>
#include <Sova/Graphics/Sprite.h>
#include <Sova/Graphics/AnimatedSprite.h>
#include <Sova/Graphics/Line.h>
#include "ButtonCard.h"

namespace DsprFrontend
{
    class UiManager : public Container
    {
    public:
        UiManager();
        void Draw(Ref<Camera> camera, int xoffset, int yoffset);
        bool captureLeftClickEvent(Ref<Point> clickPoint);
        bool captureLeftClickMinimapEvent(Ref<Point> clickPoint);
        Ref<Button> getButtonWithLeftClick(Ref<Point> clickPoint);

        Ref<Point> getMinimapPosition(Ref<Point> clickPoint);

        Ref<ButtonCard> currentButtonCard = Null<ButtonCard>();

        bool isInGameArea(Ref<Point> clickPoint);

        Ref<Button> getButtonFromKeyboardShortcut();

    private:
        Ref<Sprite> mySprite = Null<Sprite>();
        Ref<AnimatedSprite> myAnimatedSprite = Null<AnimatedSprite>();
        Ref<Sova::Line> healthBarLine = Null<Sova::Line>();
    };
}