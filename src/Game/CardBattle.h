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
namespace D2 { class C2DSeqCtrl; class C2DFnt; } namespace CardBattle { class CBattleCard; } class C2DCard; // PARTIAL: added for Game/CardBattle
namespace Message { class CMessageManager; } // PARTIAL: added for Game/CardBattle
union FVECTOR; // PARTIAL: added for Game/Player/FriendPluto

/* Placeholder for the original header's inline function asserting inUser: its strings
 * ("Game/CardBattle.h", "inUser") are in .rodata of every includer. The line number is not known. */
#ifndef CARDBATTLE_H_PATH // MATCH: path override for Game/Technic ("c:/KHCOM/MainApp/Game/CardBattle.h")
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
#if defined(GAME_CARDBATTLE_CPP) || defined(GAME_BATTLECARD_CPP) /* MATCH: GAME_BATTLECARD_CPP gate (tested 2026-11-02): Game/BattleCard inlines the original members too */
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
    static UINT isDisablePlayCard() { return m_pInstance ? m_pInstance->m_uiFlag & 0xC0000000 : 0; } // PARTIAL: added for Game/CardRevolver
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

class CCardRevolver : public CTask {
public:
    HANDLE m_hOwner;          /* 0x040 */
    UINT m_uiUser;            /* 0x044 */
    INT m_iState;             /* 0x048 */
    SHORT m_iMode;            /* 0x04c */
    SHORT m_nEnd;             /* 0x04e */
    short int m_nTotal[2];    /* 0x050 */
    short int m_nRemain[2];   /* 0x054 */
    short int m_nActive[2];   /* 0x058 */
    SHORT m_nLimit;           /* 0x05c */
    SHORT m_nExtra;           /* 0x05e */
    SHORT m_nReloadCount;     /* 0x060 */
    SHORT m_iMax;             /* 0x062 */
    SHORT m_iChargeCount;     /* 0x064 */
    SHORT m_iCharge;          /* 0x066 */
    TIMEQ m_tqCharge;         /* 0x068 */
    TIMEQ m_tqRollSpeed;      /* 0x06c */
    TIMEQ m_tqChargeRate;     /* 0x070 */
    BOOL m_bActive;           /* 0x074 */
    TIMEQ m_tqTimer;          /* 0x078 */
    UINT m_uiFlag;            /* 0x07c */
    TIMEQ m_tqDiscard;        /* 0x080 */
    TIMEQ m_tqSnatch;         /* 0x084 */
    HANDLE m_hSnatch;         /* 0x088 */
    SHORT m_nSnatchCount;     /* 0x08c */
    SHORT m_nPlutoMark;       /* 0x08e */
    CBattleCard* m_pCards;    /* 0x090 */
    CBattleCard* m_pCardsE;   /* 0x094 */
    CBattleCard* m_pCardsS[7]; /* 0x098 */
    CBattleCard* m_pActiveCard; /* 0x0b4 */
    C2DCard* m_pEmpty;        /* 0x0b8 */
    CARD* m_pDataS[7];        /* 0x0bc */
    SHORT m_iStock;           /* 0x0d8 */
    SHORT m_iStockSum;        /* 0x0da */
    SHORT m_iStockSkill;      /* 0x0dc */
    SHORT m_iExtraSkill;      /* 0x0de */
    SHORT m_iFirstSkill;      /* 0x0e0 */
    SHORT m_iAction;          /* 0x0e2 */
    SHORT m_nReloadID;        /* 0x0e4 */
    SHORT m_nShortCutID;      /* 0x0e6 */
    SHORT m_nAdd;             /* 0x0e8 */
    SHORT m_nReturn;          /* 0x0ea */
    SHORT m_nReloadCard;      /* 0x0ec */
    SHORT dum0;               /* 0x0ee */
    CARD m_cardAdd[39];       /* 0x0f0 */
    HANDLE m_hPlaySE;         /* 0x18c */
    HANDLE m_hChargeSE;       /* 0x190 */
    INT m_iReloadSEID;        /* 0x194 */
    UINT m_uiCounterState;    /* 0x198 */
    UINT m_uiScutState;       /* 0x19c */
    UINT m_uiCounterFlag;     /* 0x1a0 */
    D2::C2DSeqCtrl* m_pSeqCtrlCounter;       /* 0x1a4 */
    D2::C2DSeqCtrl* m_pSeqCtrlTrap;          /* 0x1a8 */
    D2::C2DSeqCtrl* m_pSeqCtrlCounter_Num01; /* 0x1ac */
    D2::C2DSeqCtrl* m_pSeqCtrlCounter_Num10; /* 0x1b0 */
    SHORT m_nOldMode;         /* 0x1b4 */
    SHORT m_nOldRemain;       /* 0x1b6 */
    UINT m_uiNumTexID;        /* 0x1b8 */
    UINT m_uiNumEffState;     /* 0x1bc */
    D2::C2DSeqCtrl* m_pSeqCtrlNumEff;        /* 0x1c0 */
    D2::C2DFnt* m_p2DFntStockName;           /* 0x1c4 */
    UINT m_uiStockState;      /* 0x1c8 */
    UINT m_uiStockFlag;       /* 0x1cc */
    UINT m_uiStockCategory;   /* 0x1d0 */
    D2::C2DSeqCtrl* m_pSeqCtrlStock;         /* 0x1d4 */
    D2::C2DSeqCtrl* m_pSeqCtrlStock_Num01;   /* 0x1d8 */
    D2::C2DSeqCtrl* m_pSeqCtrlStock_Num10;   /* 0x1dc */

