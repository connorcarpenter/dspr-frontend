#pragma once

//
// Created by connor on 9/1/18.
//

#include "IsoBox.h"
#include "Sova/References/Ref.h"

using namespace Sova;

namespace DsprFrontend {
    class IsoBoxCache : public Sova::Refable {
    public:
        IsoBoxCache();
        Ref<IsoBox> getIsoBox(int width, int height);
    private:

        Ref<List<IsoBox>> isoBoxList = Null<List<IsoBox>>();
    };
}