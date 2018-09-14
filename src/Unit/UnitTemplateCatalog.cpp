//
// Created by connor on 9/12/18.
//

#include "UnitTemplateCatalog.h"

namespace DsprFrontend {

    UnitTemplateCatalog::UnitTemplateCatalog() {
        this->worker = New<UnitTemplate>(0);
        this->temple = New<UnitTemplate>(1);

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