//
// Created by connor on 7/26/18.
//

#include <Sova/Graphics/AnimatedSprite.h>

namespace DsprFrontend
{
    class Unit : public Sova::AnimatedSprite
    {
        SovaTypeDecl(Unit, AnimatedSprite);
    public:
        Unit();
        void drawSelf(Ref<Camera> camera, int xoffset, int yoffset) override;
    private:
        void step();
        bool selected = false;
        bool hovering = false;
    };
}
