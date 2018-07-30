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

    Cursor::Cursor() : AnimatedSprite() {
        this->setTexture(New<Sova::String>("images/cursor.png"));
        this->frameWidth = 14;
        this->frameHeight = 12;
        this->padding = 1;
        this->OnUpdate([&](){ step(); });
        this->imageSpeed = 0;
        this->changeState(1);
        this->leftButtonDragPoint = New<Point>(0,0);
        this->selectionBox = New<Sova::Rectangle>(0,0);
        this->selectionBox->setLineStyle(1, Color::Green, 1.0f);
        this->selectionBox->setFillStyle(Color::Green, 0.25f);



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

        if (this->leftButtonDragging)
        {
            if (!InternalApp::getInternalApp()->mouseButtonPressed(MouseButton::Left))
            {
                this->leftButtonDragging = false;
                //selection event
            }
        }
        else
        {
            if (InternalApp::getInternalApp()->mouseButtonPressed(MouseButton::Left))
            {
                this->leftButtonDragging = true;
                this->leftButtonDragPoint->set(InternalApp::getInternalApp()->getMouseX()/5, InternalApp::getInternalApp()->getMouseY()/5);
            }
        }

    }

    void Cursor::changeState(int index)
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
        if (this->leftButtonDragging)
        {
            this->selectionBox->position->set(this->leftButtonDragPoint->x,
                                              this->leftButtonDragPoint->y);
            this->selectionBox->position->set(this->leftButtonDragPoint->x, this->leftButtonDragPoint->y);
            this->selectionBox->size->set((InternalApp::getMouseX()/5) - this->leftButtonDragPoint->x,
                                          (InternalApp::getMouseY()/5) - this->leftButtonDragPoint->y);
            this->selectionBox->drawSelf(camera, xoffset, yoffset);
        }

        AnimatedSprite::drawSelf(camera, xoffset, yoffset);
    }
}