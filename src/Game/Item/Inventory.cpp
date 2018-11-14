//
// Created by connor on 10/26/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include "Inventory.h"
#include "ItemTemplateCatalog.h"
#include "ItemTemplate.h"
#include "../Unit/Unit.h"

namespace DsprFrontend
{
    Inventory::Inventory(Unit* mainUnit) {
        this->items = new int[6];
        for(int i=0;i<6;i++)
            this->items[i] = -1;

        this->mainUnit = mainUnit;
    }

    Inventory::~Inventory() {
        delete [] this->items;
    }

    void Inventory::SetItemIndex(int slotIndex, Ref<ItemTemplate> itemTemplate) {
        if (itemTemplate == nullptr) {this->RemoveItem(slotIndex); return;}

        if(!this->CanPlaceInSlot(slotIndex, itemTemplate)) {
            int i = 1/0; // how did we get here?
        }

        this->items[slotIndex] = itemTemplate->index;
        this->UpdateUnitSprite(slotIndex, -1);
    }

    Ref<ItemTemplate> Inventory::GetItemAt(int slotIndex) {
        auto g = (Global*) InternalApp::getGlobal();
        return g->itemTemplateCatalog->findTemplateByIndex(this->items[slotIndex]);
    }

    void Inventory::RemoveItem(int slotIndex) {
        auto prevItem = this->items[slotIndex];
        this->items[slotIndex] = -1;
        this->UpdateUnitSprite(slotIndex, prevItem);
    }

    bool Inventory::CanPlaceInSlot(int slotIndex, Ref<ItemTemplate> itemTemplate) {
        auto partAllowed = this->slotAllowsPart(slotIndex);
        if (partAllowed == Any)return true;
        if (partAllowed == None)return false;
        if (partAllowed == itemTemplate->wornOn)return true;
        return false;
    }

    void Inventory::UpdateUnitSprite(int slotIndex, int prevItemIndex) {
        auto partAllowed = this->slotAllowsPart(slotIndex);
        if (partAllowed == Head) return this->mainUnit->updateHeadSprite(GetItemAt(slotIndex));
        if (partAllowed == Body) return this->mainUnit->updateBodySprite(GetItemAt(slotIndex));
        if (partAllowed == Hand) {
            auto leftHanded = false;
            auto itemTemplate = GetItemAt(slotIndex);
            if (itemTemplate == nullptr)
            {
                if (prevItemIndex == -1)return;
                auto g = (Global*) InternalApp::getGlobal();
                auto prevItemTemplate = g->itemTemplateCatalog->findTemplateByIndex(prevItemIndex);
                leftHanded = prevItemTemplate->leftHanded;
            }
            else
            {
                leftHanded = itemTemplate->leftHanded;
            }

            if (leftHanded) {
                return this->mainUnit->updateLeftHandSprite(itemTemplate);
            } else {
                return this->mainUnit->updateRightHandSprite(itemTemplate);
            }
        }
    }
}