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
        Ref<Point> lastPosition = Null<Point>();
        Ref<Point> nextTilePosition = Null<Point>();
        int id = -1;

        bool hovering = false;
        bool selected = false;
    private:

        void step(float deltaMs);

        const int interpolationMax = 6;
        const int interpolationStep = 1;
        const int maxWalkAmount = 6;
        const int walkSpeedStraight = maxWalkAmount / 2;
        const int walkSpeedDiagonal = maxWalkAmount / 3;
        const float walkImageSpeed = 0.1f;
        const int gameServerTickMs = 100;
        const int sight = 8; // 12 will be a circle that touches the edges of the screen if centered on the unit, 8 is standard

        float walkAmount = 0;
        int walkSpeed = walkSpeedStraight;
        Ref<Sova::String> spriteDownName = Null<Sova::String>();
        Ref<Sova::String> spriteUpName = Null<Sova::String>();
        bool checkReleaseSelectionBox = false;
        float interpolation = 0;
    };
}
