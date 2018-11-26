//
// Created by connor on 8/16/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Sova/Math/Math.h>
#include "UiManager.h"
#include "Game/Global.h"
#include "Game/Unit/Unit.h"
#include "Game/TileManager.h"
#include "Game/Unit/UnitManager.h"
#include "Cursor.h"
#include "Game/Unit/UnitTemplate.h"
#include "SpriteCatalog.h"
#include "Game/EconomyManager.h"
#include "GraphicsManager.h"
#include "ChatManager.h"
#include "IsoBox/IsoBoxCache.h"
#include "IsoBox/IsoBox.h"
#include "Game/Item/ItemManager.h"

namespace DsprFrontend
{
    UiManager::UiManager() : Container()
    {
        ///global stuff
        auto g = (Global*) InternalApp::getGlobal();
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

        this->UpdateFunction = [&](float deltaFrame){
            auto newFps = (1000 / deltaFrame);
            this->currentFps = ((this->currentFps*29) + newFps)/30;
            this->step();
        };
    }

    void UiManager::handleItemBarClick(Ref<Point> clickPoint)
    {
        if (Math::PointInBox(clickPoint->x, clickPoint->y, itemBarX, itemBarY, itemBarX + itemBarW, itemBarY + itemBarH))
        {
            //clicking within itembar
            auto g = (Global*) InternalApp::getGlobal();
            auto firstUnit = g->unitManager->getSelectedUnits()->At(0);
            if (firstUnit != nullptr && firstUnit->unitTemplate->hasInventory)
            {
                int ix = 0, iy = 0;
                for(int i=0;i<6;i++)
                {
                    auto itemTemplate = firstUnit->inventory->GetItemAt(i);

                    auto leftX = itemBarX + 6 + (ix * 11);
                    auto upY = itemBarY + 7 + (iy*13);

                    if (Math::PointInBox(g->cursor->position->x, g->cursor->position->y,
                                         leftX, upY,
                                         leftX + 10, upY + 12))
                    {
                        if (g->cursor->isItemInHand())
                        {
                            g->cursor->handleItemPutSlot(firstUnit, i);
                        }
                        else
                        {
                            if (itemTemplate != nullptr) {
                                g->cursor->handleItemClicked(firstUnit, itemTemplate, i);
                                firstUnit->inventory->RemoveItem(i);
                            }
                        }
                    }

                    ix ++; if (ix > 2) {ix=0;iy++;}
                }
            }
        }
    }

