//
// Created by connor on 7/26/18.
//

#include <Sova/Graphics/AnimatedSprite.h>
#include <Sova/Graphics/Rectangle.h>

namespace DsprFrontend
{
    class Unit;

    class Cursor : public Sova::AnimatedSprite
    {
    public:
        Cursor();

        void setImageIndex(int index);

        Ref<Point> getTilePosition();

        bool leftButtonDragging = false;
        Ref<Point> leftButtonDragPoint = Null<Point>();
        Ref<Point> worldPosition = Null<Point>();

    private:

        void step();
        void drawSelf(Ref<Camera> camera, int xoffset, int yoffset) override;

        Ref<Rectangle> selectionBox = Null<Rectangle>();
        Ref<List<Unit>> hoverList = Null<List<Unit>>();

        int leftButtonPressedTime = 0;
        const int leftButtonPressedTimeToDrag = 6;

        void clearHoverList();

        void setHoverListUnitsToHover(bool i);

        void setHoverListUnitsToSelected(bool selected);

        void toggleHoverListUnitsSelected();

        int leftButtonDoubleClickCountdown = 0;
        const int leftButtonDoubleClickWindow = 20;
    };
}
