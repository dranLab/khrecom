// PARTIAL: added for Game/Player/Summon
#ifndef GAME_PLAYER_PLAYERGAUGE_H
#define GAME_PLAYER_PLAYERGAUGE_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Vector.h"

namespace D2 { class C2DSeqCtrl; }
class CPlayer;

namespace Gauge {
class CEnemyGauge;

class CLockonMarker : public CTask {
    friend class ::CPlayer; // PARTIAL: added for Game/Player/Player (chkLockon reads the marker state)
private:
    CEnemyGauge* m_pGauge;          /* 0x40 */ // PARTIAL: split out for Game/Player/PlayerGauge
    D2::C2DSeqCtrl* m_pSeqCtrl;     /* 0x44 */
    D2::C2DSeqCtrl* m_pSeqCtrlPush; /* 0x48 */
    UINT m_nSeqNo;                  /* 0x4c */
    UINT m_nPreSeqNo;               /* 0x50 */
    FVECTOR m_vPos;             /* 0x60 */
    FVECTOR m_vUseCardPos;      /* 0x70 */
    FVECTOR m_vCameraTargetPos; /* 0x80 */
    FVECTOR m_vDispPos;         /* 0x90 */
    HANDLE m_hTarget;           /* 0xa0 */
    HANDLE m_hLockon;           /* 0xa4 */
    HANDLE m_hHoldTarget;       /* 0xa8 */
    INT m_iDist;                /* 0xac */
    INT m_iMode;                /* 0xb0 */
    BOOL m_bTarget;             /* 0xb4 */
    BOOL m_bLockon;             /* 0xb8 */
    BOOL m_bDisp;               /* 0xbc */
    BOOL m_bFlag;               /* 0xc0 */
    BOOL m_bTrg;                /* 0xc4 */
    UINT m_uiFlag;              /* 0xc8 */
    UINT m_uiKind;              /* 0xcc */
    FLOAT m_fDist;              /* 0xd0 */
    FLOAT m_fScale;             /* 0xd4 */

public:
    CLockonMarker(CTask* inParent, BOOL inFlag);
    virtual ~CLockonMarker(); // PARTIAL: declared for Game/Player/PlayerGauge
    void _initialize();
    void calcMarkerScale();
    void setTarget(HANDLE inTarget);
    void setLockon(HANDLE inTarget, INT inDist);
    void onPush(INT inKind);
    void offPush();
    void holdTarget(INT inFlag);
    void cancelLockon(INT& ioHandle, INT inFlag); // PARTIAL: added for Game/Player/PlayerRiku
    void chkLockon(INT& ioHandle);                // PARTIAL: added for Game/Player/PlayerRiku
    void setFlag(BOOL inFlag) { m_bFlag = inFlag; } // PARTIAL: added for Game/Player/PlayerRiku (original name unknown)
    void changeMode() {
        m_iMode = 1;
        m_bTrg = TRUE;
        m_hLockon = 0;
    }
    // PARTIAL: added for Game/Player/PlayerRiku (original name unknown; stores in this order)
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
};

#ifndef GAME_PLAYER_PLAYERGAUGE_CPP
// PARTIAL: added for Game/Player/Player
class CCommonGauge {
public:
    static void setDispSeq(HANDLE inOwner, INT inSeq);
};
#endif

#ifdef GAME_PLAYER_PLAYERGAUGE_CPP
/* PARTIAL: the other gauge classes, for Game/Player/PlayerGauge only */
class CNaviMap {
public:
    static BOOL m_bBattleEnd;
};
class CFriendGauge;
class CPrizeGauge;

class CCommonGauge : public CTask {
public:
    static CCommonGauge* m_pInstance;
    UINT m_uiFlag;               /* 0x40 */
    D2::C2DSeqCtrl* m_pSeqCtrls; /* 0x44 */
    FVECTOR m_vPos[7];           /* 0x50 */
    INT m_hOwner[7];             /* 0xc0 */
    CCommonGauge(CTask* p);
    virtual ~CCommonGauge();
    virtual void main();
    void _initialize();
    void dispSeqCtrl(INT inIndex, INT inFlag, INT inDisp);
};

class CPlayerGauge : public CTask {
    CNaviMap* m_pNaviMap;        /* 0x40 */
    CFriendGauge* m_pFriend;     /* 0x44 */
    CCommonGauge* m_pCommon;     /* 0x48 */
    CPrizeGauge* m_pPrize;       /* 0x4c */
    D2::C2DSeqCtrl* m_pSeqCtrl;   /* 0x50 */
    D2::C2DSeqCtrl* m_pSeqCtrlDP; /* 0x54 */
    LPCSTR m_szSeqFace;          /* 0x58 */
    UINT m_uiFlag;               /* 0x5c */
    TIMEQ m_tqRed;               /* 0x60 */
    TIMEQ m_tqTimer;             /* 0x64 */
    HANDLE m_hAlertSE;           /* 0x68 */
    USHORT m_nHP;                /* 0x6c */
    USHORT m_nRedHP;             /* 0x6e */
    USHORT m_nMaxHP;             /* 0x70 */
    USHORT m_iState;             /* 0x72 */
    USHORT m_nDP;                /* 0x74 */
    USHORT m_nMaxDP;             /* 0x76 */
    USHORT m_nStateDP;           /* 0x78 */
    USHORT dum;                  /* 0x7a */
public:
    CPlayerGauge(CTask* p);
    virtual ~CPlayerGauge();
    virtual void main();
    void _initialize();
    static void setBattleEnd();
    void setMaxHP(INT n);
    void changeTimeHP(D2::C2DSeqCtrl* p, UINT nIdx, UINT nHP);
    void setHP(INT inHP, INT inFlag);
    void exeDPGauge();
    void setDPGauge(INT n); void setDP(INT inDP, INT inMax);
};

class CEnemyGauge : public CTask {
public:
    static CEnemyGauge* m_pInstance;
    D2::C2DSeqCtrl* m_pSeqCtrl;  /* 0x40 */
    UINT m_uiFlag;               /* 0x44 */
    HANDLE m_hTarget;            /* 0x48 */
    TIMEQ m_tqRed;               /* 0x4c */
    SHORT m_nHP;                 /* 0x50 */
    SHORT m_nRedHP;              /* 0x52 */
    SHORT m_nMaxHP;              /* 0x54 */
    SHORT m_nHPS;                /* 0x56 */
    SHORT m_nRedHPS;             /* 0x58 */
    SHORT m_nMaxHPS;             /* 0x5a */
    FVECTOR m_vPos;              /* 0x60 */
    CEnemyGauge(CTask* p);
    virtual ~CEnemyGauge();
    void _initialize();
    static void resetHP();
    static HANDLE getNowTarget();
};

class CEscapeGauge : public CTask {
    UINT m_uiState;              /* 0x40 */
    UINT m_uiFlag;               /* 0x44 */
    D2::C2DSeqCtrl* m_pSeqCtrl;  /* 0x48 */
public:
    CEscapeGauge(CTask* p);
    virtual ~CEscapeGauge();
    virtual void main();
    void _initialize();
    void setCharge(FLOAT inRate);
    void destroy();
};

class CFriendGauge : public CTask {
    UINT m_uiState;              /* 0x40 */
    UINT m_uiCount;              /* 0x44 */
    UINT m_cardFriend[3];        /* 0x48 CARD */
    class C2DCard* m_pCards;     /* 0x54 */
    D2::C2DSeqCtrl* m_pSeqCtrls; /* 0x58 */
public:
    CFriendGauge(CTask* p);
    virtual ~CFriendGauge();
    void _initialize();
    virtual void main();
    void setGauge(INT n);
};

class CPrizeGauge : public CTask {
public:
    static CPrizeGauge* m_pInstance;
    UBYTE _m_0x40[0x10];         /* 0x40 CPadHandler */
    UINT m_uiMode;               /* 0x50 */
    UINT m_uiState;              /* 0x54 */
    INT m_iOldExp;               /* 0x58 */
    INT m_iExp;                  /* 0x5c */
    INT m_iFigure;               /* 0x60 */
    D2::C2DSeqCtrl* m_pSeqCtrls; /* 0x64 */
    UINT m_uiMogState;           /* 0x68 */
    INT m_iOldMp;                /* 0x6c */
    INT m_iMp;                   /* 0x70 */
    INT m_iMpFigure;             /* 0x74 */
    D2::C2DSeqCtrl* m_pMogSeqCtrls; /* 0x78 */
    INT m_iDropMp;               /* 0x7c */
    UINT m_uiCardState;          /* 0x80 */
    UINT m_cardData;             /* 0x84 CARD */
    D2::C2DSeqCtrl* m_pCardSeqCtrls; /* 0x88 */
    class C2DCard* m_pCard;      /* 0x8c */
    TIMEQ m_tqTimer;             /* 0x90 */
    CPrizeGauge(CTask* p);
    void _initialize();
    virtual ~CPrizeGauge();
    void setDispMogSeq();
    static void setDropMp(INT inMp);
    static BOOL isNowDispCard();
};
#endif

} // namespace Gauge

#endif /* GAME_PLAYER_PLAYERGAUGE_H */
