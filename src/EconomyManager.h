#pragma once

//
// Created by connor on 10/31/18.
//

#include <Sova/References/Refable.h>

namespace DsprFrontend {
    class EconomyManager : public Sova::Refable {
    public:
        int pop = 3;
        int popMax = 25;
        int mana = 0;
    };
}