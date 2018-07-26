//
// Created by connor on 7/23/18.
//

#include <Sova/Internal/OryolApp.h>
#include <iostream>
#include "TileManager.h"
#include "Sova/Common/String.h"
#include "Sova/Internal/OryolTexture.h"

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
        this->texture = OryolApp::getOryolApp()->resourceManager.textures[textureString];
        this->visible = true;

        this->gridWidth = atoi(width->AsCStr());
        this->gridHeight = atoi(height->AsCStr());
        this->receivedGrid = true;

        this->tileArrayA = initializeTileArray(this->gridWidth, this->gridHeight);
        this->tileArrayB = initializeTileArray(this->gridWidth, this->gridHeight);

        std::cout << "received Grid"<< std::endl;
    }

    void TileManager::receiveTile(Ref<Sova::String> x, Ref<Sova::String> y, Ref<Sova::String> frame)
    {
        if (!this->receivedGrid)
            return;

        int tileX = atoi(x->AsCStr());
        int tileY = atoi(y->AsCStr());
        int tileFrame = atoi(frame->AsCStr());

        int gridIndex = whichGrid(tileX, tileY);
        if (gridIndex == -1)
            return;

        int tileIndex = getIndex(gridIndex, tileX, tileY);
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

    int TileManager::whichGrid(int x, int y) {
        if (!this->receivedGrid) return -1;
        if (x < 0 || y < 0 || x >= this->gridWidth*2 || y >= this->gridHeight*2) return -1;

        if (x % 2 == 0 && y % 2 == 0) return 0;
        if ((x+1) % 2 == 0 && (y+1) % 2 == 0) return 1;
    }

    int TileManager::getIndex(int gridIndex, int x, int y) {
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

    void TileManager::Draw(int xoffset, int yoffset)
    {
        if (this->destroyed) return;
        if (!this->visible) return;

        const auto resState = Gfx::QueryResourceInfo(this->texture->textureId).State;
        if (resState == ResourceState::Valid)
        {
            int a = xoffset / -16;
            int b = yoffset / -8;

            for (int j = 0; j < 20; j++) // j should take into account grid height... right?
            {
                for (int i = 0; i < 18; i += 1) // j should take into account grid height... right?
                {
                    int x = a + i;
                    int y = b + j;

                    if (whichGrid(x*2,y*2) == -1)continue;

                    auto tile = this->tileArrayA[(y*this->gridWidth)+x];
                    if (tile == nullptr) return;
                    if (tile->frame == -1) return;

                    drawTile(((x-1)*16)+xoffset, ((y-1)*8)+yoffset, tile->frame);

                    tile = this->tileArrayB[(y*this->gridWidth)+x];
                    if (tile == nullptr) return;

                    drawTile(((x-1+0.5f)*16)+xoffset, ((y-1+0.5f)*8)+yoffset, tile->frame);
                }
            }
        }
    }

    void TileManager::drawTile(float xoffset, float yoffset, int frame)
    {
        OryolApp::getOryolApp()->drawState.FSTexture[0] = this->texture->textureId;
        const void *data = this->updateVertices(xoffset, yoffset, this->texture->width, this->texture->height,
                                                OryolApp::getOryolApp()->canvasWidth,
                                                OryolApp::getOryolApp()->canvasHeight, frame);
        Gfx::UpdateVertices(OryolApp::getOryolApp()->drawState.Mesh[0], data, OryolApp::numVertexesInQuad);
        Gfx::ApplyDrawState(OryolApp::getOryolApp()->drawState);

        Gfx::Draw();
    }

    const void * TileManager::updateVertices(float x, float y, int width, int height, int canvasWidth, int canvasHeight,
                                             int frame) {

        int vIndex = 0;

        //0 is 0, 1 is canvasWidth, canvasHeight
        float x0 = x / (float) canvasWidth;
        float y0 = y / (float) canvasHeight;
        float x1 = (14 + x) / (float) canvasWidth;
        float y1 = (8 + y) / (float) canvasHeight;

        //0 is 0, 1 is texWidth/texHeight
        //This is the texture
        float u0 = (float) ((frame * 16) + 1) / (float) width;
        float v0 = (float) 1 / (float) height;
        float u1 = (float) (((frame+1) * 16) - 1) / (float) width;
        float v1 = (float) (height - 1) / (float) height;

        vIndex = this->writeVertex(vIndex, x0, y0, u0, v0);
        vIndex = this->writeVertex(vIndex, x1, y0, u1, v0);
        vIndex = this->writeVertex(vIndex, x1, y1, u1, v1);
        vIndex = this->writeVertex(vIndex, x0, y0, u0, v0);
        vIndex = this->writeVertex(vIndex, x1, y1, u1, v1);
        vIndex = this->writeVertex(vIndex, x0, y1, u0, v1);

        return OryolApp::getOryolApp()->vertexBuffer;
    }

    int TileManager::writeVertex(int index, float x, float y, float u, float v) {
        OryolApp::getOryolApp()->vertexBuffer[index].x = x;
        OryolApp::getOryolApp()->vertexBuffer[index].y = y;
        OryolApp::getOryolApp()->vertexBuffer[index].u = u;
        OryolApp::getOryolApp()->vertexBuffer[index].v = v;
        return index + 1;
    }
}