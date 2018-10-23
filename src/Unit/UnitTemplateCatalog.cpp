//
// Created by connor on 9/12/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Global.h>
#include "UnitTemplateCatalog.h"

namespace DsprFrontend {

    UnitTemplateCatalog::UnitTemplateCatalog()
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        this->worker = New<UnitTemplate>(0);
        this->worker->sight = 6;
        this->worker->hitSound = New<Sound>(New<Sova::String>("sounds/hit.wav"));
        this->worker->selectedSound = New<Sound>(New<Sova::String>("sounds/hello.wav"));
        this->worker->dieSound = New<Sound>(New<Sova::String>("sounds/die.wav"));
        this->worker->sprSelectCircle = g->unitSelectCircle_1x1;
        this->worker->sprHoverCircle = g->unitHoverCircle_1x1;
        this->worker->hasIdleTurnBehavior = true;
        this->worker->spriteFaceLeftXoffset = -2;

        this->temple = New<UnitTemplate>(1);
        this->temple->sight = 12;
        this->temple->sprSelectCircle = g->unitSelectCircle_5x5;
        this->temple->sprHoverCircle = g->unitHoverCircle_5x5;
        this->temple->canMove = false;

        this->templateList = New<List<UnitTemplate>>();
        this->templateList->Add(this->worker);
        this->templateList->Add(this->temple);
    }

    Ref<UnitTemplate> UnitTemplateCatalog::findTemplateByIndex(int index)
    {
        return this->templateList->Find([&](Ref<UnitTemplate> unitTemplate){
            return unitTemplate->index == index;
        });
    }
}