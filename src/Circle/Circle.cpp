//
// Created by connor on 9/1/18.
//

#include "Circle.h"
#include "Sova/Math/Math.h"

namespace DsprFrontend
{
    Circle::Circle(int radius) {
        this->radius = radius;

        this->coordList = New<Sova::List<Sova::Point>>();

        for (int x = -radius; x <= radius; x++)
        {
            for (int y = -radius; y <= radius; y++)
            {
                if (!((x % 2 == 0 && y % 2 == 0) || ((x+1) % 2 == 0 && (y+1) % 2 == 0)))continue;

                if (Sova::Math::Distance(0,0,x,y) > radius)
                    continue;

                auto newCoord = New<Sova::Point>(x,y);
                this->coordList->Add(newCoord);
            }
        }
    }
}