    Ref<Button> UiManager::getButtonWithLeftClick(Ref<Point> clickPoint) {

        auto g = (Global *) InternalApp::getGlobal();

        if (Math::PointInBox(clickPoint->x, clickPoint->y, bigPortraitX, bigPortraitY, bigPortraitX + bigPortraitW, bigPortraitY + bigPortraitH)) {
            //clicking within portrait
            if (g->unitManager->getSelectedUnits()->Size() > 0)
            {
                auto firstUnit = g->unitManager->getSelectedUnits()->At(0);
                this->centerCameraOnUnit(firstUnit);
            }
            return Null<Button>();
        }


        if (Math::PointInBox(clickPoint->x, clickPoint->y, armyBarX, armyBarY, armyBarX + armyBarW, armyBarY + armyBarH)) {
            //clicking within armybar
            if (g->unitManager->getSelectedUnits()->Size() == 1) {
                auto firstUnit = g->unitManager->getSelectedUnits()->At(0);
                bool showTraining = (firstUnit->unitTemplate->hasConstructionQueue &&
                                     firstUnit->constructionQueue->isTraining());

                if (showTraining) {
                    int showIcons = 0;
                    int i = 0;
                    int j = 0;
                    int unitTrainingIndex = 0;
                    for (auto iterator = firstUnit->constructionQueue->GetIterator(); iterator->Valid(); iterator->Next()) {
                        auto unitTemplate = iterator->Get();

                        int leftX = 93 + (i * 11);
                        int upY = 116 + (j * 13);
                        auto selecting = Math::PointInBox(g->cursor->position->x, g->cursor->position->y,
                                                          leftX, upY,
                                                          leftX + 10, upY + 12);

                        if (selecting) {
                            g->unitManager->orderCurrentlySelectedUnitsToCancelTrainUnit(unitTrainingIndex);
                            return Null<Button>();
                        }

                        if (showIcons == 0) j += 1;
                        if (showIcons >= 1) i += 1;
                        showIcons += 1;
                        unitTrainingIndex += 1;
                    }
                }
            }
        }

            if (g->unitManager->getSelectedUnits()->Size() > 1) {
                int i = 0;
                int j = 0;
                for (auto iterator = g->unitManager->getSelectedUnits()->GetIterator(); iterator->Valid(); iterator->Next()) {
                    auto unit = iterator->Get();

                    if (unit != nullptr) {

                        auto leftX = armyBarX + 3 + (i * 11);
                        auto upY = armyBarY + 3 + (15 * j);
                        if(Math::PointInBox(g->cursor->position->x, g->cursor->position->y,
                                                          leftX, upY,
                                                          leftX + 10, upY + 12)){
                            //clicked unit!
                            bool shiftPressed = InternalApp::getInternalApp()->keyPressed(Key::LeftShift) ||
                                                InternalApp::getInternalApp()->keyPressed(Key::RightShift);
                            if (shiftPressed)
                            {
                                unit->selected = false;
                                g->unitManager->removeFromSelectionList(unit);
                            }
                            else {
                                g->unitManager->deselectAllUnits();
                                unit->selected = true;
                                g->unitManager->addToSelectionList(unit);
                            }

                            return Null<Button>();
                        }
                    }
                    i++;

                    if (i > 5) {
                        j++;
                        i = 0;
                    }
                }
            }

        if (Math::PointInBox(clickPoint->x, clickPoint->y, 204, 100, 256, 144))
        {
            //clicking on command card
            if (this->currentButtonCard != nullptr)
            {
                auto g = (Global*) InternalApp::getGlobal();
                auto iterator = this->currentButtonCard->buttonList->GetIterator();

                for (int j = 0; j < 3; j += 1)
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
                                button->executeBeginAction();
                                return button;
                            }
                            else
                            {
                                button->executeFinalAction();
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
        if (Math::PointInBox(clickPoint->x, clickPoint->y, portraitBarX, portraitBarY, 204, 144)) return false;
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
                auto g = (Global*) InternalApp::getGlobal();
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
            auto g = (Global*) InternalApp::getGlobal();
            auto gridSize = g->tileManager->getGridSize();
            return New<Point>(
                    ((int) ((((float) clickPoint->x - 5)/38)*gridSize->x))*2,
                    ((int) ((((float) clickPoint->y -103)/38)*gridSize->y))*2
            );
        }

        return Null<Point>();
    }

    Ref<Button> UiManager::getButtonFromKeyboardShortcut() {
        auto g = (Global*) InternalApp::getGlobal();
        if (g->chatManager->isPlayerTyping()) return Null<Button>();

        if (this->currentButtonCard != nullptr) {
            for (auto iterator = this->currentButtonCard->buttonList->GetIterator(); iterator->Valid(); iterator->Next()) {
                auto button = iterator->Get();
                if (button == nullptr) continue;
                if (InternalApp::getInternalApp()->keyPressed(button->keyboardShortcut)){
                    if (!button->needKeyUp) {
                        if (button->requiresClickOnGameArea) {
                            button->executeBeginAction();
                            return button;
                        } else {
                            button->needKeyUp = true;
                            button->executeFinalAction();
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

    void UiManager::step() {
        auto g = (Global*) InternalApp::getGlobal();

        //camera stuff
        if (g->app->keyPressed(Key::Left)) g->camera->position->x -= cameraSpeed;
        if (g->app->keyPressed(Key::Right)) g->camera->position->x += cameraSpeed;
        if (g->app->keyPressed(Key::Up)) g->camera->position->y -= cameraSpeed;
        if (g->app->keyPressed(Key::Down)) g->camera->position->y += cameraSpeed;

        //right clicking?
        if (g->cursor->isInBuildingPlacementState())
        {
        }
        else
        {
            if (g->cursor->isItemInHand())
            {

            }
            else
            {
                if (InternalApp::mouseButtonPressed(MouseButton::Right) && !rightButtonAlreadyClicked) {
                    this->rightButtonAlreadyClicked = true;
                    g->unitManager->issueUnitOrder(false);
                }

                if (!InternalApp::mouseButtonPressed(MouseButton::Right) && rightButtonAlreadyClicked)
                    rightButtonAlreadyClicked = false;
            }
        }
        //chat stuff
        g->chatManager->step();
        if (g->chatManager->isPlayerTyping()) return;

        //camera focus on unit
        if(InternalApp::keyPressed(Key::Space))
        {
            if (g->unitManager->selectionList->Size()>0)
            {
                this->centerCameraOnUnit(g->unitManager->selectionList->At(0));
            }
        }
    }

    void UiManager::centerCameraOnUnit(Ref<DsprFrontend::Unit> unit) {
        auto g = (Global *) InternalApp::getGlobal();
        g->camera->position->x = unit->position->x - (g->camera->width/2);
        g->camera->position->y = unit->position->y - ((g->camera->height - portraitBarH)/2);
    }

    void UiManager::Draw(Ref<Camera> camera, int xoffset, int yoffset)
    {
        auto g = (Global*) InternalApp::getGlobal();

        auto selectedUnitList = g->unitManager->getSelectedUnits();

        //draw rally flags
        if (selectedUnitList->Size() > 0)
        {
            for (auto iterator = selectedUnitList->GetIterator(); iterator->Valid(); iterator->Next()) {
                auto unit = iterator->Get();
                if (unit != nullptr && unit->unitTemplate->hasRallyPoint) {
                    //draw rally point!
                    auto drawX = (int) (((((float) unit->rallyPoint->x) / 2) + 0.5f) * g->tileManager->tileWidth);
                    auto drawY =
                            (int) (((((float) unit->rallyPoint->y) / 2) + 0.5f) * g->tileManager->tileHeight);


                    this->mySprite->useSpriteInfo(g->spriteCatalog->sprRallyFlag);
                    this->mySprite->tint = Color::White;
                    this->mySprite->position->set(drawX, drawY);
                    this->mySprite->drawSelf(camera, xoffset, yoffset);

                    this->mySprite->useSpriteInfo(g->spriteCatalog->sprRallyFlagTC);
                    this->mySprite->tint = unit->tcSprite->tint;
                    this->mySprite->position->set(drawX, drawY);
                    this->mySprite->drawSelf(camera, xoffset, yoffset);
                }
            }
        }

        //building placement state
        if (g->cursor->isInBuildingPlacementState())
        {
            auto worldPoint = g->tileManager->getTilePosition(g->cursor->worldPosition->x, g->cursor->worldPosition->y);

            auto unitIsoBoxBase = g->isoBoxCache->getIsoBox(g->cursor->buildingStateTemplate->tileWidth, g->cursor->buildingStateTemplate->tileHeight);

            this->mySprite->useSpriteInfo(g->spriteCatalog->tile1x1);
            this->mySprite->alpha = 0.5f;
            for (auto iterator = unitIsoBoxBase->coordList->GetIterator(); iterator->Valid(); iterator->Next())
            {
                auto coord = iterator->Get();
                this->mySprite->tint = Color::Green;
                if ((g->unitManager->getUnitAtCoord(worldPoint->x + coord->x, worldPoint->y + coord->y) != nullptr) ||
                        (!g->tileManager->getWalkable(worldPoint->x + coord->x, worldPoint->y + coord->y)) ||
                        (g->itemManager->getItemAtCoord(worldPoint->x + coord->x, worldPoint->y + coord->y)))
                    this->mySprite->tint = Color::Red;
                this->mySprite->position->set(((((float) (worldPoint->x + coord->x)/2) + 0.5f) * g->tileManager->tileWidth)-1-(g->cursor->buildingStateTemplate->sprCenterAdjust->x),
                                              ((((float) (worldPoint->y + coord->y)/2) + 0.5f) * g->tileManager->tileHeight)-(g->cursor->buildingStateTemplate->sprCenterAdjust->y));
                this->mySprite->drawSelf(camera, xoffset, yoffset);
            }
            this->mySprite->alpha = 1.0f;

            this->myAnimatedSprite->useAnimatedSequenceInfo(g->cursor->buildingStateTemplate->sprWalkDown);
            this->myAnimatedSprite->imageIndex = 0;
            this->myAnimatedSprite->tint = Color::White;
            this->myAnimatedSprite->alpha = 0.5f;

            this->myAnimatedSprite->position->x = ((((float) worldPoint->x/2) + 0.5f) * g->tileManager->tileWidth)-(g->cursor->buildingStateTemplate->sprCenterAdjust->x);
            this->myAnimatedSprite->position->y = ((((float) worldPoint->y/2) + 0.5f) * g->tileManager->tileHeight)-(g->cursor->buildingStateTemplate->sprCenterAdjust->y);
            this->myAnimatedSprite->drawSelf(camera, xoffset, yoffset);
            this->myAnimatedSprite->alpha = 1.0f;
        }

        //draw minimap
        this->mySprite->useSpriteInfo(g->spriteCatalog->sprMinimap);
        this->mySprite->tint = Color::White;
        this->mySprite->position->set(0, 100);
        this->mySprite->drawSelf(camera, 0, 0);

        //draw resourcebar
        {
            this->mySprite->useSpriteInfo(g->spriteCatalog->sprResourceBar);
            this->mySprite->tint = Color::White;
            this->mySprite->position->set(205, 0);
            this->mySprite->drawSelf(camera, 0, 0);

            g->graphicsManager->drawNumber(camera, 228, 1, g->economyManager->mana, Color::White, false);
            g->graphicsManager->drawNumber(camera, 242, 1, g->economyManager->pop, Color::White, false);
            g->graphicsManager->drawNumber(camera, 252, 1, g->economyManager->popMax, Color::White, false);
        }

        //draw fps
        g->graphicsManager->drawNumber(camera, 5, 1, (int) this->currentFps, Color::White, false);

        //draw chat bar
        if (g->chatManager->isPlayerTyping()) {
            this->mySprite->useSpriteInfo(g->spriteCatalog->sprChatBar);
            this->mySprite->tint = Color::White;
            this->mySprite->position->set(portraitBarX, portraitBarY - 6);
            this->mySprite->drawSelf(camera, 0, 0);
        }

        Ref<Unit> firstUnit = Null<Unit>();

        {
            if (selectedUnitList->Size() <= 0)
            {
                //draw empty portraitbar
                this->myAnimatedSprite->useAnimatedSpriteInfo(g->spriteCatalog->sprPortraitBar);
                this->myAnimatedSprite->tint = Color::White;
                this->myAnimatedSprite->position->set(portraitBarX, portraitBarY);
                this->myAnimatedSprite->imageIndex = 1;
                this->myAnimatedSprite->drawSelf(camera, 0, 0);

                //draw individual armybar
                this->myAnimatedSprite->useAnimatedSpriteInfo(g->spriteCatalog->sprArmyBar);
                this->myAnimatedSprite->tint = Color::White;
                this->myAnimatedSprite->position->set(armyBarX, armyBarY);
                this->myAnimatedSprite->imageIndex = 0;
                this->myAnimatedSprite->drawSelf(camera, 0, 0);

                //draw empty itembar
                this->myAnimatedSprite->useAnimatedSpriteInfo(g->spriteCatalog->sprItemBar);
                this->myAnimatedSprite->tint = Color::White;
                this->myAnimatedSprite->position->set(itemBarX, itemBarY);
                this->myAnimatedSprite->imageIndex = 1;
                this->myAnimatedSprite->drawSelf(camera, 0, 0);
            }
            else
            {
                firstUnit = selectedUnitList->At(0);
                if (firstUnit != nullptr)
                {
                    {
                        //draw empty portraitbar
                        this->myAnimatedSprite->useAnimatedSpriteInfo(g->spriteCatalog->sprPortraitBar);
                        this->myAnimatedSprite->tint = Color::White;
                        this->myAnimatedSprite->position->set(portraitBarX, portraitBarY);
                        this->myAnimatedSprite->imageIndex = 0;
                        this->myAnimatedSprite->drawSelf(camera, 0, 0);

                        //draw itembar
                        this->myAnimatedSprite->useAnimatedSpriteInfo(g->spriteCatalog->sprItemBar);
                        this->myAnimatedSprite->tint = Color::White;
                        this->myAnimatedSprite->position->set(itemBarX, itemBarY);
                        this->myAnimatedSprite->imageIndex = (firstUnit->unitTemplate->hasInventory) ? 0 : 1;
                        this->myAnimatedSprite->drawSelf(camera, 0, 0);

                        if (firstUnit->unitTemplate->hasInventory)
                        {
                            int ix = 0, iy = 0;
                            for(int i=0;i<6;i++)
                            {
                                auto itemTemplate = firstUnit->inventory->GetItemAt(i);

                                auto leftX = 166 + (ix * 11);
                                auto upY = 116 + (iy*13);
                                Color buttonTint = Color::LightGray;
                                if (Math::PointInBox(g->cursor->position->x, g->cursor->position->y,
                                                     leftX, upY,
                                                     leftX + 10, upY + 12))
                                {
                                    buttonTint = Color::White;
                                }

                                if (itemTemplate != nullptr)
                                {
                                    this->myAnimatedSprite->useAnimatedSpriteInfo(g->spriteCatalog->itemsIcons);
                                    this->myAnimatedSprite->tint = buttonTint;
                                    this->myAnimatedSprite->position->set(leftX, upY);
                                    this->myAnimatedSprite->imageIndex = itemTemplate->index;
                                    this->myAnimatedSprite->drawSelf(camera, 0, 0);
                                }

                                if (g->cursor->isItemInHand() && buttonTint == Color::White)
                                {
                                    //colored box to indicate whether it's okay to put item in hand into new slot
                                    Color boxColor = firstUnit->inventory->CanPlaceInSlot(i, g->cursor->itemInHandTemplate)
                                                     ? Color::Green : Color::Red;

                                    this->myRectangle->position->set(leftX-1, upY-1);
                                    this->myRectangle->setLineStyle(false);
                                    this->myRectangle->setFillStyle(boxColor, 0.2f);
                                    this->myRectangle->size->set(12, 14);
                                    this->myRectangle->drawSelf(camera, 0, 0);
                                }

                                ix ++; if (ix > 2) {ix=0;iy++;}
                            }
                        }
                    }

                    {
                        //portraitbar
                        this->mySprite->useSpriteInfo(firstUnit->unitTemplate->sprBigPortrait);
                        this->mySprite->tint = Color::White;
                        this->mySprite->position->set(bigPortraitX, bigPortraitY);
                        this->mySprite->drawSelf(camera, 0, 0);

//                        this->mySprite->useSpriteInfo(firstUnit->unitTemplate->sprUnitPortraitTC);
//                        this->mySprite->tint = firstUnit->tcSprite->tint;
//                        this->mySprite->position->set(55, 112);
//                        this->mySprite->drawSelf(camera, 0, 0);

                        if (firstUnit->unitTemplate->maxHealth != -1)
                        {
                            g->graphicsManager->drawNumber(camera, 63, 132, firstUnit->health, Color::Green, false);
                            g->graphicsManager->drawChar(camera, 67, 132, '/', Color::Green);
                            g->graphicsManager->drawNumber(camera, 69, 132, firstUnit->unitTemplate->maxHealth, Color::Green, true);
                        }

                        if (firstUnit->unitTemplate->maxStamina != -1)
                        {
                            g->graphicsManager->drawNumber(camera, 63, 138, firstUnit->stamina, Color::White, false);
                            g->graphicsManager->drawChar(camera, 67, 138, '/', Color::White);
                            g->graphicsManager->drawNumber(camera, 69, 138, firstUnit->unitTemplate->maxStamina, Color::White, true);
                        }
                    }

                    //select commandcard
                    this->currentButtonCard = firstUnit->unitTemplate->commandCard;
                }

                if (selectedUnitList->Size() == 1)
                {
                    //draw individual armybar
                    bool showTraining = (firstUnit->unitTemplate->hasConstructionQueue && firstUnit->constructionQueue->isTraining());

                    this->myAnimatedSprite->useAnimatedSpriteInfo(g->spriteCatalog->sprArmyBar);
                    this->myAnimatedSprite->tint = Color::White;
                    this->myAnimatedSprite->position->set(armyBarX, armyBarY);
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

                            int leftX = 93 + (i * 11);
                            int upY = 116 + (j * 13);
                            auto selecting = Math::PointInBox(g->cursor->position->x, g->cursor->position->y,
                                                              leftX, upY,
                                                              leftX + 10, upY + 12);

                            this->mySprite->useSpriteInfo(unitTemplate->sprUnitPortrait);
                            this->mySprite->tint = selecting ? Color::LightGray : Color::White;
                            this->mySprite->position->set(leftX, upY);
                            this->mySprite->drawSelf(camera, 0, 0);

                            if (showIcons == 0) j+=1;
                            if (showIcons >= 1) i+=1;
                            showIcons += 1;
                        }

                        float buildTimeBarLength = (firstUnit->constructionQueue->currentBuildTime /
                                                     firstUnit->constructionQueue->getCurrentTotalBuildTime()) *
                                                    53;
                        if(buildTimeBarLength > 1) {
                            this->myRectangle->position->set(105, 124);
                            this->myRectangle->setLineStyle(1, Color::White);
                            this->myRectangle->setFillStyle(Color::White, 1.0f);

                            this->myRectangle->size->set(buildTimeBarLength, 3);
                            this->myRectangle->drawSelf(camera, 0, 0);
                        }
                    }
                }
                else
                {
                    //draw multi-select armybar
                    this->myAnimatedSprite->useAnimatedSpriteInfo(g->spriteCatalog->sprArmyBar);
                    this->myAnimatedSprite->tint = Color::White;
                    this->myAnimatedSprite->position->set(armyBarX, armyBarY);
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
                            auto leftX = armyBarX + 3 + (i * 11);
                            auto upY = armyBarY + 3 + (15 * j);
                            auto selecting = Math::PointInBox(g->cursor->position->x, g->cursor->position->y,
                                                              leftX, upY,
                                                              leftX + 10, upY + 12);


                            this->mySprite->useSpriteInfo(unit->unitTemplate->sprUnitPortrait);
                            this->mySprite->tint = selecting ? Color::White : Color::LightGray;
                            this->mySprite->position->set(leftX, upY);
                            this->mySprite->drawSelf(camera, 0, 0);

                            this->mySprite->useSpriteInfo(unit->unitTemplate->sprUnitPortraitTC);
                            this->mySprite->tint = selecting ? unit->tcSprite->tint : Color::MixColors(unit->tcSprite->tint, Color::LightGray);
                            this->mySprite->position->set(leftX, upY);
                            this->mySprite->drawSelf(camera, 0, 0);

                            this->myLine->setLineStyle(1, Color::Green);
                            this->myLine->position->set(leftX, upY + 12 + (15 * j));
                            int barLineLength = (unit->health > 1) ? (int) (((float) unit->health / unit->unitTemplate->maxHealth) * 10) : 1;
                            this->myLine->endPosition->set(leftX + barLineLength, upY + 12 + (15 * j));
                            this->myLine->drawSelf(camera, xoffset, yoffset);

                            this->myLine->setLineStyle(1, Color::White);
                            this->myLine->position->y += 1;
                            barLineLength = (unit->stamina >= 0) ? (int) (((float) unit->stamina / unit->unitTemplate->maxStamina) * 10) : 0;
                            this->myLine->endPosition->set(leftX + barLineLength, upY + 13 + (15 * j));
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

            for (int j = 0; j < 3; j += 1)
            {
                for (int i = 0; i < 4; i += 1)
                {
                    if (!iterator->Valid()) break;

                    auto button = iterator->Get();
                    if (button == nullptr) {
                        iterator->Next();
                        continue;
                    }

                    int leftX = 208 + (11 * i);
                    int upY = 103 + 13*j;

                    Color buttonTint = Color::LightGray;

                    if (!g->cursor->isItemInHand() && Math::PointInBox(g->cursor->position->x, g->cursor->position->y,
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
}