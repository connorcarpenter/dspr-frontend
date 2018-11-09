//
// Created by connor on 10/31/18.
//

#include "EconomyManager.h"

namespace DsprFrontend {

    EconomyManager::EconomyManager() {
        this->popStr = String::getStringFromNumber(this->pop);
        this->popMaxStr = String::getStringFromNumber(this->popMax);
        this->manaStr = String::getStringFromNumber(this->mana);
    }

    int EconomyManager::getPop() {
        return this->pop;
    }

    int EconomyManager::getPopMax() {
        return this->popMax;
    }

    int EconomyManager::getMana() {
        return this->mana;
    }

    void EconomyManager::setPop(int value) {
        this->pop = value;
        this->popStr = String::getStringFromNumber(this->pop);
    }

    void EconomyManager::setPopMax(int value) {
        this->popMax = value;
        this->popMaxStr = String::getStringFromNumber(this->popMax);
    }

    void EconomyManager::setMana(int value) {
        if (value < 0) value = 0;
        if (value > 99999) value = 99999;
        if (this->mana != value) {
            this->mana = value;
            this->manaStr = String::getStringFromNumber(this->mana);
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
                this->setPop(amount);
                continue;
            }
            else
            if (propName->Equals("popMax"))
            {
                int amount = atoi(propsParts->At(1)->AsCStr());
                this->setPopMax(amount);
                continue;
            }
        }
    }
}