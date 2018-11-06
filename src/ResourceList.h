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

            resources->Add(New<String>("images/manafount.png"));
            resources->Add(New<String>("images/manaball.png"));

            resources->Add(New<String>("images/temple/temple.png"));
            resources->Add(New<String>("images/temple/temple_TC.png"));
            resources->Add(New<String>("images/temple/temple_unitPortrait.png"));
            resources->Add(New<String>("images/temple/temple_unitPortrait_TC.png"));
            resources->Add(New<String>("images/temple/temple_bigUnitPortrait.png"));
            resources->Add(New<String>("images/temple/temple_bigUnitPortrait_TC.png"));

            resources->Add(New<String>("images/worker/sprWorkerBase.png"));
            resources->Add(New<String>("images/worker/sprWorkerTC.png"));
            resources->Add(New<String>("images/worker/sprWorkerSkin.png"));
            resources->Add(New<String>("images/worker/sprWorkerHairShort.png"));
            resources->Add(New<String>("images/worker/sprWorkerHairLong.png"));
            resources->Add(New<String>("images/worker/sprWorkerArmor.png"));
            resources->Add(New<String>("images/worker/workerPortrait.png"));
            resources->Add(New<String>("images/worker/worker_bigPortrait.png"));
            resources->Add(New<String>("images/worker/workerPortrait_TC.png"));
            resources->Add(New<String>("images/worker/worker_bigPortrait_TC.png"));

            resources->Add(New<String>("images/ui/armybar.png"));
            resources->Add(New<String>("images/ui/commandcard.png"));
            resources->Add(New<String>("images/ui/itembar.png"));
            resources->Add(New<String>("images/ui/portraitbar.png"));
            resources->Add(New<String>("images/ui/minimap.png"));
            resources->Add(New<String>("images/ui/commandActions.png"));
            resources->Add(New<String>("images/ui/cursor.png"));
            resources->Add(New<String>("images/ui/unitHover.png"));
            resources->Add(New<String>("images/ui/unitSelection.png"));
            resources->Add(New<String>("images/ui/5x5_hovercircle.png"));
            resources->Add(New<String>("images/ui/5x5_selectioncircle.png"));
            resources->Add(New<String>("images/ui/2x2_hovercircle.png"));
            resources->Add(New<String>("images/ui/2x2_selectioncircle.png"));
            resources->Add(New<String>("images/ui/3x3_hovercircle.png"));
            resources->Add(New<String>("images/ui/3x3_selectioncircle.png"));
            resources->Add(New<String>("images/ui/moveMarker.png"));
            resources->Add(New<String>("images/ui/resourcebar.png"));
            resources->Add(New<String>("images/ui/myFont.png"));

            resources->Add(New<String>("sounds/hello.wav"));
            resources->Add(New<String>("sounds/hit.wav"));
            resources->Add(New<String>("sounds/die.wav"));
            resources->Add(New<String>("sounds/ready.wav"));

            return resources;
        }
    };
}