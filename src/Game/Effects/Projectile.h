#pragma once

//
// Created by connor on 10/29/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Math/FloatPoint.h>
namespace Sova {
    class Rectangle;
}

namespace DsprFrontend
{
    class Projectile : public Sova::Container
    {
    public:

        Projectile(int fromX, int fromY, int toX, int toY, int index);
        void drawSelf(Ref<Camera> camera, int xoffset, int yoffset) override;

    private:

        void step(float deltaMs);

        Ref<Rectangle> rectangle = Null<Rectangle>();
        float distance;
        int speed;
        float maxDistance;
        Ref<Point> from = Null<Point>();
        Ref<Point> to = Null<Point>();
    };
}