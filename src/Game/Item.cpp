//
// Created by connor on 11/8/18.
//

#include <Sova/Internal/InternalApp.h>
#include "Item.h"
#include "Global.h"
#include "TileManager.h"
#include "SpriteCatalog.h"

namespace DsprFrontend {

    Item::Item(int id, int x, int y, int index) : AnimatedSprite() {
        this->id = id;
        this->index = index;
        this->tilePosition = New<Point>(x,y);

        auto g = (Global*) InternalApp::getGlobal();
        this->position->x = (int) ((this->tilePosition->x/2) + 0.5f) * g->tileManager->tileWidth;
        this->position->y = (int) ((this->tilePosition->y/2) + 0.5f) * g->tileManager->tileHeight;

        this->useAnimatedSpriteInfo(g->spriteCatalog->itemsGround);

        this->SetDepth(this->tilePosition->y * -1);

        this->imageIndex = index;
    }
}