#ifndef GAME_ATEL_AVATAR_H
#define GAME_ATEL_AVATAR_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/Task.h"
#include "Game/Atel/CommandAccepter.h"

#ifndef AVATAR_H_PATH
#define AVATAR_H_PATH "c:/KHCOM/MainApp/Game/Atel/Avatar.h"
#endif

#include "Game/Entity.h"
#include "System/Fsm.h"
#include "System/Graphic/Gprim.h"
union FMATRIX;
class CPathData;
class CHermitePath;
class CAtel;

namespace ATEL {

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
    BOOL isOverTimeLimit() { return _nMoveTimeCount >= _nMoveTimeLimit; }
    BOOL isOverTimeLimit(INT t) { return _nMoveTimeCount + t >= _nMoveTimeLimit; }
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
    BOOL isOverTimeLimit() { return _nRotTimeCount >= _nRotTimeLimit; }
    BOOL isOverTimeLimit(INT t) { return _nRotTimeCount + t >= _nRotTimeLimit; }
    void stopRotRegulalry();
    BOOL isRotRegularly();
    BOOL isOverIntervalTime();
};

struct AvatarScaleStruct { // 0x30
    FVECTOR _vTargetScale;   /* 0x00 */
    FVECTOR _vScaleSpeed;    /* 0x10 */
    TIMEQ _nScaleTimeLimit;  /* 0x20 */
    TIMEQ _nScaleTimeCount;  /* 0x24 */
    BOOL isOverTimeLimit() { return _nScaleTimeCount >= _nScaleTimeLimit; }
    BOOL isOverTimeLimit(INT t) { return _nScaleTimeCount + t >= _nScaleTimeLimit; }
};

struct AvatarFadeStruct { // 0x14
    FLOAT _fTargetFade;     /* 0x00 */
    FLOAT _fFadeSpeed;      /* 0x04 */
    TIMEQ _nFadeTimeLimit;  /* 0x08 */
    TIMEQ _nFadeTimeCount;  /* 0x0c */
    INT _ModelNo;           /* 0x10 */
    BOOL isOverTimeLimit() { return _nFadeTimeCount >= _nFadeTimeLimit; }
    BOOL isOverTimeLimit(INT t) { return _nFadeTimeCount + t >= _nFadeTimeLimit; }
};

struct AvatarLookStruct { // 0x8
    INT _nTargetID;
    FLOAT _fOffsetY;
};

