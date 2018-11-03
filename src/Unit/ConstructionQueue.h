#pragma once

//
// Created by connor on 10/26/18.
//

#include <queue>
#include "UnitTemplate.h"

namespace DsprFrontend {

    class Unit;

    class ConstructionQueue : public Sova::Refable {

    public:

        ConstructionQueue();
        bool atMaxQueue();
        void enqueue(Ref<UnitTemplate> unitTemplate);
        void update(float d);
        bool isTraining();
        int getCurrentTotalBuildTime();
        void emptyQueue();

        Ref<ListIterator<UnitTemplate>> GetIterator();

        float currentBuildTime = 0;

    private:

        Ref<List<UnitTemplate>> unitQueue = Null<List<UnitTemplate>>();
        const int maxQueue = 7;
    };
}