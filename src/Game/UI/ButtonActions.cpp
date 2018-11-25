//
// Created by connor on 9/10/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include <Game/Unit/UnitOrderType.h>
#include "ButtonActions.h"
#include "Game/Unit/UnitManager.h"
#include "Game/Unit/UnitTemplateCatalog.h"

namespace DsprFrontend
{
    ButtonActions::ButtonActions()
    {
        this->g = (Global*) Sova::InternalApp::getSovaApp()->getGlobal();
    }

    void ButtonActions::moveAction() {
        this->g->unitManager->issueUnitOrder(false);
    }

    void ButtonActions::attackAction() {
        this->g->unitManager->issueUnitOrder(true);
    }

    void ButtonActions::holdAction() {
        this->g->unitManager->orderCurrentlySelectedUnits(UnitOrderType::Hold);
    }

    void ButtonActions::stopAction() {
        this->g->unitManager->orderCurrentlySelectedUnits(UnitOrderType::Stop);
    }

    void ButtonActions::createVillagerAction() {
        this->g->unitManager->orderCurrentlySelectedUnitsToBuildUnit(this->g->unitTemplateCatalog->worker);
    }

    void ButtonActions::cancelTrainAction() {
        this->g->unitManager->orderCurrentlySelectedUnitsToCancelTrainUnit(0);
    }

    void ButtonActions::gatherAction() {
        this->g->unitManager->issueUnitOrderGather();
    }

    void ButtonActions::rallyAction() {
        this->g->unitManager->issueUnitOrderRally();
    }
}