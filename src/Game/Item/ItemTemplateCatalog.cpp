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
        this->sling->wornOn = BodyPart::Hand;
        this->templateList->Add(this->sling);

        this->club = New<ItemTemplate>(2);
        this->club->wornOn = BodyPart::Hand;
        this->templateList->Add(this->club);

        this->helmet = New<ItemTemplate>(3);
        this->helmet->wornOn = BodyPart::Head;
        this->templateList->Add(this->helmet);

        this->shield = New<ItemTemplate>(4);
        this->shield->wornOn = BodyPart::Hand;
        this->templateList->Add(this->shield);

        this->armor = New<ItemTemplate>(5);
        this->armor->wornOn = BodyPart::Body;
        this->templateList->Add(this->armor);
    }

    Ref<ItemTemplate> ItemTemplateCatalog::findTemplateByIndex(int index)
    {
        return this->templateList->Find([&](Ref<ItemTemplate> itemTemplate){
            return itemTemplate->index == index;
        });
    }
}