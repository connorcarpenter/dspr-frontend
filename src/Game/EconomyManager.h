#pragma once

//
// Created by connor on 10/31/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Common/String.h>
#include "../Network/StringScanner.h"
using namespace Sova;

namespace DsprFrontend {
    class EconomyManager : public Sova::Refable {
    public:

        EconomyManager();
        int getMana();

        void receiveUpdate(Ref<StringScanner> ss);
        void setMana(int value);

        int mana = 0;
        int pop = 3;
        int popMax = 25;

        void setPop(int value);

        void setPopMax(int value);


    };
}