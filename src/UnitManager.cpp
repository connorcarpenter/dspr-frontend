//
// Created by connor on 7/31/18.
//

#include <Sova/Input.h>
#include <Sova/Internal/InternalApp.h>
#include <Sova/Common/StringBuilder.h>
#include <Sova/Math/Math.h>
#include "UnitManager.h"
#include "Global.h"

namespace DsprFrontend
{
    UnitManager::UnitManager()
    {
        this->unitList = New<List<Unit>>();
        this->selectionList = New<List<Int>>();
    }

    void UnitManager::uiUpdate()
    {
        if (InternalApp::mouseButtonPressed(MouseButton::Right) && !rightButtonAlreadyClicked)
        {
            this->rightButtonAlreadyClicked = true;

            auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
            auto tilePosition = g->cursor->getTilePosition();

            auto sb = New<Sova::StringBuilder>();
            sb->Append(New<Sova::String>("unit/1.0/order|"));
            sb->Append(g->gameServerPlayerToken);
            sb->Append(New<Sova::String>("|"));
            bool first = true;
            for (Ref<ListIterator<Int>> iterator = this->selectionList->GetIterator(); iterator->Valid(); iterator->Next())
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    sb->Append(New<Sova::String>(","));
                }

                Ref<Int> intObj = iterator->Get();
                sb->Append(intObj->ToString());
            }
            sb->Append(New<Sova::String>("|"));
            sb->Append(New<Int>(tilePosition->x)->ToString());
            sb->Append(New<Sova::String>(","));
            sb->Append(New<Int>(tilePosition->y)->ToString());

            g->gameServer->send(sb->ToString());
        }

        if(!InternalApp::mouseButtonPressed(MouseButton::Right) && rightButtonAlreadyClicked)
            rightButtonAlreadyClicked = false;
    }

    Ref<Unit> UnitManager::getUnitOverlappingWithPoint(int x, int y)
    {
        for (auto iterator = this->unitList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            if (Math::PointInBox(x, y,
                                 unit->position->x - 6, unit->position->y - 10,
                                 unit->position->x + 4, unit->position->y + 1))
                return unit;
        }

        return Null<Unit>();
    }

    Ref<List<Unit>> UnitManager::getNonHoveringUnitsWithinBox(int x1, int y1, int x2, int y2)
    {
        auto output = New<List<Unit>>();
        for (auto iterator = this->unitList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            if (unit->hovering) continue;
            if (Math::BoxesOverlap(x1, y1, x2, y2,
                                 unit->position->x - 6, unit->position->y - 10,
                                 unit->position->x + 4, unit->position->y + 1))
                output->Add(unit);
        }

        if (output->Size() == 0)
            return Null<List<Unit>>();

        return output;
    }

    Ref<List<Unit>> UnitManager::getUnitsOutsideBox(Ref<List<Unit>> hoverList, int x1, int y1, int x2, int y2)
    {
        auto output = New<List<Unit>>();
        for (auto iterator = hoverList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            if (!Math::BoxesOverlap(x1, y1, x2, y2,
                                   unit->position->x - 6, unit->position->y - 10,
                                   unit->position->x + 4, unit->position->y + 1))
                output->Add(unit);
        }

        if (output->Size() == 0)
            return Null<List<Unit>>();

        return output;
    }

    void UnitManager::deselectAllUnits()
    {
        this->selectionList->Clear();
        for (auto iterator = this->unitList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            unit->selected = false;
        }
    }

    void UnitManager::addToSelectionList(int id)
    {
        auto myInt = New<Int>(id);
        selectionList->Add(myInt);
    }

    void UnitManager::removeFromSelectionList(int id) {
        auto theListInt = this->selectionList->Find([&](Ref<Int> theInt){
            return theInt->getInt() == id;
        });
        this->selectionList->Remove(theListInt);
    }

    void UnitManager::receiveUnit(Ref<Sova::String> idStr, Ref<Sova::String> xStr, Ref<Sova::String> yStr)
    {
        int id = atoi(idStr->AsCStr());
        int x = atoi(xStr->AsCStr());
        int y = atoi(yStr->AsCStr());

        auto newUnit = New<Unit>(id, x, y);
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        g->world->AddChild(newUnit);
        this->unitList->Add(newUnit);
    }

    void UnitManager::receiveUnitOrder(Ref<Sova::String> idStr, Ref<List<Sova::String>> propsStrList)
    {
        int id = atoi(idStr->AsCStr());

        Ref<Unit> unit = this->unitList->Find([&](Ref<Unit> inspectUnit) { //replace this with a Hash lookup someday!
            return (inspectUnit->id == id);
        });

        for (auto iterator = propsStrList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto propsStr = iterator->Get();
            auto propsParts = propsStr->Split(':');

            auto propName = propsParts->At(0);

            if (propName->Equals("nextPosition"))
            {
                auto varsParts = propsParts->At(1)->Split(',');

                ///
                int x = atoi(varsParts->At(0)->AsCStr());
                int y = atoi(varsParts->At(1)->AsCStr());
                unit->newNextTilePosition(x, y);
                continue;
            }
            else
            if (propName->Equals("moveTarget"))
            {
                auto varsParts = propsParts->At(1)->Split(',');

                unit->moveTarget->x = atoi(varsParts->At(0)->AsCStr());
                unit->moveTarget->y = atoi(varsParts->At(1)->AsCStr());
                continue;
            }
        }
    }
}