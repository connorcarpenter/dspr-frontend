//
// Created by connor on 8/16/18.
//

#include <Sova/Graphics/Sprite.h>

namespace DsprFrontend
{
    class Block : public Sova::Sprite
    {
    public:
        Block(int x, int y);
        void drawSelf(Ref<Camera> camera, int xoffset, int yoffset) override;

        Ref<Point> tilePosition = Null<Point>();
    };
}