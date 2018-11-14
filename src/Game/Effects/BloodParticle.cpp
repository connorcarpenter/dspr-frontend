//
// Created by connor on 9/6/18.
//

#include <Sova/Math/Math.h>
#include <Sova/Internal/InternalApp.h>
#include "BloodParticle.h"
#include "Game/Global.h"
#include "SpriteCatalog.h"

namespace DsprFrontend
{

    BloodParticle::BloodParticle(Ref<Point> position, float z, int depth) {
        this->floatPos = New<FloatPoint>(position->x, position->y);
        this->z = z;

        this->imageSpeed = 0;
        this->imageIndex = Math::Random(0,14);

        this->zVelocity = (Math::Random(0.5f,2) * -1);

        this->scale->x = Math::Sign(Math::Random(0,2) - 1);

        auto dir = Math::Random(0,360);
        float speed = Math::Random(0.25f,1.0f);
        this->velocity = New<FloatPoint>(Math::Cos(dir)*speed, Math::Sin(dir)*speed);

        auto g = (Global*) InternalApp::getGlobal();
        this->useAnimatedSpriteInfo(g->spriteCatalog->bloodParticle);

        this->SetDepth(depth + Math::Sign((int) this->velocity->y));
    }

    void BloodParticle::drawSelf(Ref<Camera> camera, int xoffset, int yoffset) {
        if (this->z < 0) {
            this->z += this->zVelocity;
            this->zVelocity += this->zGravity;
            this->imageIndex = Math::Random(0,14);
        } else {
            this->z = 0;
            this->disappearTimer--;
            if (this->disappearTimer<=0)
                this->Destroy();
        }
        this->floatPos->x += this->velocity->x;
        this->floatPos->y += this->velocity->y;
        if (Math::Distance(0,0,this->velocity->x, this->velocity->y) > 0.001f) {
            this->velocity->x *= this->friction;
            this->velocity->y *= this->friction;
        } else {
            this->velocity->set(0,0);
        }

        this->position->x = (int) this->floatPos->x;
        this->position->y = (int) (this->floatPos->y + this->z);

        AnimatedSprite::drawSelf(camera, xoffset, yoffset);
    }
}