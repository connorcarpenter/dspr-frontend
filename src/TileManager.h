#pragma once

//
// Created by connor on 7/23/18.
//

#include <Sova/Graphics/Container.h>
#include "Tile.h"

using namespace Sova;

namespace Sova
{
    class InternalTexture;
}

namespace DsprFrontend
{
    class TileManager : public Container
    {
    public:

        TileManager();
        ~TileManager();
        void receiveGrid(Sova::Ref<Sova::String> width, Sova::Ref<Sova::String> height);
        void receiveTile(Sova::Ref<Sova::String> x, Sova::Ref<Sova::String> y, Sova::Ref<Sova::String> frame);
        void Draw(Ref<Camera> camera, int xoffset, int yoffset);

        const int tileWidth = 20;
        const int tileHeight = 10;

        int getTileFrame(int x, int y);
        Ref<Point> getGridSize();

    private:

        Tile **initializeTileArray(int width, int height);
        void destroyTileArray(Tile **tileArray, int width, int height);
        int getGridIndex(int x, int y);
        int getTileIndex(int gridIndex, int x, int y);



        int gridWidth = 0;
        int gridHeight = 0;
        bool receivedGrid = false;
        bool visible = false;

        Tile** tileArrayA = nullptr;
        Tile** tileArrayB = nullptr;

        Sova::InternalTexture* texture = nullptr;

        const void *updateVertices(float x, float y, int texWidth, int texHeight, int canWidth, int canHeight, int frameWidth,
                                           int frameHeight, int padding, int frameIndex);

        int writeVertex(int index, float x, float y, float u, float v);

        void drawTile(Ref<Camera> camera, float xoffset, float yoffset, int frame);

    };
}
