//
// Created by connor on 7/26/18.
//

#include <Sova/Internal/InternalApp.h>
#include "Sova/Math/Math.h"
#include "Sova/Graphics/Color.h"
#include <Sova/Audio/Sound.h>

#include "Game/Global.h"
#include "Cursor.h"
#include "UiManager.h"
#include "Game/Unit/Unit.h"
#include "Game/Unit/UnitManager.h"
#include "Game/TileManager.h"
#include "ButtonCardCatalog.h"
#include "DsprColors.h"
#include "Game/ItemManager.h"
#include "Game/Item.h"

#include <Modules/Gfx/private/glfw/glfwDisplayMgr.h>

#if ORYOL_EMSCRIPTEN
#include <emscripten/html5.h>
#include <emscripten.h>
#endif
#if ORYOL_LINUX
#include "GLFW/glfw3.h"
#endif

namespace DsprFrontend
{

    Cursor::Cursor() : AnimatedSprite()
    {
        this->setTexture(New<Sova::String>("images/ui/cursor.png"));
        this->frameWidth = 15;
        this->frameHeight = 16;
        this->padding = 1;
        this->OnUpdate([&](float deltaFrameMs){ step(); });
        this->imageSpeed = 0;
        this->imageIndex = 1;
        this->leftButtonDragPoint = New<Point>(0,0);
        this->selectionBox = New<Sova::Rectangle>(0,0);
        this->selectionBox->setLineStyle(1, Color::Green, 0.8f);
        this->selectionBox->setFillStyle(Color::Green, 0.5f);
        this->hoverList = New<List<Unit>>();
        this->worldPosition = New<Point>(0,0);

        //this->tint = DsprColors::Yellow;
        this->anchor->x = 8;
        this->anchor->y = 7;

#if ORYOL_LINUX
        glfwSetInputMode(Oryol::_priv::glfwDisplayMgr::getGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#endif
#if ORYOL_EMSCRIPTEN
        emscripten_hide_mouse();
#endif
    }

    void Cursor::step()
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        this->position->x = (InternalApp::getInternalApp()->getMouseX() / 5);
        this->position->y = (InternalApp::getInternalApp()->getMouseY() / 5);

        this->worldPosition->x = this->position->x  + g->camera->position->x;
        this->worldPosition->y = this->position->y  + g->camera->position->y;

        this->tint = DsprColors::Yellow;

        //Hovering over units
        if (this->leftButtonDragging) {
            //get new non-hovering units
            auto nonHoveringUnits = g->unitManager->getNonHoveringUnitsWithinBox(
                    this->leftButtonDragPoint->x + g->camera->position->x,
                    this->leftButtonDragPoint->y + g->camera->position->y,
                    this->worldPosition->x, this->worldPosition->y);

            //currently hovered units, if outside the box, should be set back to non-hovering
            auto unitsOutsideBox = g->unitManager->getUnitsOutsideBox(this->hoverList,
                                                                      this->leftButtonDragPoint->x +
                                                                      g->camera->position->x,
                                                                      this->leftButtonDragPoint->y +
                                                                      g->camera->position->y,
                                                                      this->worldPosition->x,
                                                                      this->worldPosition->y);

            if (unitsOutsideBox != nullptr && unitsOutsideBox->Size() > 0) {
                for (auto iterator = unitsOutsideBox->GetIterator(); iterator->Valid(); iterator->Next()) {
                    auto unit = iterator->Get();
                    unit->hovering = false;
                    this->hoverList->Remove(unit);
                }
            }

            if (nonHoveringUnits != nullptr && nonHoveringUnits->Size() > 0) {
                for (auto iterator = nonHoveringUnits->GetIterator(); iterator->Valid(); iterator->Next()) {
                    auto unit = iterator->Get();
                    unit->hovering = true;
                    this->hoverList->Add(unit);
                }
            }
        } else {

            auto unitHovering = g->unitManager->getUnitOverlappingWithPoint(this->worldPosition->x,
                                                                            this->worldPosition->y);

            if (unitHovering == nullptr) {

                auto itemHovering = g->itemManager->getItemOverlappingWithPoint(this->worldPosition->x,
                                                                                this->worldPosition->y);

                if (this->cursorIsHovering) {

                    if (itemHovering == nullptr)
                    {
                        this->cursorIsHovering = false;
                        this->imageIndex = 1;
                        if (this->lastHoveringItem != nullptr)
                        {
                            this->lastHoveringItem->hovering = false;
                            this->lastHoveringItem = nullptr;
                        }
                    }

                    if (this->hoverList->Size() > 0)
                    {
                        this->setHoverListUnitsToHover(false);
                        this->hoverList->Clear();
                    }
                }

                if (itemHovering != nullptr)
                {
                    this->tint = DsprColors::Yellow;

                    if (this->lastHoveringItem != nullptr && this->lastHoveringItem != itemHovering)
                        this->lastHoveringItem->hovering = false;

                    itemHovering->hovering = true;
                    this->lastHoveringItem = itemHovering;

                    this->cursorIsHovering = true;

                    if(g->unitManager->selectionList->Size() > 0) {
                        this->imageIndex = 3;
                    } else {
                        this->imageIndex = 0;
                    }
                }
            }
            else
            {
                if (this->lastHoveringItem != nullptr)
                {
                    this->lastHoveringItem->hovering = false;
                    this->lastHoveringItem = nullptr;
                    this->imageIndex = 0;
                }

                this->tint = (unitHovering->tribeIndex == g->playersTribeIndex) ? DsprColors::Green : DsprColors::Red;
                if (unitHovering->tribeIndex == -1) this->tint = DsprColors::Yellow;

                if (this->cursorIsHovering)
                {
                    if (!this->hoverList->Contains(unitHovering)){

                        if (this->hoverList->Size() > 0) {
                            this->setHoverListUnitsToHover(false);
                        }

                        this->hoverList->Clear();
                        this->hoverList->Add(unitHovering);
                        this->setHoverListUnitsToHover(true);
                    }
                }
                else
                {
                    this->imageIndex = 0;
                    this->cursorIsHovering = true;
                    this->hoverList->Clear();
                    this->hoverList->Add(unitHovering);
                    this->setHoverListUnitsToHover(true);
                }

            }
        }

        if (this->buttonOrder == nullptr) {

            this->buttonOrder = g->uiManager->getButtonFromKeyboardShortcut();

            bool leftButtonPressed = InternalApp::getInternalApp()->mouseButtonPressed(MouseButton::Left);
            //Left button clicking + dragging

            if (leftButtonPressed) {
                leftButtonPressed = g->uiManager->captureLeftClickMinimapEvent(this->position);
            }

            if (this->ignoreNextLeftButtonClicked > 0){
                this->ignoreNextLeftButtonClicked -= 1;
                leftButtonPressed = false;
            }

            if (this->leftButtonDragging) {
                if (!leftButtonPressed) {
                    ///group selection event
                    this->leftButtonDragging = false;
                    bool shiftPressed = InternalApp::getInternalApp()->keyPressed(Key::LeftShift) ||
                                        InternalApp::getInternalApp()->keyPressed(Key::RightShift);
                    if (!shiftPressed)
                        g->unitManager->deselectAllUnits();
                    this->setHoverListUnitsToSelected(true, false);
                    this->setHoverListUnitsToHover(false);
                    this->hoverList->Clear();

                    if (this->lastSelectedItem != nullptr)
                    {
                        this->lastSelectedItem->selected = false;
                        this->lastSelectedItem = nullptr;
                    }
                }
            } else {
                if (leftButtonPressed) {
                    if (this->leftButtonPressedTime == 0)
                        this->leftButtonDragPoint->set(this->position->x, this->position->y);

                    if (this->leftButtonPressedTime < this->leftButtonPressedTimeToDrag)
                        this->leftButtonPressedTime += 1;
                } else {
                    if (this->leftButtonPressedTime > 0) {

                        bool shiftPressed = InternalApp::getInternalApp()->keyPressed(Key::LeftShift) ||
                                            InternalApp::getInternalApp()->keyPressed(Key::RightShift);
                        bool ctrlPressed = InternalApp::getInternalApp()->keyPressed(Key::LeftControl) ||
                                           InternalApp::getInternalApp()->keyPressed(Key::RightControl);

                        bool clickedInGameArea = g->uiManager->isInGameArea(this->position);
                        if (clickedInGameArea) {
                            if (this->leftButtonDoubleClickCountdown == 0 && !ctrlPressed) {
                                ///single unit selection event

                                if (!shiftPressed)
                                    g->unitManager->deselectAllUnits();

                                this->setHoverListUnitsToSelected(true, shiftPressed);

                                this->leftButtonDoubleClickCountdown = this->leftButtonDoubleClickWindow;

                                if (this->lastSelectedItem != nullptr)
                                {
                                    this->lastSelectedItem->selected = false;
                                    this->lastSelectedItem = nullptr;
                                }
                                if (this->lastHoveringItem != nullptr)
                                {
                                    this->lastHoveringItem->selected = true;
                                    this->lastSelectedItem = this->lastHoveringItem;
                                }

                            } else {
                                ///its a doubleclick!

                                this->setHoverListUnitsToHover(false);
                                this->hoverList->Clear();

                                //get new non-hovering units
                                auto nonHoveringUnits = g->unitManager->getNonHoveringUnitsWithinBox(
                                        g->camera->position->x,
                                        g->camera->position->y,
                                        g->camera->position->x +
                                        g->camera->width,
                                        g->camera->position->y +
                                        g->camera->height);

                                if (nonHoveringUnits != nullptr && nonHoveringUnits->Size() > 0) {
                                    for (auto iterator = nonHoveringUnits->GetIterator(); iterator->Valid(); iterator->Next()) {
                                        auto unit = iterator->Get();
                                        this->hoverList->Add(unit);
                                    }
                                }
                                if (!shiftPressed)
                                    g->unitManager->deselectAllUnits();
                                this->setHoverListUnitsToSelected(true, false);
                                this->hoverList->Clear();
                            }
                        }
                        else{
                            this->buttonOrder = g->uiManager->getButtonWithLeftClick(this->position);
                        }
                    }
                    this->leftButtonPressedTime = 0;
                }

                if (this->leftButtonDoubleClickCountdown > 0)this->leftButtonDoubleClickCountdown -= 1;

                if (leftButtonPressed && this->leftButtonPressedTime >= this->leftButtonPressedTimeToDrag && g->uiManager->isInGameArea(this->position)) {
                    this->leftButtonDragging = true;
                    this->leftButtonPressedTime = 0;
                }

            }
        } else {
            if (InternalApp::getInternalApp()->mouseButtonPressed(MouseButton::Left)){
                this->buttonOrder->executeAction();
                this->buttonOrder = Null<Button>();
                this->leftButtonPressedTime = 0;
                this->ignoreNextLeftButtonClicked = 10;
                return;
            }
            if (InternalApp::getInternalApp()->mouseButtonPressed(MouseButton::Right)){
                this->buttonOrder = Null<Button>();
                return;
            }
        }
    }

