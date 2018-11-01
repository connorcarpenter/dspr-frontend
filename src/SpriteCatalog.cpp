//
// Created by connor on 8/29/18.
//

#include "SpriteCatalog.h"

namespace DsprFrontend
{
    SpriteCatalog::SpriteCatalog()
    {
        this->workerWalkDown = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerDown.png"), 15, 20, 1, 7, 17);
        this->workerWalkUp = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerUp.png"), 15, 20, 1, 7, 17);

        this->workerWalkDownTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/workerDown_TC.png"), 14, 11, 1, 7, 12);
        this->workerWalkUpTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/workerUp_TC.png"), 14, 11, 1, 7, 12);

        this->workerAttackUp = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerAttackUp.png"), 18, 30, 1, 13, 25);
        this->workerAttackUpRight = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerAttackUpRight.png"), 30, 26, 1, 15, 25);
        this->workerAttackRight = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerAttackRight.png"), 27, 24, 1, 12, 22);
        this->workerAttackDownRight = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerAttackDownRight.png"), 20, 25, 1, 6, 22);
        this->workerAttackDown = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerAttackDown.png"), 17, 33, 1, 4, 24);

        this->workerAttackUpTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/workerAttackUp_TC.png"), 11, 14, 1, 7, 16);
        this->workerAttackUpRightTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/workerAttackUpRight_TC.png"), 14, 13, 1, 7, 16);
        this->workerAttackRightTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/workerAttackRight_TC.png"), 12, 12, 1, 4, 15);
        this->workerAttackDownRightTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/workerAttackDownRight_TC.png"), 11, 11, 1, 4, 14);
        this->workerAttackDownTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/workerAttackDown_TC.png"), 10, 15, 1, 4, 16);

        this->workerSummonFront = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/worker_summon_front.png"), 12, 19, 1, 3, 16);
        this->workerSummonFrontTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/worker_summon_front_TC.png"), 11, 10, 1, 3, 12);
        this->workerSummonBack = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/worker_summon_back.png"), 12, 18, 1, 3, 16);
        this->workerSummonBackTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/worker_summon_back_TC.png"), 11, 10, 1, 3, 12);
        this->workerYieldFront = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/worker_yield_front.png"), 18, 21, 1, 8, 18);
        this->workerYieldFrontTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/worker_yield_front_TC.png"), 14, 14, 1, 6, 16);
        this->workerYieldBack = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/worker_yield_back.png"), 18, 20, 1, 8, 18);
        this->workerYieldBackTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/worker_yield_back_TC.png"), 14, 14, 1, 6, 16);

        this->workerDieDown = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerDieDown.png"), 18, 16, 1, 8, 14);
        this->workerDieUp = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerDieUp.png"), 19, 16, 1, 9, 13);

        this->workerDieDownTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/workerDieDown_TC.png"), 11, 14, 1, 5, 13);
        this->workerDieUpTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/workerDieUp_TC.png"), 11, 16, 1, 6, 13);

        this->workerUnitPortrait = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerPortrait.png"), 10, 12, 0, 0, 0);
        this->workerUnitPortraitTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/workerPortrait_TC.png"), 10, 4, 0, 0, -8);
        this->workerBigPortrait = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/worker_bigPortrait.png"), 29, 29, 0, 0, 0);
        this->workerBigPortraitTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/worker_bigPortrait_TC.png"), 29, 9, 0, 0, -20);

        this->temple = New<AnimatedSpriteInfo>(New<Sova::String>("images/temple/temple.png"), 100, 84, 1, 50, 57);
        this->templeTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/temple/temple_TC.png"), 100, 65, 1, 50, 59);
        this->templeUnitPortrait = New<AnimatedSpriteInfo>(New<Sova::String>("images/temple/temple_unitPortrait.png"), 10, 12, 0, 0, 0);
        this->templeUnitPortraitTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/temple/temple_unitPortrait_TC.png"), 10, 8, 0, 0, 0);
        this->templeBigPortrait = New<AnimatedSpriteInfo>(New<Sova::String>("images/temple/temple_bigUnitPortrait.png"), 29, 29, 0, 0, 0);
        this->templeBigPortraitTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/temple/temple_bigUnitPortrait_TC.png"), 29, 20, 0, 0, 0);

        this->bloodParticle = New<AnimatedSpriteInfo>(New<Sova::String>("images/bloodParticle.png"), 5, 5, 1, 2, 2);

        this->sprMinimap = New<AnimatedSpriteInfo>(New<Sova::String>("images/ui/minimap.png"), true, 0, 0);
        this->sprPortraitBar = New<AnimatedSpriteInfo>(New<Sova::String>("images/ui/portraitbar.png"), true, 0, 0);
        this->sprItemBar = New<AnimatedSpriteInfo>(New<Sova::String>("images/ui/itembar.png"), 44, 35, 0, 0, 0);
        this->sprArmyBar = New<AnimatedSpriteInfo>(New<Sova::String>("images/ui/armybar.png"), 70, 35, 0, 0, 0);
        this->sprCommandBar = New<AnimatedSpriteInfo>(New<Sova::String>("images/ui/commandcard.png"), true, 0, 0);
        this->sprResourceBar = New<AnimatedSpriteInfo>(New<Sova::String>("images/ui/resourcebar.png"), true, 0, 0);
        this->sprCommandActions = New<AnimatedSpriteInfo>(New<Sova::String>("images/ui/commandActions.png"), 10, 12, 0, 0, 0);
        this->sprMainFont = New<AnimatedSpriteInfo>(New<Sova::String>("images/ui/myFont.png"), 7, 7, 1, 0, 0);

        this->manafount = New<AnimatedSpriteInfo>(New<Sova::String>("images/manafount.png"), 41, 47, 1, 20, 34);
        this->manaball = New<AnimatedSpriteInfo>(New<Sova::String>("images/manaball.png"), 9,9, 1, 4, 4);
    }
}
