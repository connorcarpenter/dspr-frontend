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

        this->rectangle = New<Rectangle>(g->camera->width*2/g->tileManager->tileWidth, (g->camera->height-32)*2/g->tileManager->tileHeight);
        this->rectangle->setLineStyle(1, Color::White, 1.0f);
        this->rectangle->setFillStyle(false);
    }

    void MinimapUnits::drawSelf(Sova::Ref<Camera> camera, int xoffset, int yoffset)
    {
        auto unitList = this->g->unitManager->getUnits();
        for (auto iterator = unitList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();

            this->pixel->setLineStyle(1, unit->selected ? Color::White : unit->tcSprite->tint);

            this->pixel->position->set(unit->tilePosition->x, unit->tilePosition->y);
            this->pixel->drawSelf(camera, xoffset, yoffset);

            this->pixel->position->set(unit->tilePosition->x+1, unit->tilePosition->y);
            this->pixel->drawSelf(camera, xoffset, yoffset);

            this->pixel->position->set(unit->tilePosition->x+1, unit->tilePosition->y+1);
            this->pixel->drawSelf(camera, xoffset, yoffset);

            this->pixel->position->set(unit->tilePosition->x, unit->tilePosition->y+1);
            this->pixel->drawSelf(camera, xoffset, yoffset);
        }

        this->g = (Global*) InternalApp::getSovaApp()->getGlobal();
        this->rectangle->position->set(g->camera->position->x*2/g->tileManager->tileWidth, g->camera->position->y*2/g->tileManager->tileHeight);
        this->rectangle->drawSelf(camera, 0, 0);
    }
}