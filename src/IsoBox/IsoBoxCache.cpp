//
// Created by connor on 9/1/18.
//

#include "IsoBoxCache.h"

namespace DsprFrontend
{
    IsoBoxCache::IsoBoxCache() {
        this->isoBoxList = New<List<IsoBox>>();
    }

    Ref<IsoBox> IsoBoxCache::getIsoBox(int width, int height) {

        for (auto iterator = this->isoBoxList->GetIterator(); iterator->Valid(); iterator->Next()) {
            auto isoBox = iterator->Get();
            if (isoBox->width == width && isoBox->height == height)
                return isoBox;
        }

        auto newIsoBox = New<IsoBox>(width, height);
        this->isoBoxList->Add(newIsoBox);
        return newIsoBox;
    }
}