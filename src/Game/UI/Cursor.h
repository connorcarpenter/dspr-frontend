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
    class Item;

    class Cursor : public Sova::AnimatedSprite
    {

    public:

        Cursor();
        void handleItemClicked(Ref<Unit> unit, int itemIndex, int slotIndex);
        bool isItemInHand();
        void handleItemPutSlot(Ref<Unit> unit, int slotIndex);

        bool leftButtonDragging = false;
        Ref<Point> leftButtonDragPoint = Null<Point>();
        Ref<Point> worldPosition = Null<Point>();
        Ref<Button> buttonOrder = Null<Button>();

        Ref<Unit> itemInHandOwner = Null<Unit>();
        int itemInHandSlotIndex = -1;
    private:

        void step();
        void drawSelf(Ref<Camera> camera, int xoffset, int yoffset) override;
        void clearHoverList();
        void setHoverListUnitsToHover(bool i);
        void setHoverListUnitsToSelected(bool selected, bool toggle = false);
        void resetItemInHand();
        void undoItemInHandGraphic();

        Ref<Rectangle> selectionBox = Null<Rectangle>();
        Ref<List<Unit>> hoverList = Null<List<Unit>>();

        int leftButtonPressedTime = 0;
        const int leftButtonPressedTimeToDrag = 6;

        int leftButtonDoubleClickCountdown = 0;
        const int leftButtonDoubleClickWindow = 20;

        bool cursorIsHovering = false;
        int ignoreNextLeftButtonClicked = 0;

        Ref<Item> lastHoveringItem = Null<Item>();
        Ref<Item> lastSelectedItem = Null<Item>();
        int itemInHandIndex = -1;
    };
}
