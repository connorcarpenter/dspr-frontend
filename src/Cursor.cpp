//
// Created by connor on 7/26/18.
//

#include <Sova/Internal/InternalApp.h>
#include "Global.h"
#include "Sova/Math/Math.h"
#include "Sova/Graphics/Color.h"

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
        this->setTexture(New<Sova::String>("images/cursor.png"));
        this->frameWidth = 14;
        this->frameHeight = 12;
        this->padding = 1;
        this->OnUpdate([&](float deltaFrameMs){ step(); });
        this->imageSpeed = 0;
        this->setImageIndex(1);
        this->leftButtonDragPoint = New<Point>(0,0);
        this->selectionBox = New<Sova::Rectangle>(0,0);
        this->selectionBox->setLineStyle(1, Color::Green, 0.6f);
        this->selectionBox->setFillStyle(Color::Green, 0.2f);
        this->hoverList = New<List<Unit>>();

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
        this->position->x = (InternalApp::getInternalApp()->getMouseX() / 5) + g->camera->position->x;
        this->position->y = (InternalApp::getInternalApp()->getMouseY() / 5) + g->camera->position->y;

        //Hovering over units
        if (this->leftButtonDragging)
        {
            //get new non-hovering units
            auto nonHoveringUnits = g->unitManager->getNonHoveringUnitsWithinBox(this->leftButtonDragPoint->x, this->leftButtonDragPoint->y,
                                                                                 this->position->x, this->position->y);

            //currently hovered units, if outside the box, should be set back to non-hovering
            auto unitsOutsideBox = g->unitManager->getUnitsOutsideBox(this->hoverList,
                                                                      this->leftButtonDragPoint->x, this->leftButtonDragPoint->y,
                                                                      this->position->x, this->position->y);

            if (unitsOutsideBox != nullptr && unitsOutsideBox->Size() > 0)
            {
                for (auto iterator = unitsOutsideBox->GetIterator(); iterator->Valid(); iterator->Next())
                {
                    auto unit = iterator->Get();
                    unit->hovering = false;
                    this->hoverList->Remove(unit);
                }
            }

            if (nonHoveringUnits != nullptr && nonHoveringUnits->Size() > 0)
            {
                for (auto iterator = nonHoveringUnits->GetIterator(); iterator->Valid(); iterator->Next())
                {
                    auto unit = iterator->Get();
                    unit->hovering = true;
                    this->hoverList->Add(unit);
                }
            }
        }
        else
        {
            auto unitHovering = g->unitManager->getUnitOverlappingWithPoint(this->position->x, this->position->y);
            bool cursorIsHovering = (imageIndex == 0);

            if (unitHovering == nullptr && cursorIsHovering)
            {
                this->setImageIndex(1);
                if (this->hoverList->Size() > 0)
                {
                    this->setHoverListUnitsToHover(false);
                    this->hoverList->Clear();
                }
            }

            if (unitHovering != nullptr && !cursorIsHovering)
            {
                this->setImageIndex(0);
                this->hoverList->Clear();
                this->hoverList->Add(unitHovering);
                this->setHoverListUnitsToHover(true);
            }
        }

        //Left button clicking + dragging
        if (this->leftButtonDragging)
        {
            if (!InternalApp::getInternalApp()->mouseButtonPressed(MouseButton::Left))
            {
                ///group selection event
                this->leftButtonDragging = false;
                bool shiftPressed = InternalApp::getInternalApp()->keyPressed(Key::LeftShift) || InternalApp::getInternalApp()->keyPressed(Key::RightShift);
                if (!shiftPressed)
                    g->unitManager->deselectAllUnits();
                this->setHoverListUnitsToSelected(true);
                this->setHoverListUnitsToHover(false);
                this->hoverList->Clear();
            }
        }
        else
        {
            bool leftButtonPressed = InternalApp::getInternalApp()->mouseButtonPressed(MouseButton::Left);
            if (leftButtonPressed)
            {
                if (this->leftButtonPressedTime == 0)
                    this->leftButtonDragPoint->set(this->position->x, this->position->y);

                if (this->leftButtonPressedTime < this->leftButtonPressedTimeToDrag) this->leftButtonPressedTime += 1;
            }
            else
            {
                if (this->leftButtonPressedTime > 0)
                {
                    bool shiftPressed = InternalApp::getInternalApp()->keyPressed(Key::LeftShift) ||
                                        InternalApp::getInternalApp()->keyPressed(Key::RightShift);
                    bool ctrlPressed = InternalApp::getInternalApp()->keyPressed(Key::LeftControl) ||
                                        InternalApp::getInternalApp()->keyPressed(Key::RightControl);
                    if (this->leftButtonDoubleClickCountdown == 0 && !ctrlPressed)
                    {
                        ///single unit selection event

                        if (!shiftPressed)
                            g->unitManager->deselectAllUnits();
                        if (!shiftPressed) {
                            this->setHoverListUnitsToSelected(true);
                        } else {
                            this->toggleHoverListUnitsSelected();
                        }

                        this->leftButtonDoubleClickCountdown = this->leftButtonDoubleClickWindow;
                    }
                    else
                    {
                        ///its a doubleclick!

                        this->setHoverListUnitsToHover(false);
                        this->hoverList->Clear();

                        //get new non-hovering units
                        auto nonHoveringUnits = g->unitManager->getNonHoveringUnitsWithinBox(g->camera->position->x, g->camera->position->y,
                                                                                             g->camera->position->x + g->camera->width, g->camera->position->y + g->camera->height);

                        if (nonHoveringUnits != nullptr && nonHoveringUnits->Size() > 0)
                        {
                            for (auto iterator = nonHoveringUnits->GetIterator(); iterator->Valid(); iterator->Next())
                            {
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
                this->leftButtonPressedTime = 0;
            }

            if (this->leftButtonDoubleClickCountdown > 0)this->leftButtonDoubleClickCountdown -= 1;

            if (leftButtonPressed && this->leftButtonPressedTime >= this->leftButtonPressedTimeToDrag)
            {
                this->leftButtonDragging = true;
                this->leftButtonPressedTime = 0;
            }
        }
    }

    void Cursor::setImageIndex(int index)
    {
        this->imageIndex = index;
        if (this->imageIndex == 1)
        {
            this->anchor->x = 0;
            this->anchor->y = 0;
        }
        if (this->imageIndex == 0)
        {
            this->anchor->x = 6;
            this->anchor->y = 5;
        }
    }

    Ref<Point> Cursor::getTilePosition()
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        int halfTileW = (g->tileManager->tileWidth/2);
        int halfTileH = (g->tileManager->tileHeight/2);
        int quarterTileW = (g->tileManager->tileWidth/4);
        int quarterTileH = (g->tileManager->tileHeight/4);

        int posx = this->position->x - quarterTileW;
        int posy = this->position->y - quarterTileH;

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

    void Cursor::drawSelf(Ref<Camera> camera, int xoffset, int yoffset)
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        if (this->leftButtonDragging)
        {
            this->selectionBox->position->set(this->leftButtonDragPoint->x, this->leftButtonDragPoint->y);

            this->selectionBox->size->set(this->position->x - this->leftButtonDragPoint->x,
                                          this->position->y - this->leftButtonDragPoint->y);
            this->selectionBox->drawSelf(camera, xoffset, yoffset);
        }

        g->unitHoverCircle->Update(0);
        g->moveMarker->Update(0);
        AnimatedSprite::drawSelf(camera, xoffset, yoffset);
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
            unit->selected = selected;
            if (selected)
            {
                g->unitManager->addToSelectionList(unit->id);
            }
            else
            {
                g->unitManager->removeFromSelectionList(unit->id);
            }
        }
    }

    void Cursor::toggleHoverListUnitsSelected() {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        for (auto iterator = this->hoverList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            unit->selected = !unit->selected;
            if (unit->selected)
            {
                g->unitManager->addToSelectionList(unit->id);
            }
            else
            {
                g->unitManager->removeFromSelectionList(unit->id);
            }
        }
    }
}