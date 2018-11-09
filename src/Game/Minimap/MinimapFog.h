//
// Created by connor on 8/20/18.
//

#include <Sova/Graphics/Container.h>


namespace DsprFrontend
{

    class Global;

    class MinimapFog : public Sova::Container
    {
    public:
        MinimapFog();
        void drawSelf(Sova::Ref<Camera> camera, int xoffset, int yoffset) override;
    private:
        Global* g = nullptr;
        int cellX = 0;
        int cellY = 0;
        int cellW;
        int cellH;
        int gridW = -1;
        int gridH;
    };
}