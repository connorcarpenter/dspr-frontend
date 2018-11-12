#pragma once

//
// Created by connor on 10/26/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Common/Int.h>

using namespace Sova;

namespace DsprFrontend {

    class Unit;

    class Inventory : public Sova::Refable {

    public:

        Inventory();
        ~Inventory();
        void SetItemIndex(int slotIndex, int itemIndex);
        int GetItemAt(int slotIndex);

    private:

        int* items = nullptr;

    };
}