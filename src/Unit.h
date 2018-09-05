//
// Created by connor on 7/26/18.
//

#include <Sova/Graphics/AnimatedSprite.h>

namespace Sova
{
    class Sound;
}

namespace DsprFrontend
{
    enum AnimationState { Walking, Attacking };

    class Unit : public Sova::AnimatedSprite
    {
        SovaTypeDecl(Unit, AnimatedSprite);

    public:

        Unit(int id, int x, int y, int tribeIndex);
        void drawSelf(Ref<Camera> camera, int xoffset, int yoffset) override;
        void newNextTilePosition(int x, int y);
        void setAnimationState(AnimationState newState, int heading);
        //
        Ref<Point> moveTarget = Null<Point>();
        Ref<Point> tilePosition = Null<Point>();
        Ref<Point> lastPosition = Null<Point>();
        Ref<Point> nextTilePosition = Null<Point>();
        int id = -1;

        bool hovering = false;
        bool selected = false;
        Ref<AnimatedSprite> tcSprite = Null<AnimatedSprite>();
        int tribeIndex = -1;
        AnimationState animationState = Walking;
    private:

        void step(float deltaMs);

        const int interpolationMax = 6;
        const int interpolationStep = 1;
        const int maxWalkAmount = 6;
        const int walkSpeedStraight = maxWalkAmount / 2;
        const int walkSpeedDiagonal = maxWalkAmount / 3;
        const float walkImageSpeed = 0.1f;
        float attackImageSpeed = 0.2f;
        const int gameServerTickMs = 100;
        const int sight = 8; // 12 will be a circle that touches the edges of the screen if centered on the unit, 8 is standard

        float walkAmount = 0;
        int walkSpeed = walkSpeedStraight;
        bool checkReleaseSelectionBox = false;
        float interpolation = 0;

        int health = 100;
        int stamina = 100;

        void walkingStep(float deltaFrameMs);
        void attackingStep(float deltaFrameMs);

        int stillFrames = 0;

        void updatePosition();

        Ref<Sova::Sound> hitSound = Null<Sova::Sound>();
    };
}
