#pragma once

//
// Created by connor on 10/28/18.
//

#include <Sova/References/Refable.h>
#include <Sova/References/Ref.h>
#include "SpecificUnit.h"

namespace DsprFrontend {
    class Unit;
    class TempleFlying : public SpecificUnit {
    public:
        TempleFlying(Unit* masterUnit);
        void stepFunction() override;
        void specialAction(int actionIndex) override;
    private:
        Unit* masterUnit = nullptr;
        int hoverBuildup = 1;
        bool hoverUp = true;
        int hoverStep = 0;
    };
}