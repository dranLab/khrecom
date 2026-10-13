// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef GAME_CARDBATTLE_H
#define GAME_CARDBATTLE_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "Game/CollParam.h"     // PARTIAL: added for Game/Player/SummonDumbo (ATK_KIND)
#include "Game/AttackMotion.h" // PARTIAL: added for Game/Player/SummonDumbo (ATK_ATTR)

union FVECTOR; // PARTIAL: added for Game/Player/FriendPluto

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

// PARTIAL: added for Game/Player/SummonDumbo
struct SS_PARAM {
    unsigned short POWER[4];
    BYTE DUMMY;
    BYTE CATEGORY;
    BYTE ATTR;
    BYTE UP_TYPE0 : 4;
    BYTE UP_TYPE1 : 4;
    LPCSTR NAME;
};

namespace CardBattle {
extern BC_PARAM* g_pBattleCardList;
extern SS_PARAM* g_pStockSkillList; // PARTIAL: added for Game/Player/SummonDumbo

// PARTIAL: added for Game/Player/SummonDumbo
class CCardBattle {
public:
    static void calcEnchant_AttackPoint(INT& ioPower, ATK_KIND& ioKind, ATK_ATTR inAttr, UINT inUpKind);
};

// PARTIAL: added for Game/Player/FriendKing
class CCardRevolver {
public:
    void setAction(INT inAction, INT inParam);
    // PARTIAL: added for Game/Player/FriendPluto
    INT getHave(INT inKind, INT inA, INT inB, INT inC, INT inD);
    INT calcPlutoMark();
    void dropPlutoMarkCard(const FVECTOR* inPos);
};
}

#endif /* GAME_CARDBATTLE_H */
