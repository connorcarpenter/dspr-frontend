#pragma once

//
// Created by connor on 9/6/18.
//

#include <Sova/Graphics/AnimatedSprite.h>
#include <Sova/Math/FloatPoint.h>

namespace DsprFrontend {
    class BloodParticle : public Sova::AnimatedSprite {
        SovaTypeDecl(BloodParticle, AnimatedSprite);
    public:
        BloodParticle(Ref<Point> position, float z, int depth);
        void drawSelf(Ref<Camera> camera, int xoffset, int yoffset) override;
    private:
        float z;
        float zVelocity = 0;
        const float zGravity = 0.1f;
        const float friction = 1.0f - 0.125f;
        int disappearTimer = 1000;

        Ref<FloatPoint> velocity = Null<FloatPoint>();
        Ref<FloatPoint> floatPos = Null<FloatPoint>();
    };
}