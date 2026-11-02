// PARTIAL: added for Game/Player/Summon
#ifndef GAME_PLAYER_PLAYERGAUGE_H
#define GAME_PLAYER_PLAYERGAUGE_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Vector.h"
#include "Game/CardKind.h"
#include "System/PadHandler.h"

namespace D2 { class C2DSeqCtrl; }
class CPlayer;

class C2DCard;
namespace Gauge {
class CEnemyGauge;

/* MATCH: declared in reverse vtable order (CPlayerGauge first, then CLockonMarker, CEnemyGauge, CCommonGauge,
   CPrizeGauge, CEscapeGauge, CFriendGauge); the classes after CLockonMarker are not in the ref header
   (types.h only), their home here is a guess. */
class CNaviMap;
class CFriendGauge;
class CPrizeGauge;
class CCommonGauge;
class CPlayerGauge : public CTask {
private:
    CNaviMap* m_pNaviMap;         /* 0x40 */
    CFriendGauge* m_pFriend;      /* 0x44 */
    CCommonGauge* m_pCommon;      /* 0x48 */
    CPrizeGauge* m_pPrize;        /* 0x4c */
    D2::C2DSeqCtrl* m_pSeqCtrl;   /* 0x50 */
    D2::C2DSeqCtrl* m_pSeqCtrlDP; /* 0x54 */
    LPCSTR m_szSeqFace;           /* 0x58 */
    UINT m_uiFlag;                /* 0x5c */
    TIMEQ m_tqRed;                /* 0x60 */
    TIMEQ m_tqTimer;              /* 0x64 */
    HANDLE m_hAlertSE;            /* 0x68 */
    USHORT m_nHP;                 /* 0x6c */
    USHORT m_nRedHP;              /* 0x6e */
    USHORT m_nMaxHP;              /* 0x70 */
    USHORT m_iState;              /* 0x72 */
    USHORT m_nDP;                 /* 0x74 */
    USHORT m_nMaxDP;              /* 0x76 */
    USHORT m_nStateDP;            /* 0x78 */
    USHORT dum;                   /* 0x7a */

public:
    CPlayerGauge(CTask* pParent);
    virtual ~CPlayerGauge();
    virtual void main();
private:
    void _initialize();
    void setDPGauge(INT inState);
    void exeDPGauge();
    void changeTimeHP(D2::C2DSeqCtrl* inSeq, UINT nIdx, UINT inTime);
public:
    void setMaxHP(INT inHP);
    void setHP(INT inHP, INT inFlag);
    void setDamage(INT inHP, INT inKind);
    void setBattleEnd();
    void setDP(INT inDP, INT inMax);
    void setDMode(INT inMode, INT inDP, INT inMax);
};

class CLockonMarker : public CTask {
    friend class ::CPlayer; // PARTIAL: added for Game/Player/Player (chkLockon reads the marker state)
private:
    CEnemyGauge* m_pGauge;          /* 0x40 */
    D2::C2DSeqCtrl* m_pSeqCtrl;     /* 0x44 */
    D2::C2DSeqCtrl* m_pSeqCtrlPush; /* 0x48 */
    UINT m_nSeqNo;                  /* 0x4c */
    UINT m_nPreSeqNo;               /* 0x50 */
    FVECTOR m_vPos;                 /* 0x60 */
    FVECTOR m_vUseCardPos;          /* 0x70 */
    FVECTOR m_vCameraTargetPos;     /* 0x80 */
    FVECTOR m_vDispPos;             /* 0x90 */
    HANDLE m_hTarget;               /* 0xa0 */
    HANDLE m_hLockon;               /* 0xa4 */
    HANDLE m_hHoldTarget;           /* 0xa8 */
    INT m_iDist;                    /* 0xac */
    INT m_iMode;                    /* 0xb0 */
    BOOL m_bTarget;                 /* 0xb4 */
    BOOL m_bLockon;                 /* 0xb8 */
    BOOL m_bDisp;                   /* 0xbc */
    BOOL m_bFlag;                   /* 0xc0 */
    BOOL m_bTrg;                    /* 0xc4 */
    UINT m_uiFlag;                  /* 0xc8 */
    UINT m_uiKind;                  /* 0xcc */
    FLOAT m_fDist;                  /* 0xd0 */
    FLOAT m_fScale;                 /* 0xd4 */

public:
    CLockonMarker(CTask* pParent, BOOL inFlag);
    virtual ~CLockonMarker();
    virtual void main();
private:
    void _initialize();
    void calcMarkerScale();
#ifdef GAME_PLAYER_PLAYERGAUGE_CPP /* MATCH: GAME_PLAYER_PLAYERGAUGE_CPP gate, only for its own unit (tested 2026-11-02) */
    /* MATCH: the ref inlines only for Game/Player/PlayerGauge (emitted with the key function, in ref definition order); Game/Player/PlayerRiku matches with its own forms below */
    void setDistance(INT inDist) { m_iDist = inDist; }
public:
    void on() { m_bFlag = TRUE; }
    void off() { m_bFlag = FALSE; }
    BOOL isFlag() { return m_bFlag; }
    void setFlag(UINT inSet, UINT inMask) { m_uiFlag = (m_uiFlag | inSet) & ~inMask; }
#endif
public:
    void setTarget(HANDLE inTarget);
    void setLockon(HANDLE inTarget, INT inDist);
    void cancelLockon(HANDLE& inTarget, INT inFlag);
    void chkLockon(HANDLE& inTarget);
#ifdef GAME_PLAYER_PLAYERGAUGE_CPP /* MATCH: as above */
    void changeMode() {
        m_iMode ^= 1;
        m_bTrg = TRUE;
        m_hLockon = 0;
    }
    HANDLE getTarget() { return m_hTarget; }
    HANDLE getLockon() { return m_hLockon; }
    FVECTOR& getPos() { return m_vPos; }
    FVECTOR& getUseCardPos() { return m_vUseCardPos; }
    FVECTOR& getCameraTargetPos() { return m_vCameraTargetPos; }
    void changeMode(INT inMode) {
        m_iMode = inMode;
        m_bTrg = TRUE;
        m_hLockon = 0;
    }
    INT getMode() { return m_iMode; }
#else
    /* PARTIAL: Game/Player/PlayerRiku's forms of the ref inlines (names and store orders unknown) */
    void setFlag(BOOL inFlag) { m_bFlag = inFlag; }
    void changeMode() {
        m_iMode = 1;
        m_bTrg = TRUE;
        m_hLockon = 0;
    }
    void setMode(INT inMode) {
        m_iMode = inMode;
        m_hLockon = 0;
        m_bTrg = TRUE;
    }
    void changeMode(INT inMode) {
        m_bTrg = TRUE;
        m_iMode = inMode;
        m_hLockon = 0;
    }
#endif
    void onPush(INT inKind);
    void offPush();
    void holdTarget(INT inFlag);
};

class CEnemyGauge : public CTask {
private:
    static CEnemyGauge* m_pInstance;
    D2::C2DSeqCtrl* m_pSeqCtrl; /* 0x40 */
    UINT m_uiFlag;              /* 0x44 */
    HANDLE m_hTarget;           /* 0x48 */
    TIMEQ m_tqRed;              /* 0x4c */
    SHORT m_nHP;                /* 0x50 */
    SHORT m_nRedHP;             /* 0x52 */
    SHORT m_nMaxHP;             /* 0x54 */
    SHORT m_nHPS;               /* 0x56 */
    SHORT m_nRedHPS;            /* 0x58 */
    SHORT m_nMaxHPS;            /* 0x5a */
    FVECTOR m_vPos;             /* 0x60 */

public:
    CEnemyGauge(CTask* pParent);
    virtual ~CEnemyGauge();
    virtual void main();
private:
    void _initialize();
public:
    static void resetHP();
    static void setHP(HANDLE inHandle, INT inHP, INT inMaxHP);
    static void notifyHP(HANDLE inHandle, INT inHP, INT inMaxHP);
    static HANDLE getNowTarget();
};

class CCommonGauge : public CTask {
private:
    static CCommonGauge* m_pInstance;
    UINT m_uiFlag;               /* 0x40 */
    D2::C2DSeqCtrl* m_pSeqCtrls; /* 0x44 */
    FVECTOR m_vPos[7];           /* 0x50 */
    int m_hOwner[7];             /* 0xc0 */

public:
    CCommonGauge(CTask* pParent);
    virtual ~CCommonGauge();
    virtual void main();
private:
    void _initialize();
    void dispSeqCtrl(INT inIndex, INT inFlag, INT inDisp);
public:
    static void setDispSeq(HANDLE inHandle, INT inIndex);
};

class CPrizeGauge : public CTask, public CPadHandler {
private:
    static CPrizeGauge* m_pInstance;
    UINT m_uiMode;                   /* 0x50 */
    UINT m_uiState;                  /* 0x54 */
    INT m_iOldExp;                   /* 0x58 */
    INT m_iExp;                      /* 0x5c */
    INT m_iFigure;                   /* 0x60 */
    D2::C2DSeqCtrl* m_pSeqCtrls;     /* 0x64 */
    UINT m_uiMogState;               /* 0x68 */
    INT m_iOldMp;                    /* 0x6c */
    INT m_iMp;                       /* 0x70 */
    INT m_iMpFigure;                 /* 0x74 */
    D2::C2DSeqCtrl* m_pMogSeqCtrls;  /* 0x78 */
    INT m_iDropMp;                   /* 0x7c */
    UINT m_uiCardState;              /* 0x80 */
    CARD m_cardData;                 /* 0x84 */
    D2::C2DSeqCtrl* m_pCardSeqCtrls; /* 0x88 */
    ::C2DCard* m_pCard;              /* 0x8c */
    TIMEQ m_tqTimer;                 /* 0x90 */
    CARD m_cardStack[4];             /* 0x94 */
    SHORT m_nStack;                  /* 0xa4 */
    SHORT m_nTexID;                  /* 0xa6 */

public:
    CPrizeGauge(CTask* pParent);
    virtual ~CPrizeGauge();
    virtual void main();
private:
    void _initialize();
    void setDispSeq();
    void setDispMogSeq();
    INT exePrizeCard();
public:
    static void setPrizeExp(INT inExp);
    static void setPrizeMp(INT inMp);
    static void setPrizeCard(CARD& inData, INT inMode, INT inStack);
    static void setDropMp(INT inMp);
    static INT isNowDispCard();
};

class CEscapeGauge : public CTask {
private:
    UINT m_uiState;             /* 0x40 */
    UINT m_uiFlag;              /* 0x44 */
    D2::C2DSeqCtrl* m_pSeqCtrl; /* 0x48 */

public:
    CEscapeGauge(CTask* pParent);
    virtual ~CEscapeGauge();
    virtual void main();
private:
    void _initialize();
public:
    void setCharge(FLOAT inRate);
    void destroy();
};

class CFriendGauge : public CTask {
private:
    UINT m_uiState;              /* 0x40 */
    UINT m_uiCount;              /* 0x44 */
    CARD m_cardFriend[3];        /* 0x48 */
    ::C2DCard* m_pCards;         /* 0x54 */
    D2::C2DSeqCtrl* m_pSeqCtrls; /* 0x58 */

public:
    CFriendGauge(CTask* pParent);
    virtual ~CFriendGauge();
    virtual void main();
    void setGauge(INT inState);
private:
    void _initialize();
};

} // namespace Gauge

#endif /* GAME_PLAYER_PLAYERGAUGE_H */
