// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef GAME_CARDBATTLE_H
#define GAME_CARDBATTLE_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "Game/CollParam.h"     // PARTIAL: added for Game/Player/SummonDumbo (ATK_KIND)
#include "Game/AttackMotion.h" // PARTIAL: added for Game/Player/SummonDumbo (ATK_ATTR)

class CPlayer; // PARTIAL: added for Game/Player/PlayerRiku
union FVECTOR; // PARTIAL: added for Game/Player/FriendPluto

/* Placeholder for the original header's inline function asserting inUser: its strings
 * ("Game/CardBattle.h", "inUser") are in .rodata of every includer. The line number is not known. */
#ifndef CARDBATTLE_H_PATH // PARTIAL: path override added for Game/Technic ("c:/KHCOM/MainApp/Game/CardBattle.h")
#define CARDBATTLE_H_PATH "Game/CardBattle.h"
#endif
#line 1 CARDBATTLE_H_PATH
inline void _cardBattleCheckUser(void* inUser) {
    if (!inUser) AssertApp(__FILE__, __LINE__, "inUser", 0);
}

// PARTIAL: added for Game/Player/FriendAladdin
struct BC_PARAM {
    unsigned short POWER[4];
    // PARTIAL: unions completed for Game/Player/Summon
    union {
        BYTE BREAK;
        BYTE EFF_PART;
    };
    union {
        BYTE RESPONSE;
        BYTE EFF_TIMES;
        struct {
            BYTE UP_TYPE0 : 4;
            BYTE UP_TYPE1 : 4;
        };
    };
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
extern unsigned char g_DropFriendCardList[32][8]; // PARTIAL: added for Game/Player/PlayerManager
// PARTIAL: added for Game/Player/PlayerManager (Game/CardRevolver.h in the original)
extern char* g_szDbgDeckName[10];
extern char* g_szEnemyCardEffName[60];

// PARTIAL: added for Game/Player/SummonDumbo
class CCardBattle {
    // PARTIAL: members added for Game/Player/PlayerRiku
    UBYTE _m_0x0[0x17c];      /* 0x000 CTask .. 0x178 m_cardGimmick */
    BYTE m_cardEnchant[2][4]; /* 0x17c CARD[2] */
    UBYTE _m_0x184[0x18];     /* 0x184 m_nEnchantTime .. 0x198 m_cardRoom */
public:
    UINT m_uiFlag;            /* 0x19c */
    UINT m_uiDuelState;       /* 0x1a0 */
    HANDLE m_hDuelEnemy;      /* 0x1a4 */
    // PARTIAL: added for Game/Player/PlayerRiku
    INT getEnchantKind() { return m_cardEnchant[0][0] < 0x44 ? 0 : m_cardEnchant[0][0] - 0x43; }
    HANDLE getDuelEnemy() { return m_hDuelEnemy; }
    void setFlag(UINT inSet, UINT inMask) { m_uiFlag = (m_uiFlag | inSet) & ~inMask; }
    void destroyPlayCard(INT inFlag, INT inP1);
    void transferPlayerEnchant(CPlayer* inPlayer);
    void setDuelEntry(INT inFlag);
    void setDuelStart(INT inFlag);
    static void calcEnchant_AttackPoint(INT& ioPower, ATK_KIND& ioKind, ATK_ATTR inAttr, UINT inUpKind);
    static CCardBattle* getInstance(); // PARTIAL: added for Game/Player/PlayerManager
    // PARTIAL: added for Game/Enemy/Heartless
    INT setEnemyCard(HANDLE inEnemy, INT inParam, INT inNumber);
    static INT getPlayingCardPoint(INT inSide);
};

// PARTIAL: added for Game/Player/FriendKing
class CCardRevolver {
public:
    // PARTIAL: added for Game/Player/PlayerRiku
    void changeDarkMode(HANDLE inHandle, INT inFlag);
    // PARTIAL: added for Game/Player/PlayerDRiku
    void changeMode(INT inMode, INT inFlag);
    INT chargeReloadCard(INT inFlag);
    INT setPlayerDuelPlayStock();
    INT setPlayerDuelCard(INT inNo);
    void setAction(INT inAction, INT inParam);
    // PARTIAL: added for Game/Player/FriendPluto
    INT getHave(INT inKind, INT inA, INT inB, INT inC, INT inD);
    INT calcPlutoMark();
    void dropPlutoMarkCard(const FVECTOR* inPos);
};
}

#endif /* GAME_CARDBATTLE_H */
