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
        Unit(int id, int x, int y);
        void drawSelf(Ref<Camera> camera, int xoffset, int yoffset) override;
        void newNextTilePosition(int x, int y);
        //
        Ref<Point> moveTarget = Null<Point>();
        Ref<Point> tilePosition = Null<Point>();
        Ref<Point> nextTilePosition = Null<Point>();
        int id = -1;
    private:
        void step();
        bool selected = false;
        bool hovering = false;
        int walkAmount = 0;
        int walkSpeed = 10;
        Ref<Sova::String> spriteDownName = Null<Sova::String>();
        Ref<Sova::String> spriteUpName = Null<Sova::String>();
        bool checkReleaseSelectionBox = false;


    };
}
