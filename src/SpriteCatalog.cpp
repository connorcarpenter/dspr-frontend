//
// Created by connor on 8/29/18.
//

#include "SpriteCatalog.h"

namespace DsprFrontend
{
    SpriteCatalog::SpriteCatalog()
    {
        this->workerBase            = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/sprWorkerBase.png"), 19, 22, 1, 9, 18);
        this->workerTC              = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/sprWorkerTC.png"), 19, 22, 1, 9, 18);
        this->workerSkin            = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/sprWorkerSkin.png"), 19, 22, 1, 9, 18);
        this->workerHairShort       = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/sprWorkerHairShort.png"), 19, 22, 1, 9, 18);
        this->workerHairLong        = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/sprWorkerHairLong.png"), 19, 22, 1, 9, 18);

        this->workerArmor        = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/sprWorkerArmor.png"), 19, 22, 1, 9, 18);
        
        this->workerWalkDown        = New<AnimatedSequenceInfo>(this->workerBase, 0, 1);
        this->workerWalkUp          = New<AnimatedSequenceInfo>(this->workerBase, 2, 3);
        this->workerAttackUp        = New<AnimatedSequenceInfo>(this->workerBase, 4, 8);
        this->workerAttackUpRight   = New<AnimatedSequenceInfo>(this->workerBase, 9, 13);
        this->workerAttackRight     = New<AnimatedSequenceInfo>(this->workerBase, 14, 18);
        this->workerAttackDownRight = New<AnimatedSequenceInfo>(this->workerBase, 19, 23);
        this->workerAttackDown      = New<AnimatedSequenceInfo>(this->workerBase, 24, 28);
        this->workerDieDown         = New<AnimatedSequenceInfo>(this->workerBase, 29, 30);
        this->workerDieUp           = New<AnimatedSequenceInfo>(this->workerBase, 31, 32);
        this->workerSummonFront     = New<AnimatedSequenceInfo>(this->workerBase, 33, 36);
        this->workerSummonBack      = New<AnimatedSequenceInfo>(this->workerBase, 37, 40);
        this->workerYieldFront      = New<AnimatedSequenceInfo>(this->workerBase, 41, 44);
        this->workerYieldBack       = New<AnimatedSequenceInfo>(this->workerBase, 45, 48);

        this->workerUnitPortrait = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerPortrait.png"), 10, 12, 0, 0, 0);
        this->workerUnitPortraitTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerPortrait_TC.png"), 10, 4, 0, 0, -8);
        this->workerBigPortrait = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/worker_bigPortrait.png"), 29, 29, 0, 0, 0);
        this->workerBigPortraitTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/worker_bigPortrait_TC.png"), 29, 9, 0, 0, -20);

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
