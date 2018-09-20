//
// Created by connor on 9/12/18.
//

#include "UnitTemplateCatalog.h"

namespace DsprFrontend {

    UnitTemplateCatalog::UnitTemplateCatalog() {
        this->worker = New<UnitTemplate>(0);
        this->worker->sight = 6;
        this->worker->hitSound = New<Sound>(New<Sova::String>("sounds/hit.wav"));
        this->worker->selectedSound = New<Sound>(New<Sova::String>("sounds/hello.wav"));
        this->worker->dieSound = New<Sound>(New<Sova::String>("sounds/die.wav"));

        this->temple = New<UnitTemplate>(1);
        this->temple->sight = 12;

        this->templateList = New<List<UnitTemplate>>();
        this->templateList->Add(this->worker);
        this->templateList->Add(this->temple);
    }

    Ref<UnitTemplate> UnitTemplateCatalog::findTemplateByIndex(int index) {
        return this->templateList->Find([&](Ref<UnitTemplate> unitTemplate){
            return unitTemplate->index == index;
        });
    }
}