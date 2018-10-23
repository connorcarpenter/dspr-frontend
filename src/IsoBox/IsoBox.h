#pragma once

//
// Created by connor on 9/1/18.
//

#include <Sova/Math/Point.h>
#include "Sova/Common/List.h"
#include "Sova/References/Ref.h"
#include "Sova/References/Refable.h"

namespace DsprFrontend
{
    class IsoBox : public Sova::Refable
    {

    public:

        IsoBox(unsigned int width, unsigned int height);

        Sova::Ref<Sova::List<Sova::Point>> coordList = Null<Sova::List<Sova::Point>>();
        unsigned int width;
        unsigned int height;
    };
}