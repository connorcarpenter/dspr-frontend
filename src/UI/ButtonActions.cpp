//
// Created by connor on 9/10/18.
//

#include <Sova/Internal/InternalApp.h>
#include "ButtonActions.h"

namespace DsprFrontend
{
    ButtonActions::ButtonActions()
    {
        this->g = (Global*) Sova::InternalApp::getSovaApp()->getGlobal();
    }

    void ButtonActions::moveAction() {
        int i = 12;
    }

    void ButtonActions::attackAction() {
        int i = 12;
    }

    void ButtonActions::holdAction() {
        int i = 12;
    }

    void ButtonActions::stopAction() {
        int i = 12;
    }
}