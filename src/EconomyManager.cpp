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
        this->mana = value;
        this->manaStr = String::getStringFromNumber(this->mana);
    }
}