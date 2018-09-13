#pragma once

//
// Created by connor on 8/29/18.
//

#include <Sova/References/Ref.h>
#include <Sova/References/Refable.h>
#include <Sova/Graphics/AnimatedSpriteInfo.h>

namespace DsprFrontend {
    class SpriteCatalog : public Sova::Refable {
    public:
        SpriteCatalog();
        Ref<Sova::AnimatedSpriteInfo> workerWalkDown = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerWalkUp = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> workerWalkDownTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerWalkUpTC = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> workerAttackUp = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerAttackUpRight = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerAttackRight = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerAttackDownRight = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerAttackDown = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> workerAttackUpTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerAttackUpRightTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerAttackRightTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerAttackDownRightTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerAttackDownTC = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> workerDieDown = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerDieUp = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> workerDieDownTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerDieUpTC = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> bloodParticle = Null<Sova::AnimatedSpriteInfo>();
    };
}