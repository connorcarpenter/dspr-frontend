//
// Created by connor on 7/26/18.
//

#include <Sova/Internal/OryolApp.h>
#include "Global.h"
#include "Sova/Math/Math.h"

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
        this->selectionBoxGraphic = New<Graphic>();



#if ORYOL_LINUX
        glfwSetInputMode(Oryol::_priv::glfwDisplayMgr::getGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#endif
#if ORYOL_EMSCRIPTEN
        emscripten_hide_mouse();
#endif
    }

    void Cursor::step()
    {
        auto g = (Global*) OryolApp::getSovaApp()->getGlobal();
        this->position->x = (OryolApp::getOryolApp()->getMouseX() / 5) + g->camera->position->x;
        this->position->y = (OryolApp::getOryolApp()->getMouseY() / 5) + g->camera->position->y;

        if (this->leftButtonDragging)
        {
            if (!OryolApp::getOryolApp()->mouseButtonPressed(MouseButton::Left))
            {
                this->leftButtonDragging = false;
                //selection event
            }
        }
        else
        {
            if (OryolApp::getOryolApp()->mouseButtonPressed(MouseButton::Left))
            {
                this->leftButtonDragging = true;
                this->leftButtonDragPoint->set(OryolApp::getOryolApp()->getMouseX(), OryolApp::getOryolApp()->getMouseY());
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
        auto g = (Global*) OryolApp::getSovaApp()->getGlobal();
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
            this->selectionBoxGraphic->clear();
            this->selectionBoxGraphic->lineStyle(1, Color::Green, 1.0f);
            this->selectionBoxGraphic->beginFill(Color::Green, 0.5f);
            this->selectionBoxGraphic->drawRect(this->leftButtonDragPoint->x,
                                                this->leftButtonDragPoint->y,
                                                OryolApp::getOryolApp()->getMouseX(),
                                                OryolApp::getOryolApp()->getMouseY());

            this->selectionBoxGraphic->drawSelf(camera, xoffset, yoffset);
        }

        AnimatedSprite::drawSelf(camera, xoffset, yoffset);
    }
}