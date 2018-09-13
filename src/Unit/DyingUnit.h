#pragma once

//
// Created by connor on 9/5/18.
//
#include <Sova/Graphics/AnimatedSprite.h>

namespace DsprFrontend {
    class Unit;

    class DyingUnit : public Sova::AnimatedSprite
    {
        SovaTypeDecl(DyingUnit, AnimatedSprite);
    public:
        DyingUnit(Ref<Unit> unit);
        void drawSelf(Ref<Camera> camera, int xoffset, int yoffset) override;
    private:
        Ref<AnimatedSprite> tcSprite = Null<AnimatedSprite>();
        int disappearTimer;
    };
}