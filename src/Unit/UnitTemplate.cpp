//
// Created by connor on 9/12/18.
//

#include <Global.h>
#include <Sova/Internal/InternalApp.h>
#include "UnitTemplate.h"
#include "SpriteCatalog.h"

namespace DsprFrontend {

    UnitTemplate::UnitTemplate(int index) {
        this->index = index;

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        
        switch(this->index)
        {
            case 0:
            {
                this->sprWalkDown = g->spriteCatalog->workerWalkDown;
                this->sprWalkUp = g->spriteCatalog->workerWalkUp;

                this->sprWalkDownTC = g->spriteCatalog->workerWalkDownTC;
                this->sprWalkUpTC = g->spriteCatalog->workerWalkUpTC;

                this->sprAttackUp = g->spriteCatalog->workerAttackUp;
                this->sprAttackUpRight = g->spriteCatalog->workerAttackUpRight;
                this->sprAttackRight = g->spriteCatalog->workerAttackRight;
                this->sprAttackDownRight = g->spriteCatalog->workerAttackDownRight;
                this->sprAttackDown = g->spriteCatalog->workerAttackDown;

                this->sprAttackUpTC = g->spriteCatalog->workerAttackUpTC;
                this->sprAttackUpRightTC = g->spriteCatalog->workerAttackUpRightTC;
                this->sprAttackRightTC = g->spriteCatalog->workerAttackRightTC;
                this->sprAttackDownRightTC = g->spriteCatalog->workerAttackDownRightTC;
                this->sprAttackDownTC = g->spriteCatalog->workerAttackDownTC;

                this->sprDieDown = g->spriteCatalog->workerDieDown;
                this->sprDieUp = g->spriteCatalog->workerDieUp;

                this->sprDieDownTC = g->spriteCatalog->workerDieDownTC;
                this->sprDieUpTC = g->spriteCatalog->workerDieUpTC;
            }
                break;

            case 1:
            {
                this->sprWalkDown = g->spriteCatalog->temple;
            }
                break;
        }
    }
}