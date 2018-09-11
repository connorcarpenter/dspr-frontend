//
// Created by connor on 9/10/18.
//

#include <Sova/References/Refable.h>
#include "Sova/References/Ref.h"
#include "Button.h"
#include "Sova/Common/List.h"

using namespace Sova;

namespace DsprFrontend {
    class ButtonCard : public Sova::Refable {
    public:
        ButtonCard();
        void AddButton(Ref<Button> button);
    private:
        Ref<List<Button>> buttonList = Null<List<Button>>();
    };
}