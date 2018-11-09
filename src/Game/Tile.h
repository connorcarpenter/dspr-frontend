#pragma once

//
// Created by connor on 7/23/18.
//

#include <Sova/References/Refable.h>

namespace DsprFrontend
{
    class Tile : public Sova::Refable
    {

    public:
        Tile(int frame);

        int frame = -1;
    };
}
