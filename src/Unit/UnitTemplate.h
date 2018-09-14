#pragma once

//
// Created by connor on 9/12/18.
//

#include <Sova/References/Refable.h>

namespace DsprFrontend {
    class UnitTemplate : public Sova::Refable {
    public:
        UnitTemplate(int index);
        int index;
    };
}