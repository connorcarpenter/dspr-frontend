#pragma once

//
// Created by connor on 9/10/18.
//

#include <Sova/References/Refable.h>

namespace DsprFrontend {
    class Global;

    class ButtonActions : public Sova::Refable {
    public:
        ButtonActions();

        void moveAction();
        void stopAction();
        void holdAction();
        void attackAction();
        void gatherAction();

        void createVillagerAction();
        void rallyAction();
        void cancelTrainAction();

        void specialAction(int actionIndex);

    private:
        Global* g = nullptr;


    };
}