    void Cursor::setHoverListUnitsToHover(bool hovering)
    {
        for (auto iterator = this->hoverList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            unit->hovering = hovering;
        }
    }

    void Cursor::setHoverListUnitsToSelected(bool selected, bool toggle)
    {
        if (this->hoverList->Size() == 0) return;

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        bool playedSoundYet = false;

        for (auto iterator = this->hoverList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            if (unit->tribeIndex != g->playersTribeIndex) continue;

            bool shouldSelect = (toggle) ? (!unit->selected) : selected;

            if (shouldSelect)
            {
                if (g->unitManager->selectionList->Size() >= g->unitManager->maxSelectedUnits) continue;

                unit->selected = true;

                if (!playedSoundYet){
                    playedSoundYet = true;
                    unit->playSelectedSound();
                }

                g->unitManager->addToSelectionList(unit);
            }
            else
            {
                unit->selected = false;

                g->unitManager->removeFromSelectionList(unit);
            }
        }
    }

    void Cursor::drawSelf(Ref<Camera> camera, int xoffset, int yoffset)
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        if (this->buttonOrder != nullptr) {
            this->imageIndex = 2;
        } else {
            //this->imageIndex = this->cursorIsHovering ? 0 : 1;
        }

        AnimatedSprite::drawSelf(camera, xoffset, yoffset);

        if (this->leftButtonDragging)
        {
            this->selectionBox->position->set(this->leftButtonDragPoint->x, this->leftButtonDragPoint->y);

            this->selectionBox->size->set(this->position->x - this->leftButtonDragPoint->x,
                                          this->position->y - this->leftButtonDragPoint->y);
            this->selectionBox->drawSelf(camera, xoffset, yoffset);
        }

        g->unitHoverCircle_1x1->Update(0);
        g->unitHoverCircle_2x2->Update(0);
        g->unitHoverCircle_3x3->Update(0);
        g->unitHoverCircle_5x5->Update(0);
        g->moveMarker->Update(0);
    }
}