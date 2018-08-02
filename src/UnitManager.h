#pragma once

//
// Created by connor on 7/31/18.
//

#include <Sova/References/Refable.h>
#include "Sova/Common/Int.h"

#include "Unit.h"

namespace DsprFrontend
{
    class UnitManager : public Sova::Refable
    {
    public:
        UnitManager();
        void receiveUnit(Ref<Sova::String> id, Ref<Sova::String> x, Ref<Sova::String> y);

        void addToSelectionList(int id);
        void clearSelectionList();

        void step();

    private:
        Ref<List<Unit>> unitList = Null<List<Unit>>();
        Ref<List<Sova::Int>> selectionList = Null<List<Sova::Int>>();
        bool rightButtonAlreadyClicked = false;

    };
}
