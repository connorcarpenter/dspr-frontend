//
// Created by connor on 9/12/18.
//

#include "UnitTemplate.h"

namespace DsprFrontend {

    UnitTemplate::UnitTemplate(int index) {
        this->index = index;
        this->sprCenterAdjust = New<Point>(0,0);
    }

    void UnitTemplate::setWalkSpeed(int straightTicks, int diagTicks) {
        if (straightTicks == 0 || diagTicks == 0) return;
        this->walkMax = straightTicks * diagTicks;
        this->walkSpeedStraight = this->walkMax / straightTicks;
        this->walkSpeedDiagonal = this->walkMax / diagTicks;
    }
}