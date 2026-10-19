// PARTIAL: added for Game/Atel/CameraMan (only what that unit uses)
#ifndef GAME_ATEL_AVATAR_H
#define GAME_ATEL_AVATAR_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/Task.h"
#include "Game/Atel/CommandAccepter.h"

#ifndef AVATAR_H_PATH
#define AVATAR_H_PATH "c:/KHCOM/MainApp/Game/Atel/Avatar.h"
#endif

#ifdef GAME_ATEL_AVATAR_CPP
#include "Game/Entity.h"
#include "System/Fsm.h"
union FMATRIX;
class CPathData;
class CHermitePath;
class CAtel;
#endif

namespace ATEL {

#ifdef GAME_ATEL_AVATAR_CPP
extern "C" char D_0048AB50[]; /* "c:/KHCOM/MainApp/Game/Atel/Avatar.h" (PARTIAL: no SOURCE DATA yet) */
extern "C" char D_0048AB88[];
struct ETMoveInfo;
class CVSM;

struct AvatarMoveStruct { // 0x70
    FVECTOR _vStartPos;            /* 0x00 */
    FVECTOR _vTargetPos;           /* 0x10 */
    FVECTOR _vStartPosLocal;       /* 0x20 */
    FVECTOR _vTargetPosLocal;      /* 0x30 */
    FMATRIX* _pMatRide;            /* 0x40 */
    TIMEQ _nMoveTimeLimit;         /* 0x44 */
    TIMEQ _nMoveTimeCount;         /* 0x48 */
    FLOAT _fSpeed;                 /* 0x4c */
    FLOAT _fRevolutionRotY;        /* 0x50 */
    FLOAT _fRevolutionTargetRotY;  /* 0x54 */
    FLOAT _fRevolutionRadius;      /* 0x58 */
    CPathData* _pPathData;         /* 0x5c */
    CHermitePath* _pHermiteHelper; /* 0x60 */
    USHORT _nMoveType;             /* 0x64 */
    USHORT _nMoveMotion;           /* 0x66 */

