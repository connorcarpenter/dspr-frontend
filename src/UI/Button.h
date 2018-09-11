//
// Created by connor on 9/10/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/Color.h>

using namespace Sova;

namespace DsprFrontend {
    class Button : public Sova::Refable {
    public:
        Button(int imageIndex);
    private:
        int imageIndex;
        Color tint = Color::White;
    };
}