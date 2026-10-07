// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef GAME_CARDBATTLE_H
#define GAME_CARDBATTLE_H

#include "khtypes.h"
#include "System/MainApp.h"

/* Placeholder for the original header's inline function asserting inUser: its strings
 * ("Game/CardBattle.h", "inUser") are in .rodata of every includer. The line number is not known. */
#line 1 "Game/CardBattle.h"
inline void _cardBattleCheckUser(void* inUser) {
    if (!inUser) AssertApp(__FILE__, __LINE__, "inUser", 0);
}

#endif /* GAME_CARDBATTLE_H */
