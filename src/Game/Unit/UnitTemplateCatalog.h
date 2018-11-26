#pragma once

//
// Created by connor on 9/12/18.
//

#include <Sova/References/Refable.h>
#include <Sova/References/Ref.h>
#include <Sova/Common/List.h>
#include "UnitTemplate.h"

using namespace Sova;

namespace DsprFrontend {
    class UnitTemplateCatalog : public Sova::Refable {
    public:
        UnitTemplateCatalog();
        Ref<UnitTemplate> worker = Null<UnitTemplate>();
        Ref<UnitTemplate> templeBuilding = Null<UnitTemplate>();
        Ref<UnitTemplate> templeFlying = Null<UnitTemplate>();
        Ref<UnitTemplate> manafount = Null<UnitTemplate>();
        Ref<UnitTemplate> ashwalker = Null<UnitTemplate>();

        Ref<UnitTemplate> findTemplateByIndex(int index);
    private:
        Ref<List<UnitTemplate>> templateList = Null<List<UnitTemplate>>();

        void initWorker();
        void initTempleBuilding();
        void initTempleFlying();
        void initManafount();
        void initAshwalker();
    };
}