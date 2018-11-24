//
// Created by connor on 10/29/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include <Sova/Graphics/Rectangle.h>
#include <Sova/Math/Math.h>
#include "Projectile.h"
#include "GraphicsManager.h"
#include "SpriteCatalog.h"
#include "Game/TileManager.h"
#include "Sova/Graphics/Rectangle.h"

namespace DsprFrontend {

    Projectile::Projectile(int fromX, int fromY, int toX, int toY, int index) {

        auto g = (Global*) InternalApp::getGlobal();
        this->from = New<Point>(fromX, fromY);
        this->to = New<Point>(toX, toY);
        this->position->x = (int) ((((float) fromX/2) + 0.5f) * g->tileManager->tileWidth);
        this->position->y = (int) ((((float) fromY/2) + 0.5f) * g->tileManager->tileHeight);

        this->distance = Math::Distance(fromX, fromY, toX, toY);
        this->maxDistance = this->distance;

        this->speed = 3;


        this->rectangle = New<Rectangle>(1, 1);
        this->rectangle->setLineStyle(false);
        this->rectangle->setFillStyle(Color::Gray, 1.0f);
        this->rectangle->size->set(2,2);

        this->OnUpdate([&](float deltaFrameMs){ step(deltaFrameMs); });
    }

    void Projectile::step(float deltaMs)
    {
        auto g = (Global*) InternalApp::getGlobal();
        this->distance -= this->speed * (deltaMs / g->gameServerTickMs);
        if (this->distance <= 0) this->Destroy();

        this->position->x = (int) (((Math::Lerp(this->to->x, this->from->x, this->distance/this->maxDistance)/2) + 0.5f) * g->tileManager->tileWidth);
        this->position->y = (int) (((Math::Lerp(this->to->y, this->from->y, this->distance/this->maxDistance)/2) + 0.5f) * g->tileManager->tileHeight) - 7;
    }

    void Projectile::drawSelf(Ref<Camera> camera, int xoffset, int yoffset) {

        this->rectangle->position->set(this->position->x, this->position->y);

        this->rectangle->drawSelf(camera, xoffset, yoffset);
    }
}