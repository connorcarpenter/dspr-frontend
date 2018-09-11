//
// Created by connor on 7/26/18.
//

#include <Sova/Internal/InternalApp.h>
#include "Sova/Math/Math.h"
#include "Sova/Graphics/Color.h"
#include <Sova/Audio/Sound.h>

#include "Global.h"
#include "Cursor.h"
#include "UiManager.h"
#include "Unit.h"
#include "UnitManager.h"
#include "TileManager.h"

#include <Modules/Gfx/private/glfw/glfwDisplayMgr.h>

#if ORYOL_EMSCRIPTEN
#include <emscripten/html5.h>
#include <emscripten.h>
#endif
#if ORYOL_LINUX
#include "GLFW/glfw3.h"
#include "DsprColors.h"

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
        this->helloSound = New<Sound>(New<Sova::String>("sounds/hello.wav"));
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

            if (unitHovering == nullptr && this->cursorIsHovering) {
                this->cursorIsHovering = false;
                this->imageIndex = 1;
                if (this->hoverList->Size() > 0) {
                    this->setHoverListUnitsToHover(false);
                    this->hoverList->Clear();
                }
            }

            if (unitHovering != nullptr) {
                this->tint = (unitHovering->tribeIndex == g->playersTribeIndex) ? DsprColors::Green : DsprColors::Red;

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

        if (!this->attackOrderSelected) {

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
                    this->setHoverListUnitsToSelected(true);
                    this->setHoverListUnitsToHover(false);
                    this->hoverList->Clear();
                }
            } else {
                if (InternalApp::getInternalApp()->keyPressed(Key::A))
                    this->attackOrderSelected = true;

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

                        bool clickedOnGameArea = g->uiManager->captureLeftClickEvent(this->position);

                        if (clickedOnGameArea) {
                            if (this->leftButtonDoubleClickCountdown == 0 && !ctrlPressed) {
                                ///single unit selection event

                                if (!shiftPressed)
                                    g->unitManager->deselectAllUnits();
                                if (!shiftPressed) {
                                    if (this->hoverList->Size() > 0)
                                        this->helloSound->Play();
                                    this->setHoverListUnitsToSelected(true);
                                } else {
                                    this->toggleHoverListUnitsSelected();
                                }

                                this->leftButtonDoubleClickCountdown = this->leftButtonDoubleClickWindow;
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
                                this->setHoverListUnitsToSelected(true);
                                this->hoverList->Clear();
                            }
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
                this->attackOrderSelected = false;
                g->unitManager->issueUnitOrder(true);
                this->leftButtonPressedTime = 0;
                this->ignoreNextLeftButtonClicked = 10;
                return;
            }
            if (InternalApp::getInternalApp()->mouseButtonPressed(MouseButton::Right)){
                this->attackOrderSelected = false;
                return;
            }
        }
    }

    Ref<Point> Cursor::getTilePosition()
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        int halfTileW = (g->tileManager->tileWidth/2);
        int halfTileH = (g->tileManager->tileHeight/2);
        int quarterTileW = (g->tileManager->tileWidth/4);
        int quarterTileH = (g->tileManager->tileHeight/4);

        int posx = this->worldPosition->x - quarterTileW;
        int posy = this->worldPosition->y - quarterTileH;

        int x = (posx) / halfTileW;
        int y = (posy) / halfTileH;
        if ((x % 2 == 0 && y % 2 == 0) || ((x+1) % 2 == 0 && (y+1) % 2 == 0)) return New<Point>(x, y);
        int modx = (posx % halfTileW) - quarterTileW;
        int mody = ((posy % halfTileH) - quarterTileH)*2;
        if (Sova::Math::Abs(modx) > Sova::Math::Abs(mody))
        {
            return New<Point>(x+Sova::Math::Sign(modx), y);
        }
        else
        {
            return New<Point>(x, y+Sova::Math::Sign(mody));
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

    void Cursor::setHoverListUnitsToSelected(bool selected)
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        for (auto iterator = this->hoverList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            if (unit->tribeIndex != g->playersTribeIndex) continue;
            if (g->unitManager->selectionList->Size() < g->unitManager->maxSelectedUnits)
            unit->selected = selected;
            if (selected)
            {
                g->unitManager->addToSelectionList(unit);
            }
            else
            {
                g->unitManager->removeFromSelectionList(unit);
            }
        }
    }

    void Cursor::toggleHoverListUnitsSelected() {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        for (auto iterator = this->hoverList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            if (unit->tribeIndex != g->playersTribeIndex) continue;
            unit->selected = !unit->selected;
            if (unit->selected)
            {
                g->unitManager->addToSelectionList(unit);
            }
            else
            {
                g->unitManager->removeFromSelectionList(unit);
            }
        }
    }

    void Cursor::drawSelf(Ref<Camera> camera, int xoffset, int yoffset)
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        if (this->attackOrderSelected) {
            this->imageIndex = 2;
        } else {
            this->imageIndex = this->cursorIsHovering ? 0 : 1;
        }

        AnimatedSprite::drawSelf(camera, xoffset, yoffset);

        if (this->leftButtonDragging)
        {
            this->selectionBox->position->set(this->leftButtonDragPoint->x, this->leftButtonDragPoint->y);

            this->selectionBox->size->set(this->position->x - this->leftButtonDragPoint->x,
                                          this->position->y - this->leftButtonDragPoint->y);
            this->selectionBox->drawSelf(camera, xoffset, yoffset);
        }

        g->unitHoverCircle->Update(0);
        g->moveMarker->Update(0);
    }
}