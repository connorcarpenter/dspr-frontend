//
// Created by connor on 9/10/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Global.h>
#include <UnitOrder.h>
#include "ButtonActions.h"
#include "UnitManager.h"

namespace DsprFrontend
{
    ButtonActions::ButtonActions()
    {
        this->g = (Global*) Sova::InternalApp::getSovaApp()->getGlobal();
    }

    void ButtonActions::moveAction() {
        int i = 12;
    }

    void ButtonActions::attackAction() {
        int i = 12;
    }

    void ButtonActions::holdAction() {
        this->g->unitManager->orderCurrentlySelectedUnits(UnitOrder::Hold);
    }

    void ButtonActions::stopAction() {
        this->g->unitManager->orderCurrentlySelectedUnits(UnitOrder::Stop);
    }
}