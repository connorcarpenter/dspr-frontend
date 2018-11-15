#pragma once

//
// Created by connor on 8/16/18.
//

#include <Sova/Graphics/Container.h>
#include <Sova/Graphics/Sprite.h>
#include <Sova/Graphics/AnimatedSprite.h>
#include <Sova/Graphics/Line.h>
#include <Sova/Graphics/Rectangle.h>
#include "ButtonCard.h"

namespace DsprFrontend
{
    class Unit;
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

        bool rightButtonAlreadyClicked = false;

        void handleItemBarClick(Ref<Point> clickPoint);

    private:

        void step();
        void centerCameraOnUnit(Ref<Unit> unit);

        Ref<Sprite> mySprite = Null<Sprite>();
        Ref<AnimatedSprite> myAnimatedSprite = Null<AnimatedSprite>();
        Ref<Sova::Line> myLine = Null<Sova::Line>();
        Ref<Sova::Rectangle> myRectangle = Null<Sova::Rectangle>();

        float currentFps = 60.0f;

        //consts
        const int itemBarX = 160;
        const int itemBarY = 109;
        const int itemBarW = 44;
        const int itemBarH = 35;
        const int armyBarX = 90;
        const int armyBarY = 109;
        const int armyBarW = 70;
        const int armyBarH = 35;
        const int portraitBarX = 48;
        const int portraitBarY = 109;
        const int portraitBarW = 42;
        const int portraitBarH = 35;
        const int bigPortraitX = 59;
        const int bigPortraitY = 111;
        const int bigPortraitW = 20;
        const int bigPortraitH = 19;
        const int cameraSpeed = 2;
    };
}