// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef GAME_CARDBATTLE_H
#define GAME_CARDBATTLE_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "Game/CollParam.h"     // PARTIAL: added for Game/Player/SummonDumbo (ATK_KIND)
#include "Game/AttackMotion.h" // PARTIAL: added for Game/Player/SummonDumbo (ATK_ATTR)
#include "System/Vector.h"     // PARTIAL: added for Game/CardBattle (m_vBreakPos)
#include "Game/CardKind.h"     // PARTIAL: added for Game/CardBattle (CARD members)
#include "System/Task.h"        // PARTIAL: added for Game/CardBattle (CCardBattle is a CTask)

class CPlayer; // PARTIAL: added for Game/Player/PlayerRiku
class CTask; // PARTIAL: added for Game/CardBattle
namespace D2 { class C2DSeqCtrl; } // PARTIAL: added for Game/CardBattle
namespace Message { class CMessageManager; } // PARTIAL: added for Game/CardBattle
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

// PARTIAL: added for Game/CardBattle (the card info sent with the duel messages)
struct MES_CARD_INFO {
    CARD m_cardData[7];
    SHORT m_nStock;
    BYTE m_nExtra;
    BYTE m_nFirst;
    BYTE m_nBreak;
    BYTE m_nBonus;
    BYTE m_nWork0;
    BYTE m_nWork1;
};
namespace CardBattle {
extern BC_PARAM* g_pBattleCardList;
extern SS_PARAM* g_pStockSkillList; // PARTIAL: added for Game/Player/SummonDumbo
extern unsigned char g_DropFriendCardList[32][8]; // PARTIAL: added for Game/Player/PlayerManager
void getDropFriendCard(CARD& outCard, INT inRate); void calcFreezeDamage(INT& ioDamage); // PARTIAL: calcFreezeDamage added for Game/Enemy/BattleEnemy // PARTIAL: added for Game/Enemy/BattleEnemy
// PARTIAL: added for Game/Player/PlayerManager (Game/CardRevolver.h in the original)
extern char* g_szDbgDeckName[10];
extern char* g_szEnemyCardEffName[60];

// PARTIAL: added for Game/Player/Player
struct ENCHANT_PARAM {
    UINT uiUser;        /* 0x00 */
    HANDLE hOwner;      /* 0x04 */
    ATK_KIND AtkKind;   /* 0x08 */
    ATK_ATTR AtkAttr;   /* 0x0c */
    USHORT uiState;     /* 0x10 */
    USHORT UpKind;      /* 0x12 */
    SHORT iAttackPoint; /* 0x14 */
    SHORT iHp;          /* 0x16 */
    SHORT iHpMax;       /* 0x18 */
    SHORT iFinish;      /* 0x1a */
    SHORT iGuard;       /* 0x1c */
    SHORT iNormal;      /* 0x1e */
    void init(INT inOwner, UINT inUser);
};

// PARTIAL: added for Game/Player/SummonDumbo
class CCardBattle : public CTask {
    /* 0x000 CTask */
    // PARTIAL: members added for Game/Player/PlayerRiku
    // PARTIAL: split out for Game/CardBattle
    // PARTIAL: split for Game/CardBattle
    Message::CMessageManager* m_pMesage; /* 0x040 */
    MES_CARD_INFO m_mesCardInfo[2]; /* 0x044 */
    TIMEQ m_tqMainTimer;           /* 0x08c */
    TIMEQ m_tqSubTimer;            /* 0x090 */
    HANDLE m_hOwner[2];       /* 0x094 */
    UINT m_uiState[2];        /* 0x09c */
    UINT m_uiResult[2];       /* 0x0a4 */
    INT m_tqPlayTimer[2];     /* 0x0ac */
    CARD m_cardData[2][7];    /* 0x0b4 */
    class CBattleCard* m_pCard[2][7]; /* 0x0ec */
    class CBattleCard* m_pEnchant[2]; /* 0x124 */
    class CCardRevolver* m_pRevolver[2]; /* 0x12c */
    class CStockFolder* m_pStockFolder[2]; /* 0x134 */
    INT m_iCount[2];          /* 0x13c */
    INT m_iPoint[2];          /* 0x144 */
    INT m_iStock[2];          /* 0x14c */
    INT m_iExtra[2];          /* 0x154 */
    INT m_iFirst[2];          /* 0x15c */
    INT m_iCountGimmick;      /* 0x164 */
    INT m_iCountFriend;       /* 0x168 */
    INT m_iCountPluto;        /* 0x16c */
    INT m_iPointBreak;        /* 0x170 */
    INT m_iPointBonus;        /* 0x174 */
    CARD m_cardGimmick;       /* 0x178 */
    CARD m_cardEnchant[2];    /* 0x17c */
    INT m_nEnchantTime[2];    /* 0x184 */
    INT m_tqEnchantTimer[2];  /* 0x18c */
    HANDLE m_hGimmickOwner;   /* 0x194 */
    CARD m_cardRoom;          /* 0x198 */
public: BOOL setEnemyDuelCard(HANDLE inHandle, INT inKind, INT inNumber); void setDuelBreakCount(INT inSide, INT inCount); // PARTIAL: added for Game/Enemy/BattleEnemy (parameter names guessed)
    UINT m_uiFlag;            /* 0x19c */
    UINT m_uiDuelState;       /* 0x1a0 */
    HANDLE m_hDuelEnemy;      /* 0x1a4 */
    // PARTIAL: added for Game/CardBattle
    USHORT m_nDuelBreakCount;    /* 0x1a8 */
    USHORT m_nDuelBreakCountMax; /* 0x1aa */
    INT m_iDuelSkill;            /* 0x1ac */
    D2::C2DSeqCtrl* m_pSeqCtrlEnemy; /* 0x1b0 */
    D2::C2DSeqCtrl* m_pSeqCtrlBreak; /* 0x1b4 */
    D2::C2DSeqCtrl* m_pSeqCtrlSpeed; /* 0x1b8 */
    HANDLE m_hBreak;             /* 0x1bc */
    HANDLE m_hSpeed;             /* 0x1c0 */
    FVECTOR m_vBreakPos;         /* 0x1d0 */
    class CEnchantGauge* m_pEnchantGauge[2]; /* 0x1e0 */
    class CDuelGauge* m_pDuelGauge; /* 0x1e8 */
#ifdef GAME_CARDBATTLE_CPP
    /* the original's inline members, in its definition order (Game/CardBattle emits them all with the key function) */
    void disablePlayCard(UINT inSet) { m_uiFlag = (m_uiFlag | inSet) & ~(inSet ^ 0xC0000000); }
    static UINT isDisablePlayCard() { return m_pInstance ? m_pInstance->m_uiFlag & 0xC0000000 : 0; }
    static UINT isCardBattleEnd() { return m_pInstance ? m_pInstance->m_uiFlag & 0x4 : 0; }
    void disableDuelEntry(UINT inSet) { m_uiFlag = (m_uiFlag | inSet) & ~(inSet ^ 0x400000); }
    HANDLE getDuelEnemy() const { return m_hDuelEnemy; }
    USHORT getDuelBreakCountMax() const { return m_nDuelBreakCountMax; }
    INT isNowDuelEntry() const { return m_uiDuelState == 2; }
    INT isNowDuel() const { return m_uiDuelState == 5; }
    INT isNowSetEnemyDuelCard() const { return m_uiState[1] == 10; }
    static INT getDuelSkill() { return m_pInstance ? m_pInstance->m_iDuelSkill : 0; }
    INT isPronounceMode() const { return m_uiDuelState >= 9; }
    INT isNowPronounce() const { return m_uiDuelState == 0xB; }
    INT isNowSetEnemyPronounce() const { return m_uiState[1] == 15; }
    void setSubTimer(TIMEQ inTime) { m_tqSubTimer = inTime; }
    TIMEQ getSubTimer() const { return m_tqSubTimer; }
    INT getEnchantKind(INT inUser) const {
#line 641
        if (!(inUser)) AssertApp(__FILE__, __LINE__, "inUser", 0);
        return m_cardEnchant[inUser - 1].m_nKind < 0x44 ? 0 : m_cardEnchant[inUser - 1].m_nKind - 0x43;
    }
    static CARD* getRoomCard() { return m_pInstance ? &m_pInstance->m_cardRoom : NULL; }
    static CARD* getEnchantCard(INT inUser) {
#line 644
        if (!(inUser)) AssertApp(__FILE__, __LINE__, "inUser", 0);
        return m_pInstance ? &m_pInstance->m_cardEnchant[inUser - 1] : NULL;
    }
    static INT getStockCount(INT inUser) {
#line 658
        if (!(inUser)) AssertApp(__FILE__, __LINE__, "inUser", 0);
        return m_pInstance ? m_pInstance->m_iCount[inUser - 1] : 0;
    }
#else
    // PARTIAL: added for Game/Player/PlayerRiku
    INT getEnchantKind() { return m_cardEnchant[0].m_nKind < 0x44 ? 0 : m_cardEnchant[0].m_nKind - 0x43; }
    // PARTIAL: added for Game/Enemy/HumanBoss (the enemy side's enchant)
    INT getEnchantKind(INT inSide) { return m_cardEnchant[inSide].m_nKind < 0x44 ? 0 : m_cardEnchant[inSide].m_nKind - 0x43; }
    HANDLE getDuelEnemy() { return m_hDuelEnemy; }
    // PARTIAL: added for Game/Player/PlayerSora (out of line in Game/CardBattle, inlined elsewhere)
    INT isNowDuelEntry() const { return m_uiDuelState == 2; } // PARTIAL: added for Game/CardBattle
    INT isNowDuel() const { return m_uiDuelState == 5; }
    INT isNowSetEnemyDuelCard() const { return m_uiState[1] == 10; }
    INT isPronounceMode() const { return m_uiDuelState >= 9; }
    INT isNowPronounce() const { return m_uiDuelState == 0xB; }
    INT isNowSetEnemyPronounce() const { return m_uiState[1] == 15; }
    USHORT getDuelBreakCountMax() const { return m_nDuelBreakCountMax; }
    static INT getDuelSkill() { return m_pInstance ? m_pInstance->m_iDuelSkill : 0; }
    void disableDuelEntry(UINT inSet) { setFlag(inSet, inSet ^ 0x400000); }
    void setSubTimer(TIMEQ inTime) { m_tqSubTimer = inTime; }
    TIMEQ getSubTimer() const { return m_tqSubTimer; }
    ::CTask* asTask() { return this; } // PARTIAL: added for Game/CardBattle



