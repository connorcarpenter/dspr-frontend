//
// Created by connor on 10/26/18.
//

#include "Inventory.h"
#include "Unit.h"

namespace DsprFrontend
{
    Inventory::Inventory() {
        this->items = new int[6];
        for(int i=0;i<6;i++)
            this->items[i] = -1;

    }

    Inventory::~Inventory() {
        delete [] this->items;
    }

    void Inventory::SetItemIndex(int slotIndex, int itemIndex) {
        this->items[slotIndex] = itemIndex;
    }

    int Inventory::GetItemAt(int slotIndex) {
        return this->items[slotIndex];
    }

    void Inventory::RemoveItem(int slotIndex) {
        this->items[slotIndex] = -1;
    }
}