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
#include "Network/NetworkManager.h"

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

    Ref<Unit> UnitManager::getUnitAtCoord(int x, int y){
        if (!this->receivedGrid) return Null<Unit>();

        return this->unitGrid->get(x, y);
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



        bool addToSelection = false;

        auto existingUnitWithId = this->unitMap->At(id);
        if (existingUnitWithId != nullptr)
        {
            if (this->selectionList->Contains(existingUnitWithId))
            {
                addToSelection = true;
            }

            this->receiveUnitDelete(id, false);
        }

        auto unitTemplate = g->unitTemplateCatalog->findTemplateByIndex(templateIndex);
        auto newUnit = New<Unit>(id, x, y, tribeIndex, unitTemplate);
        g->world->AddChild(newUnit);
        this->unitMap->Insert(id, newUnit);

        this->updateUnitPosition(newUnit, Null<Point>(), newUnit->nextTilePosition);

        if (addToSelection)
        {
            newUnit->selected = true;
            this->selectionList->Add(newUnit);
        }

        if(existingUnitWithId == nullptr && unitTemplate.obj == g->unitTemplateCatalog->templeBuilding.obj)
        {
            g->uiManager->centerCameraOnUnit(newUnit);
        }
    }

    void UnitManager::receiveUnitSpecialAction(int id, int actionIndex)
    {
        auto existingUnitWithId = this->unitMap->At(id);
        existingUnitWithId->specialAction(actionIndex);
    }

    void UnitManager::receiveUnitUpdate(const DsprMessage::UnitUpdateMsgV1& updateMsg) {
        Ref<Unit> unit = this->unitMap->At(updateMsg.id.get());

        if (unit == nullptr) return;

        if (updateMsg.nextPosition.getWasSet())
        {
            int x = updateMsg.nextPosition.get(0);
            int y = updateMsg.nextPosition.get(1);
            this->updateUnitPosition(unit, unit->nextTilePosition, New<Point>(x, y));
            unit->newNextTilePosition(x, y);
        }

        if (updateMsg.moveTarget.getWasSet())
        {
            unit->moveTarget->x = updateMsg.moveTarget.get(0);
            unit->moveTarget->y = updateMsg.moveTarget.get(1);
        }

        if (updateMsg.animationState.getWasSet())
        {
            unit->setAnimationState(static_cast<Unit::UnitAnimationState >(updateMsg.animationState.get(0)),
                                    updateMsg.animationState.get(1));
        }

        if (updateMsg.health.getWasSet())
        {
            unit->health = updateMsg.health.get();
        }

        if (updateMsg.bleed.getWasSet())
        {
            auto g = (Global*) InternalApp::getGlobal();
            if (unit->unitTemplate->bleeds && updateMsg.bleed.get() == 1)
            {
                auto bloodPartNum = Math::Random(1, 2);
                for (int i = 0; i < bloodPartNum; i++)
                    g->world->AddChild(New<BloodParticle>(unit->position, -2 - Math::Random(0, 6), unit->depth, unit->unitTemplate->bloodColor));
            }
        }

        if (updateMsg.targetUnitId.getWasSet())
        {
            unit->targetUnitId  = updateMsg.targetUnitId.get();
        }

        if (updateMsg.gatherYield.getWasSet())
        {
            int gatherRate = updateMsg.gatherYield.get(0);
            if (gatherRate != 0) {
                unit->gatherYield(gatherRate);
                auto g = (Global *) InternalApp::getSovaApp()->getGlobal();
                if (g->tribeManager->isPlayer(unit->tribeIndex))
                    g->economyManager->setMana(updateMsg.gatherYield.get(1));
            }
        }

        if (updateMsg.constructionQueue.getWasSet())
        {
            DsprMessage::ConstructionQueueMsgV1 cqMsg(updateMsg.constructionQueue);

            if (cqMsg.buildTime.getWasSet())
            {
                unit->constructionQueue->currentBuildTime = cqMsg.buildTime.get();
            }

            if (cqMsg.queue.getWasSet())
            {
                auto g = (Global *) InternalApp::getSovaApp()->getGlobal();

                unit->constructionQueue->emptyQueue();
                for (int i = 0; i < cqMsg.queue.size(); i++) {
                    int index = cqMsg.queue.get(i);
                    Ref<UnitTemplate> ut = g->unitTemplateCatalog->findTemplateByIndex(index);
                    unit->constructionQueue->enqueue(ut);
                }
            }
        }

        if (updateMsg.inventory.getWasSet())
        {
            auto g = (Global*) InternalApp::getGlobal();

            for(int i=0;i<updateMsg.inventory.size();i++) {
                int itemIndex = updateMsg.inventory.get(i);
                if (itemIndex == 0)continue;
                itemIndex--;
                if (unit == g->cursor->itemInHandOwner && g->cursor->itemInHandSlotIndex == i) {
                    g->cursor->setItemInHandTemplate(g->itemTemplateCatalog->findTemplateByIndex(itemIndex));
                } else {
                    unit->inventory->SetItemIndex(i, g->itemTemplateCatalog->findTemplateByIndex(itemIndex));
                }
            }
        }

        if (updateMsg.rallyPoint.getWasSet())
        {
            if (unit->unitTemplate->hasRallyPoint) {
                unit->rallyPoint->x = updateMsg.rallyPoint.get(0);
                unit->rallyPoint->y = updateMsg.rallyPoint.get(1);
            }
        }

        if (updateMsg.rallyUnitId.getWasSet())
        {
            if (unit->unitTemplate->hasRallyPoint) {
                unit->rallyUnitId = updateMsg.rallyUnitId.get();
            }
        }
    }

    void UnitManager::receiveUnitDelete(int id, bool dead)
    {
        //int id = atoi(idStr->AsCStr());

        Ref<Unit> unit = this->unitMap->At(id);

        if (unit == nullptr) return;

        auto g = (Global*) InternalApp::getGlobal();

        if (dead == 1)
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

        if (g->tribeManager->givesSight(unit->tribeIndex))
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

        UnitOrderType orderIndex = Move;

        auto targetedUnit = g->unitManager->getUnitOverlappingWithPoint(g->cursor->worldPosition->x, g->cursor->worldPosition->y);
        int targetId = -1;
        Ref<Unit> firstSelectedUnit = this->selectionList->At(0);
        if (targetedUnit != nullptr)
        {
            targetId = targetedUnit->id;

            orderIndex = (targetedUnit->tribeIndex == firstSelectedUnit->tribeIndex || targetedUnit->tribeIndex==-1) ? //change this later to actually check if tribes are enemies or not (to support allies, neutral)
                         Follow : AttackTargetStrong;
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
            orderIndex = (orderIndex == Move) ? AttackMove : AttackTargetStrong;

        Ref<Point> tilePosition = Null<Point>();

        if (orderIndex == Move || orderIndex == AttackMove)
        {
            auto mmPosition = g->uiManager->getMinimapPosition(g->cursor->position);
            tilePosition = (mmPosition == nullptr) ?
                           g->tileManager->getTilePosition(g->cursor->worldPosition->x, g->cursor->worldPosition->y) : mmPosition;
        }

        auto idList = New<List<Int>>();
        auto otherNumberList = New<List<Int>>();

        for (Ref<ListIterator<Unit>> iterator = this->selectionList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            Ref<Int> intObj = New<Int>(unit->id);
            idList->Add(intObj);
            unit->currentOrder = orderIndex;
        }

        if (orderIndex == Move || orderIndex == AttackMove) {
            otherNumberList->Add(New<Int>(tilePosition->x));
            otherNumberList->Add(New<Int>(tilePosition->y));
        }
        if (orderIndex == Follow || orderIndex == AttackTargetStrong || orderIndex == Gather || orderIndex == Pickup){
            otherNumberList->Add(New<Int>(targetId));
        }

        g->networkManager->messageSender->sendUnitOrderMessage(idList, New<Int>(orderIndex), otherNumberList);
    }

    void UnitManager::issueUnitOrderGather()
    {
        if (this->selectionList->Size() <= 0) return;

        auto g = (Global*) InternalApp::getGlobal();

        UnitOrderType orderIndex;

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

        auto idList = New<List<Int>>();
        auto otherNumberList = New<List<Int>>();

        for (auto iterator = this->selectionList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            Ref<Int> intObj = New<Int>(unit->id);
            idList->Add(intObj);

            unit->currentOrder = orderIndex;
        }

        otherNumberList->Add(New<Int>(orderIndex));
        otherNumberList->Add(New<Int>(targetId));

        g->networkManager->messageSender->sendUnitOrderMessage(idList, New<Int>(orderIndex), otherNumberList);
    }

    void UnitManager::orderCurrentlySelectedUnits(UnitOrderType orderIndex)
    {
        if (this->selectionList->Size() <= 0) return;

        auto g = (Global*) InternalApp::getGlobal();

        auto idList = New<List<Int>>();
        auto otherNumberList = New<List<Int>>();

        for (auto iterator = this->selectionList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            Ref<Int> intObj = New<Int>(unit->id);
            idList->Add(intObj);

            unit->currentOrder = orderIndex;
        }

        g->networkManager->messageSender->sendUnitOrderMessage(idList, New<Int>(orderIndex));
    }

    void UnitManager::orderCurrentlySelectedUnitsToBuildUnit(Ref<UnitTemplate> unitTemplate)
    {
        if (this->selectionList->Size() <= 0) return;

        auto orderIndex = UnitOrderType::Train;

        auto g = (Global*) InternalApp::getGlobal();

        auto idList = New<List<Int>>();
        auto otherNumberList = New<List<Int>>();

        for (auto iterator = this->selectionList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            Ref<Int> intObj = New<Int>(unit->id);
            idList->Add(intObj);

            unit->currentOrder = orderIndex;
            unit->trainUnit(unitTemplate);
        }

        otherNumberList->Add(New<Int>(unitTemplate->index));

        g->networkManager->messageSender->sendUnitOrderMessage(idList, New<Int>(orderIndex), otherNumberList);
    }

    void UnitManager::orderCurrentlySelectedUnitsToCancelTrainUnit(int queueIndex)
    {
        auto unit = this->selectionList->At(0);
        
        auto orderIndex = UnitOrderType::CancelTrain;

        auto idList = New<List<Int>>();
        auto otherNumberList = New<List<Int>>();

        Ref<Int> intObj = New<Int>(unit->id);
        idList->Add(intObj);
        unit->currentOrder = orderIndex;
        unit->cancelTrainUnit(queueIndex);

        otherNumberList->Add(New<Int>(queueIndex));

        auto g = (Global*) InternalApp::getGlobal();
        g->networkManager->messageSender->sendUnitOrderMessage(idList, New<Int>(orderIndex), otherNumberList);
    }

    void UnitManager::issueUnitOrderSpecialAction(int actionIndex)
    {
        auto unit = this->selectionList->At(0);

        auto orderIndex = UnitOrderType::SpecialAction;

        auto idList = New<List<Int>>();
        auto otherNumberList = New<List<Int>>();

        Ref<Int> intObj = New<Int>(unit->id);
        idList->Add(intObj);
        unit->currentOrder = orderIndex;
        unit->specialAction(actionIndex);

        otherNumberList->Add(New<Int>(actionIndex));

        auto g = (Global*) InternalApp::getGlobal();
        g->networkManager->messageSender->sendUnitOrderMessage(idList, New<Int>(orderIndex), otherNumberList);
    }

    void UnitManager::issueUnitOrderSpecialAction(int actionIndex, int x, int y)
    {
        auto unit = this->selectionList->At(0);

        auto orderIndex = UnitOrderType::SpecialAction;

        auto idList = New<List<Int>>();
        auto otherNumberList = New<List<Int>>();

        Ref<Int> intObj = New<Int>(unit->id);
        idList->Add(intObj);
        unit->currentOrder = orderIndex;
        unit->specialAction(actionIndex);

        otherNumberList->Add(New<Int>(actionIndex));
        otherNumberList->Add(New<Int>(x));
        otherNumberList->Add(New<Int>(y));

        auto g = (Global*) InternalApp::getGlobal();
        g->networkManager->messageSender->sendUnitOrderMessage(idList, New<Int>(orderIndex), otherNumberList);
    }

    void UnitManager::orderUnitSwapInventory(Ref<Unit> unit, int beforeSlotIndex, int afterSlotIndex)
    {
        if (beforeSlotIndex == afterSlotIndex)return;

        auto orderIndex = UnitOrderType::ItemSwap;

        auto g = (Global*) InternalApp::getGlobal();

        auto idList = New<List<Int>>();
        auto otherNumberList = New<List<Int>>();

        idList->Add(New<Int>(unit->id));

        otherNumberList->Add(New<Int>(beforeSlotIndex));
        otherNumberList->Add(New<Int>(afterSlotIndex));

        g->networkManager->messageSender->sendUnitOrderMessage(idList, New<Int>(orderIndex), otherNumberList);
    }

    void UnitManager::orderUnitDropItem(Ref<Unit> unit, int slotIndex, Ref<Point> position)
    {
        auto orderIndex = UnitOrderType::ItemDrop;

        auto g = (Global*) InternalApp::getGlobal();

        Ref<Point> tilePosition = Null<Point>();
        auto mmPosition = g->uiManager->getMinimapPosition(g->cursor->position);
        tilePosition = (mmPosition == nullptr) ?
                       g->tileManager->getTilePosition(position->x, position->y) : mmPosition;

        auto idList = New<List<Int>>();
        auto otherNumberList = New<List<Int>>();

        idList->Add(New<Int>(unit->id));

        otherNumberList->Add(New<Int>(slotIndex));
        otherNumberList->Add(New<Int>(tilePosition->x));
        otherNumberList->Add(New<Int>(tilePosition->y));

        g->networkManager->messageSender->sendUnitOrderMessage(idList, New<Int>(orderIndex), otherNumberList);
    }

    void UnitManager::orderUnitGiveItem(Ref<Unit> unit, int slotIndex, int targetUnitId)
    {
        auto orderIndex = UnitOrderType::ItemGive;

        auto g = (Global*) InternalApp::getGlobal();

        auto idList = New<List<Int>>();
        auto otherNumberList = New<List<Int>>();

        idList->Add(New<Int>(unit->id));

        otherNumberList->Add(New<Int>(slotIndex));
        otherNumberList->Add(New<Int>(targetUnitId));

        g->networkManager->messageSender->sendUnitOrderMessage(idList, New<Int>(orderIndex), otherNumberList);
    }

    void UnitManager::issueUnitOrderRally(){
        if (this->selectionList->Size() <= 0) return;

        auto g = (Global*) InternalApp::getGlobal();

        UnitOrderType orderIndex = RallyPoint;

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

        auto idList = New<List<Int>>();
        auto otherNumberList = New<List<Int>>();

        for (auto iterator = this->selectionList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto unit = iterator->Get();
            idList->Add(New<Int>(unit->id));

            unit->currentOrder = orderIndex;
        }

        if (orderIndex == RallyPoint) {
            otherNumberList->Add(New<Int>(tilePosition->x));
            otherNumberList->Add(New<Int>(tilePosition->y));
        }
        if (orderIndex == RallyUnit){
            otherNumberList->Add(New<Int>(targetId));
        }

        g->networkManager->messageSender->sendUnitOrderMessage(idList, New<Int>(orderIndex), otherNumberList);
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

    Ref<MapIterator<Unit>> UnitManager::getUnitsIterator() {
        return this->unitMap->GetIterator();
    }

    int UnitManager::getNumberOfUnits() {
        return this->unitMap->Size();
    }
}