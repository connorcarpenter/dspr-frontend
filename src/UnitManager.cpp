//
// Created by connor on 7/31/18.
//

#include <Sova/Input.h>
#include <Sova/Internal/InternalApp.h>
#include <Sova/Common/StringBuilder.h>
#include "UnitManager.h"
#include "Global.h"

namespace DsprFrontend
{
    UnitManager::UnitManager()
    {
        this->unitList = New<List<Unit>>();
        this->selectionList = New<List<Int>>();
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

    void UnitManager::step()
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
}