//
// Created by connor on 10/31/18.
//

#include "EconomyManager.h"

namespace DsprFrontend {

    EconomyManager::EconomyManager() {
    }

    int EconomyManager::getMana() {
        return this->mana;
    }

    void EconomyManager::setMana(int value) {
        if (value < 0) value = 0;
        if (value > 99999) value = 99999;
        if (this->mana != value) {
            this->mana = value;
        }
    }

    void EconomyManager::receiveUpdate(Ref<List<Sova::String>> propsStrList)
    {
        for (auto iterator = propsStrList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto propsStr = iterator->Get();
            auto propsParts = propsStr->Split(':');

            auto propName = propsParts->At(0);

            if (propName->Equals("mana"))
            {
                int amount = atoi(propsParts->At(1)->AsCStr());
                this->setMana(amount);
                continue;
            }
            else
            if (propName->Equals("pop"))
            {
                int amount = atoi(propsParts->At(1)->AsCStr());
                this->pop = amount;
                continue;
            }
            else
            if (propName->Equals("popMax"))
            {
                int amount = atoi(propsParts->At(1)->AsCStr());
                this->popMax = amount;
                continue;
            }
        }
    }
}