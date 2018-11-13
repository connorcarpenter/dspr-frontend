#pragma once

//
// Created by connor on 11/13/18.
//

#include <Sova/References/Refable.h>
#include <Sova/References/Ref.h>
#include <Sova/Common/List.h>
#include "ItemTemplate.h"

using namespace Sova;

namespace DsprFrontend {
    class ItemTemplateCatalog : public Sova::Refable {
    public:
        ItemTemplateCatalog();
        Ref<ItemTemplate> sling = Null<ItemTemplate>();
        Ref<ItemTemplate> club = Null<ItemTemplate>();
        Ref<ItemTemplate> helmet = Null<ItemTemplate>();
        Ref<ItemTemplate> shield = Null<ItemTemplate>();
        Ref<ItemTemplate> armor = Null<ItemTemplate>();

        Ref<ItemTemplate> findTemplateByIndex(int index);
    private:
        Ref<List<ItemTemplate>> templateList = Null<List<ItemTemplate>>();
    };
}