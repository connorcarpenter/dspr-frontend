#pragma once

//
// Created by connor on 7/26/18.
//

#include <Sova/Graphics/AnimatedSprite.h>
#include "UnitOrder.h"
#include "ConstructionQueue.h"

namespace Sova
{
    class Sound;
}

namespace DsprFrontend
{
    class UnitTemplate;
    class Manaball;

    class Unit : public Sova::AnimatedSprite
    {
        SovaTypeDecl(Unit, AnimatedSprite);

    public:

        Unit(int id, int x, int y, int tribeIndex, Ref<UnitTemplate> unitTemplate);
        ~Unit();
        enum UnitAnimationState { Walking, Attacking, Gathering };
        void drawSelf(Ref<Camera> camera, int xoffset, int yoffset) override;
        void newNextTilePosition(int x, int y);
        void setAnimationState(UnitAnimationState newState, int heading);
        void playSelectedSound();
        void playDeathSound();
        void trainUnit(Ref<UnitTemplate> unitTemplate);
        void gatherYield(int yieldAmount);
        //
        Ref<Point> moveTarget = Null<Point>();
        Ref<Point> tilePosition = Null<Point>();
        Ref<Point> lastPosition = Null<Point>();
        Ref<Point> nextTilePosition = Null<Point>();
        int id = -1;

        bool hovering = false;
        bool selected = false;
        Ref<AnimatedSprite> tcSprite = Null<AnimatedSprite>();
        Ref<AnimatedSprite> skinSprite = Null<AnimatedSprite>();
        Ref<AnimatedSprite> hairSprite = Null<AnimatedSprite>();
        int tribeIndex = -1;
        UnitAnimationState animationState = Walking;
        int health;
        bool facingDown = true;

        UnitOrder currentOrder = Move;
        Ref<UnitTemplate> unitTemplate = Null<UnitTemplate>();
        Ref<ConstructionQueue> constructionQueue = Null<ConstructionQueue>();
        Ref<SpecificUnit> specificUnit = Null<SpecificUnit>();
        int targetUnitId = -1;
        Ref<Manaball> myManaball = Null<Manaball>();

    private:

        void step(float deltaMs);

        const int interpolationMax = 6;
        const int interpolationStep = 1;
        const int maxWalkAmount = 6;
        const int walkSpeedStraight = maxWalkAmount / 2;
        const int walkSpeedDiagonal = maxWalkAmount / 3;
        const float walkImageSpeed = 0.1f;
        const int gameServerTickMs = 100;

        float attackFrameIndex = 0;
        float attackWaitIndex = 0;
        const float attackAnimationSpeed = 1;
        const float attackWaitSpeed = 1.0f / 1.7f;
        const int attackFramesNumber = 5;
        const int attackWaitFrames = 5;
        const int acquisition = 6; // 12 will be a circle that touches the edges of the screen if centered on the unit, 8 is standard
        float walkAmount = 0;
        int walkSpeed = walkSpeedStraight;
        bool checkReleaseSelectionBox = false;
        float interpolation = 0;
        int stamina = 100;
        int stillFrames = 0;

        float gatherFrameIndex = 0;
        const int gatherFramesToYield = 95;
        bool gatherYielding = false;

        void walkingStep(float deltaFrameMs);
        void attackingStep(float deltaFrameMs);

        void updatePosition();

        void updateTilePosition(Ref<Point> newPosition);

        void handleFacing(int heading);

        void gatheringStep(float deltaFrameMs);

    };
}
