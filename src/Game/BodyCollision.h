// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef GAME_BODYCOLLISION_H
#define GAME_BODYCOLLISION_H

#include "khtypes.h"
#include "System/MainApp.h"

/* Placeholder for the original header's inline function checking a collision kind: its strings
 * ("Game/BodyCollision.h", "0 <= inKind && inKind < COLLISION_KIND_MAX") are in .rodata of every includer.
 * The line number is not known (it is not part of the data). */
#line 1 "Game/BodyCollision.h"
inline void _bodyCollCheckKind(INT inKind) {
    if (!(0 <= inKind && inKind < 2)) AssertApp(__FILE__, __LINE__, "0 <= inKind && inKind < COLLISION_KIND_MAX", 0);
}

#endif /* GAME_BODYCOLLISION_H */
