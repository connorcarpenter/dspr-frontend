#pragma once

//
// Created by connor on 7/13/18.
//

#include <Sova/Graphics/Sprite.h>

namespace DsprFrontend
{
    class MoveSprite : public Sova::Sprite
    {
    public:
        explicit MoveSprite(Ref<App> app);
        virtual const char* getClassName() { return "MoveSprite"; }

        Ref<Point> velocity = Null<Point>();

        void Update();
    private:
        Ref<App> app = Null<App>();
    };
}