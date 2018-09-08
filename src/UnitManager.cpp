//
// Created by connor on 7/31/18.
//

#include <Sova/Input.h>
#include <Sova/Internal/InternalApp.h>
#include <Sova/Common/StringBuilder.h>
#include <Sova/Math/Math.h>
#include "UnitManager.h"
#include "Global.h"
#include "UiManager.h"
#include "UnitOrder.h"
#include "DyingUnit.h"
#include "Unit.h"
#include "BloodParticle.h"
#include "FogManager.h"
#include "Cursor.h"
#include "Minimap/Minimap.h"

namespace DsprFrontend
{
    UnitManager::UnitManager()
    {
        this->unitList = New<List<Unit>>();
        this->selectionList = New<List<Int>>();
        this->minimapPixel = New<Pixel>();
        this->minimapPixel->setLineStyle(1, Color::White);
    }

    void UnitManager::uiUpdate()
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        if (InternalApp::mouseButtonPressed(MouseButton::Right) && !rightButtonAlreadyClicked)
        {
            this->rightButtonAlreadyClicked = true;

            if (g->cursor->attackOrderSelected){
                g->cursor->attackOrderSelected = false;
            } else {
                this->issueUnitOrder(false);
            }
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
        auto foundInt = this->selectionList->Find([&](Ref<Int> theInt){
            return theInt->getInt() == id;
        });

        if (foundInt == nullptr) {
            auto myInt = New<Int>(id);
            selectionList->Add(myInt);
        }
    }

    void UnitManager::removeFromSelectionList(int id) {
        auto theListInt = this->selectionList->Find([&](Ref<Int> theInt){
            return theInt->getInt() == id;
        });

        if (theListInt != nullptr) {
            this->selectionList->Remove(theListInt);
        }
    }

    void UnitManager::receiveUnit(Ref<Sova::String> idStr, Ref<Sova::String> xStr, Ref<Sova::String> yStr, Ref<Sova::String> tribeIndexStr)
    {
        int id = atoi(idStr->AsCStr());
        int x = atoi(xStr->AsCStr());
        int y = atoi(yStr->AsCStr());
        int tribeIndex = atoi(tribeIndexStr->AsCStr());

        auto newUnit = New<Unit>(id, x, y, tribeIndex);
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        g->world->AddChild(newUnit);
        this->unitList->Add(newUnit);
    }

    void UnitManager::receiveUnitUpdate(Ref<Sova::String> idStr, Ref<List<Sova::String>> propsStrList)
    {
        int id = atoi(idStr->AsCStr());

        Ref<Unit> unit = this->unitList->Find([&](Ref<Unit> inspectUnit) { //replace this with a Hash lookup someday!
            return (inspectUnit->id == id);
        });

        if (unit == nullptr) return;

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
            else
            if (propName->Equals("animationState"))
            {
                auto varsParts = propsParts->At(1)->Split(',');

                unit->setAnimationState(static_cast<Unit::UnitAnimationState >(atoi(varsParts->At(0)->AsCStr())),
                                        atoi(varsParts->At(1)->AsCStr()));
                continue;
            }
            else
            if (propName->Equals("health"))
            {
                auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
                unit->health = atoi(propsParts->At(1)->AsCStr());
                auto bloodPartNum = Math::Random(1,4);
                for (int i = 0;i<bloodPartNum;i++)
                    g->world->AddChild(New<BloodParticle>(unit->position, -2 - Math::Random(0,6), unit->depth));
                continue;
            }
        }
    }

    void UnitManager::receiveUnitDelete(Ref<Sova::String> idStr, Ref<Sova::String> propsStr)
    {
        int id = atoi(idStr->AsCStr());

        Ref<Unit> unit = this->unitList->Find([&](Ref<Unit> inspectUnit) { //replace this with a Hash lookup someday!
            return (inspectUnit->id == id);
        });

        if (unit == nullptr) return;

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        auto deleteModifier = atoi(propsStr->AsCStr());

        if (deleteModifier == 1){
            auto dieSprite = New<DyingUnit>(unit);
            g->world->AddChild(dieSprite);

            auto bloodPartNum = 6;
            for (int i = 0;i<bloodPartNum;i++)
                g->world->AddChild(New<BloodParticle>(unit->position, -2 - Math::Random(0,6), unit->depth));
        }

        if (unit->tribeIndex == g->playersTribeIndex){
            g->fogManager->conceilFog(unit->tilePosition->x, unit->tilePosition->y, unit->sight);
        }

        this->unitList->Remove(unit);
        this->selectionList->Remove(New<Int>(id));
        unit->Destroy();
    }

    Ref<List<Int>> UnitManager::getSelectedUnits() {
        return this->selectionList;
    }

    Ref<List<Unit>> UnitManager::getUnits() {
        return this->unitList;
    }

    Ref<Unit> UnitManager::getUnitWithId(int id) {
        return this->unitList->Find([&](Ref<Unit> evalUnit){
            return evalUnit->id ==  id;
        });
    }

    void UnitManager::issueUnitOrder(bool attackOrderSelected) {

        if (this->selectionList->Size() <= 0) return;

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        UnitOrder orderIndex = Move;

        auto targetedUnit = g->unitManager->getUnitOverlappingWithPoint(g->cursor->worldPosition->x, g->cursor->worldPosition->y);
        int targetedUnitId = -1;
        if (targetedUnit != nullptr){
            targetedUnitId = targetedUnit->id;
            Ref<Unit> firstSelectedUnit = this->getUnitWithId(this->selectionList->At(0)->getInt());
            orderIndex = (targetedUnit->tribeIndex == firstSelectedUnit->tribeIndex) ? //change this later to actually check if tribes are enemies or not (to support allies, neutral)
                         Follow : AttackTarget;
        }

        if (attackOrderSelected)
            orderIndex = (orderIndex == Move) ? AttackMove : AttackTarget;

        Ref<Point> tilePosition = Null<Point>();

        if (orderIndex == Move || orderIndex == AttackMove) {
            tilePosition = g->cursor->getTilePosition();
            auto mmPosition = g->uiManager->getMinimapPosition(g->cursor->position);
            if (mmPosition != nullptr)
                tilePosition->set(mmPosition);
        }

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
        sb->Append(New<Int>(orderIndex)->ToString());
        sb->Append(New<Sova::String>(","));
        if (orderIndex == Move || orderIndex == AttackMove) {
            sb->Append(New<Int>(tilePosition->x)->ToString());
            sb->Append(New<Sova::String>(","));
            sb->Append(New<Int>(tilePosition->y)->ToString());
        }
        if (orderIndex == Follow || orderIndex == AttackTarget){
            sb->Append(New<Int>(targetedUnitId)->ToString());
        }

        g->gameServer->send(sb->ToString());
    }
}