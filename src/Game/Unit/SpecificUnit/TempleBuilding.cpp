//
// Created by connor on 10/28/18.
//

#include <Sova/Math/Math.h>
#include "TempleBuilding.h"
#include "Game/Unit/Unit.h"

namespace DsprFrontend
{
    TempleBuilding::TempleBuilding(Unit* masterUnit) {
        this->masterUnit = masterUnit;
    }

    void TempleBuilding::stepFunction() {
    }

    void TempleBuilding::specialAction(int actionIndex) {
        switch(actionIndex)
        {
            case 0:
            {

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
