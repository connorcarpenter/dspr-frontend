#pragma once

//
// Created by connor on 10/31/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Common/String.h>

using namespace Sova;

namespace DsprFrontend {
    class EconomyManager : public Sova::Refable {
    public:

        EconomyManager();
        int getMana();

        void receiveUpdate(Ref<List<String>> propsStrList);

        void setMana(int value);

        int mana = 0;
        int pop = 3;
        int popMax = 25;
    private:

    };
}