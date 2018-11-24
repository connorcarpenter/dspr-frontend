//
// Created by connor on 8/20/18.
//

#include <Sova/Internal/InternalApp.h>

#include <Game/Global.h>
#include "MinimapUnits.h"
#include "Game/Unit/Unit.h"
#include "Game/Unit/UnitManager.h"
#include "Game/TileManager.h"
#include "IsoBox/IsoBoxCache.h"

namespace DsprFrontend
{
    MinimapUnits::MinimapUnits()
    {
        this->g = (Global*) InternalApp::getGlobal();
        this->rectangle = New<Rectangle>(1, 1);
    }

    void MinimapUnits::drawSelf(Sova::Ref<Camera> camera, int xoffset, int yoffset)
    {
        for (auto iterator = this->g->unitManager->getUnitsIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();

            if (unit->unitTemplate->tileWidth == 1 && unit->unitTemplate->tileHeight == 1)
            {
                drawTile(camera, unit->tilePosition->x, unit->tilePosition->y, unit->selected ? Color::White : unit->tcSprite->tint);
            }
            else
            {
                auto unitIsoBoxBase = g->isoBoxCache->getIsoBox(unit->unitTemplate->tileWidth, unit->unitTemplate->tileHeight);

                for (auto iterator = unitIsoBoxBase->coordList->GetIterator(); iterator->Valid(); iterator->Next())
                {
                    auto coord = iterator->Get();
                    drawTile(camera, unit->tilePosition->x + coord->x, unit->tilePosition->y + coord->y, unit->selected ? Color::White : unit->tcSprite->tint);
                }
            }

        }

        this->rectangle->setLineStyle(1, Color::White, 1.0f);
        this->rectangle->setFillStyle(false);
        this->rectangle->position->set(g->camera->position->x*2/g->tileManager->tileWidth, g->camera->position->y*2/g->tileManager->tileHeight);
        this->rectangle->size->set(g->camera->width*2/g->tileManager->tileWidth, (g->camera->height-32)*2/g->tileManager->tileHeight);
        this->rectangle->drawSelf(camera, 0, 0);
    }

    void MinimapUnits::drawTile(Sova::Ref<Camera> camera, int x, int y, Color color)
    {
        this->rectangle->setLineStyle(false);
        this->rectangle->setFillStyle(color, 1.0f);
        this->rectangle->position->set(x, y);
        this->rectangle->size->set(2,2);
        this->rectangle->drawSelf(camera, 0, 0);
    }
}