//
// Created by connor on 7/26/18.
//

#include <Sova/Internal/OryolApp.h>
#include <Modules/Gfx/private/glfw/glfwDisplayMgr.h>
#include "Cursor.h"
#include "Sova/Common/String.h"

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

#if ORYOL_LINUX
        glfwSetInputMode(Oryol::_priv::glfwDisplayMgr::getGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#endif
#if ORYOL_EMSCRIPTEN
        emscripten_hide_mouse();
#endif
    }

    void Cursor::step() {

        this->position->x = (OryolApp::getOryolApp()->getMouseX() / 5);
        this->position->y = (OryolApp::getOryolApp()->getMouseY() / 5);

        //this->position->x = 40;
        //this->position->y = 60;
    }

    void Cursor::changeState(int index) {
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
}