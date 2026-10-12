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

// PARTIAL: added for Game/Player/FriendAladdin
struct BC_PARAM {
    unsigned short POWER[4];
    BYTE BREAK;
    BYTE RESPONSE;
    BYTE COST;
    BYTE ATTR;
    LPCSTR EFF_NAME;
};

namespace CardBattle {
extern BC_PARAM* g_pBattleCardList;
}

#endif /* GAME_CARDBATTLE_H */
