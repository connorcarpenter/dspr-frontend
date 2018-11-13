//
// Created by connor on 11/8/18.
//

#include <Sova/Internal/InternalApp.h>
#include <DsprColors.h>
#include <Game/Effects/Sparkle.h>
#include <Sova/Math/Math.h>
#include "Item.h"
#include "Game/Global.h"
#include "Game/TileManager.h"
#include "SpriteCatalog.h"

namespace DsprFrontend {

    Item::Item(int id, int x, int y, int index) : AnimatedSprite()
    {
        this->id = id;
        this->index = index;
        this->tilePosition = New<Point>(x,y);

        auto g = (Global*) InternalApp::getGlobal();
        this->position->x = (int) (((this->tilePosition->x/2) + 0.5f) * g->tileManager->tileWidth);
        this->position->y = (int) (((this->tilePosition->y/2) + 0.5f) * g->tileManager->tileHeight);

        this->useAnimatedSpriteInfo(g->spriteCatalog->itemsGround);

        this->SetDepth(this->tilePosition->y * -1);

        this->imageIndex = index;

        this->OnUpdate([&](float deltaFrameMs){ step(deltaFrameMs); });

        this->sparkleTimer = Math::Random(30, 600);

        //if (this->position->x % 2 == 1)this->scale->x *= -1;
        //if (this->position->y % 2 == 0)this->scale->y *= -1;
    }

    void Item::drawSelf(Ref<Camera> camera, int xoffset, int yoffset)
    {
            auto g = (Global*) InternalApp::getGlobal();

            if (this->selected)
            {
                    g->unitSelectCircle_1x1->tint = DsprColors::LightYellow;
                    g->unitSelectCircle_1x1->position->set(this->position->x, this->position->y);
                    g->unitSelectCircle_1x1->drawSelf(camera, xoffset, yoffset);
            }

            if (this->hovering)
            {
                    g->unitHoverCircle_1x1->tint = DsprColors::LightYellow;
                    g->unitHoverCircle_1x1->position->set(this->position->x, this->position->y);
                    g->unitHoverCircle_1x1->drawSelf(camera, xoffset, yoffset);
            }

            AnimatedSprite::drawSelf(camera, xoffset, yoffset);
    }

    void Item::step(float deltaMs) {
            if (this->sparkleTimer == 0)
            {
                this->sparkleTimer = Math::Random(240, 600);
                auto sparklePos = New<Point>(this->position->x + Math::Random(-2,3), this->position->y + Math::Random(-2, 3));
                auto sparkle = New<Sparkle>(sparklePos);
                sparkle->SetDepth(this->depth-1);

                auto g = (Global*) InternalApp::getGlobal();
                g->world->AddChild(sparkle);
            }

        this->sparkleTimer--;

    }
}