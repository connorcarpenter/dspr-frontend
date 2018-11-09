#pragma once

//
// Created by connor on 10/28/18.
//

#include <Sova/References/Refable.h>
#include <Sova/References/Ref.h>
#include "SpecificUnit.h"

namespace DsprFrontend {
    class Unit;
    class Manafount : public SpecificUnit {
    public:
        Manafount(Unit* masterUnit);
        virtual void stepFunction();
        int gatherRate = 10;
    private:
        Unit* masterUnit = nullptr;
        unsigned short wait = 100;
        unsigned short lastAnim = 0;
        unsigned short lastLastAnim = 0;
        unsigned short baseFrame = 0;
        float imageIndex = 0;
    };
}