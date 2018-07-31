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

        void addToUnitList(Ref<Unit> unit);
        void addToSelectionList(int id);
        void clearSelectionList();

    private:
        Ref<List<Unit>> unitList = Null<List<Unit>>();
        Ref<List<Int>> selectionList = Null<List<Int>>();

        void step();
    };
}
