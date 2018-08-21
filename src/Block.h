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
        Ref<Point> tilePosition = Null<Point>();
    private:

        void step();
    };
}