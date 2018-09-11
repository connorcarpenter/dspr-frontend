//
// Created by connor on 9/10/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/Color.h>
#include <functional>

using namespace Sova;

namespace DsprFrontend {
    class Button : public Sova::Refable {
    public:
        Button(int imageIndex);

        void executeAction();
        void setAction(std::function<void()> actionFunction);

        int imageIndex;
    private:
        std::function<void()> action = nullptr;
    };
}