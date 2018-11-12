//
// Created by connor on 10/31/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include "ShadowManager.h"
#include "UnitManager.h"
#include "Unit.h"

namespace DsprFrontend {

    ShadowManager::ShadowManager() {

    }

    void ShadowManager::drawSelf(Ref<Camera> camera, int xoffset, int yoffset) {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        for (auto iterator = g->unitManager->getUnits()->GetIterator(); iterator->Valid(); iterator->Next()) {
            auto unit = iterator->Get();
            if (!unit->unitTemplate->hasShadow) continue;
            unit->drawShadow(camera, xoffset, yoffset);
        }
    }
}