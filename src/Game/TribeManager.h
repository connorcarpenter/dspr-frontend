#pragma once

//
// Created by connor on 12/10/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Common/Int.h>
#include <Sova/Common/Map.h>
#include <Sova/Graphics/Color.h>

namespace DsprFrontend {

    enum TribeStatus
    {
        Player,
        Ally,
        Neutral,
        Enemy
    };

    class TribeManager : public Sova::Refable {
    public:
        TribeManager();
        void setTribeStatus(int tribeIndex, int status);
        int getPlayerTribeIndex();
        bool isEnemy(int tribeIndex);
        bool isPlayer(int tribeIndex);
        bool givesSight(int tribeIndex);
        Sova::Color getSelectionColor(int tribeIndex);
        Sova::Color getCursorColor(int tribeIndex);
        Sova::Color getTeamColor(int tribeIndex);

    private:
        Ref<Sova::Map<Sova::Int>> tribeMap = Null<Sova::Map<Sova::Int>>();
        int playersTribeIndex = -1;
    };
}