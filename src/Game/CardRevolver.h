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
