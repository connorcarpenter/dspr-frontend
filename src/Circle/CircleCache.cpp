//
// Created by connor on 9/1/18.
//

#include "CircleCache.h"

namespace DsprFrontend
{
    CircleCache::CircleCache() {
        this->circleList = New<List<Circle>>();
    }

    Ref<Circle> CircleCache::getCircle(int radius) {

        for (auto iterator = this->circleList->GetIterator(); iterator->Valid(); iterator->Next()) {
            auto circle = iterator->Get();
            if (circle->radius == radius)
                return circle;
        }

        auto newCircle = New<Circle>(radius);
        this->circleList->Add(newCircle);
        return newCircle;
    }
}