#pragma once

//
// Created by connor on 10/28/18.
//

#include <Sova/References/Refable.h>
#include <Sova/References/Ref.h>

namespace DsprFrontend {
    class SpecificUnit : public Sova::Refable {
    public:
        virtual void stepFunction() = 0;
    };
}
