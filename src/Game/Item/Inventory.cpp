//
// Created by connor on 10/26/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include "Inventory.h"
#include "ItemTemplateCatalog.h"

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

    void Inventory::SetItemIndex(int slotIndex, Ref<ItemTemplate> itemTemplate) {
        if (itemTemplate == nullptr) {this->items[slotIndex] = -1; return;}

        if(!this->CanPlaceInSlot(slotIndex, itemTemplate)) {
            int i = 1/0; // how did we get here?
        }

        this->items[slotIndex] = itemTemplate->index;
    }

    Ref<ItemTemplate> Inventory::GetItemAt(int slotIndex) {
        auto g = (Global*) InternalApp::getGlobal();
        return g->itemTemplateCatalog->findTemplateByIndex(this->items[slotIndex]);
    }

    void Inventory::RemoveItem(int slotIndex) {
        this->items[slotIndex] = -1;
    }

    bool Inventory::CanPlaceInSlot(int slotIndex, Ref<ItemTemplate> itemTemplate) {
        auto partAllowed = this->slotAllowsPart(slotIndex);
        if (partAllowed == Any)return true;
        if (partAllowed == None)return false;
        if (partAllowed == itemTemplate->wornOn)return true;
        return false;
    }
}