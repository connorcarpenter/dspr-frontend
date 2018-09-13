//
// Created by connor on 9/5/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Sova/Math/Math.h>
#include "DyingUnit.h"
#include "Global.h"
#include "SpriteCatalog.h"
#include "Unit/Unit.h"
#include "Sova/Audio/Sound.h"

namespace DsprFrontend
{
    DyingUnit::DyingUnit(Ref<DsprFrontend::Unit> unit)
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        this->tcSprite = New<AnimatedSprite>();

        if (unit->facingDown)
        {
            this->useAnimatedSpriteInfo(g->spriteCatalog->workerDieDown);
            this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerDieDownTC);
        }
        else
        {
            this->useAnimatedSpriteInfo(g->spriteCatalog->workerDieUp);
            this->tcSprite->useAnimatedSpriteInfo(g->spriteCatalog->workerDieUpTC);
        }

        switch (unit->tribeIndex)
        {
            case 0:
                this->tcSprite->tint = Color::Red;
                break;
            case 1:
                this->tcSprite->tint = Color::Blue;
                break;
        }

        this->imageSpeed = 0.1f;

        this->position = unit->position;
        this->scale = unit->scale;

        auto dieSound = New<Sound>(New<Sova::String>("sounds/die.wav"));
        dieSound->Play();

        this->disappearTimer = 1000;

        this->depth = unit->depth;
    }

    void DyingUnit::drawSelf(Ref<Camera> camera, int xoffset, int yoffset)
    {
        if (this->imageSpeed != 0) {
            if (Math::Floor(this->imageIndex) == 1)
                this->imageSpeed = 0;
        } else {
            this->disappearTimer--;
            if (this->disappearTimer < 0)
                this->Destroy();
        }

        AnimatedSprite::drawSelf(camera, xoffset + (this->scale->x == -1 ? -2 : 0), yoffset);

        //TC

        this->tcSprite->imageIndex = imageIndex;
        this->tcSprite->position->set(this->position);
        this->tcSprite->scale->x = this->scale->x;
        this->tcSprite->drawSelf(camera, xoffset + (this->scale->x == -1 ? -2 : 0), yoffset);
    }
}

