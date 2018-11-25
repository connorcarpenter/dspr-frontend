#pragma once

//
// Created by connor on 8/29/18.
//

namespace DsprFrontend {
    enum UnitOrderType {
        Move,
        AttackMove,
        Follow,
        AttackTargetStrong,
        AttackTargetWeak,
        Stop,
        Hold,
        Train,
        Gather,
        Pickup,
        ItemSwap,
        ItemDrop,
        ItemGive,
        RallyPoint,
        RallyUnit,
        CancelTrain
    };
}