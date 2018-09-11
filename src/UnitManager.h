#pragma once

//
// Created by connor on 7/31/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/Pixel.h>
#include "Sova/Common/Int.h"
#include "UnitOrder.h"

namespace DsprFrontend
{
    class Unit;

    class UnitManager : public Sova::Refable
    {
    public:

        UnitManager();
        void receiveUnit(Ref<Sova::String> id, Ref<Sova::String> x, Ref<Sova::String> y, Ref<Sova::String> tribeIndex);
        void receiveUnitUpdate(Ref<Sova::String> id, Ref<List<Sova::String>> propsStrList);
        void addToSelectionList(Ref<Unit> unit);
        void removeFromSelectionList(Ref<Unit> unit);
        void clearSelectionList();
        void uiUpdate();
        Ref<Unit> getUnitOverlappingWithPoint(int x, int y);
        Ref<List<Unit>> getNonHoveringUnitsWithinBox(int x1, int y1, int x2, int y2);
        Ref<List<Unit>> getUnitsOutsideBox(Ref<List<Unit>> hoverList, int x1, int y1, int x2, int y2);
        void deselectAllUnits();
        Ref<List<Unit>> getUnits();
        void issueUnitOrder(bool attackOrderSelected);
        Ref<List<Unit>> getSelectedUnits();
        Ref<Unit> getUnitWithId(int id);
        void receiveUnitDelete(Ref<Sova::String> idStr, Ref<Sova::String> propsStr);

        void orderCurrentlySelectedUnits(DsprFrontend::UnitOrder orderIndex);
        void holdCurrentlySelectedUnits();

        Ref<List<Unit>> selectionList = Null<List<Unit>>();
        const int maxSelectedUnits = 12;
    private:
        Ref<List<Unit>> unitList = Null<List<Unit>>();
        bool rightButtonAlreadyClicked = false;
        Ref<Sova::Pixel> minimapPixel = Null<Sova::Pixel>();
    };
}
