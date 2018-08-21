//
// Created by connor on 8/16/18.
//

#include <Sova/Internal/InternalApp.h>
#include "Block.h"
#include "Global.h"
#include "FogManager.h"

namespace DsprFrontend
{
    Block::Block(int x, int y) : Sprite()
    {
        this->setTexture(New<Sova::String>("images/block.png"));
        this->setVisible(false);
        this->tilePosition = New<Point>(x,y);

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        this->anchor->set(11, 14);
        this->position->set((this->tilePosition->x+1) * g->tileManager->tileWidth / 2,
                            (this->tilePosition->y+1) * g->tileManager->tileHeight / 2);
        this->SetDepth(this->tilePosition->y * -1);

        this->OnUpdate([&](float deltaFrameMs){ step(); });
    }

    void Block::step()
    {
        if (!this->getVisible())
        {
            auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
            if (!g->fogManager->tileIsInShroud(this->tilePosition->x, this->tilePosition->y))
            {
                this->setVisible(true);
                g->minimap->DrawTile(this->tilePosition->x, this->tilePosition->y, Color::Gray);
            }
        }
    }
}

