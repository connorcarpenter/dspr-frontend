//
// Created by connor on 7/31/18.
//

#include <Sova/Input.h>
#include <Sova/Internal/InternalApp.h>
#include <Sova/Common/StringBuilder.h>
#include <Sova/Math/Math.h>
#include "UnitManager.h"
#include "Game/EconomyManager.h"
#include "Game/Global.h"
#include "Game/UI/UiManager.h"
#include "DyingUnit.h"
#include "Unit.h"
#include "Game/Effects/BloodParticle.h"
#include "Game/FogManager.h"
#include "Game/UI/Cursor.h"
#include "Game/Minimap/Minimap.h"
#include "Game/UI/ButtonCardCatalog.h"
#include "Game/Unit/UnitTemplateCatalog.h"
#include "Game/TileManager.h"
#include "IsoBox/IsoBoxCache.h"
#include "Game/Item/ItemManager.h"
#include "Game/Item/Item.h"
#include "Game/Item/ItemTemplateCatalog.h"

namespace DsprFrontend
{
    UnitManager::UnitManager()
    {
        this->unitMap = New<Map<Unit>>();
        this->selectionList = New<List<Int>>();
        this->minimapPixel = New<Pixel>();
        this->minimapPixel->setLineStyle(1, Color::White);
    }

    Ref<Unit> UnitManager::getUnitOverlappingWithPoint(int x, int y)
    {
        if (!this->receivedGrid) return Null<Unit>();

        auto g = (Global*) InternalApp::getGlobal();

        auto tilePos = g->tileManager->getTilePosition(x, y);

        return this->unitGrid->get(tilePos->x, tilePos->y);
    }

    Ref<List<Unit>> UnitManager::getNonHoveringUnitsWithinBox(int x1, int y1, int x2, int y2)
    {
        if (!this->receivedGrid) return Null<List<Unit>>();

        auto g = (Global*) InternalApp::getGlobal();

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

        if (output->Size() == 0)
            return Null<List<Unit>>();

        return output;
    }

