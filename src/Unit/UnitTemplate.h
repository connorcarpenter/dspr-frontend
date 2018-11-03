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
        Ref<Sova::AnimatedSpriteInfo> sprWalkDown = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprWalkUp = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> sprWalkDownTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprWalkUpTC = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> sprAttackUp = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprAttackUpRight = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprAttackRight = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprAttackDownRight = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprAttackDown = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> sprAttackUpTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprAttackUpRightTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprAttackRightTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprAttackDownRightTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprAttackDownTC = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> sprDieDown = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprDieUp = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> sprDieDownTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprDieUpTC = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> sprUnitPortrait = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprUnitPortraitTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprBigPortrait = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprBigPortraitTC = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> sprSummonFront = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprSummonBack = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprSummonFrontTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprSummonBackTC = Null<Sova::AnimatedSpriteInfo>();

        Ref<Sova::AnimatedSpriteInfo> sprYieldFront = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprYieldBack = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprYieldFrontTC = Null<Sova::AnimatedSpriteInfo>();
        Ref<Sova::AnimatedSpriteInfo> sprYieldBackTC = Null<Sova::AnimatedSpriteInfo>();

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