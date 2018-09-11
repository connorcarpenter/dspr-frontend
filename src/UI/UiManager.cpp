//
// Created by connor on 8/16/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Sova/Math/Math.h>
#include "UiManager.h"
#include "Global.h"
#include "Unit.h"
#include "TileManager.h"
#include "UnitManager.h"

namespace DsprFrontend
{

    UiManager::UiManager() : Container()
    {
        ///global stuff
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        g->unitHoverCircle = New<AnimatedSprite>(New<Sova::String>("images/ui/unitHover.png"), 16, 12, 1);
        g->unitHoverCircle->imageSpeed = 0.1f;
        g->unitHoverCircle->anchor->set(8, 6);

        g->unitSelectCircle = New<Sprite>(New<Sova::String>("images/ui/unitSelection.png"));
        g->unitSelectCircle->anchor->set(8, 5);

        g->moveMarker = New<AnimatedSprite>(New<Sova::String>("images/ui/moveMarker.png"), 11, 9, 1);
        g->moveMarker->imageSpeed = 0.1;
        g->moveMarker->anchor->set(5, 4);
        /////////////////////////

        this->minimap = New<Sprite>(New<Sova::String>("images/ui/minimap.png"));
        this->armybar = New<Sprite>(New<Sova::String>("images/ui/armybar.png"));
        this->command = New<Sprite>(New<Sova::String>("images/ui/commandcard.png"));
        this->unitPortrait = New<Sprite>(New<Sova::String>("images/worker/unitPortrait.png"));
        this->commandActions = New<AnimatedSprite>(New<Sova::String>("images/ui/commandActions.png"), 10, 12, 0);

        this->healthBarLine = New<Sova::Line>();
        this->healthBarLine->setLineStyle(1, Color::Green);
    }

    bool UiManager::captureLeftClickEvent(Ref<Point> clickPoint) {
        if (Math::PointInBox(clickPoint->x,clickPoint->y,0,100,48,144))
        {
            //clicking within minimap container
            if (Math::PointInBox(clickPoint->x,clickPoint->y,5,103,43,141))
            {
                //clicking within actual minimap
                auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
                auto gridSize = g->tileManager->getGridSize();
                g->camera->position->set(
                        (int) (((((float) clickPoint->x - 5)/38)*gridSize->x*g->tileManager->tileWidth) - g->camera->width/2),
                        (int) (((((float) clickPoint->y -103)/38)*gridSize->y*g->tileManager->tileHeight) - (g->camera->height-32)/2)
                );
            }
            return false;
        }

        return true;
    }

    Ref<Point> UiManager::getMinimapPosition(Ref<Point> clickPoint)
    {
        //clicking within minimap container
        if (Math::PointInBox(clickPoint->x,clickPoint->y,5,103,43,141))
        {
            //clicking within actual minimap
            auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
            auto gridSize = g->tileManager->getGridSize();
            return New<Point>(
                    ((int) ((((float) clickPoint->x - 5)/38)*gridSize->x))*2,
                    ((int) ((((float) clickPoint->y -103)/38)*gridSize->y))*2
            );
        }

        return Null<Point>();
    }

    void UiManager::Draw(Ref<Camera> camera, int xoffset, int yoffset) {

        this->minimap->position->set(0, 100);
        this->minimap->drawSelf(camera, 0, 0);

        //draw armybar
        this->armybar->position->set(48, 116);
        this->armybar->drawSelf(camera, 0, 0);

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        {
            auto selectedUnitList = g->unitManager->getSelectedUnits();
            int i = 0;
            for (auto iterator = selectedUnitList->GetIterator(); iterator->Valid(); iterator->Next())
            {
                auto unit = iterator->Get();

                if (unit != nullptr) {
                    this->unitPortrait->position->set(48 + 6 + (i * 12), 116 + 5);
                    this->unitPortrait->drawSelf(camera, 0, 0);

                    this->healthBarLine->setLineStyle(1, Color::Green);
                    this->healthBarLine->position->set(48 + 6 + (i * 12), 116 + 5 + 15);
                    int healthBarLineLength = (unit->health >= 0) ? (int) (((float) unit->health / unit->maxHealth) *
                                                                           11) : 0;
                    this->healthBarLine->endPosition->set(48 + 6 + (i * 12) + healthBarLineLength, 116 + 5 + 15);
                    this->healthBarLine->drawSelf(camera, xoffset, yoffset);

                    this->healthBarLine->setLineStyle(1, Color::White);
                    this->healthBarLine->position->y += 2;
                    this->healthBarLine->endPosition->set(48 + 6 + 11 + (i * 12), 116 + 5 + 2 + 15);
                    this->healthBarLine->drawSelf(camera, xoffset, yoffset);

                    i++;
                }
            }
        }

        //draw command card
        this->command->position->set(204, 106);
        this->command->drawSelf(camera, 0, 0);

        if (this->currentButttonCard != nullptr)
        {
            for (int i = 0; i < 4; i += 1) {
                this->commandActions->position->set(204 + 3 + (12 * i), 106 + 5);
                this->commandActions->imageIndex = i;
                this->commandActions->drawSelf(camera, 0, 0);
            }

            for (int i = 0; i < 4; i += 1) {
                this->commandActions->position->set(204 + 3 + (12 * i), 106 + 21);
                this->commandActions->imageIndex = i + 4;
                this->commandActions->drawSelf(camera, 0, 0);
            }
        }

        Container::Draw(camera, xoffset, yoffset);
    }
}