#pragma once

//
// Created by connor on 10/26/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Common/Int.h>
#include "ItemTemplate.h"

using namespace Sova;

namespace DsprFrontend {

    class Unit;

    class Inventory : public Sova::Refable {

    public:

        Inventory();
        ~Inventory();
        void SetItemIndex(int slotIndex, Ref<ItemTemplate> itemTemplate);
        Ref<ItemTemplate> GetItemAt(int slotIndex);
        void RemoveItem(int slotIndex);
        bool CanPlaceInSlot(int slotIndex, Ref<ItemTemplate> itemTemplate);

    private:

        static const BodyPart slotAllowsPart(int index){
            static const BodyPart slotAllowsArr[] = { Head, Hand, Hand, Body, Any, Any };
            return slotAllowsArr[index];
        }

        int* items = nullptr;

    };
}