#pragma once

//
// Created by connor on 10/29/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/AnimatedSprite.h>
#include <Sova/Math/FloatPoint.h>

namespace DsprFrontend
{
    class FloatingNumber : public Sova::Container
    {
    public:

        FloatingNumber(Ref<Point> position, Color color, int number);
        void drawSelf(Ref<Camera> camera, int xoffset, int yoffset) override;

        Ref<FloatPoint> floatPosition = Null<FloatPoint>();
        float vspeed = 0;

        int ttl = 30;
    private:

        void step(float deltaMs);
        int number;
        Color color;
    };
}