#pragma once

//
// Created by connor on 11/8/18.
//

#include <Sova/Graphics/AnimatedSprite.h>

namespace DsprFrontend {
    class Item : public Sova::AnimatedSprite {
    public:
        Item(int id, int x, int y, int index);
        Ref<Point> tilePosition = Null<Point>();
        int id = -1;
        int index = -1;
    };
}