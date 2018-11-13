//
// Created by connor on 11/13/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include "ItemTemplateCatalog.h"

namespace DsprFrontend {

    ItemTemplateCatalog::ItemTemplateCatalog() {
        this->templateList = New<List<ItemTemplate>>();

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        this->sling = New<ItemTemplate>(0);
        this->templateList->Add(this->sling);

        this->club = New<ItemTemplate>(2);
        this->templateList->Add(this->club);

        this->helmet = New<ItemTemplate>(3);
        this->templateList->Add(this->helmet);

        this->shield = New<ItemTemplate>(4);
        this->templateList->Add(this->shield);

        this->armor = New<ItemTemplate>(5);
        this->templateList->Add(this->armor);
    }

    Ref<ItemTemplate> ItemTemplateCatalog::findTemplateByIndex(int index)
    {
        return this->templateList->Find([&](Ref<ItemTemplate> itemTemplate){
            return itemTemplate->index == index;
        });
    }
}