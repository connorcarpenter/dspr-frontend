//
// Created by connor on 8/20/18.
//

#include <Sova/Internal/InternalApp.h>

#include <Game/Global.h>
#include "MinimapUnits.h"
#include "Game/Unit/Unit.h"
#include "Game/Unit/UnitManager.h"
#include "Game/TileManager.h"

namespace DsprFrontend
{
    MinimapUnits::MinimapUnits()
    {
        this->g = (Global*) InternalApp::getGlobal();
        this->pixel = New<Pixel>();
        this->pixel->setLineStyle(1, Color::White);

        this->rectangle = New<Rectangle>(g->camera->width*2/g->tileManager->tileWidth, (g->camera->height-32)*2/g->tileManager->tileHeight);
        this->rectangle->setLineStyle(1, Color::White, 1.0f);
        this->rectangle->setFillStyle(false);
    }

    void MinimapUnits::drawSelf(Sova::Ref<Camera> camera, int xoffset, int yoffset)
    {
        for (auto iterator = this->g->unitManager->getUnitsIterator(); iterator->Valid(); iterator->Next())
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

        this->g = (Global*) InternalApp::getGlobal();
        this->rectangle->position->set(g->camera->position->x*2/g->tileManager->tileWidth, g->camera->position->y*2/g->tileManager->tileHeight);
        this->rectangle->drawSelf(camera, 0, 0);
    }
}