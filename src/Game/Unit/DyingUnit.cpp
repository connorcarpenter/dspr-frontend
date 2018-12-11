//
// Created by connor on 9/5/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Sova/Math/Math.h>
#include "DyingUnit.h"
#include "Game/Global.h"
#include "SpriteCatalog.h"
#include "Game/Unit/Unit.h"
#include "Sova/Audio/Sound.h"
#include "UnitManager.h"

namespace DsprFrontend
{
    DyingUnit::DyingUnit(Ref<DsprFrontend::Unit> unit)
    {
        auto g = (Global*) InternalApp::getGlobal();

        this->useAnimatedSpriteInfo(unit->unitTemplate->sprBase);
        this->tcSprite = New<AnimatedSprite>();
        this->tcSprite->useAnimatedSpriteInfo(unit->unitTemplate->sprTC);
        this->tcSprite->tint = g->tribeManager->getTeamColor(unit->tribeIndex);

        if (unit->facingDown)
            this->useAnimatedSequenceInfo(unit->unitTemplate->sprDieDown);
        else
            this->useAnimatedSequenceInfo(unit->unitTemplate->sprDieUp);

        this->imageSpeed = 0.1f;

        this->position = unit->position;
        this->scale = unit->scale;

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

        this->tcSprite->imageIndex = imageIndex + this->frameStartIndex;
        this->tcSprite->position->set(this->position);
        this->tcSprite->scale->x = this->scale->x;
        this->tcSprite->drawSelf(camera, xoffset + (this->scale->x == -1 ? -2 : 0), yoffset);
    }
}

