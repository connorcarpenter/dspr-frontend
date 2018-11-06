#pragma once

//
// Created by connor on 9/12/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/AnimatedSpriteInfo.h>
#include <Sova/Audio/Sound.h>
#include <Sova/Graphics/Sprite.h>
#include <Sova/Graphics/AnimatedSprite.h>
#include <UI/ButtonCard.h>
#include <Unit/SpecificUnit/SpecificUnit.h>
#include <Sova/Graphics/AnimatedSequenceInfo.h>

namespace DsprFrontend {
    class Unit;
    class UnitTemplate : public Sova::Refable {
    public:
        UnitTemplate(int index);
        int index;

        //stats
        int sight;
        int maxHealth;
        unsigned int tileWidth = 1;
        unsigned int tileHeight = 1;
        int buildTime;
        Ref<Sova::Point> sprCenterAdjust = Null<Sova::Point>();

        //other
        bool hasIdleTurnBehavior = false;
        bool canMove = true;
        short spriteFaceLeftXoffset = 0;
        bool bleeds;
        bool hasDeathAnimation;
        bool hasInventory = false;
        Ref<ButtonCard> commandCard = Null<ButtonCard>();
        bool hasConstructionQueue = false;
        bool isGatherable = false;
        bool canGather = false;

        //sprites
        Ref<Sova::AnimatedSpriteInfo> sprBase = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprSkin = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprHairShort = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprHairLong = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprArmor = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSequenceInfo> sprWalkDown = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> sprWalkUp = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> sprAttackUp = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> sprAttackUpRight = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> sprAttackRight = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> sprAttackDownRight = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> sprAttackDown = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> sprDieDown = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> sprDieUp = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> sprSummonFront = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> sprSummonBack = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> sprYieldFront = Null<Sova::AnimatedSequenceInfo>();
        Ref<Sova::AnimatedSequenceInfo> sprYieldBack = Null<Sova::AnimatedSequenceInfo>();

        Ref<Sova::AnimatedSpriteInfo> sprUnitPortrait = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprBigPortrait = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprUnitPortraitTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprBigPortraitTC = Null<Sova::AnimatedSpriteInfo>();

        //sounds
        Ref<Sova::Sound> hitSound = Null<Sova::Sound>();
        Ref<Sova::Sound> selectedSound = Null<Sova::Sound>();
        Ref<Sova::Sound> dieSound = Null<Sova::Sound>();
        Ref<Sova::Sound> readySound = Null<Sova::Sound>();

        //ui
        Ref<Sova::Sprite> sprSelectCircle = Null<Sova::Sprite>();
        Ref<Sova::AnimatedSprite> sprHoverCircle = Null<Sova::AnimatedSprite>();

        //special
        std::function<Ref<SpecificUnit>(Unit* unit)> createSpecificUnitFunction = nullptr;
    };
}