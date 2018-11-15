#pragma once

//
// Created by connor on 10/31/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/AnimatedSprite.h>

using namespace Sova;

namespace DsprFrontend
{
    class GraphicsManager : public Sova::Refable
    {
    public:

        GraphicsManager();

        void drawText(Ref<Camera> camera, int x, int y, Ref<String> str, Color color, bool alignLeft);

        void drawNumber(Ref<Camera> camera, int x, int y, int number, Color color, bool alignLeft);

        void drawChar(Ref<Camera> camera, int x, int y, char c, Color color);

    private:

        Ref<AnimatedSprite> myFont = Null<AnimatedSprite>();

    };
}