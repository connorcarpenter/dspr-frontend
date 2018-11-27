#pragma once

//
// Created by connor on 10/28/18.
//

#include <Sova/References/Refable.h>
#include <Sova/References/Ref.h>
#include "SpecificUnit.h"

namespace DsprFrontend {
    class Unit;
    class TempleBuilding : public SpecificUnit {
    public:
        TempleBuilding(Unit* masterUnit);
        void stepFunction() override;
        void specialAction(int actionIndex) override;
    private:
        Unit* masterUnit = nullptr;
        float imageSpeed = 0;
    };
}