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
#include "SpriteCatalog.h"

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

        g->unitHoverCircle_3x3 = New<AnimatedSprite>(New<Sova::String>("images/ui/3x3_hovercircle.png"), 66, 38, 1);
        g->unitHoverCircle_3x3->imageSpeed = 0.1f;
        g->unitHoverCircle_3x3->anchor->set(33, 19);

        g->unitSelectCircle_3x3 = New<Sprite>(New<Sova::String>("images/ui/3x3_selectioncircle.png"));
        g->unitSelectCircle_3x3->anchor->set(31, 17);

        g->unitHoverCircle_2x2 = New<AnimatedSprite>(New<Sova::String>("images/ui/2x2_hovercircle.png"), 46, 28, 1);
        g->unitHoverCircle_2x2->imageSpeed = 0.1f;
        g->unitHoverCircle_2x2->anchor->set(23, 14);

        g->unitSelectCircle_2x2 = New<Sprite>(New<Sova::String>("images/ui/2x2_selectioncircle.png"));
        g->unitSelectCircle_2x2->anchor->set(21, 27);

        g->unitHoverCircle_5x5 = New<AnimatedSprite>(New<Sova::String>("images/ui/5x5_hovercircle.png"), 106, 58, 1);
        g->unitHoverCircle_5x5->imageSpeed = 0.1f;
        g->unitHoverCircle_5x5->anchor->set(53, 28);

        g->unitSelectCircle_5x5 = New<Sprite>(New<Sova::String>("images/ui/5x5_selectioncircle.png"));
        g->unitSelectCircle_5x5->anchor->set(52, 27);

        g->moveMarker = New<AnimatedSprite>(New<Sova::String>("images/ui/moveMarker.png"), 11, 9, 1);
        g->moveMarker->imageSpeed = 0.1;
        g->moveMarker->anchor->set(5, 4);
        /////////////////////////

        this->mySprite = New<Sprite>(New<Sova::String>("images/ui/minimap.png"));
        this->myAnimatedSprite = New<AnimatedSprite>(New<Sova::String>("images/ui/commandActions.png"), 10, 12, 0);

        this->myLine = New<Sova::Line>();
        this->myLine->setLineStyle(1, Color::Green);

        this->myRectangle = New<Sova::Rectangle>(0,4);
        this->myRectangle->setLineStyle(false);
    }

    Ref<Button> UiManager::getButtonWithLeftClick(Ref<Point> clickPoint) {

        if (Math::PointInBox(clickPoint->x, clickPoint->y, 48, 109, 204, 144))
        {
            //clicking within armybar
            return Null<Button>();
        }

        if (Math::PointInBox(clickPoint->x, clickPoint->y, 204, 100, 256, 144))
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

                        int leftX = 208 + (11 * i);
                        int upY = 103 + 13*j;

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
        if (Math::PointInBox(clickPoint->x, clickPoint->y, 48, 109, 204, 144)) return false;
        if (Math::PointInBox(clickPoint->x, clickPoint->y, 204, 100, 256, 144)) return false;
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

    void UiManager::Draw(Ref<Camera> camera, int xoffset, int yoffset)
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        //draw minimap
        this->mySprite->useSpriteInfo(g->spriteCatalog->sprMinimap);
        this->mySprite->tint = Color::White;
        this->mySprite->position->set(0, 100);
        this->mySprite->drawSelf(camera, 0, 0);

        //draw portraitbar
        this->mySprite->useSpriteInfo(g->spriteCatalog->sprPortraitBar);
        this->mySprite->tint = Color::White;
        this->mySprite->position->set(48, 109);
        this->mySprite->drawSelf(camera, 0, 0);

        Ref<Unit> firstUnit = Null<Unit>();

        {
            if (g->unitManager->getSelectedUnits()->Size() <= 0)
            {
                //draw individual armybar
                this->myAnimatedSprite->useAnimatedSpriteInfo(g->spriteCatalog->sprArmyBar);
                this->myAnimatedSprite->tint = Color::White;
                this->myAnimatedSprite->position->set(90, 109);
                this->myAnimatedSprite->imageIndex = 0;
                this->myAnimatedSprite->drawSelf(camera, 0, 0);

                //draw empty itembar
                this->myAnimatedSprite->useAnimatedSpriteInfo(g->spriteCatalog->sprItemBar);
                this->myAnimatedSprite->tint = Color::White;
                this->myAnimatedSprite->position->set(160, 109);
                this->myAnimatedSprite->imageIndex = 1;
                this->myAnimatedSprite->drawSelf(camera, 0, 0);
            }
            else
            {
                auto selectedUnitList = g->unitManager->getSelectedUnits();

                firstUnit = selectedUnitList->At(0);
                if (firstUnit != nullptr)
                {
                    //draw itembar
                    this->myAnimatedSprite->useAnimatedSpriteInfo(g->spriteCatalog->sprItemBar);
                    this->myAnimatedSprite->tint = Color::White;
                    this->myAnimatedSprite->position->set(160, 109);
                    this->myAnimatedSprite->imageIndex = (firstUnit->unitTemplate->hasInventory) ? 0 : 1;
                    this->myAnimatedSprite->drawSelf(camera, 0, 0);

                    this->mySprite->useSpriteInfo(firstUnit->unitTemplate->sprBigPortrait);
                    this->mySprite->tint = Color::White;
                    this->mySprite->position->set(55, 112);
                    this->mySprite->drawSelf(camera, 0, 0);

                    this->mySprite->useSpriteInfo(firstUnit->unitTemplate->sprBigPortraitTC);
                    this->mySprite->tint = firstUnit->tcSprite->tint;
                    this->mySprite->position->set(55, 112);
                    this->mySprite->drawSelf(camera, 0, 0);

                    //select commandcard
                    this->currentButtonCard = firstUnit->unitTemplate->commandCard;
                }

                if (g->unitManager->getSelectedUnits()->Size() == 1)
                {
                    //draw individual armybar
                    bool showTraining = (firstUnit->unitTemplate->hasConstructionQueue && firstUnit->constructionQueue->isTraining());

                    this->myAnimatedSprite->useAnimatedSpriteInfo(g->spriteCatalog->sprArmyBar);
                    this->myAnimatedSprite->tint = Color::White;
                    this->myAnimatedSprite->position->set(90, 109);
                    this->myAnimatedSprite->imageIndex = showTraining ? 2 : 0;
                    this->myAnimatedSprite->drawSelf(camera, 0, 0);

                    //show individual unit stats

                    //show training
                    if (showTraining)
                    {
                        int showIcons = 0;
                        int i = 0;
                        int j = 0;
                        for (auto iterator = firstUnit->constructionQueue->GetIterator(); iterator->Valid(); iterator->Next()) {
                            auto unitTemplate = iterator->Get();
                            this->mySprite->useSpriteInfo(unitTemplate->sprUnitPortrait);
                            this->mySprite->tint = Color::White;
                            this->mySprite->position->set(87 + 6 + (i * 11), 107 + 9 + (13 * j));
                            this->mySprite->drawSelf(camera, 0, 0);

                            if (showIcons == 0) j+=1;
                            if (showIcons >= 1) i+=1;
                            showIcons += 1;
                        }

                        float healthBarLineLength = (firstUnit->constructionQueue->currentBuildTime /
                                                     firstUnit->constructionQueue->getCurrentTotalBuildTime()) *
                                                    53;
                        if(healthBarLineLength > 1) {
                            this->myRectangle->position->set(105, 124);
                            this->myRectangle->setLineStyle(1, Color::White);
                            this->myRectangle->setFillStyle(Color::White, 1.0f);

                            this->myRectangle->size->set(healthBarLineLength, 3);
                            this->myRectangle->drawSelf(camera, 0, 0);
                        }
                    }
                }
                else
                {
                    //draw multi-select armybar
                    this->myAnimatedSprite->useAnimatedSpriteInfo(g->spriteCatalog->sprArmyBar);
                    this->myAnimatedSprite->tint = Color::White;
                    this->myAnimatedSprite->position->set(90, 109);
                    this->myAnimatedSprite->imageIndex = 1;
                    this->myAnimatedSprite->drawSelf(camera, 0, 0);

                    //show all selected units

                    int i = 0;
                    int j = 0;
                    for (auto iterator = selectedUnitList->GetIterator(); iterator->Valid(); iterator->Next())
                    {
                        auto unit = iterator->Get();

                        if (unit != nullptr)
                        {
                            this->mySprite->useSpriteInfo(unit->unitTemplate->sprUnitPortrait);
                            this->mySprite->tint = Color::White;
                            this->mySprite->position->set(87 + 6 + (i * 11), 107 + 5 + (15 * j));
                            this->mySprite->drawSelf(camera, 0, 0);

                            this->mySprite->useSpriteInfo(unit->unitTemplate->sprUnitPortraitTC);
                            this->mySprite->tint = unit->tcSprite->tint;
                            this->mySprite->position->set(87 + 6 + (i * 11), 107 + 5 + (15 * j));
                            this->mySprite->drawSelf(camera, 0, 0);

                            this->myLine->setLineStyle(1, Color::Green);
                            this->myLine->position->set(87 + 6 + (i * 11), 107 + 2 + 15 + (15 * j));
                            int healthBarLineLength = (unit->health >= 0) ? (int) (
                                    ((float) unit->health / unit->unitTemplate->maxHealth) *
                                    10) : 0;
                            this->myLine->endPosition->set(87 + 6 + (i * 11) + healthBarLineLength,
                                                                  107 + 2 + 15 + (15 * j));
                            this->myLine->drawSelf(camera, xoffset, yoffset);

                            this->myLine->setLineStyle(1, Color::White);
                            this->myLine->position->y += 1;
                            this->myLine->endPosition->set(87 + 6 + 10 + (i * 11), 107 + 1 + 2 + 15 + (15 * j));
                            this->myLine->drawSelf(camera, xoffset, yoffset);

                            i++;
                        }

                        if (i > 5)
                        {
                            j++;
                            i = 0;
                        }
                    }
                }
            }
        }

        //draw command card
        this->mySprite->useSpriteInfo(g->spriteCatalog->sprCommandBar);
        this->mySprite->tint = Color::White;
        this->mySprite->position->set(204, 100);
        this->mySprite->drawSelf(camera, 0, 0);

        if (firstUnit != nullptr && this->currentButtonCard != nullptr)
        {
            auto iterator = this->currentButtonCard->buttonList->GetIterator();

            for (int j = 0; j < 2; j += 1)
            {
                for (int i = 0; i < 4; i += 1)
                {
                    if (!iterator->Valid()) break;

                    auto button = iterator->Get();

                    int leftX = 208 + (11 * i);
                    int upY = 103 + 13*j;

                    Color buttonTint = Color::LightGray;

                    if (Math::PointInBox(g->cursor->position->x, g->cursor->position->y,
                                         leftX, upY,
                                         leftX + 10, upY + 12))
                    {
                        buttonTint = Color::White;
                    }

                    if (button->usesSubframes)
                    {
                        this->myAnimatedSprite->useAnimatedSpriteInfo(button->spriteInfo);
                        this->myAnimatedSprite->position->set(leftX, upY);
                        this->myAnimatedSprite->tint = buttonTint;
                        this->myAnimatedSprite->imageIndex = button->imageIndex;
                        this->myAnimatedSprite->drawSelf(camera, 0, 0);
                    }
                    else
                    {
                        this->mySprite->useSpriteInfo(button->spriteInfo);
                        this->mySprite->position->set(leftX, upY);
                        this->mySprite->tint = buttonTint;
                        this->mySprite->drawSelf(camera, 0, 0);

                        if (button->usesTeamColor)
                        {
                            this->mySprite->useSpriteInfo(button->tcSpriteInfo);
                            this->mySprite->position->set(leftX, upY);
                            this->mySprite->tint = Color::MixColors(buttonTint, firstUnit->tcSprite->tint);
                            this->mySprite->drawSelf(camera, 0, 0);
                        }
                    }

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