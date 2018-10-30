#pragma once

//
// Created by connor on 10/29/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/AnimatedSprite.h>
#include <Sova/Math/FloatPoint.h>

namespace DsprFrontend {
    class Manaball : public Sova::AnimatedSprite {
    public:
        Manaball(Ref<Point> position, int size);
        Ref<FloatPoint> floatPosition = Null<FloatPoint>();
        Ref<Point> moveToPosition = Null<Point>();
        Ref<Manaball> moveToManaball = Null<Manaball>();
        int size = 1;
        bool destroyAfterArrival = false;
        float imgIndex = 0.0f;
        float imgSpeed = 0.2f;
        float speed = 1.0f;
    private:
        void step(float deltaMs);
    };
}