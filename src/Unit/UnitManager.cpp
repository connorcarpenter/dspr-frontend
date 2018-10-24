//
// Created by connor on 7/31/18.
//

#include <Sova/Input.h>
#include <Sova/Internal/InternalApp.h>
#include <Sova/Common/StringBuilder.h>
#include <Sova/Math/Math.h>
#include "UnitManager.h"
#include "Global.h"
#include "UI/UiManager.h"
#include "DyingUnit.h"
#include "Unit.h"
#include "BloodParticle.h"
#include "FogManager.h"
#include "UI/Cursor.h"
#include "Minimap/Minimap.h"
#include "UI/ButtonCardCatalog.h"
#include "Unit/UnitTemplateCatalog.h"
#include "TileManager.h"
#include "IsoBox/IsoBoxCache.h"

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
            this->issueUnitOrder(false);
        }

        if(!InternalApp::mouseButtonPressed(MouseButton::Right) && rightButtonAlreadyClicked)
            rightButtonAlreadyClicked = false;
    }

    /* CHANGE HERE*/
    Ref<Unit> UnitManager::getUnitOverlappingWithPoint(int x, int y)
    {
        if (!this->receivedGrid) return Null<Unit>();

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        auto tilePos = g->tileManager->getTilePosition(x, y);

        return this->unitGrid->get(tilePos->x, tilePos->y);

//        for (auto iterator = this->unitList->GetIterator(); iterator->Valid(); iterator->Next())
//        {
//            auto unit = iterator->Get();
//            if (Math::PointInBox(x, y,
//                                 unit->position->x - 6, unit->position->y - 10,
//                                 unit->position->x + 4, unit->position->y + 1))
//                return unit;
//        }
//
//        return Null<Unit>();
    }

    Ref<List<Unit>> UnitManager::getNonHoveringUnitsWithinBox(int x1, int y1, int x2, int y2)
    {
        if (!this->receivedGrid) return Null<List<Unit>>();

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        auto output = New<List<Unit>>();

        auto minX = Math::Min(x1, x2);
        auto minY = Math::Min(y1, y2);
        auto maxX = Math::Max(x1, x2)+12;
        auto maxY = Math::Max(y1, y2)+6;

        auto minTilePos = g->tileManager->getTilePosition(minX, minY);
        auto maxTilePos = g->tileManager->getTilePosition(maxX, maxY);

        for (int x = minTilePos->x; x < maxTilePos->x; x++)
        {
            for (int y = minTilePos->y; y < maxTilePos->y; y++)
            {
                auto unit = this->unitGrid->get(x,y);
                if (unit == nullptr) continue;
                if (unit->hovering) continue;
                output->Add(unit);
            }
        }

        /*
        for (auto iterator = this->unitList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            if (unit->hovering) continue;
            if (Math::BoxesOverlap(x1, y1, x2, y2,
                                 unit->position->x - 6, unit->position->y - 10,
                                 unit->position->x + 4, unit->position->y + 1))
                output->Add(unit);
        }
         */

        if (output->Size() == 0)
            return Null<List<Unit>>();

        return output;
    }

    Ref<List<Unit>> UnitManager::getUnitsOutsideBox(Ref<List<Unit>> hoverList, int x1, int y1, int x2, int y2)
    {
        if (!this->receivedGrid) return Null<List<Unit>>();

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        auto output = New<List<Unit>>(hoverList);

        auto minX = Math::Min(x1, x2);
        auto minY = Math::Min(y1, y2);
        auto maxX = Math::Max(x1, x2)+12;
        auto maxY = Math::Max(y1, y2)+6;

        auto minTilePos = g->tileManager->getTilePosition(minX, minY);
        auto maxTilePos = g->tileManager->getTilePosition(maxX, maxY);

        for (int x = minTilePos->x; x < maxTilePos->x; x++)
        {
            for (int y = minTilePos->y; y < maxTilePos->y; y++)
            {
                auto unit = this->unitGrid->get(x,y);
                if (unit == nullptr) continue;
                if (output->Contains(unit))
                    output->Remove(unit);
            }
        }

        if (output->Size() == 0)
            return Null<List<Unit>>();

        return output;
    }
    /* CHANGE HERE*/

    void UnitManager::deselectAllUnits()
    {
        this->selectionList->Clear();
        for (auto iterator = this->unitList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            unit->selected = false;
        }

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        g->uiManager->currentButtonCard = Null<ButtonCard>();
    }

    void UnitManager::addToSelectionList(Ref<Unit> unit)
    {
        if (!this->selectionList->Contains(unit)) {
            selectionList->Add(unit);
        }

        if (this->selectionList->Size() > 0)
        {
            auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
            g->uiManager->currentButtonCard = g->buttonCardCatalog->basicCommandCard;
        }
    }

    void UnitManager::removeFromSelectionList(Ref<Unit> unit) {
        if (this->selectionList->Contains(unit)) {
            selectionList->Remove(unit);
        }

        if (this->selectionList->Size() == 0)
        {
            auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
            g->uiManager->currentButtonCard = Null<ButtonCard>();
        }
    }

    void UnitManager::receiveUnit(Ref<Sova::String> idStr, Ref<Sova::String> xStr, Ref<Sova::String> yStr,
                                      Ref<Sova::String> tribeIndexStr, Ref<Sova::String> templateIndexStr)
    {
        int id = atoi(idStr->AsCStr());
        int x = atoi(xStr->AsCStr());
        int y = atoi(yStr->AsCStr());
        int tribeIndex = atoi(tribeIndexStr->AsCStr());
        int templateIndex = atoi(templateIndexStr->AsCStr());

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        auto unitTemplate = g->unitTemplateCatalog->findTemplateByIndex(templateIndex);
        auto newUnit = New<Unit>(id, x, y, tribeIndex, unitTemplate);

        g->world->AddChild(newUnit);
        this->unitList->Add(newUnit);

        this->updateUnitPosition(newUnit, Null<Point>(), newUnit->nextTilePosition);
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
                this->updateUnitPosition(unit, unit->nextTilePosition, New<Point>(x, y));
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
                if (unit->unitTemplate->bleeds)
                {
                    auto bloodPartNum = Math::Random(1, 2);
                    for (int i = 0; i < bloodPartNum; i++)
                        g->world->AddChild(New<BloodParticle>(unit->position, -2 - Math::Random(0, 6), unit->depth));
                }
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

        if (deleteModifier == 1)
        {
            unit->playDeathSound();

            if (unit->unitTemplate->hasDeathAnimation)
            {
                auto dieSprite = New<DyingUnit>(unit);
                g->world->AddChild(dieSprite);
            }

            if (unit->unitTemplate->bleeds)
            {
                for (int i = 0;i<6;i++)
                    g->world->AddChild(New<BloodParticle>(unit->position, -2 - Math::Random(0,6), unit->depth));
            }
        }

        if (unit->tribeIndex == g->playersTribeIndex)
        {
            g->fogManager->conceilFog(unit->tilePosition->x, unit->tilePosition->y, unit->unitTemplate->sight);
        }

        this->unitList->Remove(unit);
        this->selectionList->Remove(unit);
        this->updateUnitPosition(unit, unit->nextTilePosition, Null<Point>());
        unit->Destroy();
    }

    Ref<List<Unit>> UnitManager::getSelectedUnits() {
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

    void UnitManager::issueUnitOrder(bool attackOrderSelected)
    {
        if (this->selectionList->Size() <= 0) return;

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        UnitOrder orderIndex = Move;

        auto targetedUnit = g->unitManager->getUnitOverlappingWithPoint(g->cursor->worldPosition->x, g->cursor->worldPosition->y);
        int targetedUnitId = -1;
        if (targetedUnit != nullptr)
        {
            targetedUnitId = targetedUnit->id;
            Ref<Unit> firstSelectedUnit = this->selectionList->At(0);
            orderIndex = (targetedUnit->tribeIndex == firstSelectedUnit->tribeIndex) ? //change this later to actually check if tribes are enemies or not (to support allies, neutral)
                         Follow : AttackTarget;
        }

        if (attackOrderSelected)
            orderIndex = (orderIndex == Move) ? AttackMove : AttackTarget;

        Ref<Point> tilePosition = Null<Point>();

        if (orderIndex == Move || orderIndex == AttackMove)
        {
            auto mmPosition = g->uiManager->getMinimapPosition(g->cursor->position);
            tilePosition = (mmPosition == nullptr) ?
                           g->tileManager->getTilePosition(g->cursor->worldPosition->x, g->cursor->worldPosition->y) : mmPosition;
        }

        auto sb = New<Sova::StringBuilder>();
        sb->Append(New<Sova::String>("unit/1.0/order|"));
        sb->Append(g->gameServerPlayerToken);
        sb->Append(New<Sova::String>("|"));
        bool first = true;
        for (Ref<ListIterator<Unit>> iterator = this->selectionList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            if (first)
            {
                first = false;
            }
            else
            {
                sb->Append(New<Sova::String>(","));
            }

            auto unit = iterator->Get();
            Ref<Int> intObj = New<Int>(unit->id);
            sb->Append(intObj->ToString());

            unit->currentOrder = orderIndex;
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

    void UnitManager::orderCurrentlySelectedUnits(UnitOrder orderIndex)
    {
        if (this->selectionList->Size() <= 0) return;

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        auto sb = New<Sova::StringBuilder>();
        sb->Append(New<Sova::String>("unit/1.0/order|"));
        sb->Append(g->gameServerPlayerToken);
        sb->Append(New<Sova::String>("|"));
        bool first = true;
        for (Ref<ListIterator<Unit>> iterator = this->selectionList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            if (first)
            {
                first = false;
            }
            else
            {
                sb->Append(New<Sova::String>(","));
            }

            auto unit = iterator->Get();
            Ref<Int> intObj = New<Int>(unit->id);
            sb->Append(intObj->ToString());

            unit->currentOrder = orderIndex;
        }
        sb->Append(New<Sova::String>("|"));
        sb->Append(New<Int>(orderIndex)->ToString());

        g->gameServer->send(sb->ToString());
    }

    void UnitManager::updateUnitPosition(Ref<Unit> unit, Ref<Point> oldPosition, Ref<Point> newPosition)
    {
        if (!this->receivedGrid) return;

        if (unit->unitTemplate->tileWidth == 1 && unit->unitTemplate->tileHeight == 1)
        {
            if (oldPosition != nullptr)
                this->unitGrid->set(oldPosition->x, oldPosition->y, Null<Unit>());

            if (newPosition != nullptr)
                this->unitGrid->set(newPosition->x, newPosition->y, unit);
        }
        else
        {
            auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
            auto unitIsoBoxBase = g->isoBoxCache->getIsoBox(unit->unitTemplate->tileWidth, unit->unitTemplate->tileHeight);

            if (oldPosition != nullptr)
            {
                for (auto iterator = unitIsoBoxBase->coordList->GetIterator(); iterator->Valid(); iterator->Next())
                {
                    auto coord = iterator->Get();
                    this->unitGrid->set(oldPosition->x + coord->x, oldPosition->y + coord->y, Null<Unit>());
                }
            }

            if (newPosition != nullptr)
            {
                for (auto iterator = unitIsoBoxBase->coordList->GetIterator(); iterator->Valid(); iterator->Next())
                {
                    auto coord = iterator->Get();
                    this->unitGrid->set(newPosition->x + coord->x, newPosition->y + coord->y, unit);
                }
            }
        }
    }

    void UnitManager::receiveGrid(int w, int h)
    {
        this->receivedGrid = true;
        this->gridWidth = w;
        this->gridHeight = h;

        this->unitGrid = New<RefIsoGrid<Unit>>();
        this->unitGrid->initialize(this->gridWidth * 2, this->gridHeight * 2);
    }
}