    CCardRevolver(CTask* pParent, CARD* inDeck, INT inUser, INT inShortCut);
    virtual ~CCardRevolver();
    virtual void main();
    void destroy();
    void _init();
    void _initialize(CARD* inDeck, INT inShortCut);
    void _initCounter();
    void exeWait();
    void exeMain();
    void initReload(INT inMode);
    void exeReload();
    void initAction();
    void exeAction();
    void initHide();
    INT exeAddCard();
    INT exeSnatchCard();
    void snatchCard();
    INT returnSnatchCard(INT inNum);
    INT exeReturnCard();
    INT exeDiscard();
    void discard();
    void setDeck(CARD* pDeck, INT inShortCut);
    TIMEQ getRollSpeed() const { return m_tqRollSpeed; }
    void rollDeck(INT inNum, INT inFlag, TIMEQ inTime);
    void changeMode(INT inMode, INT inFlag);
    void rollShortCut(INT inMode);
    INT isActive() const { return m_bActive; }
    CBattleCard* getActiveCard() const;
    CARD* getActiveCardData();
    INT setPlayCard(INT inFlag, INT inTest);
    INT isEnableCharge();
    INT chargeReloadCard(INT inMode);
    void setChargeRate(INT inRate) { m_tqChargeRate = inRate; }
    void setReloadNG() {}
    INT getMode() const { return m_iMode; }
    INT getRemain(INT inMode, INT inKind, INT inCategory);
    INT getHave(INT inCategory, INT inPlay, INT inOwner, INT inReload, INT inNoGimmick);
    INT checkBreakCard(INT inNum, INT inZero, BOOL inCheckGreater);
    void setStockCard();
    INT returnStockCard(INT inFlag);
    INT setPlayStock(INT inFlag, INT inTest);
    void setAction(INT inID, INT inNum);
    void addCard(CARD* pData, INT inReturn);
    INT isNowAddCount() const { return m_nAdd; }
    void setNumberEffect(INT inKind);
    void setPlayNG(INT inFlag, INT inCategory);
    INT getStock(INT inFlag);
    INT getReloadID() const { return m_nReloadID; }
    INT getStockSkill() const { return m_iStockSkill; }
    void changeOwner(HANDLE inHandle) { m_hOwner = inHandle; }
    void changeDarkMode(HANDLE inHandle, INT inFlag);
    INT setPlayerDuelCard(INT inTest);
    void setPlayerDuelStock(CBattleCard* inCard, UINT inPri);
    INT setPlayerDuelPlayStock();
    void setSnatch(HANDLE inHandle);
    INT returnSnatch(INT inNum);
    INT canSnatch();
    void setTrapEffect();
    INT exeTrapEffect(INT inFlag);
    INT setPlayerPronounceCard(INT inTest);
    void setDiscard();
    void resetCardOwner(INT inOld, INT inNew);
    INT isDiscard() { return m_uiFlag & 0x8000; }
    INT calcPlutoMark();
    void dropPlutoMarkCard(const FVECTOR* inPos);
    void initCounter();
    void exeCounter();
    void destroyCounter();
    void deleteCounter();
    void _initNumEffect();
    void exeNumEffect();
    INT calcStockSkill();
    void setStockInfo();
    INT calcEnemyStockSkill();
    void initStock();
    void initStockSkillName(D2::C2DSeqCtrl* inSeq);
    void exeStock();
    void deleteStock();
};
}

#endif /* GAME_CARDBATTLE_H */
