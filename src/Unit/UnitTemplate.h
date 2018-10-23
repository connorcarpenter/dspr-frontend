#pragma once

//
// Created by connor on 9/12/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/AnimatedSpriteInfo.h>
#include <Sova/Audio/Sound.h>

namespace DsprFrontend {
    class UnitTemplate : public Sova::Refable {
    public:
        UnitTemplate(int index);
        int index;

        //stats
        int sight;
        unsigned tileWidth = 1;
        unsigned tileHeight = 1;

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

        //sounds
        Ref<Sova::Sound> hitSound = Null<Sova::Sound>();
        Ref<Sova::Sound> selectedSound = Null<Sova::Sound>();
        Ref<Sova::Sound> dieSound = Null<Sova::Sound>();

        //ui
        Ref<Sova::Sprite> sprSelectCircle = Null<Sova::Sprite>();
        Ref<Sova::AnimatedSprite> sprHoverCircle = Null<Sova::AnimatedSprite>();

        //other
        bool hasIdleTurnBehavior = false;
        bool canMove = true;
        short spriteFaceLeftXoffset = 0;
    };
}