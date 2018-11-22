//
// Created by connor on 7/23/18.
//

#include <Sova/Internal/InternalApp.h>
#include <iostream>
#include <Sova/Graphics/Sprite.h>
#include <Sova/Math/Math.h>
#include "TileManager.h"
#include "Sova/Common/String.h"
#include "Sova/Graphics/Internal/InternalTexture.h"
#include "Sova/Graphics/Internal/InternalCamera.h"
#include "Global.h"
#include "Block.h"
#include "FogManager.h"
#include "Game/Minimap/Minimap.h"
#include "Game/Unit/UnitManager.h"
#include "Game/Item/ItemManager.h"

using namespace Sova;
using namespace Oryol;

namespace DsprFrontend
{
    TileManager::TileManager()
    {
    }

    TileManager::~TileManager()
    {
    }

    void TileManager::receiveGrid(int width, int height)
    {
        if (this->receivedGrid) return;

        Oryol::String textureString = Oryol::String("images/tiles.png");
        this->texture = InternalApp::getInternalApp()->resourceManager.textures[textureString];
        this->visible = true;

        this->gridWidth = width;
        this->gridHeight = height;
        this->receivedGrid = true;

        this->tileGrid = New<RefIsoGrid<Tile>>();
        this->tileGrid->initialize(this->gridWidth * 2, this->gridHeight * 2);

        auto g = (Global*) InternalApp::getGlobal();
        g->fogManager->receiveGrid(this->gridWidth, this->gridHeight);
        g->unitManager->receiveGrid(this->gridWidth, this->gridHeight);
        g->itemManager->receiveGrid(this->gridWidth, this->gridHeight);

        std::cout << "received Grid"<< std::endl;
    }

    void TileManager::receiveTile(int tileX, int tileY, int tileFrame)
    {
        if (!this->receivedGrid)
            return;

        auto g = (Global*) InternalApp::getGlobal();

        auto newTile = New<Tile>(tileFrame-1);
        this->tileGrid->set(tileX, tileY, newTile);

        g->minimap->DrawTile(tileX,tileY);
        g->fogManager->shroudToFog(tileX, tileY);

        if (tileFrame == 0)
        {
            auto newBlock = New<Block>(tileX, tileY);

            g->world->AddChild(newBlock);
        }

        //std::cout << "received tile: "<<tileX<<", "<<tileY<<", "<<tileFrame<< std::endl;
    }

    int TileManager::getTileFrame(int x, int y)
    {
        Ref<Tile> foundTile = this->tileGrid->get(x,y);
        if (foundTile != nullptr) return foundTile->frame;
        return -1;
    }

    Ref<Point> TileManager::getTilePosition(int inX, int inY)
    {
        int halfTileW = (this->tileWidth/2);
        int halfTileH = (this->tileHeight/2);
        int quarterTileW = (this->tileWidth/4);
        int quarterTileH = (this->tileHeight/4);

        int posx = inX - quarterTileW;
        int posy = inY - quarterTileH;

        int x = (posx) / halfTileW;
        int y = (posy) / halfTileH;
        if ((x % 2 == 0 && y % 2 == 0) || ((x+1) % 2 == 0 && (y+1) % 2 == 0)) return New<Point>(x, y);
        int modx = (posx % halfTileW) - quarterTileW;
        int mody = ((posy % halfTileH) - quarterTileH)*2;
        if (Sova::Math::Abs(modx) > Sova::Math::Abs(mody))
        {
            return New<Point>(x+Sova::Math::Sign(modx), y);
        }
        else
        {
            return New<Point>(x, y+Sova::Math::Sign(mody));
        }
    }

