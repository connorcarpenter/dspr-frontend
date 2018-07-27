//
// Created by connor on 7/26/18.
//

#include <Sova/Graphics/AnimatedSprite.h>

namespace DsprFrontend
{
    class Cursor : public Sova::AnimatedSprite
    {
    public:
        Cursor();

        void changeState(int index);

    private:
        void step();

    };
}
