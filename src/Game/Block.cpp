//
// Created by connor on 8/16/18.
//

#include <Sova/Internal/InternalApp.h>

#include "Block.h"
#include "Global.h"
#include "FogManager.h"
#include "TileManager.h"
#include "Game/Minimap/Minimap.h"

namespace DsprFrontend
{
    Block::Block(int x, int y) : Sprite()
    {
        this->setTexture(New<Sova::String>("images/block.png"));
        this->visible = false;
        this->tilePosition = New<Point>(x,y);

        auto g = (Global*) InternalApp::getGlobal();
        this->anchor->set(11, 14);
        this->position->set((this->tilePosition->x+1) * g->tileManager->tileWidth / 2,
                            (this->tilePosition->y+1) * g->tileManager->tileHeight / 2);
        this->SetDepth(this->tilePosition->y * -1);

        this->OnUpdate([&](float deltaFrameMs){ step(); });
    }

    void Block::step()
    {
        if (!this->visible)
        {
            auto g = (Global*) InternalApp::getGlobal();
            if (!g->fogManager->tileIsInShroud(this->tilePosition->x, this->tilePosition->y))
            {
                this->visible = true;
                g->minimap->DrawTile(this->tilePosition->x, this->tilePosition->y, Color::Gray);
            }
        }
    }
}

