//
// Created by connor on 9/1/18.
//

#include <Sova/Math/Point.h>
#include "Sova/Common/List.h"
#include "Sova/References/Ref.h"
#include "Sova/References/Refable.h"

namespace DsprFrontend
{
    class Circle : public Sova::Refable
    {

    public:

        Circle(int radius);

        Sova::Ref<Sova::List<Sova::Point>> coordList = Null<Sova::List<Sova::Point>>();
        int radius;
    };
}