    AvatarMoveStruct() {
        _pMatRide = NULL;
        _pPathData = NULL;
        _pHermiteHelper = NULL;
    }
    ~AvatarMoveStruct();
    BOOL setupPath(const CHAR* pszName, USHORT nType);
    void getPosOnPath(FVECTOR& vPos);
    BOOL updatePosOnPath(FLOAT fTime);
};

struct AvatarRotateStruct { // 0x50
    FVECTOR _vStartRot;            /* 0x00 */
    FVECTOR _vTargetRot;           /* 0x10 */
    FVECTOR _vRotSpeed;            /* 0x20 */
    TIMEQ _nRotTimeLimit;          /* 0x30 */
    TIMEQ _nRotTimeCount;          /* 0x34 */
    TIMEQ _nRotIntervalTime;       /* 0x38 */
    TIMEQ _nRotIntervalTimeCount;  /* 0x3c */
    UINT _nRotTargetRegularly;     /* 0x40 */
    USHORT _nRotateType;           /* 0x44 */
    AvatarRotateStruct() { _nRotIntervalTime = 0; }
};

struct AvatarScaleStruct { // 0x30
    FVECTOR _vTargetScale;   /* 0x00 */
    FVECTOR _vScaleSpeed;    /* 0x10 */
    TIMEQ _nScaleTimeLimit;  /* 0x20 */
    TIMEQ _nScaleTimeCount;  /* 0x24 */
};

struct AvatarFadeStruct { // 0x14
    FLOAT _fTargetFade;     /* 0x00 */
    FLOAT _fFadeSpeed;      /* 0x04 */
    TIMEQ _nFadeTimeLimit;  /* 0x08 */
    TIMEQ _nFadeTimeCount;  /* 0x0c */
    INT _ModelNo;           /* 0x10 */
};

struct AvatarLookStruct { // 0x8
    INT _nTargetID;
    FLOAT _fOffsetY;
};

/* PARTIAL: the full class, for Game/Atel/Avatar only */
class CAvatar : public ICommandAccepter, public CEntity {
    UINT m_nIndex;                         /* 0x1e0 */
    UINT m_nParentActorIdx;                /* 0x1e4 */
    INT m_nState;                          /* 0x1e8 */
    UINT m_nActionTrg;                     /* 0x1ec */
    BYTE m_nMotInValidFlag;                /* 0x1f0 */
    AvatarLookStruct* m_pLookStruct;       /* 0x1f4 */
    AvatarMoveStruct* m_pMoveStruct;       /* 0x1f8 */
    AvatarRotateStruct* m_pRotateStruct;   /* 0x1fc */
    AvatarScaleStruct* m_pScaleStruct;     /* 0x200 */
    AvatarFadeStruct* m_pFadeStruct[5];    /* 0x204 */
    CStateTemplate<CAvatar> m_MainState[2]; /* 0x218 */
    UBYTE lineX[0x70];                     /* 0x260 C3DLine */
    UBYTE lineY[0x70];                     /* 0x2d0 C3DLine */
    UBYTE lineZ[0x70];                     /* 0x340 C3DLine */

public:
    CAvatar(CTask* pParent);
    virtual ~CAvatar();
    void InitAvatar(INT flag);
    virtual void disp();
    UINT getParentActorIdx() const { return m_nParentActorIdx; }
    UINT getActorIdx() const { return m_nIndex; }
    INT getAvatarState() const { return m_nState; }
    void getNowPosition(FVECTOR& vPos) const;
    void getPrevPosition(FVECTOR& vPos) const;
    void getMoveInfo(ETMoveInfo& info) const;
    CMovement* getMovement() const { return m_pMove; }
    void getScale(FVECTOR& vScale) const;
    void getRotation(FVECTOR& vRot) const;
    void getFade(INT nModel, FLOAT& fFade) const;
    FLOAT getDirection() const;
    virtual INT getTypeID() const;
    void setAvatarState(INT nState, BOOL bOn) {
        if (bOn) m_nState |= nState;
        else m_nState &= ~nState;
    }
    BOOL isAvatarBusy() const { return m_nState & 0x2; }
    BOOL isAvatarActive() const { return m_nState & 0x1; }
    BOOL isAvatarVisible() const { return !((m_nState >> 2) & 1); }
    BOOL isAvatarMoving() const { return m_nState & 0x10; }
    BOOL isAvatarSuspendingMove() const { return m_nState & 0x20; }
    BOOL isAvatarRotating() const { return m_nState & 0x40; }
    BOOL isAvatarScaling() const { return m_nState & 0x80; }
    BOOL isAvatarAnimating() const { return m_nState & 0x100; }
    BOOL isAvatarFadeing() const { return m_nState & 0x200; }
    virtual INT reqCommand(INT nCmd, void* pParam, INT nID);

private:
    BOOL createMoveStruct();
    BOOL createRotateStruct();
    BOOL createScaleStruct();
    AvatarFadeStruct* createFadeStruct();

protected:
    CAtel& getAtel();
    CVSM& getVSM();
    INT onInit(INT nCmd, void* pParam, INT nID);
    INT onSetActive(INT nCmd, void* pParam, INT nID);
    INT onSetVisible(INT nCmd, void* pParam, INT nID);
    INT onSetVisibleModel(INT nCmd, void* pParam, INT nID);
    INT onSetGravity(INT nCmd, void* pParam, INT nID);
    INT onSetCollision(INT nCmd, void* pParam, INT nID);
    INT onSetMapCollision(INT nCmd, void* pParam, INT nID);
    INT onSetIK(INT nCmd, void* pParam, INT nID);
    INT onCreateLegIK(INT nCmd, void* pParam, INT nID);
    INT onSetLockOn(INT nCmd, void* pParam, INT nID);
    INT onSetNearClip(INT nCmd, void* pParam, INT nID);
    INT onEnableNotSemiModel(INT nCmd, void* pParam, INT nID);
    INT onSetPosistion(INT nCmd, void* pParam, INT nID);
    INT onSetPosPathTop(INT nCmd, void* pParam, INT nID);
    INT onMove(INT nCmd, void* pParam, INT nID);
    INT onStopMove(INT nCmd, void* pParam, INT nID);
    INT onContinueMove(INT nCmd, void* pParam, INT nID);
    INT onRotate(INT nCmd, void* pParam, INT nID);
    INT onSetDirection(INT nCmd, void* pParam, INT nID);
    INT onScaling(INT nCmd, void* pParam, INT nID);
    INT onFadeing(INT nCmd, void* pParam, INT nID);
    INT onPlayMotion(INT nCmd, void* pParam, INT nID);
    INT onPlayMotionDx(INT nCmd, void* pParam, INT nID);
    INT onPlayMotionEx(INT nCmd, void* pParam, INT nID);
    INT onPlayMotionExDx(INT nCmd, void* pParam, INT nID);
    INT onPlayMotionEfx(INT nCmd, void* pParam, INT nID);
    INT onSetTexAnimSeq(INT nCmd, void* pParam, INT nID);
    INT onSetMotionFrame(INT nCmd, void* pParam, INT nID);
    INT onRemoveAvatar(INT nCmd, void* pParam, INT nID);
    INT onSetMakeUpMotionInValid(INT nCmd, void* pParam, INT nID);
    INT onAvatarStop(INT nCmd, void* pParam, INT nID);
    INT onAvatarStart(INT nCmd, void* pParam, INT nID);
    INT onAvatarAttach(INT nCmd, void* pParam, INT nID);
    INT onAvatarAttachEffect(INT nCmd, void* pParam, INT nID);
    INT onAvatarSetLookPosition(INT nCmd, void* pParam, INT nID);
    INT onAvatarSetLookUpdatePosition(INT nCmd, void* pParam, INT nID);
    INT onAvatarSetLookAngle(INT nCmd, void* pParam, INT nID);
    INT onAvatarSetLookUpDateAngle(INT nCmd, void* pParam, INT nID);
    INT onAvatarLookStart(INT nCmd, void* pParam, INT nID);
    INT onAvatarLookStop(INT nCmd, void* pParam, INT nID);
    INT onAvatarLookEnd(INT nCmd, void* pParam, INT nID);
    INT onAvatarChangeTexture(INT nCmd, void* pParam, INT nID);
    void changeAvatarMainState(INT nState);
    void beginStateMain();
    void stateMain();
    void stateMainEnd();
    void stateSkipMain();
    virtual void updateBefore();
    virtual void updateAfter();
    virtual void lastUpdate();

public:
    void FadeAvatarEnd();
    void cancelFadeAvatar();
    void ScaleAvatarEnd();
    void cancelScaleAvatar();
    void setRotAhead();
    void MoveAvatarEnd();
    void setAvatarGravity(BOOL bOn);
    void setAvatarRotTimeLimit(TIMEQ nTime);
    void clearAvatarScaleSpeed() {
#line 582 AVATAR_H_PATH
        if (!(m_pScaleStruct)) AssertApp(D_0048AB50, __LINE__, D_0048AB88, 0);
        m_pScaleStruct->_vScaleSpeed._q = 0;
    }
    BOOL updateAvatarPosOnPath(TIMEQ nTime);
    FVECTOR* getPathStartPos(const CHAR* pszName) const;
};
#else
/* CEntity is the second base in the original (CTask at 0x10). */
class CAvatar : public ICommandAccepter, public CTask {
public:
    CAvatar();
    CAvatar(CTask* pParent); // PARTIAL: added for Game/Atel/Atel
    void InitAvatar(INT flag); // PARTIAL: added for Game/Atel/Atel
    virtual INT reqCommand(INT nCmd, void* pParam, INT nID); // PARTIAL: added for Game/Atel/Atel
};
#endif

/* Placeholder for the original header's inline functions: their assert strings are in .rodata of every
 * includer, in this order. Line numbers are not known. */
#line 1 AVATAR_H_PATH
inline void _avatarCheckStructs(void* in_pFadeStruct, void* m_pScaleStruct, void* m_pRotateStruct, void* m_pMoveStruct) {
    if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
    if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
    if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
    if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
}

} // namespace ATEL

#endif /* GAME_ATEL_AVATAR_H */
