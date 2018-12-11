#pragma once

//
// Created by connor on 7/31/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/Pixel.h>
#include <Sova/Common/Map.h>
#include <DsprMessage/ToClientMsg.h>
#include "Sova/Common/Int.h"
#include "UnitOrderType.h"
#include "RefIsoGrid.h"
#include "UnitTemplate.h"

namespace DsprFrontend
{
    class Unit;

    class UnitManager : public Sova::Refable
    {
    public:

        UnitManager();
        void receiveUnit(int id, int x, int y, int tribeIndex,
                         int templateIndex);
        void receiveUnitUpdate(const DsprMessage::UnitUpdateMsgV1 &);
        void receiveUnitUpdate(int id, Ref<List<String>> propsSplitStr);
        void receiveUnitDelete(int id, bool dead);
        void receiveUnitSpecialAction(int id, int actionIndex);

        void addToSelectionList(Ref<Unit> unit);
        void removeFromSelectionList(Ref<Unit> unit);
        void clearSelectionList();
        Ref<Unit> getUnitOverlappingWithPoint(int x, int y);
        Ref<List<Unit>> getNonHoveringUnitsWithinBox(int x1, int y1, int x2, int y2);
        Ref<List<Unit>> getUnitsOutsideBox(Ref<List<Unit>> hoverList, int x1, int y1, int x2, int y2);
        void deselectAllUnits();
        Ref<MapIterator<Unit>> getUnitsIterator();
        void issueUnitOrder(bool attackOrderSelected);
        Ref<List<Unit>> getSelectedUnits();
        Ref<Unit> getUnitWithId(int id);
        void orderCurrentlySelectedUnits(DsprFrontend::UnitOrderType orderIndex);
        void orderCurrentlySelectedUnitsToBuildUnit(Ref<UnitTemplate> unitTemplate);
        void receiveGrid(int w, int h);
        void issueUnitOrderGather();

        Ref<List<Unit>> selectionList = Null<List<Unit>>();
        const int maxSelectedUnits = 12;

        void orderUnitSwapInventory(Ref<Unit> unit, int beforeSlotIndex, int afterSlotIndex);

        void orderUnitDropItem(Ref<Unit> unit, int slotIndex, Ref<Point> position);

        void orderUnitGiveItem(Ref<Unit> unit, int slotIndex, int targetUnitId);

        void issueUnitOrderRally();

        int getNumberOfUnits();

        void updateUnitPosition(Ref<Unit> unit, Ref<Point> oldPosition, Ref<Point> newPosition);

        void orderCurrentlySelectedUnitsToCancelTrainUnit(int queueIndex);
        void issueUnitOrderSpecialAction(int actionIndex);

        Ref<Unit> getUnitAtCoord(int x, int y);

        void issueUnitOrderSpecialAction(int actionIndex, int x, int y);

    private:
        Ref<Map<Unit>> unitMap = Null<Map<Unit>>();
        Ref<Sova::Pixel> minimapPixel = Null<Sova::Pixel>();
        Ref<RefIsoGrid<Unit>> unitGrid = Null<RefIsoGrid<Unit>>();
        bool receivedGrid = false;
        int gridWidth;
        int gridHeight;



    };
}
