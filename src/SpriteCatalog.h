#pragma once

//
// Created by connor on 8/29/18.
//

#include <Sova/References/Ref.h>
#include <Sova/References/Refable.h>
#include <Sova/Graphics/AnimatedSpriteInfo.h>
#include <Sova/Graphics/AnimatedSequenceInfo.h>

namespace DsprFrontend {
    class SpriteCatalog : public Sova::Refable {
    public:
        SpriteCatalog();

        //worker
        Ref<Sova::AnimatedSpriteInfo> workerBase = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerSkin = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerHairShort = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerHairLong = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerArmor = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerHelmet = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerClub = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerShield = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerSling = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSequenceInfo> workerWalkDown = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerWalkUp = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerDieDown = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerDieUp = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerSummonFront = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerSummonBack = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerYieldFront = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerYieldBack = Null<Sova::AnimatedSequenceInfo>();

        Ref<Sova::AnimatedSequenceInfo> workerAttackUp2HS = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerAttackUpRight2HS = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerAttackRight2HS = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerAttackDownRight2HS = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerAttackDown2HS = Null<Sova::AnimatedSequenceInfo>();

        Ref<Sova::AnimatedSequenceInfo> workerAttackUp1HS = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerAttackUpRight1HS = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerAttackRight1HS = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerAttackDownRight1HS = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> workerAttackDown1HS = Null<Sova::AnimatedSequenceInfo>();

        Ref<Sova::AnimatedSpriteInfo> workerUnitPortrait = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerUnitPortraitTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerBigPortrait = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> workerBigPortraitTC = Null<Sova::AnimatedSpriteInfo>();

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
        Ref<Sova::AnimatedSpriteInfo> itemsGround = Null<Sova::AnimatedSpriteInfo>();

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