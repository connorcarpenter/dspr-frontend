//
// Created by connor on 10/26/18.
//

#include "ConstructionQueue.h"
#include "Unit.h"

namespace DsprFrontend
{
    ConstructionQueue::ConstructionQueue() {
        this->unitQueue = New<List<UnitTemplate>>();
    }

    bool ConstructionQueue::atMaxQueue()
    {
        return this->unitQueue->Size() >= 7;
    }

    void ConstructionQueue::enqueue(Ref<UnitTemplate> unitTemplate)
    {
        this->unitQueue->Add(unitTemplate);
    }

    void ConstructionQueue::update(float delta)
    {
        if (this->unitQueue->Size() > 0)
        {
            this->currentBuildTime += delta;
//            auto currentlyBuildingUnit = unitQueue->At(0);
//            if (this->currentBuildTime >= currentlyBuildingUnit->buildTime)
//            {
//                this->currentBuildTime = 0;
//                unitQueue->Remove(currentlyBuildingUnit);
//                if (currentlyBuildingUnit->readySound != nullptr){
//                    currentlyBuildingUnit->readySound->Play();
//                }
//            }
        }
    }

    bool ConstructionQueue::isTraining() {
        return this->unitQueue->Size() > 0;
    }

    Ref<ListIterator<UnitTemplate>> ConstructionQueue::GetIterator()
    {
        return this->unitQueue->GetIterator();
    }

    int ConstructionQueue::getCurrentTotalBuildTime()
    {
        if (this->unitQueue->Size() <= 0) return 0;
        return this->unitQueue->At(0)->buildTime;
    }

    void ConstructionQueue::emptyQueue() {
        this->unitQueue->Clear();
    }

    void ConstructionQueue::cancelAtIndex(int index) {
        if (index == 0) this->currentBuildTime = 0;
        this->unitQueue->Remove(index);
    }
}