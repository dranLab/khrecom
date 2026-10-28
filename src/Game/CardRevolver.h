#ifndef GAME_CARDREVOLVER_H
#define GAME_CARDREVOLVER_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Graphic/2d/2DFnt.h"
#include "Game/BattleCard.h"

namespace CardBattle {
struct BTL_DECK {
    USHORT m_nKind;
    SHORT m_nNumber;
};
void convBtlDeckToCard(BTL_DECK* inDeck);

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

class CEnchantGauge : public CTask {
public:
    UINT m_uiUser;            /* 0x40 */
    UINT m_uiState;           /* 0x44 */
    UINT m_nCount;            /* 0x48 */
    UINT m_nNext;             /* 0x4c */
    UINT m_nStack;            /* 0x50 */
    CARD m_cardData;          /* 0x54 */
    D2::C2DSeqCtrl* m_pSeqCtrls; /* 0x58 */
    D2::C2DFnt* m_pFntName;   /* 0x5c */

    CEnchantGauge(CTask* pParent, INT inUser, CARD& inCard);
    virtual ~CEnchantGauge();
    virtual void main();
    void _initialize();
    void setText();
    INT isEnableCount() { return m_uiState >= 2; }
    void changeCount(INT inNum);
    void destroy();
};

class CStockFolder : public CTask {
public:
    HANDLE m_hOwner;          /* 0x40 */
    UINT m_uiUser;            /* 0x44 */
    CBattleCard* m_pCards[7]; /* 0x48 */
    CARD* m_pData[7];         /* 0x64 */
    SHORT m_iStock;           /* 0x80 */
    SHORT m_iStockSum;        /* 0x82 */
    SHORT m_iStockSkill;      /* 0x84 */
    SHORT m_iExtraSkill;      /* 0x86 */
    SHORT m_iFirstSkill;      /* 0x88 */
    SHORT m_nNumTexID;        /* 0x8a */
    UINT m_uiState;           /* 0x8c */
    UINT m_uiFlag;            /* 0x90 */
    UINT m_uiCategory;        /* 0x94 */
    D2::C2DSeqCtrl* m_pSeqCtrl;       /* 0x98 */
    D2::C2DSeqCtrl* m_pSeqCtrl_Num01; /* 0x9c */
    D2::C2DSeqCtrl* m_pSeqCtrl_Num10; /* 0xa0 */
    D2::C2DFnt* m_p2DFntStockName;    /* 0xa4 */

    CStockFolder(CTask* pParent, INT inUser);
    virtual ~CStockFolder();
    virtual void main();
    void destroy();
    void _init();
    INT setStockCard(INT inKind, INT inNumber);
    INT getStockSkill() const { return m_iStockSkill; }
    INT getStockSum() const { return m_iStockSum; }
    void setStockInfo();
    INT calcEnemyStockSkill();
    INT returnStockCard();
    INT setPlayStock(INT inFlag, INT inTest);
    INT getStock(INT inFlag);
    INT setPronounceCard(INT inKind, INT inNumber);
    INT setPlayPronounce();
    void initGauge();
    void initName(D2::C2DSeqCtrl* inSeq);
    void exeGauge();
    void deleteGauge();
};
}

#endif /* GAME_CARDREVOLVER_H */
