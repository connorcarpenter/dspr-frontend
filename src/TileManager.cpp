//
// Created by connor on 7/23/18.
//

#include <Sova/Internal/InternalApp.h>
#include <iostream>
#include <Sova/Graphics/Sprite.h>
#include "TileManager.h"
#include "Sova/Common/String.h"
#include "Sova/Graphics/Internal/InternalTexture.h"
#include "Sova/Graphics/Internal/InternalCamera.h"
#include "Global.h"
#include "Block.h"
#include "FogManager.h"

using namespace Sova;

namespace DsprFrontend
{
    TileManager::TileManager()
    {
    }

    TileManager::~TileManager()
    {
        if (this->receivedGrid)
        {
            destroyTileArray(this->tileArrayA, this->gridWidth, this->gridHeight);
            destroyTileArray(this->tileArrayB, this->gridWidth, this->gridHeight);
        }
    }

    void TileManager::receiveGrid(Ref<Sova::String> width, Ref<Sova::String> height)
    {
        if (this->receivedGrid) return;

        Oryol::String textureString = Oryol::String("images/tiles.png");
        this->texture = InternalApp::getInternalApp()->resourceManager.textures[textureString];
        this->visible = true;

        this->gridWidth = atoi(width->AsCStr());
        this->gridHeight = atoi(height->AsCStr());
        this->receivedGrid = true;

        this->tileArrayA = initializeTileArray(this->gridWidth, this->gridHeight);
        this->tileArrayB = initializeTileArray(this->gridWidth, this->gridHeight);

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        g->fogManager->receiveGrid(this->gridWidth, this->gridHeight);

        std::cout << "received Grid"<< std::endl;
    }

    void TileManager::receiveTile(Ref<Sova::String> x, Ref<Sova::String> y, Ref<Sova::String> frame)
    {
        if (!this->receivedGrid)
            return;

        int tileX = atoi(x->AsCStr());
        int tileY = atoi(y->AsCStr());
        int tileFrame = atoi(frame->AsCStr());

        int gridIndex = getGridIndex(tileX, tileY);
        if (gridIndex == -1)
            return;

        if (tileFrame == -1)
        {
            auto newBlock = New<Block>(tileX, tileY);

            auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
            g->world->AddChild(newBlock);

            return;
        }

        int tileIndex = getTileIndex(gridIndex, tileX, tileY);
        if (gridIndex == 0)
        {
            this->tileArrayA[tileIndex] = new Tile(tileFrame);
        }
        else
        {
            this->tileArrayB[tileIndex] = new Tile(tileFrame);
        }

        //std::cout << "received tile: "<<tileX<<", "<<tileY<<", "<<tileFrame<< std::endl;
    }

    Tile** TileManager::initializeTileArray(int width, int height)
    {
        Tile** output = new Tile*[width * height];
        for (int j = 0; j < height; j += 1)
        {
            for (int i = 0;i< width; i+=1)
            {
                output[(j*width)+i] = nullptr;
            }
        }
        return output;
    }

    void TileManager::destroyTileArray(Tile** tileArray, int width, int height)
    {
        for (int j = 0; j < height; j += 1)
        {
            for (int i = 0;i< width; i+=1) {
                int index = (j * width) + i;
                if (tileArray[index] != nullptr) {
                    delete tileArray[index];
                    tileArray[index] = nullptr;
                }
            }
        }

        delete[] tileArray;
    }

    int TileManager::getGridIndex(int x, int y) {
        if (!this->receivedGrid) return -1;
        if (x < 0 || y < 0 || x >= this->gridWidth*2 || y >= this->gridHeight*2) return -1;

        if (x % 2 == 0 && y % 2 == 0) return 0;
        if ((x+1) % 2 == 0 && (y+1) % 2 == 0) return 1;
    }

    int TileManager::getTileIndex(int gridIndex, int x, int y) {
        if (gridIndex == 0)
        {
            int xsmall = x / 2;
            int ysmall = y / 2;
            return (ysmall * this->gridWidth) + xsmall;
        }
        else
        {
            int xsmall = (x-1) / 2;
            int ysmall = (y-1) / 2;
            return (ysmall * this->gridWidth) + xsmall;
        }
    }

    void TileManager::Draw(Ref<Camera> camera, int xoffset, int yoffset)
    {
        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        if (this->destroyed) return;
        if (!this->visible) return;

        const auto resState = Gfx::QueryResourceInfo(this->texture->textureId).State;
        if (resState == ResourceState::Valid)
        {
            int a = xoffset / -tileWidth;
            int b = yoffset / -tileHeight;

            for (int j = -1; j < 16; j++) // j should take into account grid height... right?
            {
                for (int i = -1; i < 14; i += 1) // j should take into account grid height... right?
                {
                    int x = a + i;
                    int y = b + j;

                    if (getGridIndex(x * 2, y * 2) == -1)continue; // checking if out of bounds

                    if (!g->fogManager->tileIsInShroud(x * 2, y * 2))
                    {
                        auto tile = this->tileArrayA[(y * this->gridWidth) + x];
                        if (tile != nullptr) {
                            drawTile(camera, ((x) * tileWidth) + xoffset, ((y) * tileHeight) + yoffset,
                                     tile->frame);
                        }
                    }

                    if (!g->fogManager->tileIsInShroud((x * 2)+1, (y * 2)+1)) {
                        auto tile = this->tileArrayB[(y * this->gridWidth) + x];
                        if (tile != nullptr) {
                            drawTile(camera, ((x + 0.5f) * tileWidth) + xoffset,
                                     ((y + 0.5f) * tileHeight) + yoffset, tile->frame);
                        }
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
        return index + 1;
    }
}