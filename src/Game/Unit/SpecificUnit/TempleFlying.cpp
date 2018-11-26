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
    }

    void TempleFlying::specialAction(int actionIndex) {
        int i = 0;
    }
}
