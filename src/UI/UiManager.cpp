//
// Created by connor on 8/16/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Sova/Math/Math.h>
#include "UiManager.h"
#include "Global.h"
#include "Unit/Unit.h"
#include "TileManager.h"
#include "Unit/UnitManager.h"
#include "Cursor.h"
#include "Unit/UnitTemplate.h"

namespace DsprFrontend
{
    UiManager::UiManager() : Container()
    {
        ///global stuff
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        g->unitHoverCircle_1x1 = New<AnimatedSprite>(New<Sova::String>("images/ui/unitHover.png"), 16, 12, 1);
        g->unitHoverCircle_1x1->imageSpeed = 0.1f;
        g->unitHoverCircle_1x1->anchor->set(8, 6);

        g->unitSelectCircle_1x1 = New<Sprite>(New<Sova::String>("images/ui/unitSelection.png"));
        g->unitSelectCircle_1x1->anchor->set(8, 5);

        g->unitHoverCircle_5x5 = New<AnimatedSprite>(New<Sova::String>("images/ui/5x5_hovercircle.png"), 106, 58, 1);
        g->unitHoverCircle_5x5->imageSpeed = 0.1f;
        g->unitHoverCircle_5x5->anchor->set(53, 28);

        g->unitSelectCircle_5x5 = New<Sprite>(New<Sova::String>("images/ui/5x5_selectioncircle.png"));
        g->unitSelectCircle_5x5->anchor->set(52, 27);

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

    Ref<Button> UiManager::getButtonWithLeftClick(Ref<Point> clickPoint) {

        if (Math::PointInBox(clickPoint->x, clickPoint->y, 48, 116, 204, 144))
        {
            //clicking within armybar
            return Null<Button>();
        }

        if (Math::PointInBox(clickPoint->x, clickPoint->y, 204, 106, 256, 144))
        {
            //clicking on command card
            if (this->currentButtonCard != nullptr)
            {
                auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
                auto iterator = this->currentButtonCard->buttonList->GetIterator();

                for (int j = 0; j < 2; j += 1)
                {
                    for (int i = 0; i < 4; i += 1)
                    {
                        if (!iterator->Valid()) break;

                        auto button = iterator->Get();

                        int leftX = 207 + (12 * i);
                        int upY = 111 + 16*j;

                        if (Math::PointInBox(g->cursor->position->x, g->cursor->position->y,
                                             leftX, upY,
                                             leftX + 10, upY + 12)) {
                            if (button->requiresClickOnGameArea)
                            {
                                return button;
                            }
                            else
                            {
                                button->executeAction();
                                return Null<Button>();
                            }
                        }

                        iterator->Next();
                    }
                }
            }
        }

        return Null<Button>();
    }

    bool UiManager::isInGameArea(Ref<Point> clickPoint){
        if (Math::PointInBox(clickPoint->x,clickPoint->y,0,100,48,144)) return false;
        if (Math::PointInBox(clickPoint->x, clickPoint->y, 48, 116, 204, 144)) return false;
        if (Math::PointInBox(clickPoint->x, clickPoint->y, 204, 106, 256, 144)) return false;
        return true;
    }

    bool UiManager::captureLeftClickMinimapEvent(Ref<Point> clickPoint) {
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

                if (unit != nullptr)
                {
                    this->unitPortrait->position->set(48 + 6 + (i * 12), 116 + 5);
                    this->unitPortrait->drawSelf(camera, 0, 0);

                    this->healthBarLine->setLineStyle(1, Color::Green);
                    this->healthBarLine->position->set(48 + 6 + (i * 12), 116 + 5 + 15);
                    int healthBarLineLength = (unit->health >= 0) ? (int) (((float) unit->health / unit->unitTemplate->maxHealth) *
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

        if (this->currentButtonCard != nullptr)
        {
            auto iterator = this->currentButtonCard->buttonList->GetIterator();

            for (int j = 0; j < 2; j += 1)
            {
                for (int i = 0; i < 4; i += 1)
                {
                    if (!iterator->Valid()) break;

                    auto button = iterator->Get();

                    int leftX = 207 + (12 * i);
                    int upY = 111 + 16*j;

                    if (Math::PointInBox(g->cursor->position->x, g->cursor->position->y,
                                         leftX, upY,
                                         leftX + 10, upY + 12)) {
                        this->commandActions->tint = Color::White;
                    } else {
                        this->commandActions->tint = Color::LightGray;
                    }

                    this->commandActions->position->set(leftX, upY);
                    this->commandActions->imageIndex = button->imageIndex;
                    this->commandActions->drawSelf(camera, 0, 0);

                    iterator->Next();
                }
            }
        }

        Container::Draw(camera, xoffset, yoffset);
    }

    Ref<Button> UiManager::getButtonFromKeyboardShortcut() {

        if (this->currentButtonCard != nullptr) {
            for (auto iterator = this->currentButtonCard->buttonList->GetIterator(); iterator->Valid(); iterator->Next()) {
                auto button = iterator->Get();
                if (InternalApp::getInternalApp()->keyPressed(button->keyboardShortcut)){
                    if (!button->needKeyUp) {
                        if (button->requiresClickOnGameArea) {
                            return button;
                        } else {
                            button->needKeyUp = true;
                            button->executeAction();
                            return Null<Button>();
                        }
                    }
                } else {
                    if (button->needKeyUp) button->needKeyUp = false;
                }
            }
        }

        return Null<Button>();
    }
}