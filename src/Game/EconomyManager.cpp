//
// Created by connor on 10/31/18.
//

#include <Network/StringScanner.h>
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

    void EconomyManager::setPop(int value) {
        this->pop = value;
    }

    void EconomyManager::setPopMax(int value) {
        this->popMax = value;
    }

    void EconomyManager::receiveUpdate(Ref<StringScanner> ss)
    {
        while(!ss->IsAtEnd()) {
            if (ss->EqualsUntil("mana", ':')) {
                ss->Advance(5);
                if (ss->IsAtEnd())return;
                char *cstr = ss->CstrAndAdvanceUntil('&');
                if (cstr == nullptr) return;
                int amount = atoi(cstr);
                ss->CleanCstr();
                this->setMana(amount);
            } else if (ss->EqualsUntil("pop", ':')) {
                ss->Advance(4);
                if (ss->IsAtEnd())return;
                char *cstr = ss->CstrAndAdvanceUntil('&');
                if (cstr == nullptr) return;
                int amount = atoi(cstr);
                ss->CleanCstr();
                this->pop = amount;

            } else if (ss->EqualsUntil("popMax", ':')) {
                ss->Advance(7);
                if (ss->IsAtEnd())return;
                char *cstr = ss->CstrAndAdvanceUntil('&');
                if (cstr == nullptr) return;
                int amount = atoi(cstr);
                ss->CleanCstr();
                this->popMax = amount;
            }
        }
    }
}