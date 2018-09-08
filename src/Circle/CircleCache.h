//
// Created by connor on 9/1/18.
//

#include "Circle.h"
#include "Sova/References/Ref.h"

using namespace Sova;

namespace DsprFrontend {
    class CircleCache : public Sova::Refable {
    public:
        CircleCache();
        Ref<Circle> getCircle(int radius);
    private:

        Ref<List<Circle>> circleList = Null<List<Circle>>();
    };
}