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

        //worker
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

        Ref<Sova::AnimatedSpriteInfo> workerUnitPortrait = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerUnitPortraitTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerBigPortrait = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerBigPortraitTC = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> workerSummonFront = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerSummonFrontTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerSummonBack = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerSummonBackTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerYieldFront = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerYieldFrontTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerYieldBack = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerYieldBackTC = Null<Sova::AnimatedSpriteInfo>();

        //temple
        Ref<Sova::AnimatedSpriteInfo> temple = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> templeTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> templeUnitPortrait = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> templeUnitPortraitTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> templeBigPortrait = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> templeBigPortraitTC = Null<Sova::AnimatedSpriteInfo>();

        //other
        Ref<Sova::AnimatedSpriteInfo> bloodParticle = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> manafount = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> manaball = Null<Sova::AnimatedSpriteInfo>();

        //ui
        Ref<Sova::AnimatedSpriteInfo> sprMainFont = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprMinimap = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprArmyBar = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprCommandBar = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprResourceBar = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprCommandActions = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprItemBar = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprPortraitBar = Null<Sova::AnimatedSpriteInfo>();
    };
}