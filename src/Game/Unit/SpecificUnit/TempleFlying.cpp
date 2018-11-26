//
// Created by connor on 10/28/18.
//

#include <Sova/Math/Math.h>
#include "TempleFlying.h"
#include "Game/Unit/Unit.h"

namespace DsprFrontend
{
    TempleFlying::TempleFlying(Unit* masterUnit) {
        this->masterUnit = masterUnit;
    }

    void TempleFlying::stepFunction() {
        if (hoverStep < 40)
        {
            hoverStep++;
        }
            else
        {
            hoverStep = 0;
            if (hoverBuildup < 2)
            {
                this->masterUnit->z += (hoverUp) ? -1 : 1;
                hoverBuildup += 1;
            }
            else
            {
                hoverUp = !hoverUp;
                hoverBuildup = 0;
                hoverStep = -40;
            }
        }
    }

    void TempleFlying::specialAction(int actionIndex) {
        int i = 0;
    }
}
