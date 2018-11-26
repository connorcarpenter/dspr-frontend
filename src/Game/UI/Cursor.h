#pragma once

//
// Created by connor on 7/26/18.
//

#include <Sova/Graphics/AnimatedSprite.h>
#include <Sova/Graphics/Rectangle.h>
#include <Game/Unit/UnitTemplate.h>

namespace Sova
{
    class Sound;
}

namespace DsprFrontend
{
    class Unit;
    class Button;
    class Item;
    class ItemTemplate;

    class Cursor : public Sova::AnimatedSprite
    {

    public:

        Cursor();
        void handleItemClicked(Ref<Unit> unit, Ref<ItemTemplate> itemTemplate, int slotIndex);
        bool isItemInHand();
        void handleItemPutSlot(Ref<Unit> unit, int slotIndex);
        void setItemInHandTemplate(Ref<ItemTemplate> itemTemplate);
        void beginBuildingState(Ref<UnitTemplate> buildingTemplate);
        bool isInBuildingPlacementState();
        void undoBuildingPlacementState();

        bool leftButtonDragging = false;
        Ref<Point> leftButtonDragPoint = Null<Point>();
        Ref<Point> worldPosition = Null<Point>();
        Ref<Button> buttonOrder = Null<Button>();

        Ref<Unit> itemInHandOwner = Null<Unit>();
        int itemInHandSlotIndex = -1;
        Ref<ItemTemplate> itemInHandTemplate = Null<ItemTemplate>();
        Ref<UnitTemplate> buildingStateTemplate = Null<UnitTemplate>();
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
        bool enabled = true;


    };
}
