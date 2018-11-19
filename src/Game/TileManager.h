#pragma once

//
// Created by connor on 7/23/18.
//

#include <Sova/Graphics/Container.h>
#include "Tile.h"
#include "RefIsoGrid.h"

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
        void receiveGrid(int width, int height);
        void receiveTile(int tileX, int tileY, int tileFrame);
        void Draw(Ref<Camera> camera, int xoffset, int yoffset);

        const int tileWidth = 20;
        const int tileHeight = 10;

        int getTileFrame(int x, int y);
        Ref<Point> getGridSize();

        Ref<Point> getTilePosition(int x, int y);

    private:

        int gridWidth = 0;
        int gridHeight = 0;
        bool receivedGrid = false;
        bool visible = false;

        Ref<RefIsoGrid<Tile>> tileGrid = Null<RefIsoGrid<Tile>>();

        Sova::InternalTexture* texture = nullptr;

        const void *updateVertices(float x, float y, int texWidth, int texHeight, int canWidth, int canHeight, int frameWidth,
                                           int frameHeight, int padding, int frameIndex);

        int writeVertex(int index, float x, float y, float u, float v);

        void drawTile(Ref<Camera> camera, float xoffset, float yoffset, int frame);

    };
}