    void TileManager::Draw(Ref<Camera> camera, int xoffset, int yoffset)
    {
        auto g = (Global*) InternalApp::getGlobal();
        if (this->destroyed) return;
        if (!this->visible) return;

        yoffset-=1;

        const auto resState = Gfx::QueryResourceInfo(this->texture->textureId).State;
        if (resState == ResourceState::Valid)
        {
            int a = xoffset / -tileWidth;
            int b = yoffset / -tileHeight;

            for (int j = -1; j < 12; j++) // j should take into account grid height... right?
            {
                for (int i = -1; i < 14; i += 1) // j should take into account grid height... right?
                {
                    int x = a + i;
                    int y = b + j;

                    if (!g->fogManager->tileIsInShroud(x * 2, y * 2))
                    {
                        Ref<Tile> tile = this->tileGrid->get(x * 2, y * 2);
                        if (tile == nullptr) continue;

                        drawTile(camera, ((x) * tileWidth) + xoffset, ((y) * tileHeight) + yoffset,
                                 tile->frame);
                    }

                    if (!g->fogManager->tileIsInShroud((x * 2)+1, (y * 2)+1))
                    {
                        Ref<Tile> tile = this->tileGrid->get((x * 2)+1, (y * 2)+1);
                        if (tile == nullptr) continue;

                        drawTile(camera, ((x + 0.5f) * tileWidth) + xoffset, ((y + 0.5f) * tileHeight) + yoffset,
                                 tile->frame);
                    }
                }
            }
        }
    }

    void TileManager::drawTile(Ref<Camera> camera, float xoffset, float yoffset, int frame)
    {
        Oryol::DrawState drawState = camera->getInternalCamera()->getDrawState();
        drawState.FSTexture[0] = this->texture->textureId;
        const void *data = this->updateVertices(xoffset, yoffset, this->texture->width, this->texture->height,
                                                camera->getInternalCamera()->getWidth(),
                                                camera->getInternalCamera()->getHeight(), tileWidth, tileHeight+2, 1, frame);
        Gfx::UpdateVertices(drawState.Mesh[0], data, InternalApp::numVertexesInQuad);
        Gfx::ApplyDrawState(drawState);

        Gfx::Draw();
    }

    const void * TileManager::updateVertices(float x, float y, int texWidth, int texHeight, int canWidth, int canHeight,
                                             int frameWidth, int frameHeight, int padding, int frameIndex) {

        int vIndex = 0;

        //0 is 0, 1 is canvasWidth, canvasHeight
        float x0 = x / (float) canWidth;
        float y0 = y / (float) canHeight;
        float x1 = (frameWidth - (padding*2) + x) / (float) canWidth;
        float y1 = (frameHeight - (padding*2) + y) / (float) canHeight;

        //0 is 0, 1 is texWidth/texHeight
        //This is the texture
        float u0 = (float) ((frameIndex * frameWidth) + padding) / (float) texWidth;
        float v0 = (float) padding / (float) texHeight;
        float u1 = (float) (((frameIndex+1) * frameWidth) - padding) / (float) texWidth;
        float v1 = (float) (frameHeight - padding) / (float) texHeight;

        vIndex = this->writeVertex(vIndex, x0, y0, u0, v0);
        vIndex = this->writeVertex(vIndex, x1, y0, u1, v0);
        vIndex = this->writeVertex(vIndex, x1, y1, u1, v1);
        vIndex = this->writeVertex(vIndex, x0, y0, u0, v0);
        vIndex = this->writeVertex(vIndex, x1, y1, u1, v1);
        vIndex = this->writeVertex(vIndex, x0, y1, u0, v1);

        return InternalApp::getInternalApp()->vertexBuffer;
    }

    int TileManager::writeVertex(int index, float x, float y, float u, float v) {
        InternalApp::getInternalApp()->vertexBuffer[index].x = x;
        InternalApp::getInternalApp()->vertexBuffer[index].y = y;
        InternalApp::getInternalApp()->vertexBuffer[index].u = u;
        InternalApp::getInternalApp()->vertexBuffer[index].v = v;
        InternalApp::getInternalApp()->vertexBuffer[index].r = 1.0f;
        InternalApp::getInternalApp()->vertexBuffer[index].g = 1.0f;
        InternalApp::getInternalApp()->vertexBuffer[index].b = 1.0f;
        return index + 1;
    }

    Ref<Point> TileManager::getGridSize() {
        return New<Point>(gridWidth, gridHeight);
    }
}