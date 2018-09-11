//
// Created by connor on 9/10/18.
//

#include <Sova/References/Refable.h>
#include <Sova/References/Ref.h>

using namespace Sova;

namespace DsprFrontend {
    class ButtonCard;

    class ButtonCardCatalog : public Sova::Refable {
    public:
        ButtonCardCatalog();
        Ref<ButtonCard> basicCommandCard = Null<ButtonCard>();
    };
}