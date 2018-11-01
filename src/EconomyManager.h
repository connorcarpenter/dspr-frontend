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
        int getPop();
        int getPopMax();
        int getMana();
        void setPop(int value);
        void setPopMax(int value);
        void setMana(int value);

        Ref<String> popStr = Null<String>();
        Ref<String> popMaxStr = Null<String>();
        Ref<String> manaStr = Null<String>();

    private:
        int pop = 3;
        int popMax = 25;
        int mana = 0;
    };
}