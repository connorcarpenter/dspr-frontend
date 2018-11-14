//
// Created by connor on 10/29/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include "Sparkle.h"
#include "GraphicsManager.h"
#include "SpriteCatalog.h"

namespace DsprFrontend {

    Sparkle::Sparkle(Ref<Point> position) : AnimatedSprite() {
        this->position = position;
        this->OnUpdate([&](float deltaFrameMs){ step(deltaFrameMs); });

        auto g = (Global*) InternalApp::getGlobal();
        this->useAnimatedSpriteInfo(g->spriteCatalog->sprSparkle);

        this->imageSpeed = 0.2f;
    }

    void Sparkle::step(float deltaMs) {

        this->ttl--;
        if (this->ttl < 0)this->Destroy();
    }
}