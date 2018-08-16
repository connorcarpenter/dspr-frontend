//
// Created by connor on 8/16/18.
//

#include <Sova/References/Refable.h>

namespace DsprFrontend
{
    class FogManager : public Sova::Refable
    {
    public:
        void receiveGrid(int w, int h);
    private:
        int gridWidth = 0;
        int gridHeight = 0;
        bool receivedGrid = false;
        int** fogArrayA = nullptr;
        int** fogArrayB = nullptr;
        const int tileWidth = 20;
        const int tileHeight = 10;
    };
}