    Ref<List<Unit>> UnitManager::getUnitsOutsideBox(Ref<List<Unit>> hoverList, int x1, int y1, int x2, int y2)
    {
        if (!this->receivedGrid) return Null<List<Unit>>();

        auto g = (Global*) InternalApp::getGlobal();

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

    void UnitManager::deselectAllUnits()
    {
        this->selectionList->Clear();
        for (auto iterator = this->unitMap->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            unit->selected = false;
        }
    }

    void UnitManager::addToSelectionList(Ref<Unit> unit)
    {
        if (!this->selectionList->Contains(unit))
        {
            selectionList->Add(unit);
        }
    }

    void UnitManager::removeFromSelectionList(Ref<Unit> unit)
    {
        if (this->selectionList->Contains(unit)) {
            selectionList->Remove(unit);
        }
    }

    void UnitManager::receiveUnit(int id, int x, int y,
                                  int tribeIndex, int templateIndex)
    {
        auto g = (Global*) InternalApp::getGlobal();

        auto unitTemplate = g->unitTemplateCatalog->findTemplateByIndex(templateIndex);
        auto newUnit = New<Unit>(id, x, y, tribeIndex, unitTemplate);

        g->world->AddChild(newUnit);
        this->unitMap->Insert(id, newUnit);

        this->updateUnitPosition(newUnit, Null<Point>(), newUnit->nextTilePosition);
    }

    void UnitManager::receiveUnitUpdate(DsprMessage::UnitUpdateMsgV1 *updateMsg) {
        Ref<Unit> unit = this->unitMap->At(updateMsg->id.get());

        if (unit == nullptr) return;

        if (updateMsg->nextPosition.wasSet)
        {
            int x = updateMsg->nextPosition.get(0);
            int y = updateMsg->nextPosition.get(1);
            this->updateUnitPosition(unit, unit->nextTilePosition, New<Point>(x, y));
            unit->newNextTilePosition(x, y);
        }

        if (updateMsg->moveTarget.wasSet)
        {
            unit->moveTarget->x = updateMsg->moveTarget.get(0);
            unit->moveTarget->y = updateMsg->moveTarget.get(1);
        }

        if (updateMsg->animationState.wasSet)
        {
            unit->setAnimationState(static_cast<Unit::UnitAnimationState >(updateMsg->animationState.get(0)),
                                    updateMsg->animationState.get(1));
        }

        if (updateMsg->health.wasSet)
        {
            unit->health = updateMsg->health.get();
        }

        if (updateMsg->bleed.wasSet)
        {
            auto g = (Global*) InternalApp::getGlobal();
            if (unit->unitTemplate->bleeds && updateMsg->bleed.get() == 1)
            {
                auto bloodPartNum = Math::Random(1, 2);
                for (int i = 0; i < bloodPartNum; i++)
                    g->world->AddChild(New<BloodParticle>(unit->position, -2 - Math::Random(0, 6), unit->depth, unit->unitTemplate->bloodColor));
            }
        }

        if (updateMsg->targetUnitId.wasSet)
        {
            unit->targetUnitId  = updateMsg->targetUnitId.get();
        }

        if (updateMsg->gatherYield.wasSet)
        {
            int gatherRate = updateMsg->gatherYield.get(0);
            if (gatherRate != 0) {
                unit->gatherYield(gatherRate);
                auto g = (Global *) InternalApp::getSovaApp()->getGlobal();
                if (unit->tribeIndex == g->playersTribeIndex)
                    g->economyManager->setMana(updateMsg->gatherYield.get(1));
            }
        }

        if (updateMsg->constructionQueue.wasSet)
        {
            DsprMessage::ConstructionQueueMsgV1 cqMsg(updateMsg->constructionQueue.getCstr());

            if (cqMsg.buildTime.wasSet)
            {
                unit->constructionQueue->currentBuildTime = cqMsg.buildTime.get();
            }

            if (cqMsg.queue.wasSet)
            {
                if (cqMsg.queue.size() != 0) {

                    auto g = (Global *) InternalApp::getSovaApp()->getGlobal();

                    unit->constructionQueue->emptyQueue();
                    for (int i = 0; i < cqMsg.queue.size(); i++) {
                        int index = cqMsg.queue.get(i);
                        Ref<UnitTemplate> ut = g->unitTemplateCatalog->findTemplateByIndex(index);
                        unit->constructionQueue->enqueue(ut);
                    }
                }
            }
        }

        if (updateMsg->inventory.wasSet)
        {
            auto g = (Global*) InternalApp::getGlobal();

            for(int i=0;i<updateMsg->inventory.size();i++) {
                int itemIndex = updateMsg->inventory.get(i);
                if (itemIndex == 0)continue;
                itemIndex--;
                if (unit == g->cursor->itemInHandOwner && g->cursor->itemInHandSlotIndex == i) {
                    g->cursor->setItemInHandTemplate(g->itemTemplateCatalog->findTemplateByIndex(itemIndex));
                } else {
                    unit->inventory->SetItemIndex(i, g->itemTemplateCatalog->findTemplateByIndex(itemIndex));
                }
            }
        }

        if (updateMsg->rallyPoint.wasSet)
        {
            if (unit->unitTemplate->hasRallyPoint) {
                unit->rallyPoint->x = updateMsg->rallyPoint.get(0);
                unit->rallyPoint->y = updateMsg->rallyPoint.get(1);
            }
        }

        if (updateMsg->rallyUnitId.wasSet)
        {
            if (unit->unitTemplate->hasRallyPoint) {
                unit->rallyUnitId = updateMsg->rallyUnitId.get();
            }
        }
    }

    void UnitManager::receiveUnitUpdate(int id, Ref<List<String>> propsSplitStr)
    {
        Ref<Unit> unit = this->unitMap->At(id);

        if (unit == nullptr) return;

        for (auto iterator = propsSplitStr->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto propsStr = iterator->Get();
            auto propsParts = propsStr->Split(':');

            auto propName = propsParts->At(0);

            if (propName->Equals("nextPosition"))
            {
                auto varsStr = propsParts->At(1);
                if (varsStr == nullptr) continue;
                auto varsParts = varsStr->Split(',');

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
                auto g = (Global*) InternalApp::getGlobal();
                unit->health = atoi(propsParts->At(1)->AsCStr());
                continue;
            }
            else
            if (propName->Equals("bleed"))
            {
                auto g = (Global*) InternalApp::getGlobal();
                if (unit->unitTemplate->bleeds && atoi(propsParts->At(1)->AsCStr()) == 1)
                {
                    auto bloodPartNum = Math::Random(1, 2);
                    for (int i = 0; i < bloodPartNum; i++)
                        g->world->AddChild(New<BloodParticle>(unit->position, -2 - Math::Random(0, 6), unit->depth, unit->unitTemplate->bloodColor));
                }
                continue;
            }
            else
            if (propName->Equals("targetUnitId"))
            {
                unit->targetUnitId  = atoi(propsParts->At(1)->AsCStr());
                continue;
            }
            else
            if (propName->Equals("gatherYield"))
            {
                auto varsParts = propsParts->At(1)->Split(',');
                int gatherRate = atoi(varsParts->At(0)->AsCStr());
                if (gatherRate != 0) {
                    unit->gatherYield(gatherRate);
                    auto g = (Global *) InternalApp::getSovaApp()->getGlobal();
                    if (unit->tribeIndex == g->playersTribeIndex)
                        g->economyManager->setMana(atoi(varsParts->At(1)->AsCStr()));
                }
                continue;
            }
            else
            if (propName->Equals("cq"))
            {
                auto varsParts = propsParts->At(1)->Split('+');

                auto index = 0;

                while(index < varsParts->Size())
                {
                    auto intVarsParts = varsParts->At(index)->Split('-');
                    if (intVarsParts->At(0)->Equals("bt"))
                    {
                        unit->constructionQueue->currentBuildTime = atoi(intVarsParts->At(1)->AsCStr());
                    }
                    else
                    if (intVarsParts->At(0)->Equals("q"))
                    {
                        if (intVarsParts->At(1)->Length() == 0) {index++;continue;}

                        auto g = (Global*) InternalApp::getGlobal();
                        auto queueParts = intVarsParts->At(1)->Split(',');
                        unit->constructionQueue->emptyQueue();
                        for(int i = 0;i<queueParts->Size();i++){
                            int index = atoi(queueParts->At(i)->AsCStr());
                            Ref<UnitTemplate> ut = g->unitTemplateCatalog->findTemplateByIndex(index);
                            unit->constructionQueue->enqueue(ut);
                        }
                    }
                    index++;
                }
                continue;
            }
            else
            if (propName->Equals("inventory"))
            {
                auto g = (Global*) InternalApp::getGlobal();

                auto varsParts = propsParts->At(1)->Split(',');

                auto index = 0;

                while(index < varsParts->Size())
                {
                    int itemIndex = atoi(varsParts->At(index)->AsCStr());
                    if (unit == g->cursor->itemInHandOwner && g->cursor->itemInHandSlotIndex == index)
                    {
                        g->cursor->setItemInHandTemplate(g->itemTemplateCatalog->findTemplateByIndex(itemIndex));
                    }
                    else
                    {
                        unit->inventory->SetItemIndex(index, g->itemTemplateCatalog->findTemplateByIndex(itemIndex));
                    }

                    index++;
                }
                continue;
            }
            else
            if (propName->Equals("rallyPoint"))
            {
                auto varsParts = propsParts->At(1)->Split(',');
                if (unit->unitTemplate->hasRallyPoint) {
                    unit->rallyPoint->x = atoi(varsParts->At(0)->AsCStr());
                    unit->rallyPoint->y = atoi(varsParts->At(1)->AsCStr());
                }
                continue;
            }
            else
            if (propName->Equals("rallyUnitId"))
            {
                auto varsParts = propsParts->At(1)->Split(',');
                if (unit->unitTemplate->hasRallyPoint) {
                    unit->rallyUnitId = atoi(varsParts->At(0)->AsCStr());
                }
                continue;
            }
        }
    }

    void UnitManager::receiveUnitDelete(int id, Ref<Sova::String> propsStr)
    {
        //int id = atoi(idStr->AsCStr());

        Ref<Unit> unit = this->unitMap->At(id);

        if (unit == nullptr) return;

        auto g = (Global*) InternalApp::getGlobal();

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
                    g->world->AddChild(New<BloodParticle>(unit->position, -2 - Math::Random(0,6), unit->depth, unit->unitTemplate->bloodColor));
            }
        }

