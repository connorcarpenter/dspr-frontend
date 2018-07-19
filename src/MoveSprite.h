#pragma once

//
// Created by connor on 7/13/18.
//

#include <Sova/Sprite.h>

namespace DsprFrontend
{
    class MoveSprite : public Sova::Sprite
    {
    public:
        explicit MoveSprite(Ref<App> app);
        virtual const char* getClassName() { return "MoveSprite"; }

        Ref<Point> velocity = NullRef<Point>();

        void Update();
    private:
        Ref<App> app = NullRef<App>();
    };
}