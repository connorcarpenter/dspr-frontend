#pragma once

//
// Created by connor on 7/31/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/Pixel.h>
#include "Sova/Common/Int.h"

#include "Unit.h"

namespace DsprFrontend
{
    class UnitManager : public Sova::Refable
    {
    public:
        UnitManager();
        void receiveUnit(Ref<Sova::String> id, Ref<Sova::String> x, Ref<Sova::String> y, Ref<Sova::String> tribeIndex);
        void receiveUnitUpdate(Ref<Sova::String> id, Ref<List<Sova::String>> propsStrList);

        void addToSelectionList(int id);
        void removeFromSelectionList(int id);
        void clearSelectionList();

        void uiUpdate();
        Ref<Unit> getUnitOverlappingWithPoint(int x, int y);
        Ref<List<Unit>> getNonHoveringUnitsWithinBox(int x1, int y1, int x2, int y2);
        Ref<List<Unit>> getUnitsOutsideBox(Ref<List<Unit>> hoverList, int x1, int y1, int x2, int y2);
        void deselectAllUnits();
        int getNumberSelectedUnits();

        Ref<List<Unit>> getUnits();

        void issueUnitOrder(bool attackOrderSelected);

    private:
        Ref<List<Unit>> unitList = Null<List<Unit>>();
        Ref<List<Sova::Int>> selectionList = Null<List<Sova::Int>>();
        bool rightButtonAlreadyClicked = false;
        Ref<Sova::Pixel> minimapPixel = Null<Sova::Pixel>();

        Ref<Unit> getUnitWithId(int id);

    };
}
