//
// Created by connor on 7/31/18.
//

#include <Sova/Common/Int.h>
#include <Sova/Input.h>
#include <Sova/Internal/InternalApp.h>
#include <Sova/Common/StringBuilder.h>
#include "UnitManager.h"
#include "Global.h"

namespace DsprFrontend
{
    UnitManager::UnitManager()
    {
        Ref<List<Unit>> unitList = New<List<Unit>>();
        Ref<List<Int>> selectionList = New<List<Int>>();
    }

    void UnitManager::addToSelectionList(int id)
    {
        auto myInt = New<Int>(id);
        selectionList->Add(myInt);
    }

    void UnitManager::clearSelectionList()
    {
        selectionList->Clear();
    }

    void UnitManager::addToUnitList(Ref<Unit> unit)
    {
        unitList->Add(unit);
    }

    void UnitManager::step()
    {
        if (InternalApp::mouseButtonPressed(MouseButton::Right))
        {
            auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
            auto tilePosition = g->cursor->getTilePosition();

            auto sb = New<Sova::StringBuilder>();
            sb->Append(New<String>("order/1.0/move|"));
            bool first = true;
            for (Ref<ListIterator<Int>> iterator = this->selectionList->GetIterator(); iterator->Valid(); iterator->Next())
            {
                if (first)
                {
                    sb->Append(New<String>(","));
                    first = false;
                }

                Ref<Int> intObj = iterator->Get();
                sb->Append(intObj->ToString());
            }
            sb->Append(New<String>("|"));
            sb->Append(New<Int>(tilePosition->x)->ToString());
            sb->Append(New<String>(","))
            sb->Append(New<Int>(tilePosition->y)->ToString());

            g->gameServer->send(sb->ToString());
        }
    }
}