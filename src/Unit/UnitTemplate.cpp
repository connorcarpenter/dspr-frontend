//
// Created by connor on 9/12/18.
//

#include "UnitTemplate.h"

namespace DsprFrontend {

    UnitTemplate::UnitTemplate(int index) {
        this->index = index;
        this->sprCenterAdjust = New<Point>(0,0);
    }
}