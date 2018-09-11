#pragma once

//
// Created by connor on 7/26/18.
//

#include <Sova/Graphics/AnimatedSprite.h>
#include <Sova/Graphics/Rectangle.h>

namespace Sova
{
    class Sound;
}

namespace DsprFrontend
{
    class Unit;
    class Button;

    class Cursor : public Sova::AnimatedSprite
    {
    public:
        Cursor();

        Ref<Point> getTilePosition();

        bool leftButtonDragging = false;
        Ref<Point> leftButtonDragPoint = Null<Point>();
        Ref<Point> worldPosition = Null<Point>();

        Ref<Button> buttonOrder = Null<Button>();
    private:

        void step();
        void drawSelf(Ref<Camera> camera, int xoffset, int yoffset) override;
        void clearHoverList();
        void setHoverListUnitsToHover(bool i);
        void setHoverListUnitsToSelected(bool selected);
        void toggleHoverListUnitsSelected();

        Ref<Rectangle> selectionBox = Null<Rectangle>();
        Ref<List<Unit>> hoverList = Null<List<Unit>>();

        Ref<Sova::Sound> helloSound = Null<Sova::Sound>();

        int leftButtonPressedTime = 0;
        const int leftButtonPressedTimeToDrag = 6;

        int leftButtonDoubleClickCountdown = 0;
        const int leftButtonDoubleClickWindow = 20;

        bool cursorIsHovering = false;
        int ignoreNextLeftButtonClicked = 0;
    };
}
