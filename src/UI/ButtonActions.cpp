//
// Created by connor on 9/10/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Global.h>
#include <Unit/UnitOrder.h>
#include "ButtonActions.h"
#include "Unit/UnitManager.h"
#include "Unit/UnitTemplateCatalog.h"

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
        this->g->unitManager->orderCurrentlySelectedUnits(UnitOrder::Hold);
    }

    void ButtonActions::stopAction() {
        this->g->unitManager->orderCurrentlySelectedUnits(UnitOrder::Stop);
    }

    void ButtonActions::createVillagerAction() {
        this->g->unitManager->orderCurrentlySelectedUnitsToBuildUnit(this->g->unitTemplateCatalog->worker);
    }
}