        if (unit->tribeIndex == g->playersTribeIndex)
        {
            g->fogManager->conceilFog(unit->tilePosition->x, unit->tilePosition->y, unit->unitTemplate->sight);
        }

        this->unitMap->Erase(unit->id);
        this->selectionList->Remove(unit);
        this->updateUnitPosition(unit, unit->nextTilePosition, Null<Point>());
        unit->Destroy();
    }

    Ref<List<Unit>> UnitManager::getSelectedUnits() {
        return this->selectionList;
    }

    Ref<Unit> UnitManager::getUnitWithId(int id) {
        return this->unitMap->At(id);
    }

    void UnitManager::issueUnitOrder(bool attackOrderSelected)
    {
        if (this->selectionList->Size() <= 0) return;

        auto g = (Global*) InternalApp::getGlobal();

        UnitOrder orderIndex = Move;

        auto targetedUnit = g->unitManager->getUnitOverlappingWithPoint(g->cursor->worldPosition->x, g->cursor->worldPosition->y);
        int targetId = -1;
        Ref<Unit> firstSelectedUnit = this->selectionList->At(0);
        if (targetedUnit != nullptr)
        {
            targetId = targetedUnit->id;

            orderIndex = (targetedUnit->tribeIndex == firstSelectedUnit->tribeIndex || targetedUnit->tribeIndex==-1) ? //change this later to actually check if tribes are enemies or not (to support allies, neutral)
                         Follow : AttackTarget;
            if (targetedUnit->unitTemplate->isGatherable && firstSelectedUnit->unitTemplate->canGather)
                orderIndex = Gather;
        }
        else
        {
            auto targetedItem = g->itemManager->getItemOverlappingWithPoint(g->cursor->worldPosition->x, g->cursor->worldPosition->y);
            if (targetedItem != nullptr){
                if (firstSelectedUnit->unitTemplate->hasInventory) {
                    targetId = targetedItem->id;
                    orderIndex = Pickup;
                }
            }
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
        if (orderIndex == Follow || orderIndex == AttackTarget || orderIndex == Gather || orderIndex == Pickup){
            sb->Append(New<Int>(targetId)->ToString());
        }

        g->gameServer->send(sb->ToString());
    }

    void UnitManager::issueUnitOrderGather()
    {
        if (this->selectionList->Size() <= 0) return;

        auto g = (Global*) InternalApp::getGlobal();

        UnitOrder orderIndex;

        auto targetedUnit = g->unitManager->getUnitOverlappingWithPoint(g->cursor->worldPosition->x, g->cursor->worldPosition->y);
        int targetId = -1;
        if (targetedUnit == nullptr)
        {
            auto targetedItem = g->itemManager->getItemOverlappingWithPoint(g->cursor->worldPosition->x, g->cursor->worldPosition->y);
            if (targetedItem == nullptr) return;
            targetId = targetedItem->id;
            orderIndex = Pickup;
        }
        else
        {
            if (!targetedUnit->unitTemplate->isGatherable) return;
            targetId = targetedUnit->id;
            orderIndex = Gather;
        }

        auto sb = New<Sova::StringBuilder>();
        sb->Append(New<Sova::String>("unit/1.0/order|"));
        sb->Append(g->gameServerPlayerToken);
        sb->Append(New<Sova::String>("|"));
        bool first = true;
        for (auto iterator = this->selectionList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            if (!unit->unitTemplate->canGather) continue;

            if (first)
            {
                first = false;
            }
            else
            {
                sb->Append(New<Sova::String>(","));
            }

            Ref<Int> intObj = New<Int>(unit->id);
            sb->Append(intObj->ToString());

            unit->currentOrder = orderIndex;
        }
        if(first)return;
        sb->Append(New<Sova::String>("|"));
        sb->Append(New<Int>(orderIndex)->ToString());
        sb->Append(New<Sova::String>(","));

        sb->Append(New<Int>(targetId)->ToString());

        g->gameServer->send(sb->ToString());
    }

    void UnitManager::orderCurrentlySelectedUnits(UnitOrder orderIndex)
    {
        if (this->selectionList->Size() <= 0) return;

        auto g = (Global*) InternalApp::getGlobal();

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

    void UnitManager::orderCurrentlySelectedUnitsToBuildUnit(Ref<UnitTemplate> unitTemplate)
    {
        if (this->selectionList->Size() <= 0) return;

        auto orderIndex = UnitOrder::Train;

        auto g = (Global*) InternalApp::getGlobal();

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
            unit->trainUnit(unitTemplate);
        }
        sb->Append(New<Sova::String>("|"));
        sb->Append(New<Int>(orderIndex)->ToString());
        sb->Append(New<Sova::String>(","));
        sb->Append(New<Int>(unitTemplate->index)->ToString());

        g->gameServer->send(sb->ToString());
    }

    void UnitManager::orderUnitSwapInventory(Ref<Unit> unit, int beforeSlotIndex, int afterSlotIndex)
    {
        if (beforeSlotIndex == afterSlotIndex)return;

        auto orderIndex = UnitOrder::ItemSwap;

        auto g = (Global*) InternalApp::getGlobal();

        auto sb = New<Sova::StringBuilder>();
        sb->Append(New<Sova::String>("unit/1.0/order|"));
        sb->Append(g->gameServerPlayerToken);
        sb->Append(New<Sova::String>("|"));
        Ref<Int> intObj = New<Int>(unit->id);
        sb->Append(intObj->ToString());
        sb->Append(New<Sova::String>("|"));
        sb->Append(New<Int>(orderIndex)->ToString());
        sb->Append(New<Sova::String>(","));
        sb->Append(New<Int>(beforeSlotIndex)->ToString());
        sb->Append(New<Sova::String>(","));
        sb->Append(New<Int>(afterSlotIndex)->ToString());

        g->gameServer->send(sb->ToString());
    }

    void UnitManager::orderUnitDropItem(Ref<Unit> unit, int slotIndex, Ref<Point> position)
    {
        auto orderIndex = UnitOrder::ItemDrop;

        auto g = (Global*) InternalApp::getGlobal();

        Ref<Point> tilePosition = Null<Point>();
        auto mmPosition = g->uiManager->getMinimapPosition(g->cursor->position);
        tilePosition = (mmPosition == nullptr) ?
                       g->tileManager->getTilePosition(position->x, position->y) : mmPosition;

        auto sb = New<Sova::StringBuilder>();
        sb->Append(New<Sova::String>("unit/1.0/order|"));
        sb->Append(g->gameServerPlayerToken);
        sb->Append(New<Sova::String>("|"));
        Ref<Int> intObj = New<Int>(unit->id);
        sb->Append(intObj->ToString());
        sb->Append(New<Sova::String>("|"));
        sb->Append(New<Int>(orderIndex)->ToString());
        sb->Append(New<Sova::String>(","));
        sb->Append(New<Int>(slotIndex)->ToString());
        sb->Append(New<Sova::String>(","));
        sb->Append(New<Int>(tilePosition->x)->ToString());
        sb->Append(New<Sova::String>(","));
        sb->Append(New<Int>(tilePosition->y)->ToString());

        g->gameServer->send(sb->ToString());
    }

    void UnitManager::orderUnitGiveItem(Ref<Unit> unit, int slotIndex, int targetUnitId)
    {
        auto orderIndex = UnitOrder::ItemGive;

        auto g = (Global*) InternalApp::getGlobal();

        auto sb = New<Sova::StringBuilder>();
        sb->Append(New<Sova::String>("unit/1.0/order|"));
        sb->Append(g->gameServerPlayerToken);
        sb->Append(New<Sova::String>("|"));
        Ref<Int> intObj = New<Int>(unit->id);
        sb->Append(intObj->ToString());
        sb->Append(New<Sova::String>("|"));
        sb->Append(New<Int>(orderIndex)->ToString());
        sb->Append(New<Sova::String>(","));
        sb->Append(New<Int>(slotIndex)->ToString());
        sb->Append(New<Sova::String>(","));
        sb->Append(New<Int>(targetUnitId)->ToString());

        g->gameServer->send(sb->ToString());
    }

    void UnitManager::issueUnitOrderRally(){
        if (this->selectionList->Size() <= 0) return;

        auto g = (Global*) InternalApp::getGlobal();

        UnitOrder orderIndex = RallyPoint;

        auto targetedUnit = g->unitManager->getUnitOverlappingWithPoint(g->cursor->worldPosition->x, g->cursor->worldPosition->y);
        int targetId = -1;

        if (targetedUnit != nullptr)
        {
            targetId = targetedUnit->id;
            orderIndex = RallyUnit;
        }

        Ref<Point> tilePosition = Null<Point>();

        auto mmPosition = g->uiManager->getMinimapPosition(g->cursor->position);
        tilePosition = (mmPosition == nullptr) ?
                       g->tileManager->getTilePosition(g->cursor->worldPosition->x, g->cursor->worldPosition->y) : mmPosition;

        auto sb = New<Sova::StringBuilder>();
        sb->Append(New<Sova::String>("unit/1.0/order|"));
        sb->Append(g->gameServerPlayerToken);
        sb->Append(New<Sova::String>("|"));
        bool first = true;
        for (auto iterator = this->selectionList->GetIterator(); iterator->Valid(); iterator->Next())
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
        if (orderIndex == RallyPoint) {
            sb->Append(New<Int>(tilePosition->x)->ToString());
            sb->Append(New<Sova::String>(","));
            sb->Append(New<Int>(tilePosition->y)->ToString());
        }
        if (orderIndex == RallyUnit){
            sb->Append(New<Int>(targetId)->ToString());
        }

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
            auto g = (Global*) InternalApp::getGlobal();
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

    Color UnitManager::getColorFromTribeIndex(int index) {
        switch (index)
        {
            case 0:
                return Color::Red;
                break;
            case 1:
                return Color::Blue;
                break;
        }
    }

    Ref<MapIterator<Unit>> UnitManager::getUnitsIterator() {
        return this->unitMap->GetIterator();
    }

    int UnitManager::getNumberOfUnits() {
        return this->unitMap->Size();
    }
}