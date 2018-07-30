//
// Created by connor on 7/26/18.
//

#include <Sova/Graphics/AnimatedSprite.h>
#include <Sova/Graphics/Rectangle.h>

namespace DsprFrontend
{
    class Cursor : public Sova::AnimatedSprite
    {
    public:
        Cursor();

        void changeState(int index);

        Ref<Point> getTilePosition();

    private:
        void step();

        bool leftButtonDragging = false;

        Ref<Point> leftButtonDragPoint = Null<Point>();

        Ref<Rectangle> selectionBox = Null<Rectangle>();

        void drawSelf(Ref<Camera> camera, int xoffset, int yoffset) override;
    };
}
