#pragma once

//
// Created by connor on 10/29/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/AnimatedSprite.h>
#include <Sova/Math/FloatPoint.h>

namespace DsprFrontend
{
    class Sparkle : public Sova::AnimatedSprite
    {
    public:

        Sparkle(Ref<Point> position);

        int ttl = 30;
    private:

        void step(float deltaMs);
    };
}