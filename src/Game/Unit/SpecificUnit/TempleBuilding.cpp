//
// Created by connor on 10/28/18.
//

#include <Sova/Math/Math.h>
#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include "TempleBuilding.h"
#include "Game/Unit/Unit.h"
#include "SpriteCatalog.h"

namespace DsprFrontend
{
    TempleBuilding::TempleBuilding(Unit* masterUnit) {
        this->masterUnit = masterUnit;
    }

    void TempleBuilding::stepFunction() {
        if (imageSpeed != 0)
        {
            this->masterUnit->imageIndex += imageSpeed;
            if (this->masterUnit->imageIndex >= this->masterUnit->imageNumber)
            {
                this->masterUnit->imageIndex = this->masterUnit->imageNumber-1;
                this->imageSpeed = 0;
            }
        }
    }

    void TempleBuilding::specialAction(int actionIndex) {
        switch(actionIndex)
        {
            case 0:
            {
                auto g = (Global*) InternalApp::getGlobal();
                this->masterUnit->useAnimatedSequenceInfo(g->spriteCatalog->templeBuildingToGround);
                this->imageSpeed = 0.2f;
                this->masterUnit->imageIndex = 0;
            }
                break;
            default:
            {
                int i = 1/0; // ?
            }
                break;
        }
    }
}
