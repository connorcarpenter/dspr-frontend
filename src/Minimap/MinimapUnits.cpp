//
// Created by connor on 8/20/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Global.h>
#include "MinimapUnits.h"

namespace DsprFrontend
{
    MinimapUnits::MinimapUnits()
    {
        this->g = (Global*) InternalApp::getSovaApp()->getGlobal();
        this->pixel = New<Pixel>();
        this->pixel->setLineStyle(1, Color::White);
    }

    void MinimapUnits::drawSelf(Sova::Ref<Camera> camera, int xoffset, int yoffset)
    {
        auto unitList = this->g->unitManager->getUnits();
        for (auto iterator = unitList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            this->pixel->position->set(unit->position->x, unit->position->x);
            this->pixel->drawSelf(camera, xoffset, yoffset);
        }
    }
}