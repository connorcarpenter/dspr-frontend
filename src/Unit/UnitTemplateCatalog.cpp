//
// Created by connor on 9/12/18.
//

#include "UnitTemplateCatalog.h"

namespace DsprFrontend {

    UnitTemplateCatalog::UnitTemplateCatalog() {
        this->worker = New<UnitTemplate>();
        this->temple = New<UnitTemplate>();
    }
}