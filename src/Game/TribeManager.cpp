//
// Created by connor on 12/10/18.
//

#include <DsprColors.h>
#include "TribeManager.h"

namespace DsprFrontend
{
    TribeManager::TribeManager() {
        this->tribeMap = New<Map<Int>>();
    }

    void TribeManager::setTribeStatus(int tribeIndex, int status) {
        this->tribeMap->Insert(tribeIndex, New<Int>(status));
        if (status == TribeStatus::Player)
            this->playersTribeIndex = tribeIndex;
    }

    bool TribeManager::isEnemy(int tribeIndex) {
        return this->tribeMap->At(tribeIndex)->getInt() == TribeStatus::Enemy;
    }

    bool TribeManager::givesSight(int tribeIndex) {
        return this->tribeMap->At(tribeIndex)->getInt() == TribeStatus::Player || this->tribeMap->At(tribeIndex)->getInt() == TribeStatus::Ally;
    }

    bool TribeManager::isPlayer(int tribeIndex) {
        return tribeIndex == this->playersTribeIndex;
    }

    Sova::Color TribeManager::getSelectionColor(int tribeIndex) {
        switch (this->tribeMap->At(tribeIndex)->getInt())
        {
            case TribeStatus::Player:
                return DsprColors::LightGreen;
                break;
            case TribeStatus::Ally:
            case TribeStatus::Neutral:
                return DsprColors::LightYellow;
                break;
            case TribeStatus::Enemy:
                return DsprColors::LightRed;
                break;
            default:
                int i = 1/0;
                break;
        }
    }

    Sova::Color TribeManager::getCursorColor(int tribeIndex) {
        switch (this->tribeMap->At(tribeIndex)->getInt())
        {
            case TribeStatus::Player:
                return DsprColors::Green;
                break;
            case TribeStatus::Ally:
            case TribeStatus::Neutral:
                return DsprColors::Yellow;
                break;
            case TribeStatus::Enemy:
                return DsprColors::Red;
                break;
            default:
                int i = 1/0;
                break;
        }
    }

    Sova::Color TribeManager::getTeamColor(int tribeIndex) {
        switch (this->tribeMap->At(tribeIndex)->getInt())
        {
            case TribeStatus::Player:
                return Color::Red;
                break;
            case TribeStatus::Ally:
                return Color::Blue;
                break;
            case TribeStatus::Neutral:
                return Color::Yellow;
                break;
            case TribeStatus::Enemy:
                return DsprColors::AshwalkerBlood;
                break;
            default:
                int i = 1/0;
                break;
        }
    }

    int TribeManager::getPlayerTribeIndex() {
        return this->playersTribeIndex;
    }


}