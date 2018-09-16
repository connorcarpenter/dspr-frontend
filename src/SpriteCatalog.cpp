//
// Created by connor on 8/29/18.
//

#include "SpriteCatalog.h"

namespace DsprFrontend
{
    SpriteCatalog::SpriteCatalog()
    {
        this->workerWalkDown = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerDown.png"), 15, 20, 1, 7, 18);
        this->workerWalkUp = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerUp.png"), 15, 20, 1, 7, 18);

        this->workerWalkDownTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/workerDown_TC.png"), 14, 11, 1, 7, 13);
        this->workerWalkUpTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/workerUp_TC.png"), 14, 11, 1, 7, 13);

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

        this->workerDieDown = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerDieDown.png"), 18, 16, 1, 8, 14);
        this->workerDieUp = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/workerDieUp.png"), 19, 16, 1, 9, 13);

        this->workerDieDownTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/workerDieDown_TC.png"), 11, 14, 1, 5, 13);
        this->workerDieUpTC = New<AnimatedSpriteInfo>(New<Sova::String>("images/worker/TC/workerDieUp_TC.png"), 11, 16, 1, 6, 13);

        this->temple = New<AnimatedSpriteInfo>(New<Sova::String>("images/temple.png"), 100, 84, 1, 50, 76);

        this->bloodParticle = New<AnimatedSpriteInfo>(New<Sova::String>("images/bloodParticle.png"), 5, 5, 1, 2, 2);
    }
}