class CAvatar : public ICommandAccepter, public CEntity {
    friend class ::CAtel; // Game/Atel/Atel calls the private InitAvatar
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
    C3DLine lineX;                         /* 0x260 */
    C3DLine lineY;                         /* 0x2d0 */
    C3DLine lineZ;                         /* 0x340 */

private:
    void setParentActorIdx(UINT nIdx) { m_nParentActorIdx = nIdx; }
    void setActorIdx(UINT nIdx) { m_nIndex = nIdx; }
    BOOL createMoveStruct();
    BOOL createRotateStruct();
    BOOL createScaleStruct();
    AvatarFadeStruct* createFadeStruct();
    void InitAvatar(INT flag);
    void TexAnimTalk(INT nMotion);
    virtual void disp();

public:
    CAvatar(CTask* pParent);
    virtual ~CAvatar();
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
    BOOL isAvatarMovingFuture() const { return m_nState & 0x400; }
    BOOL isAvatarSuspendingMoveFuture() const { return m_nState & 0x800; }
    BOOL isAvatarRotatingFuture() const { return m_nState & 0x1000; }
    BOOL isAvatarScalingFuture() const { return m_nState & 0x2000; }
    BOOL isAvatarAnimatingFuture() const { return m_nState & 0x4000; }
    BOOL isAvatarFadeingFuture() const { return m_nState & 0x8000; }
    virtual INT reqCommand(INT nCmd, void* pParam, INT nID) {
        if (nCmd == 8) return onSetActive(nCmd, pParam, nID);
        if (nCmd == 6) return onSetVisible(nCmd, pParam, nID);
        if (nCmd == 7) return onSetVisibleModel(nCmd, pParam, nID);
        if (nCmd == 0x1d) return onSetGravity(nCmd, pParam, nID);
        if (nCmd == 0x1f) return onSetCollision(nCmd, pParam, nID);
        if (nCmd == 0x1e) return onSetMapCollision(nCmd, pParam, nID);
        if (nCmd == 0x21) return onSetIK(nCmd, pParam, nID);
        if (nCmd == 0x24) return onCreateLegIK(nCmd, pParam, nID);
        if (nCmd == 0x22) return onSetLockOn(nCmd, pParam, nID);
        if (nCmd == 0x23) return onSetNearClip(nCmd, pParam, nID);
        if (nCmd == 0xa) return onSetPosistion(nCmd, pParam, nID);
        if (nCmd == 0xb) return onSetPosPathTop(nCmd, pParam, nID);
        if (nCmd == 0xd) return onMove(nCmd, pParam, nID);
        if (nCmd == 0x11) return onStopMove(nCmd, pParam, nID);
        if (nCmd == 0x12) return onContinueMove(nCmd, pParam, nID);
        if (nCmd == 0xe) return onRotate(nCmd, pParam, nID);
        if (nCmd == 0xc) return onSetDirection(nCmd, pParam, nID);
        if (nCmd == 0xf) return onScaling(nCmd, pParam, nID);
        if (nCmd == 0x10) return onFadeing(nCmd, pParam, nID);
        if (nCmd == 0x13) return onPlayMotion(nCmd, pParam, nID);
        if (nCmd == 0x14) return onPlayMotionDx(nCmd, pParam, nID);
        if (nCmd == 0x15) return onPlayMotionEx(nCmd, pParam, nID);
        if (nCmd == 0x16) return onPlayMotionExDx(nCmd, pParam, nID);
        if (nCmd == 0x17) return onPlayMotionEfx(nCmd, pParam, nID);
        if (nCmd == 0x19) return onSetMotionFrame(nCmd, pParam, nID);
        if (nCmd == 0x1a) return onSetMakeUpMotionInValid(nCmd, pParam, nID);
        if (nCmd == 1) return onRemoveAvatar(nCmd, pParam, nID);
        if (nCmd == 2) return onInit(nCmd, pParam, nID);
        if (nCmd == 0x2e) return onAvatarStop(nCmd, pParam, nID);
        if (nCmd == 0x2f) return onAvatarStart(nCmd, pParam, nID);
        if (nCmd == 5) return onAvatarAttach(nCmd, pParam, nID);
        if (nCmd == 0x3a) return onAvatarAttachEffect(nCmd, pParam, nID);
        if (nCmd == 0x30) return onAvatarSetLookPosition(nCmd, pParam, nID);
        if (nCmd == 0x31) return onAvatarSetLookUpdatePosition(nCmd, pParam, nID);
        if (nCmd == 0x32) return onAvatarSetLookAngle(nCmd, pParam, nID);
        if (nCmd == 0x33) return onAvatarSetLookUpDateAngle(nCmd, pParam, nID);
        if (nCmd == 0x34) return onAvatarLookStart(nCmd, pParam, nID);
        if (nCmd == 0x35) return onAvatarLookStop(nCmd, pParam, nID);
        if (nCmd == 0x36) return onAvatarLookEnd(nCmd, pParam, nID);
        if (nCmd == 0x37) return onAvatarChangeTexture(nCmd, pParam, nID);
        if (nCmd == 0x38) return onSetTexAnimSeq(nCmd, pParam, nID);
        if (nCmd == 0x39) return onEnableNotSemiModel(nCmd, pParam, nID);
        return 0;
    }

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
    TIMEQ secTQ(FLOAT f) const { return f * 3000.0f; }
    FLOAT getTimeCountRate(INT nLimit, INT nCount) const {
        if (nCount < nLimit) {
            if (nLimit > 0) return (FLOAT)nCount / nLimit;
            return 0.0f;
        }
        return 1.0f;
    }
    void setAvatarFadeTimeLimit(AvatarFadeStruct* in_pFadeStruct, INT t) {
#line 494 AVATAR_H_PATH
        if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
        in_pFadeStruct->_nFadeTimeLimit = t;
    }
    void setAvatarFadeTimeLimit(FLOAT f) { setAvatarFadeTimeLimit(secTQ(f)); } // MATCH: the original's overload calls itself (TIMEQ -> FLOAT)
    void setAvatarTargetFade(AvatarFadeStruct* in_pFadeStruct, const FLOAT& f) {
#line 498 AVATAR_H_PATH
        if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
        in_pFadeStruct->_fTargetFade = f;
    }
    void setAvatarFadeSpeed(AvatarFadeStruct* in_pFadeStruct, const FLOAT& f) {
#line 500 AVATAR_H_PATH
        if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
        in_pFadeStruct->_fFadeSpeed = f;
    }
    FLOAT& getAvatarTargetFade(AvatarFadeStruct* in_pFadeStruct) const {
#line 506 AVATAR_H_PATH
        if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
        return in_pFadeStruct->_fTargetFade;
    }
    FLOAT& getAvatarFadeSpeed(AvatarFadeStruct* in_pFadeStruct) const {
#line 508 AVATAR_H_PATH
        if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
        return in_pFadeStruct->_fFadeSpeed;
    }
    TIMEQ getAvatarFadeTimeLimit(AvatarFadeStruct* in_pFadeStruct) const {
#line 510 AVATAR_H_PATH
        if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
        return in_pFadeStruct->_nFadeTimeLimit;
    }
    TIMEQ getAvatarFadeTimeCount(AvatarFadeStruct* in_pFadeStruct) const {
#line 512 AVATAR_H_PATH
        if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
        return in_pFadeStruct->_nFadeTimeCount;
    }
    FLOAT getAvatarFadeTimeCountRate(AvatarFadeStruct* in_pFadeStruct) const {
#line 514 AVATAR_H_PATH
        if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
        return getTimeCountRate(in_pFadeStruct->_nFadeTimeLimit, in_pFadeStruct->_nFadeTimeCount);
    }
    BOOL isOverAvatarFadeTimeLimit(AvatarFadeStruct* in_pFadeStruct) const {
#line 520 AVATAR_H_PATH
        if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
        return in_pFadeStruct->isOverTimeLimit();
    }
    BOOL isOverAvatarFadeTimeLimit(AvatarFadeStruct* in_pFadeStruct, INT t) const {
#line 521 AVATAR_H_PATH
        if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
        return in_pFadeStruct->isOverTimeLimit(t);
    }
    void clearAvatarFadeSpeed(AvatarFadeStruct* in_pFadeStruct) {
#line 528 AVATAR_H_PATH
        if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
        in_pFadeStruct->_fFadeSpeed = 0.0f;
    }
    void FadeAvatarEnd();
    void cancelFadeAvatar();
    void clearAvatarFadeTimeCount(AvatarFadeStruct* in_pFadeStruct) {
#line 534 AVATAR_H_PATH
        if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
        in_pFadeStruct->_nFadeTimeCount = 0;
    }
    void countAvatarFadeTime(AvatarFadeStruct* in_pFadeStruct, INT t) {
#line 536 AVATAR_H_PATH
        if (!(in_pFadeStruct)) AssertApp(__FILE__, __LINE__, "in_pFadeStruct", 0);
        in_pFadeStruct->_nFadeTimeCount += t;
    }
    BOOL FadeAvatarMain();
    void setAvatarScaleTimeLimit(INT t) {
#line 547 AVATAR_H_PATH
        if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
        m_pScaleStruct->_nScaleTimeLimit = t;
    }
    void setAvatarScaleTimeLimit(FLOAT f) { setAvatarScaleTimeLimit(secTQ(f)); }
    void setAvatarTargetScale(const FVECTOR& v) {
#line 551 AVATAR_H_PATH
        if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
        m_pScaleStruct->_vTargetScale.set(v);
    }
    void setAvatarScaleSpeed(const FVECTOR& v) {
#line 553 AVATAR_H_PATH
        if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
        m_pScaleStruct->_vScaleSpeed.set(v);
    }
    FVECTOR& getAvatarTargetScale() const {
#line 560 AVATAR_H_PATH
        if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
        return m_pScaleStruct->_vTargetScale;
    }
    FVECTOR& getAvatarScaleSpeed() const {
#line 562 AVATAR_H_PATH
        if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
        return m_pScaleStruct->_vScaleSpeed;
    }
    TIMEQ getAvatarScaleTimeLimit() const {
#line 564 AVATAR_H_PATH
        if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
        return m_pScaleStruct->_nScaleTimeLimit;
    }
    TIMEQ getAvatarScaleTimeCount() const {
#line 566 AVATAR_H_PATH
        if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
        return m_pScaleStruct->_nScaleTimeCount;
    }
    FLOAT getAvatarScaleTimeCountRate() const {
#line 568 AVATAR_H_PATH
        if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
        return getTimeCountRate(m_pScaleStruct->_nScaleTimeLimit, m_pScaleStruct->_nScaleTimeCount);
    }
    BOOL isOverAvatarScaleTimeLimit() const {
#line 574 AVATAR_H_PATH
        if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
        return m_pScaleStruct->_nScaleTimeCount >= m_pScaleStruct->_nScaleTimeLimit;
    }
    BOOL isOverAvatarScaleTimeLimit(INT t) const {
#line 575 AVATAR_H_PATH
        if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
        return m_pScaleStruct->_nScaleTimeCount + t >= m_pScaleStruct->_nScaleTimeLimit;
    }
    void clearAvatarScaleSpeed() {
#line 582 AVATAR_H_PATH
        if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
        m_pScaleStruct->_vScaleSpeed._q = 0;
    }
    void ScaleAvatarEnd();
    void cancelScaleAvatar();
    void clearAvatarScaleTimeCount() {
#line 588 AVATAR_H_PATH
        if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
        m_pScaleStruct->_nScaleTimeCount = 0;
    }
    void countAvatarScaleTime(INT t) {
#line 590 AVATAR_H_PATH
        if (!(m_pScaleStruct)) AssertApp(__FILE__, __LINE__, "m_pScaleStruct", 0);
        m_pScaleStruct->_nScaleTimeCount += t;
    }
    BOOL ScaleAvatarMain();
    void setAvatarRotTimeLimit(TIMEQ nTime);
    void setAvatarRotTimeLimit(FLOAT f) { setAvatarRotTimeLimit(secTQ(f)); }
    void setAvatarStartRot(const FVECTOR& v) {
#line 605 AVATAR_H_PATH
        if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
        m_pRotateStruct->_vStartRot.set(v);
    }
    void setAvatarTargetRot(const FVECTOR& v) {
#line 607 AVATAR_H_PATH
        if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
        m_pRotateStruct->_vTargetRot.set(v);
    }
    void setAvatarRotSpeed(const FVECTOR& v) {
#line 609 AVATAR_H_PATH
        if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
        m_pRotateStruct->_vRotSpeed.set(v);
    }
    USHORT getAvatarRotType() const {
#line 615 AVATAR_H_PATH
        if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
        return m_pRotateStruct->_nRotateType & 0xf;
    }
    TIMEQ getAvatarRotTimeLimit() const {
#line 617 AVATAR_H_PATH
        if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
        return m_pRotateStruct->_nRotTimeLimit;
    }
    FVECTOR& getAvatarTargetRot() const {
#line 619 AVATAR_H_PATH
        if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
        return m_pRotateStruct->_vTargetRot;
    }
    FLOAT getAvatarDirY2Target(const FVECTOR* pvTarget, const FMATRIX* pMatRide);
    FMATRIX* getAvatarRideMatrix() const {
#line 623 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        return m_pMoveStruct->_pMatRide;
    }
    FVECTOR& getAvatarStartRot() const {
#line 625 AVATAR_H_PATH
        if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
        return m_pRotateStruct->_vStartRot;
    }
    FVECTOR& getAvatarRotSpeed() const {
#line 627 AVATAR_H_PATH
        if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
        return m_pRotateStruct->_vRotSpeed;
    }
    BOOL isAvatarRotEternal() const {
#line 633 AVATAR_H_PATH
        if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
        return m_pRotateStruct->_nRotateType & 0x10;
    }
    BOOL isOverAvatarRotTimeLimit() const {
#line 635 AVATAR_H_PATH
        if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
        return !isAvatarRotEternal() && m_pRotateStruct->isOverTimeLimit();
    }
    BOOL isOverAvatarRotTimeLimit(INT t) const {
#line 636 AVATAR_H_PATH
        if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
        return !isAvatarRotEternal() && m_pRotateStruct->isOverTimeLimit(t);
    }
    void clearAvatarRotTimeCount() {
#line 643 AVATAR_H_PATH
        if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
        m_pRotateStruct->_nRotTimeCount = 0;
    }
    void clearAvatarRotSpeed() {
#line 645 AVATAR_H_PATH
        if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
        m_pRotateStruct->_vRotSpeed._q = 0;
    }
    void countAvatarRotTime(INT t) {
#line 647 AVATAR_H_PATH
        if (!(m_pRotateStruct)) AssertApp(__FILE__, __LINE__, "m_pRotateStruct", 0);
        m_pRotateStruct->_nRotTimeCount += t;
    }
    BOOL RotateAvatarMain();
    void RotateAvatarEnd();
    void setRotAhead();
    void setAvatarStartPos(const FVECTOR& v) {
#line 659 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        m_pMoveStruct->_vStartPos.set(v);
        m_pMoveStruct->_vStartPos.w = 1.0f;
    }
    FVECTOR& getAvatarStartPos() const {
#line 662 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        return m_pMoveStruct->_vStartPos;
    }
    FVECTOR& getAvatarTargetPos() const {
#line 665 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        return m_pMoveStruct->_vTargetPos;
    }
    FVECTOR& getAvatarStartPosLocal() const { return m_pMoveStruct->_vStartPosLocal; }
    FVECTOR& getAvatarTargetPosLocal() const { return m_pMoveStruct->_vTargetPosLocal; }
    void setAvatarTargetPos(const FVECTOR& v) {
#line 674 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        m_pMoveStruct->_vTargetPos.set(v);
        m_pMoveStruct->_vTargetPos.w = 1.0f;
    }
    void clearAvatarMoveTimeCount() {
#line 677 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        m_pMoveStruct->_nMoveTimeCount = 0;
    }
    void countAvatarMoveTime(INT t) {
#line 680 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        m_pMoveStruct->_nMoveTimeCount += t;
    }
    void setAvatarMoveTimeLimit(INT t) {
#line 683 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        m_pMoveStruct->_nMoveTimeLimit = t;
    }
    void setAvatarMoveTimeLimit(FLOAT f) { setAvatarMoveTimeLimit(secTQ(f)); }
    void setAvatarSpeed(FLOAT f) {
#line 689 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        m_pMoveStruct->_fSpeed = f * 0.16666667f;
    }
    void calcAvatarMoveSpeed(FVECTOR& vOut);
    USHORT getAvatarMoveType() const {
#line 695 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        return m_pMoveStruct->_nMoveType & 0xf;
    }
    TIMEQ getAvatarMoveTimeLimit() const {
#line 698 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        return m_pMoveStruct->_nMoveTimeLimit;
    }
    FLOAT getAvatarSpeed() const {
#line 701 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        return m_pMoveStruct->_fSpeed;
    }
    BOOL isAvatarMoveWithGravity() const {
#line 704 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        return m_pMoveStruct->_nMoveType & 0x200;
    }
    BOOL isOverAvatarMoveTimeLimit() const {
#line 707 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        return m_pMoveStruct->isOverTimeLimit();
    }
    BOOL isOverAvatarMoveTimeLimit(INT t) const {
#line 708 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        return m_pMoveStruct->isOverTimeLimit(t);
    }
    USHORT getAvatarMoveMotion() const {
#line 711 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        return m_pMoveStruct->_nMoveMotion;
    }
    BOOL updateAvatarPosOnPath(TIMEQ nTime);
    void getAvatarPosOnPath(FVECTOR& vPos) {
#line 717 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        m_pMoveStruct->getPosOnPath(vPos);
    }
    BOOL isAvatarRotAhead() const {
#line 720 AVATAR_H_PATH
        if (!(m_pMoveStruct)) AssertApp(__FILE__, __LINE__, "m_pMoveStruct", 0);
        return !isAvatarRotating() && (m_pMoveStruct->_nMoveType & 0x10) != 0;
    }
    BOOL isArrivingTarget();
    BOOL isArrivingTargetFuture();
    BOOL MoveAvatarMain();
    void MoveAvatarEnd();
    void setAvatarGravity(BOOL bOn);
    FVECTOR* getPathStartPos(const CHAR* pszName) const;
};


} // namespace ATEL

#endif /* GAME_ATEL_AVATAR_H */
