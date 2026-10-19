#ifndef GAME_BATTLECARD_H
#define GAME_BATTLECARD_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Fsm.h"
#include "System/Vector.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DCard.h"
#include "Game/CardKind.h"
#include "Game/CardBattle.h"

struct CB_LAYOUT {
    SHORT x;
    SHORT y;
    SHORT layer;
    SHORT scale;
};

namespace CardBattle {
extern unsigned char g_Seq_toLayout[26];
extern CB_LAYOUT g_LayoutData[65];
void initLayoutData();

class CBattleCard : public CTask, public CFSM {
    CStateTemplate<CBattleCard> m_State[7]; /* 0x06c */
    INT m_iState;            /* 0x14c */
    CARD m_cardData;         /* 0x150 */
    UINT m_uiFlag;           /* 0x154 */
    CHAR m_nNumber;          /* 0x158 */
    CHAR m_nNumberR;         /* 0x159 */
    CHAR m_nNumberE;         /* 0x15a */
    BYTE m_iStep;            /* 0x15b */
    BYTE m_uiUser;           /* 0x15c */
    BYTE m_uiMode;           /* 0x15d */
    CHAR m_iStock;           /* 0x15e */
    CHAR m_iStockCount;      /* 0x15f */
    CHAR m_iHand;            /* 0x160 */
    CHAR m_iHandMax;         /* 0x161 */
    CHAR m_iLayer;           /* 0x162 */
    BYTE m_uiOwner;          /* 0x163 */
    TIMEQ m_tqTimer;         /* 0x164 */
    G_XY m_Pos;              /* 0x168 */
    G_XY m_StartPos;         /* 0x170 */
    G_XY m_EndPos;           /* 0x178 */
    C2DCard* m_pCard;        /* 0x180 */
    FVECTOR m_vRot;          /* 0x190 */
    FLOAT m_fStartDir;       /* 0x1a0 */
    FLOAT m_fEndDir;         /* 0x1a4 */
    FLOAT m_fStartScale;     /* 0x1a8 */
    FLOAT m_fEndScale;       /* 0x1ac */
    FLOAT m_fBright;         /* 0x1b0 */
    CCardRevolver* m_pRevolver; /* 0x1b4 */
    HANDLE m_hTarget;        /* 0x1b8 */

public:
    CBattleCard(CTask* pParent, INT inIndex, INT inNumber, INT inUser, INT inState);
    virtual ~CBattleCard();
    virtual void main();
    void dispCard();

private:
    void _initialize();
    void changeState(INT inState);
    INT getCurrentState() { return m_iState; }
    void preStateUse();
    void stateUse();
    void preStateStock();
    void stateStock();
    void preStateHand();
    void stateHand();
    void preStateDestroy();
    void stateDestroy();
    void preStateDuel();
    void stateDuel();
    void preStateSnatch();
    void stateSnatch();
    void preStatePronounce();
    void statePronounce();

public:
    UINT getMode() { return m_uiMode; }
    INT getLayer() { return m_iLayer; }
    INT isDisp() { return m_uiFlag & 0x1; }
    void testDisp();
    void setPlayCard();
    void setPlayStock(INT inCount);
    void setPlayEnd();
    void setUseEnd();
    void setRefrect();
    void setBreak(INT inFlag);
    void destroy(INT inFlag);
    INT isSet() { return m_uiFlag & 0x2; }
    void setStock(UINT inPri);
    INT isNowStock() { return m_uiFlag & 0x4; }
    void setEnchant();
    void setHand(const CARD& inData, INT inNum, INT inMax, INT inMode);
    void setHandLocate(INT inNum, INT inMax);
    INT getHand() { return m_iHand; }
    INT getStock() { return m_iStock; }
    INT getHandMax() { return m_iHandMax; }
    UINT getFlag() { return m_uiFlag; }
    CARD* getCardData() { return &m_cardData; }
    void moveCard(TIMEQ inTime, TIMEQ inRoll);
    G_XY& getPos() { return m_Pos; }
    INT isNowSelect();
    void hideHand();
    void outHand();
    void updateReload(INT inNum);
    void resetHand(INT inNum, INT inMax);
    BOOL isReloadNG() { return m_uiFlag & 0x20; }
    void setReloadNG() { m_uiFlag |= 0x20; }
    void resetReloadNG() { m_uiFlag &= ~0x20; }
    BOOL isNowPlaying() { return m_uiFlag & 0x40; }
    BOOL isNowReloadNG() { return m_uiFlag & 0x60; }
    INT isDModeNG() { return m_uiFlag & 0x8000; }
    BOOL isPlutoON() { return m_uiFlag & 0x40000; }
    void setPlutoON() { m_uiFlag |= 0x40000; }
    void resetPlutoON() { m_uiFlag &= ~0x40000; }
    BOOL isPlayNG() { return m_uiFlag & 0x80000; }
    void setPlayNG() { m_uiFlag |= 0x80000; }
    void resetPlayNG() { m_uiFlag &= ~0x80000; }
    BOOL isReloadSet() { return m_uiFlag & 0x80000000; }
    void setReloadSet() { m_uiFlag |= 0x80000000; }
    void resetReloadSet() { m_uiFlag &= ~0x80000000; }
    void setStartIn() { m_uiFlag |= 0x100000; }
    BOOL isFolderON() { return m_uiFlag & 0x40000000; }
    void setFolderON() { m_uiFlag |= 0x40000000; }
    BOOL isDuelStock() { return m_uiFlag & 0x200000; }
    CCardRevolver* getRevolver() const { return m_pRevolver; }
    INT getCardKind() const { return m_cardData.m_nKind; }
    INT getCardCategory() const { return m_cardData.m_nCategory1; }
    INT getBaseCardNumber() const { return m_nNumber; }
    void setBaseCardNumber() { m_cardData.m_nNumber = m_nNumber; }
    void setSelect(INT inFlag);
    void changeData(CARD& inData);
    void setRoomEffect(const CARD* inData);
    void setEnchantEffect(INT inKind, UINT inFlag);
    void resetEnchantEffect(INT inKind);
    void setNumberBreak(INT inNum);
    void updateNumberEffect();
    void setDuel(INT inMode);
    void setFlagDuelPlay() { m_uiFlag |= 0x10000; }
    INT isFlagDuelPlay() { return m_uiFlag & 0x10000; }
    void setDuelRefrect();
    void setDuelStock(UINT inPri);
    void setPlayDuelStock(INT inCount);
    void setCardOwner(INT inOwner) { m_uiOwner = inOwner; }
    INT getCardOwner() const { return m_uiOwner; }
    void setSnatch(HANDLE inHandle);
    void resetSnatch();
    void setPronounce(UINT inPri);
    void setFlagPronouncePlay() { m_uiFlag |= 0x20000; }
    INT isFlagPronouncePlay() { return m_uiFlag & 0x20000; }
    void setPlayPronounce();
    void setDiscard(INT inFlag);
};
}

#endif /* GAME_BATTLECARD_H */
