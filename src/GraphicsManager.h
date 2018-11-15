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

        int drawText(Ref<Camera> camera, int x, int y, Ref<String> str, Color color, bool alignLeft);

        void drawNumber(Ref<Camera> camera, int x, int y, int number, Color color, bool alignLeft);

        void drawChar(Ref<Camera> camera, int x, int y, char c, Color color);

    private:

        Ref<AnimatedSprite> myFont = Null<AnimatedSprite>();

        static const int letterWidth(int index){
            ////////////////////////////////////////////////////////////////////////////////0
            static const int letterWidth[] = { 1, 3, 5, 5, 5, 5, 3, 2, 2, 3, 3, 2, 3, 2, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
            //////////////A/////////////////////////////////////////////////Z
            1,1,3,3,3,4,5,4,4,3,4,3,3,4,4,3,4,4,3,5,4,4,4,5,4,3,3,4,3,5,3,4,3,
            ////////////a/////////////////////////////////////////////////z
            2,5,2,5,5,3,4,4,3,4,4,3,4,4,1,2,3,2,5,4,4,3,3,3,3,3,4,3,5,3,4,3,
            //////~
            3,2,3,5
            };
            return letterWidth[index];
        }
    };
}