    void setFlag(UINT inSet, UINT inMask) { m_uiFlag = (m_uiFlag | inSet) & ~inMask; }
    void disablePlayCard(UINT inSet) { setFlag(inSet, inSet ^ 0xC0000000); } // PARTIAL: added for Game/CardBattle
#endif
    void destroyPlayCard(INT inFlag, INT inP1);
    void transferPlayerEnchant(CPlayer* inPlayer);
    void setDuelEntry(INT inFlag);
    void setDuelStart(INT inFlag);
    static void calcEnchant_AttackPoint(INT& ioPower, ATK_KIND& ioKind, ATK_ATTR inAttr, UINT inUpKind);
    static CCardBattle* getInstance(); // PARTIAL: added for Game/Player/PlayerManager
    CCardBattle(); // PARTIAL: added for Game/CardBattle
    static INT getGimmickCount(); // PARTIAL: added for Game/Prize
    // PARTIAL: added for Game/Enemy/Heartless
    INT setEnemyCard(HANDLE inEnemy, INT inParam, INT inNumber);
    static INT getPlayingCardPoint(INT inSide);
    // PARTIAL: added for Game/CardBattle
    static CCardBattle* m_pInstance;
    virtual ~CCardBattle();
    virtual void main();
    virtual void disp();
    void resetNowPlaying(INT inFlag);
    // PARTIAL: added for Game/CardRevolver
    BOOL testBreakingCard(UINT inSide);
    static BOOL isExecutingCard(INT inSide);
    INT setPlayCard(HANDLE inOwner, INT inSide, class CBattleCard* inCard, CARD* inData, INT inTest);
    INT setDuelPlayStock(class CBattleCard** inCards, INT inSkill);
    INT setEnemyPronounceCard(HANDLE inOwner, class CBattleCard* inCard);
    INT setPronounceStart(INT inFlag); // PARTIAL: added for Game/Player/PlayerSora
    void setPronounceEnd(INT inCount); // PARTIAL: added for Game/Player/PlayerSora
    static void decEnchantTime(INT inSide, INT inKind, INT inFlag);
    friend class CCardRevolver;
    INT setPlayStock(HANDLE inOwner, INT inSide, class CBattleCard** inCards, INT inSkill, INT inExtra, INT inFirst, INT inTest);
    static HANDLE getPlayingHandle(INT inSide);
    static BOOL isPlayingCard(INT inSide);
    static BOOL isPlayingEnemyCard();
    static INT getPlayingCardID(INT inSide, INT inIdx);
    static void addGimmickCount(INT n);
    static void addFriendCount(INT n);
    static INT getFriendCount();
    static void addPlutoCount(INT n);
    static INT getPlutoCount();
    static void setGimmickOwner(HANDLE h);
    static HANDLE getGimmickOwner();
    static BOOL isPlayingGimmickCard(); static BOOL isStandingCard(INT inSide); // PARTIAL: isStandingCard added for Game/Player/Player
    INT calcPointBreak(INT a, INT b);
    void destroyEnemyCard(INT inHandle);
    void destroyRevolver(INT inSide);
    void setRevolver(INT inSide, CCardRevolver* pRevolver);
    CCardRevolver* getRevolver(INT inSide);
    static BOOL isBattleEnd();
    static void setSuspend(BOOL b);
    static BOOL isSuspend();
    static BOOL isLevitate(); static void notifyCompleteGimmick(); static void notifyMakeGimmick(CARD* inCard, INT inFlag); // PARTIAL: notifyCompleteGimmick, notifyMakeGimmick added for Game/Player/Player
    void calcEnchantParam(ENCHANT_PARAM& ioParam); // PARTIAL: added for Game/Player/Player
    static void setBlack(BOOL b);
    static BOOL isBlack();
    void destroyStockFolder(INT inSide);
    static class CStockFolder* getStockFolder(INT inSide);
    static INT getStockFolderStockSkill(INT inSide);
    static INT calcStockFolderEnemyStockSkill();
    static void setStockFolderStockInfo(INT inSide);
    static BOOL isDuelEntryingCard(INT inSide);
    BOOL checkNowPlaying(INT inSide, class CBattleCard* pCard);
    static BOOL isPlayingCardStandNow(INT inSide);
    static void setBattleEnd();
    void initCardBreak(INT n, UINT f);
    void resetEnchantCard(INT inSide);
    // PARTIAL: added for Game/CardBattle
    void init();
    void calcPlayingCard();
    void exePlaying(INT inUser);
    void setPlayerEnchant(INT inKind);
    void resetPlayerEnchant(INT inKind);
    void exeRegeneration();
    void exeBio();
    void exeDuel();
    void initPlayingEnemy(HANDLE inHandle);
    void dispPlayingEnemy();
    void dispCardBreak();
    void setNumberBreak(UINT inPlayer, UINT inEnemy);
    INT testPlayingCard(UINT inPlayer);
    INT setEnchantCard(HANDLE inHandle, INT inUser, class CBattleCard* inCard);
    static class CStockFolder* setStockFolder(::CTask* inParent, INT inUser);
    void setDuelEnd();
    INT setPronouncePlay(INT inFlag);
};

#ifndef GAME_CARDREVOLVER_CPP /* full class in Game/CardRevolver.h */
// PARTIAL: added for Game/Player/FriendKing
class CCardRevolver {
    UBYTE _m_0x00[0x4c]; // PARTIAL: added for Game/Enemy/HumanBoss
    SHORT m_sMode;       /* 0x4c */ // PARTIAL: name unknown
    UBYTE _m_0x4e[0x22]; // PARTIAL: added for Game/Player/Player
    TIMEQ m_tqChargeRate; /* 0x70 */ // PARTIAL: added for Game/CardBattle
    BOOL m_bActive;      /* 0x74 */ // PARTIAL: added for Game/Player/Player
    UBYTE _m_0x78[0x168]; /* 0x078 .. sizeof 0x1e0 */ // PARTIAL: added for Game/Player/Player (makeRevolver)
public:
    CCardRevolver(class CTask* pParent, CARD* inDeck, INT inUser, INT inShortCut); void setDiscard(); // PARTIAL: added for Game/Player/Player
    INT isActive() const { return m_bActive; } // PARTIAL: added for Game/Player/Player
    // PARTIAL: added for Game/Enemy/HumanBoss
    INT getMode() const { return m_sMode; } // PARTIAL: name unknown
    void setChargeRate(INT inRate) { m_tqChargeRate = inRate; } // PARTIAL: added for Game/CardBattle
    void setPlayNG(INT inFlag, INT inCategory);
    INT getRemain(INT inKind, INT inCardKind, INT inFlag);
    BOOL isEnableCharge();
    CARD* getActiveCardData();
    void destroy();
    INT getStock(INT inFlag);
    BOOL returnStockCard(INT inFlag);
    void rollDeck(INT inFlag, INT inParam, TIMEQ inTime); void rollShortCut(INT inFlag); // PARTIAL: rollShortCut added for Game/Player/Player
    BOOL setPlayCard(INT inA, INT inB);
    BOOL setPlayStock(INT inA, INT inB);
    void setStockCard(); INT checkBreakCard(INT inNum, INT inZero, BOOL inCheckGreater); // PARTIAL: checkBreakCard added for Game/Enemy/BattleEnemy
    // PARTIAL: added for Game/Player/PlayerRiku
    void changeDarkMode(HANDLE inHandle, INT inFlag);
    // PARTIAL: added for Game/Player/PlayerDRiku
    void changeMode(INT inMode, INT inFlag);
    INT chargeReloadCard(INT inFlag);
    INT setPlayerDuelPlayStock();
    INT setPlayerDuelStock(class CBattleCard* inCard, UINT inPri); // PARTIAL: added for Game/BattleCard
    INT setPlayerDuelCard(INT inNo);
    void setAction(INT inAction, INT inParam); INT calcStockSkill(); // PARTIAL: calcStockSkill added for Game/CardBattle
    // PARTIAL: added for Game/Player/FriendPluto
    INT getHave(INT inKind, INT inA, INT inB, INT inC, INT inD);
    INT calcPlutoMark();
    void dropPlutoMarkCard(const FVECTOR* inPos);
    void setSnatch(HANDLE inHandle); // PARTIAL: added for Game/Player/Player
    INT setPlayerPronounceCard(INT inTest); // PARTIAL: added for Game/Player/PlayerSora
    void addCard(CARD* pData, INT inReturn); void resetCardOwner(INT inOld, INT inNew); // PARTIAL: added for Game/Player/Player
};
#endif
}

#endif /* GAME_CARDBATTLE_H */
