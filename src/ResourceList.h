#pragma once

//
// Created by connor on 10/24/18.
//

#include <Sova/References/Ref.h>

namespace DsprFrontend
{
    class ResourceList : public Sova::Refable
    {
    public:
        Ref<List<String>> getResources()
        {
            Ref<List<String>> resources = New<List<String>>();

            resources->Add(New<String>("images/tiles.png"));
            resources->Add(New<String>("images/trees.png"));
            resources->Add(New<String>("images/block.png"));
            resources->Add(New<String>("images/fogTile.png"));
            resources->Add(New<String>("images/bloodParticle.png"));

            resources->Add(New<String>("images/temple/temple.png"));
            resources->Add(New<String>("images/temple/temple_TC.png"));
            resources->Add(New<String>("images/temple/temple_unitPortrait.png"));
            resources->Add(New<String>("images/temple/temple_unitPortrait_TC.png"));

            resources->Add(New<String>("images/worker/workerDown.png"));
            resources->Add(New<String>("images/worker/workerUp.png"));
            resources->Add(New<String>("images/worker/workerAttackUp.png"));
            resources->Add(New<String>("images/worker/workerAttackUpRight.png"));
            resources->Add(New<String>("images/worker/workerAttackRight.png"));
            resources->Add(New<String>("images/worker/workerAttackDownRight.png"));
            resources->Add(New<String>("images/worker/workerAttackDown.png"));
            resources->Add(New<String>("images/worker/workerDieUp.png"));
            resources->Add(New<String>("images/worker/workerDieDown.png"));
            resources->Add(New<String>("images/worker/workerPortrait.png"));
            resources->Add(New<String>("images/worker/TC/workerDown_TC.png"));
            resources->Add(New<String>("images/worker/TC/workerUp_TC.png"));
            resources->Add(New<String>("images/worker/TC/workerAttackUp_TC.png"));
            resources->Add(New<String>("images/worker/TC/workerAttackUpRight_TC.png"));
            resources->Add(New<String>("images/worker/TC/workerAttackRight_TC.png"));
            resources->Add(New<String>("images/worker/TC/workerAttackDownRight_TC.png"));
            resources->Add(New<String>("images/worker/TC/workerAttackDown_TC.png"));
            resources->Add(New<String>("images/worker/TC/workerDieUp_TC.png"));
            resources->Add(New<String>("images/worker/TC/workerDieDown_TC.png"));
            resources->Add(New<String>("images/worker/TC/workerPortrait_TC.png"));

            resources->Add(New<String>("images/ui/armybar.png"));
            resources->Add(New<String>("images/ui/commandcard.png"));
            resources->Add(New<String>("images/ui/minimap.png"));
            resources->Add(New<String>("images/ui/commandActions.png"));
            resources->Add(New<String>("images/ui/cursor.png"));
            resources->Add(New<String>("images/ui/unitHover.png"));
            resources->Add(New<String>("images/ui/unitSelection.png"));
            resources->Add(New<String>("images/ui/5x5_hovercircle.png"));
            resources->Add(New<String>("images/ui/5x5_selectioncircle.png"));
            resources->Add(New<String>("images/ui/moveMarker.png"));

            resources->Add(New<String>("sounds/hello.wav"));
            resources->Add(New<String>("sounds/hit.wav"));
            resources->Add(New<String>("sounds/die.wav"));

            return resources;
        }
    };
}