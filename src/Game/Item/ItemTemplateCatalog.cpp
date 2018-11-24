//
// Created by connor on 11/13/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include "ItemTemplateCatalog.h"
#include "SpriteCatalog.h"

namespace DsprFrontend {

    ItemTemplateCatalog::ItemTemplateCatalog() {
        this->templateList = New<List<ItemTemplate>>();

        auto g = (Global*) InternalApp::getGlobal();

        this->sling = New<ItemTemplate>(0);
        this->sling->wornOn = BodyPart::Hand;
        this->sling->leftHanded = true;
        this->sling->wornSpriteInfo = g->spriteCatalog->workerSling;
        this->sling->useSound = New<Sound>(New<Sova::String>("sounds/bowfire.wav"));
        this->templateList->Add(this->sling);

        this->club = New<ItemTemplate>(2);
        this->club->wornOn = BodyPart::Hand;
        this->club->leftHanded = true;
        this->club->wornSpriteInfo = g->spriteCatalog->workerClub;
        this->templateList->Add(this->club);

        this->helmet = New<ItemTemplate>(3);
        this->helmet->wornOn = BodyPart::Head;
        this->helmet->wornSpriteInfo = g->spriteCatalog->workerHelmet;
        this->templateList->Add(this->helmet);

        this->shield = New<ItemTemplate>(4);
        this->shield->wornOn = BodyPart::Hand;
        this->shield->leftHanded = false;
        this->shield->wornSpriteInfo = g->spriteCatalog->workerShield;
        this->templateList->Add(this->shield);

        this->armor = New<ItemTemplate>(5);
        this->armor->wornOn = BodyPart::Body;
        this->armor->wornSpriteInfo = g->spriteCatalog->workerArmor;
        this->templateList->Add(this->armor);
    }

    Ref<ItemTemplate> ItemTemplateCatalog::findTemplateByIndex(int index)
    {
        return this->templateList->Find([&](Ref<ItemTemplate> itemTemplate){
            return itemTemplate->index == index;
        });
    }
}