#include "khtypes.h"
#define ENTITY_H_SETFLAG_DIRECT /* MATCH: setEntityFlag on the CEntity base subobject (constructor, InitAvatar) */
#define TASK_H_NO_SLC_OVERFLOW_MSG /* MATCH: the original .rodata has no Task.h container-overflow message */
#include "Game/Atel/Avatar.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Model.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Atel/Atel.h"
#include "Game/Atel/VSME.h"
#include "Game/Movement.h"
#include "System/Graphic/HermitePath.h"
#include "System/Graphic/PathData.h"
#include "System/Debug.h"
#include "System/Math3D.h"
#include "System/Graphic/Gprim.h"
#include "Game/Debug/DebugWork.h"
#include "Game/LockOnManager.h"
#include "include_asm.h"

void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

using namespace ATEL;

namespace ATEL {
struct AvatarMoveParam { // 0x20
    union {
        FVECTOR _vTarget;
        struct {
            FLOAT _fCx;
            FLOAT _fCz;
            FLOAT _fTargetRotY;
        };
        struct {
            CHAR* _pszPathFile;
            USHORT _nMotion;
        };
    };                  /* 0x00 */
    UINT _nType;        /* 0x10 */
    union {
        FLOAT _fTime;
        FLOAT _fSpeed;
    };                  /* 0x14 */
};
struct AvatarRotateParam { // 0x20
    union {
        FVECTOR _vTarget;
        UINT _nTargetID;
    };                  /* 0x00 */
    UINT _nType;        /* 0x10 */
    FLOAT _fTime;       /* 0x14 */
};
struct AvatarMotionParam { // 0xc
    FLOAT _fPlaySpeed;        /* 0x0 */
    INT _iChangeHokanRate;    /* 0x4 */
    SHORT _nMotionID;         /* 0x8 */
    SHORT _nFlag;             /* 0xa */
};
struct AvatarMotionParamEx2 { // 0x14
    UINT _nRsrcID;      /* 0x00 */
    UINT _nInfo;        /* 0x04 */
    UINT _nMotionID;    /* 0x08 */
    FLOAT _fPlaySpeed;  /* 0x0c */
    UINT _nHokanFrame;  /* 0x10 */
};
struct AvatarMotionParamEfx { // 0x1c
    UINT _nRsrcID;      /* 0x00 */
    UINT _nMotionID;    /* 0x04 */
    FLOAT _fPlaySpeed;  /* 0x08 */
    UINT _StartFrame;   /* 0x0c */
    UINT _nHokanFrame;  /* 0x10 */
    UINT _nEfxNo;       /* 0x14 */
    UINT _nFlag;        /* 0x18 */
};
struct AvatarFadeParam { // 0x8
    FLOAT _fTarget;    /* 0x0 */
    UINT _nTime;       /* 0x4 */
};
struct AvatarScaleParam { // 0x20
    FVECTOR _vTarget;  /* 0x00 */
    UINT _nTime;       /* 0x10 */
};
struct AvatarEffectAttach { // 0x10
    char* EffectName;  /* 0x0 */
    INT EfxNo;         /* 0x4 */
    char* BoneName;    /* 0x8 */
    INT Type;          /* 0xc */
};
struct AvatarSetLook { // 0x30
    FVECTOR LookMin;   /* 0x00 */
    FVECTOR LookMax;   /* 0x10 */
    FLOAT Rate;        /* 0x20 */
    FLOAT OffsetY;     /* 0x24 */
    INT TargetID;      /* 0x28 */
    char* BoneName;    /* 0x2c */
};
}
static inline const FVECTOR& _crdWorldPos(const CCoord& crd) { return *(const FVECTOR*)((const BYTE*)&crd + 0xc0); }

extern "C" float atan2f(float, float);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT t);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
#include "System/RsrcDB.h"




extern "C" {
void sceVu0RotMatrix(FMATRIX& m0, const FMATRIX& m1, const FVECTOR& rot);
void sceVu0RotMatrixY(FMATRIX& m0, const FMATRIX& m1, FLOAT ry);
}
static inline void _unitMatrixVu0(FMATRIX& m) {
    asm volatile("vmr32.xyzw $vf1, $vf0\n"
                 "sqc2 $vf0, 0x30(%0)\n"
                 "vmr32.xyzw $vf2, $vf1\n"
                 "sqc2 $vf1, 0x20(%0)\n"
                 "vmr32.xyzw $vf3, $vf2\n"
                 "sqc2 $vf2, 0x10(%0)\n"
                 "sqc2 $vf3, 0x0(%0)\n"
                 :
                 : "r"(&m)
                 : "memory");
}
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
inline void* operator new(size_t, void* p) throw() { return p; }

CAvatar::CAvatar(CTask* pParent)
    : CEntity(pParent, (ObjType::eObjType)4), m_nState(0) {
    m_MainState[0].set(this, &CAvatar::beginStateMain, &CAvatar::stateMain, &CAvatar::stateMainEnd);
    m_MainState[1].set(this, &CAvatar::beginStateMain, &CAvatar::stateSkipMain, NULL);
    changeAvatarMainState(0);
    m_nMotInValidFlag = 0;
    setEntityFlag(0x1000, 0);
    m_pMoveStruct = NULL;
    m_pRotateStruct = NULL;
    m_pScaleStruct = NULL;
    m_pLookStruct = NULL;
    for (INT i = 4; i >= 0; i--) m_pFadeStruct[i] = NULL;
}
/* Parked: 6 lines - the implicit lineY/lineX destructor calls get s5/s4 instead of s4/s5: their address pseudos
   tie on priority (2 refs / 17 insns) and ours numbers lineX first (lreg dump); no body form changed it.
   The draft needs the full classes of Gprim.h (inline ~C3DLine/~CGPrim; was GPRIM_H_FULL_CLASSES, fa94b30^). */
#if 0
ATEL::CAvatar::~CAvatar() {
    delete m_pMoveStruct;
    if (m_pRotateStruct) com_free(m_pRotateStruct);
    if (m_pScaleStruct) com_free(m_pScaleStruct);
    if (m_pLookStruct) com_free(m_pLookStruct);
    for (INT i = 0; i < 5; i++) {
        void* pv = m_pFadeStruct[i];
        if (pv) com_free(pv);
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", _$_Q24ATEL7CAvatar);
#endif
void CAvatar::InitAvatar(INT in_flag) {
    m_pMove->setFlag(0x40, FALSE);
    m_pMove->setFlag(0x200, TRUE);
    enableFade2Path(1, -1);
    setTexAnmSeq(0, "EYE_BLINK");
    if (in_flag == 1) setEntityFlag(1, 0);
    lineX.m_pCrd = &Crd(0);
    lineY.m_pCrd = &Crd(0);
    lineZ.m_pCrd = &Crd(0);
    lineX.m_C0.set(255, 0, 0, 255);
    lineX.m_C1.set(255, 0, 0, 255);
    lineY.m_C0.set(0, 255, 0, 255);
    lineY.m_C1.set(0, 255, 0, 255);
    lineZ.m_C0.set(0, 0, 255, 255);
    lineZ.m_C1.set(0, 0, 255, 255);
    lineX.m_P0.set(0.0f, 0.0f, 0.0f, 1.0f);
    lineX.m_P1.set(1.0f, 0.0f, 0.0f, 1.0f);
    lineY.m_P0.set(0.0f, 0.0f, 0.0f, 1.0f);
    lineY.m_P1.set(0.0f, 1.0f, 0.0f, 1.0f);
    lineZ.m_P0.set(0.0f, 0.0f, 0.0f, 1.0f);
    lineZ.m_P1.set(0.0f, 0.0f, 1.0f, 1.0f);
    if ((UINT)getID() % 10000 == 1007 && m_pMove) {
        m_pMove->setRadius(0.15f);
        m_pMove->setShiftY(0.18f);
    }
    getSysTask(0x20);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
}
void CAvatar::changeAvatarMainState(INT nState) {
    if (!((0 <= nState) && (2 > nState))) AssertApp(__FILE__, 154, "(0 <= nState) && (_MAIN_STATE_MAX > nState)", 0);
    gotoState(m_MainState[nState]);
}
void CAvatar::beginStateMain() {
    if (isAvatarRotating()) setAvatarState(0x40, FALSE);
    if (isAvatarMoving()) setAvatarState(0x30, FALSE);
    if (isAvatarScaling()) setAvatarState(0x80, FALSE);
    if (isAvatarFadeing()) setAvatarState(0x200, FALSE);
    if (isAvatarRotatingFuture()) setAvatarState(0x1000, FALSE);
    if (isAvatarMovingFuture()) setAvatarState(0xc00, FALSE);
    if (isAvatarScalingFuture()) setAvatarState(0x2000, FALSE);
    if (isAvatarFadeingFuture()) setAvatarState(0x8000, FALSE);
}
void CAvatar::stateMain() {
    if (m_nState & 0x10000) {
        FVECTOR v = _crdWorldPos(getAtel().getVSM()->getAvatar(m_pLookStruct->_nTargetID)->Crd(0));
        v.y += m_pLookStruct->_fOffsetY;
        m_pAnim->updateLookPosition(v, 0.0f, 0);
    }
    if (m_pAnim && m_pAnim->isLoop(-1)) setAvatarState(0x4100, TRUE);
    if (isAvatarAnimating() && isMotionEnd()) {
        setAvatarState(0x100, FALSE);
        setAvatarState(0x4000, FALSE);
    }
    if (m_pAnim && m_pAnim->getTotalTqCrd() - m_pAnim->getCurrentTqCrd() <= getFrameRate()) setAvatarState(0x4000, FALSE);
    if (isAvatarRotating() && RotateAvatarMain()) {
        RotateAvatarEnd();
        setAvatarState(0x40, FALSE);
        setAvatarState(0x1000, FALSE);
    }
    if (isAvatarMoving() && MoveAvatarMain()) {
        MoveAvatarEnd();
        setAvatarState(0x10, FALSE);
        setAvatarState(0x400, FALSE);
    }
    if (isAvatarScaling() && ScaleAvatarMain()) {
        ScaleAvatarEnd();
        setAvatarState(0x80, FALSE);
        setAvatarState(0x2000, FALSE);
    }
    if (isAvatarFadeing() && FadeAvatarMain()) {
        setAvatarState(0x200, FALSE);
        setAvatarState(0x8000, FALSE);
    }
}
void CAvatar::stateMainEnd() {}
void CAvatar::stateSkipMain() {}
void CAvatar::updateBefore() {
    FVECTOR vPos;
    if (m_pBodyColl && m_pBodyColl->collideBody(&vPos, NULL, NULL)) {
        FVECTOR v = getPos();
        v.x = vPos.x;
        v.z = vPos.z;
        setPos(v);
    }
}
void CAvatar::updateAfter() {}
void CAvatar::lastUpdate() {
    if (m_pBodyColl) m_pBodyColl->update();
}
CAtel& CAvatar::getAtel() {
    CAtel* pAtel = (CAtel*)getParent();
    if (!(pAtel)) AssertApp(__FILE__, 360, "pAtel", 0);
    return *pAtel;
}
CVSM& CAvatar::getVSM() {
    CVSM* pVSM = getAtel().getVSM();
    if (!(pVSM)) AssertApp(__FILE__, 370, "pVSM", 0);
    return *pVSM;
}
void CAvatar::getNowPosition(FVECTOR& vPos) const { vPos = getPos(); }
void CAvatar::getPrevPosition(FVECTOR& vPos) const { vPos = getOldPos(); }
void CAvatar::getMoveInfo(ETMoveInfo& info) const {}
void CAvatar::getRotation(FVECTOR& vRot) const { vRot = getRot(); }
void CAvatar::getScale(FVECTOR& vScale) const { vScale = CEntity::getScale(); }
void CAvatar::getFade(INT nModel, FLOAT& fFade) const {
    CModel* pModel;
    if (nModel == -1) pModel = m_pModels;
    else pModel = (CModel*)(nModel * sizeof(CModel) + (UINT)m_pModels);
    fFade = pModel->getAlphaScale();
}
FLOAT CAvatar::getDirection() const { return 0.0f; }
INT CAvatar::getTypeID() const { return 0x4000; }
BOOL CAvatar::createMoveStruct() {
    if (m_pMoveStruct) return TRUE;
    m_pMoveStruct = new (com_malloc_d(sizeof(AvatarMoveStruct), __FILE__ "(458)", RET_ADDR())) AvatarMoveStruct;
    if (!m_pMoveStruct) {
        TRACE(" ... Fail to create MoveStructure\n");
        return FALSE;
    }
    return TRUE;
}
BOOL CAvatar::createRotateStruct() {
    if (m_pRotateStruct) return TRUE;
    m_pRotateStruct = new (com_malloc_d(sizeof(AvatarRotateStruct), __FILE__ "(477)", RET_ADDR())) AvatarRotateStruct;
    if (!m_pRotateStruct) {
        TRACE(" ... Fail to create RotateStructure\n");
        return FALSE;
    }
    return TRUE;
}
BOOL CAvatar::createScaleStruct() {
    if (m_pScaleStruct) return TRUE;
    m_pScaleStruct = (AvatarScaleStruct*)com_malloc_d(sizeof(AvatarScaleStruct), __FILE__ "(494)", RET_ADDR());
    if (!m_pScaleStruct) {
        TRACE(" ... Fail to create ScalingStructure\n");
        return FALSE;
    }
    return TRUE;
}
AvatarFadeStruct* CAvatar::createFadeStruct() {
    for (INT i = 0; i < 5; i++) {
        if (!m_pFadeStruct[i]) {
            m_pFadeStruct[i] = (AvatarFadeStruct*)com_malloc_d(sizeof(AvatarFadeStruct), __FILE__ "(514)", RET_ADDR());
            if (!m_pFadeStruct[i]) {
                TRACE(" ... Fail to create FadeStructure\n");
                return NULL;
            }
            return m_pFadeStruct[i];
        }
    }
    return NULL;
}
INT CAvatar::onInit(INT nCmd, void* pParam, INT nID) {
    BYTE* p = (BYTE*)pParam;
    setPos(*(FVECTOR*)p);
    setRot(*(FVECTOR*)(p + 0x10));
    warp(getPos());
    if (*(INT*)(p + 0x20)) {
        m_pMove->enableGravity(TRUE);
        m_pMove->land(5.0f, 1.0f, FALSE);
    }
    updateCoord();
    coordFix();
    return -1;
}
INT CAvatar::onSetActive(INT nCmd, void* pParam, INT nID) { return -1; }
INT CAvatar::onSetVisible(INT nCmd, void* pParam, INT nID) {
    if (nID) setEntityFlag(1, 0x10000);
    else {
        setEntityFlag(0, 1);
        setEntityFlag(0x10000, 0);
    }
    return -1;
}
INT CAvatar::onSetVisibleModel(INT nCmd, void* pParam, INT nID) {
    INT* param = (INT*)pParam;
    if (param[0] == -1) {
        for (int i = 0; i < getModelCount(); i++) {
            if (param[1] == 3) setDispOffFlag(1 << i, 0);
            else setDispOffFlag(0, 1 << i);
        }
    } else {
        if (param[1] == 3) setDispOffFlag(1 << param[0], 0);
        else setDispOffFlag(0, 1 << param[0]);
    }
    return -1;
}
INT CAvatar::onSetGravity(INT nCmd, void* pParam, INT nID) {
    if (nID) m_pMove->enableGravity(TRUE);
    else m_pMove->enableGravity(FALSE);
    return -1;
}
INT CAvatar::onSetCollision(INT nCmd, void* pParam, INT nID) {
    if (!m_pBodyColl) return -1;
    if (nID) {
        m_pBodyColl->onBody(-1);
        m_pBodyColl->on(-1);
    } else {
        m_pBodyColl->offBody(-1);
        m_pBodyColl->off(-1);
    }
    return -1;
}
INT CAvatar::onSetMapCollision(INT nCmd, void* pParam, INT nID) {
    if (nID) m_pMove->setFlag(1, FALSE);
    else m_pMove->setFlag(1, TRUE);
    return -1;
}
INT CAvatar::onCreateLegIK(INT nCmd, void* pParam, INT nID) {
    createLegIK(-1.0f, -1.0f);
    return -1;
}
INT CAvatar::onSetIK(INT nCmd, void* pParam, INT nID) {
    if (nID) setGroundIK(TRUE);
    else setGroundIK(FALSE);
    return -1;
}
INT CAvatar::onSetLockOn(INT nCmd, void* pParam, INT nID) {
    if (nID) setEntityFlag(0x10000, 0);
    else setEntityFlag(0, 0x10000);
    return -1;
}
INT CAvatar::onSetNearClip(INT nCmd, void* pParam, INT nID) {
    if (nID) setEntityFlag(0, 0x1000);
    else setEntityFlag(0x1000, 0);
    return -1;
}
INT CAvatar::onSetPosistion(INT nCmd, void* pParam, INT nID) {
    if (!isAvatarMoving()) setAvatarState(0x30, FALSE);
    FVECTOR v = *(FVECTOR*)pParam;
    setPos(v);
    warp(getPos());
    if (m_pMove->isGravity()) m_pMove->land(5.0f, 1.0f, FALSE);
    updateCoord();
    coordFix();
    return -1;
}
INT CAvatar::onSetPosPathTop(INT nCmd, void* pParam, INT nID) {
    if (!m_pMoveStruct && !createMoveStruct()) return -1;
    FVECTOR* pV = getPathStartPos((const CHAR*)pParam);
    if (!pV) return -1;
    setAvatarTargetPos(*pV);
    return -1;
}
/* Parked: 8 lines - the original reads p->_nType once with lhu for nType and nFlag, keeps the signed slti
   and tests nType twice before setVel; INT nType (lw) is the only form with the original registers (USHORT
   loads move p/CEntity to other s-registers), permuter found nothing. */
#if 0
INT CAvatar::onMove(INT nCmd, void* pParam, INT nID) {
    AvatarMoveParam* p = (AvatarMoveParam*)pParam;
    if (!(p)) AssertApp(__FILE__, 779, "p", 0);
    if (!m_pMoveStruct && !createMoveStruct()) return -1;
    m_nState = ((m_nState | 0x10) & ~0x20) | 0x400;
    FVECTOR vOrigin;
    FVECTOR vVel;
    vOrigin = vVel = getPos();
    setAvatarStartPos(vOrigin);
    m_pMoveStruct->_nMoveType = p->_nType;
    m_pMoveStruct->_nMoveMotion = p->_nMotion;
    INT nType = p->_nType & 0xf;
    USHORT nFlag = p->_nType & 0xfff0;
    switch (nType) {
    case 1:
        if (nFlag & 0x80) {
            FVECTOR* pV = getPathStartPos(p->_pszPathFile);
            if (!pV) return -1;
            m_pMoveStruct->_nMoveMotion = p->_nMotion;
            changeMotion(getAvatarMoveMotion(), 0, FALSE, -1, 0, 0);
            setAvatarTargetPos(*pV);
        } else {
            setAvatarTargetPos(p->_vTarget);
        }
        clearAvatarMoveTimeCount();
        setAvatarMoveTimeLimit(p->_fTime);
        break;
    case 0:
        if (nFlag & 0x80) {
            FVECTOR* pV = getPathStartPos(p->_pszPathFile);
            if (!pV) return -1;
            setAvatarTargetPos(*pV);
        } else {
            setAvatarTargetPos(p->_vTarget);
        }
    case 8:
        setAvatarSpeed(p->_fSpeed);
        break;
    case 7:
        if (!m_pMoveStruct->setupPath(p->_pszPathFile, p->_nMotion)) return -1;
        changeMotion(getAvatarMoveMotion(), 0, FALSE, -1, 0, 0);
        break;
    case 9:
        clearSpeed();
        m_nState &= ~0x10;
        return -1;
    }
    calcAvatarMoveSpeed(vVel);
    if (nType < 3) {
        if (nType == 0) m_pMove->setVel(vVel);
    }
    return -1;
}
#else
/* MATCH: placeholder for the asm onMove's assert string */
inline void _onMoveStringPlaceholder(void* p) {
    if (!(p)) AssertApp(__FILE__, 779, "p", 0);
}
/* MATCH: the parked function's jump table (compiled jump tables of the other switches sit around it in .rodata) */
asm(".rdata\n.align 4\njtbl_0048AF40:\n"
    ".word .L00236FD4\n.word .L00236E44\n.word .L00237160\n.word .L00237160\n.word .L00237160\n.word .L00237160\n.word .L00237160\n.word .L002370C4\n.word .L00237080\n.word .L00237140\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onMove__Q24ATEL7CAvatariPvi);
#endif
void CAvatar::calcAvatarMoveSpeed(FVECTOR& vOut) {
    switch (getAvatarMoveType()) {
    case 1:
    case 5:
    case 6:
        SubVector(vOut, getAvatarTargetPos(), getAvatarStartPos());
        ScaleVector(vOut, vOut, 50.0f / getAvatarMoveTimeLimit());
        vOut.w = 0.0f;
        break;
    case 8: {
        FVECTOR v;
        FMATRIX m;
        _unitMatrixVu0(m);
        v._q = 0;
        v.z = getAvatarSpeed();
        sceVu0RotMatrix(m, m, getRot());
        ApplyMatrix(vOut, m, v);
        vOut.w = 0.0f;
        break;
    }
    case 0: {
        FMATRIX m;
        FVECTOR v;
        _unitMatrixVu0(m);
        sceVu0RotMatrixY(m, m, getAvatarDirY2Target(NULL, NULL));
        v._q = 0;
        v.z = getAvatarSpeed();
        ApplyMatrix(vOut, m, v);
        vOut.w = 0.0f;
        break;
    }
    default:
        vOut._q = 0;
        break;
    }
}
void CAvatar::setAvatarGravity(BOOL bOn) {
    if (bOn) m_pMove->enableGravity(TRUE);
    else m_pMove->enableGravity(FALSE);
}
INT CAvatar::onStopMove(INT nCmd, void* pParam, INT nID) { return -1; }
INT CAvatar::onContinueMove(INT nCmd, void* pParam, INT nID) { return -1; }
void CAvatar::setRotAhead() {
    FVECTOR& vVel = m_pMove->getVel();
    getRot().y = atan2f(vVel.x, vVel.z);
}
BOOL CAvatar::isArrivingTarget() {
    CMovement* pMove = m_pMove;
    FLOAT fPrev = pMove->getVel().x * pMove->getVel().x + pMove->getVel().y * pMove->getVel().y + pMove->getVel().z * pMove->getVel().z;
    FVECTOR v;
    SubVector(v, getAvatarTargetPos(), getPos());
    FLOAT fNext = pMove->getVel().x * v.x + pMove->getVel().y * v.y + pMove->getVel().z * v.z;
    return fNext * fPrev < 0.0f;
}
BOOL CAvatar::isArrivingTargetFuture() {
    FVECTOR vel = m_pMove->getVel();
    ScaleVector(vel, vel, getFrameRate() * 0.020000001f * 2.0f);
    FVECTOR vpos = getPos();
    AddVector(vpos, vpos, vel);
    FLOAT fPrev = vel.x * vel.x + vel.y * vel.y + vel.z * vel.z;
    FVECTOR v;
    SubVector(v, getAvatarTargetPos(), vpos);
    FLOAT fNext = vel.x * v.x + vel.y * v.y + vel.z * v.z;
    return fNext * fPrev < 0.0f;
}
BOOL CAvatar::MoveAvatarMain() {
    BOOL bRes = TRUE;
    TIMEQ nTQ = getFrameRate();
    FVECTOR vPos;
    FVECTOR v;
    switch (getAvatarMoveType()) {
    case 1:
        calcAvatarMoveSpeed(v);
        m_pMove->setVel(v);
    case 5:
    case 6:
        countAvatarMoveTime(nTQ);
        bRes = isOverAvatarMoveTimeLimit();
        if (!bRes) {
            if (isAvatarRotAhead()) setRotAhead();
            if (isOverAvatarMoveTimeLimit(nTQ)) m_nState &= ~0x400;
        }
        break;
    case 0:
        bRes = isArrivingTarget();
        if (!bRes) {
            if (isArrivingTargetFuture()) m_nState &= ~0x400;
            calcAvatarMoveSpeed(v);
            m_pMove->setVel(v);
            if (isAvatarRotAhead()) setRotAhead();
        }
        break;
    case 8:
        calcAvatarMoveSpeed(v);
        m_pMove->setVel(v);
        bRes = FALSE;
        break;
    case 7: {
        bRes = updateAvatarPosOnPath(nTQ);
        FVECTOR v;
        getAvatarPosOnPath(v);
        if (m_pMove->isGravity()) v.y = getPos().y;
        if (isAvatarRotAhead()) {
            FVECTOR vVel;
            SubVector(vVel, v, getPos());
            getRot().y = atan2f(vVel.x, vVel.z);
        }
        setPos(v);
        break;
    }
    }
    if ((UINT)getID() % 10000 == 1007) land(0.25f, 1.0f, FALSE);
    return bRes;
}
BOOL CAvatar::updateAvatarPosOnPath(TIMEQ nTime) {
    BOOL bRet = TRUE;
    if (m_pMoveStruct) bRet = m_pMoveStruct->updatePosOnPath(nTime * 0.02f);
    return bRet;
}
void CAvatar::MoveAvatarEnd() { clearSpeed(); }
static inline void _shortestRad(FLOAT& f, FLOAT fStart) {
    FLOAT fDist = f - fStart;
    if (3.1415927f < fDist) f = f - 6.2831855f;
    else if (fDist < -3.1415927f) f = f + 6.2831855f;
}
INT CAvatar::onRotate(INT nCmd, void* pParam, INT nID) {
    AvatarRotateParam* p = (AvatarRotateParam*)pParam;
    if (!(p)) AssertApp(__FILE__, 1232, "p", 0);
    if (!m_pRotateStruct && !createRotateStruct()) return -1;
    m_pRotateStruct->_nRotateType = p->_nType;
    USHORT nType = getAvatarRotType();
    if (nType == 4) {
        if (p->_fTime < 0.01f) {
            m_pRotateStruct->_nRotIntervalTime = 0;
            setAvatarState(0x40, FALSE);
            setAvatarState(0x1000, FALSE);
        } else {
            m_pRotateStruct->_nRotIntervalTimeCount = 0;
            m_pRotateStruct->_nRotIntervalTime = secTQ(p->_fTime);
            m_pRotateStruct->_nRotTargetRegularly = p->_nTargetID;
        }
        return 1;
    }
    setAvatarState(0x1040, TRUE);
    clearAvatarRotTimeCount();
    setAvatarRotTimeLimit(p->_fTime);
    FVECTOR vRot;
    getRotation(vRot);
    setAvatarStartRot(vRot);
    BOOL bPos2Rot = FALSE;
    BOOL bShortest = TRUE;
    FVECTOR vTargetPos;
    switch (nType) {
    case 1:
    case 7:
    case 8:
        AddVector(vRot, p->_vTarget, vRot);
        bShortest = FALSE;
        break;
    case 0:
    case 5:
    case 6:
        vRot = p->_vTarget;
        break;
    case 9:
    case 10:
        bShortest = FALSE;
    case 2: {
        CAvatar* pAvatar = getAtel().getVSM()->getAvatar(p->_nTargetID);
        if (pAvatar && pAvatar != this) {
            pAvatar->getNowPosition(vTargetPos);
            bPos2Rot = TRUE;
        }
        break;
    }
    case 3:
        vTargetPos = p->_vTarget;
        bPos2Rot = TRUE;
        break;
    }
    if (bPos2Rot) {
        vRot._q = 0;
        vRot.y = getAvatarDirY2Target(&vTargetPos, NULL);
    }
    if (bShortest) {
        FVECTOR& vStart = getAvatarStartRot();
        _shortestRad(vRot.x, vStart.x);
        _shortestRad(vRot.y, vStart.y);
        _shortestRad(vRot.z, vStart.z);
    }
    setAvatarTargetRot(vRot);
    FVECTOR vVel;
    if (nType == 7 || nType == 5 || nType == 9) {
        SubVector(vVel, getAvatarStartRot(), getAvatarTargetRot());
        if (vVel.y > 0.0f) vVel.y = -vVel.y;
        else vVel.y = -6.2831855f - vVel.y;
    } else if (nType == 8 || nType == 6 || nType == 10) {
        SubVector(vVel, getAvatarStartRot(), getAvatarTargetRot());
        if (vVel.y < 0.0f) vVel.y = -vVel.y;
        else vVel.y = 6.2831855f - vVel.y;
    } else {
        SubVector(vVel, getAvatarTargetRot(), getAvatarStartRot());
    }
    if (getAvatarRotTimeLimit()) ScaleVector(vVel, vVel, 1.0f / getAvatarRotTimeLimit());
    setAvatarRotSpeed(vVel);
    setAvatarState(0x1040, TRUE);
    return -1;
}
void CAvatar::setAvatarRotTimeLimit(TIMEQ nTime) {
    if (!(m_pRotateStruct)) AssertApp(__FILE__, 1381, "m_pRotateStruct", 0);
    BOOL bEternal = FALSE;
    if (nTime < 0) {
        bEternal = TRUE;
        nTime = -nTime;
    }
    m_pRotateStruct->_nRotTimeLimit = nTime;
    AvatarRotateStruct* pRot = m_pRotateStruct;
    if (bEternal) pRot->_nRotateType |= 0x10;
    else pRot->_nRotateType &= ~0x10;
}
void roundPI(FVECTOR& vInOut, INT nInClip);
/* Parked: 8 lines - the first isOverAvatarRotTimeLimit() comes out as xori; the original builds bRes with
   li 1 + movz (its weak copy is xori too). Inline/caller forms and 8 min of permuter (9k candidates) did not
   change it. The VU0 add/clip block (FVECTOR::addRot in the ref) is written out here: through an inline
   parameter the vRotSpeed address lands in v1 instead of sp. */
#if 0
BOOL CAvatar::RotateAvatarMain() {
    BOOL bRes;
    TIMEQ nTQ = getFrameRate();
    countAvatarRotTime(nTQ);
    bRes = isOverAvatarRotTimeLimit();
    if (!bRes) {
        FVECTOR& vRot = getRot();
        FVECTOR vRotSpeed;
        ScaleVectorXYZ(vRotSpeed, getAvatarRotSpeed(), nTQ);
        {
            FLOAT fPI = 3.1415927f;
            INT nFlag;
            __asm__ volatile("qmtc2.ni %0, $vf3\n"
                             "vmr32.w $vf3, $vf3\n" : : "r"(fPI));
            __asm__ volatile("lqc2 $vf1, 0x0(%1)\n"
                             "lqc2 $vf2, 0x0(%2)\n"
                             "vadd.xyz $vf1, $vf1, $vf2\n"
                             "vclipw.xyz $vf1, $vf3w\n"
                             "vnop\n"
                             "vnop\n"
                             "vnop\n"
                             "sqc2 $vf1, 0x0(%1)\n"
                             "vnop\n"
                             "cfc2.ni %0, $vi18\n"
                             : "=r"(nFlag) : "r"(&vRot), "r"(&vRotSpeed) : "memory");
            if (nFlag & 0x3f) roundPI(vRot, nFlag);
        }
        if (isOverAvatarRotTimeLimit(nTQ)) setAvatarState(0x1000, FALSE);
    }
    return bRes;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", RotateAvatarMain__Q24ATEL7CAvatar);
#endif
static inline FLOAT _wrapRad(FLOAT f) {
    if (3.1415927f < f) {
        f -= (INT)(f / 6.2831855f) * 6.2831855f;
        if (3.1415927f < f) f -= 6.2831855f;
    } else if (f < -3.1415927f) {
        f -= (INT)(f / 6.2831855f) * 6.2831855f;
        if (f < -3.1415927f) f += 6.2831855f;
    }
    return f;
}
void CAvatar::RotateAvatarEnd() {
    clearAvatarRotSpeed();
    if (isAvatarRotEternal()) return;
    FVECTOR& v = getRot();
    v.x = _wrapRad(getAvatarTargetRot().x);
    v.y = _wrapRad(getAvatarTargetRot().y);
    v.z = _wrapRad(getAvatarTargetRot().z);
}
INT CAvatar::onSetDirection(INT nCmd, void* pParam, INT nID) { return -1; }
INT CAvatar::onFadeing(INT nCmd, void* pParam, INT nID) {
    AvatarFadeParam* p = (AvatarFadeParam*)pParam;
    AvatarFadeStruct* pFadeStruct;
    FLOAT alp;
    FLOAT f;
    if (!(p)) AssertApp(__FILE__, 1468, "p", 0);
    pFadeStruct = createFadeStruct();
    if (!pFadeStruct) return -1;
    pFadeStruct->_ModelNo = nID;
    pFadeStruct->_fTargetFade = p->_fTarget;
    getFade(nID, alp);
    if (p->_fTarget == 1.0f && alp == 1.0f) {
        if (pFadeStruct->_ModelNo == -1) setModelAlphaScale(0.0f, -1);
        else m_pModels[pFadeStruct->_ModelNo].setAlphaScale(0.0f);
    }
    getFade(pFadeStruct->_ModelNo, f);
    INT t = p->_nTime * 50;
    FLOAT fRate = 1.0f / t;
    f = (p->_fTarget - f) * fRate;
    setAvatarFadeSpeed(pFadeStruct, f);
    clearAvatarFadeTimeCount(pFadeStruct);
    setAvatarFadeTimeLimit(pFadeStruct, t);
    setAvatarState(0x200, TRUE);
    return -1;
}
BOOL CAvatar::FadeAvatarMain() {
    TIMEQ nTQ = getFrameRate();
    for (INT i = 0; i < 5; i++) {
        if (m_pFadeStruct[i]) {
            countAvatarFadeTime(m_pFadeStruct[i], nTQ);
            if (!isOverAvatarFadeTimeLimit(m_pFadeStruct[i])) {
                if (isOverAvatarFadeTimeLimit(m_pFadeStruct[i], nTQ)) setAvatarState(0x8000, FALSE);
                FLOAT fFadeSpeed = getAvatarFadeSpeed(m_pFadeStruct[i]) * nTQ;
                FLOAT fFade;
                if (m_pFadeStruct[i]->_ModelNo == -1) {
                    getFade(-1, fFade);
                    fFade += fFadeSpeed;
                    setModelAlphaScale(fFade, -1);
                } else {
                    fFade = ((CModel*)(m_pFadeStruct[i]->_ModelNo * sizeof(CModel) + (UINT)m_pModels))->getAlphaScale() + fFadeSpeed;
                    m_pModels[m_pFadeStruct[i]->_ModelNo].setAlphaScale(fFade);
                }
            } else {
                if (m_pFadeStruct[i]->_ModelNo == -1) setModelAlphaScale(m_pFadeStruct[i]->_fTargetFade, -1);
                else m_pModels[m_pFadeStruct[i]->_ModelNo].setAlphaScale(m_pFadeStruct[i]->_fTargetFade);
                com_free(m_pFadeStruct[i]);
                m_pFadeStruct[i] = NULL;
            }
        }
    }
    return FALSE;
}
void CAvatar::FadeAvatarEnd() {}
void CAvatar::cancelFadeAvatar() {}
INT CAvatar::onScaling(INT nCmd, void* pParam, INT nID) {
    AvatarScaleParam* p = (AvatarScaleParam*)pParam;
    FVECTOR v;
    if (!(p)) AssertApp(__FILE__, 1607, "p", 0);
    if (!m_pScaleStruct && !createScaleStruct()) return -1;
    setAvatarTargetScale(p->_vTarget);
    getScale(v);
    INT f = p->_nTime * 50;
    FLOAT fRate = 1.0f / f;
    SubVector(v, p->_vTarget, v);
    ScaleVector(v, v, fRate);
    setAvatarScaleSpeed(v);
    clearAvatarScaleTimeCount();
    setAvatarScaleTimeLimit(f);
    setAvatarState(0x80, TRUE);
    return -1;
}
BOOL CAvatar::ScaleAvatarMain() {
    TIMEQ nTQ = getFrameRate();
    countAvatarScaleTime(nTQ);
    BOOL bOver = isOverAvatarScaleTimeLimit();
    if (!bOver) {
        FVECTOR vScale;
        FVECTOR vScaleSpeed;
        getScale(vScale);
        ScaleVectorXYZ(vScaleSpeed, getAvatarScaleSpeed(), nTQ);
        AddVector(vScale, vScale, vScaleSpeed);
        setScale(vScale);
        if (isOverAvatarScaleTimeLimit(nTQ)) setAvatarState(0x2000, FALSE);
    } else {
        setScale(m_pScaleStruct->_vTargetScale);
    }
    return bOver;
}
void CAvatar::ScaleAvatarEnd() { clearAvatarScaleSpeed(); }
void CAvatar::cancelScaleAvatar() { ScaleAvatarEnd(); }
void CAvatar::TexAnimTalk(INT in_MotID) {
    char* pTalkName = "LIP_TALK";
    CEntity* pEntity = this;
    switch ((UINT)pEntity->getID() % 10000) {
    case 307:
    case 1001:
        break;
    case 2:
        if (in_MotID == 5) pEntity->setTexAnmSeq(1, pTalkName);
        break;
    case 9:
        if (in_MotID == 6) pEntity->setTexAnmSeq(1, pTalkName);
        break;
    case 106:
        if (in_MotID == 9) pEntity->setTexAnmSeq(1, pTalkName);
        break;
    case 205:
        if (in_MotID == 10) pEntity->setTexAnmSeq(1, pTalkName);
        break;
    case 805:
        if (in_MotID == 5) pEntity->setTexAnmSeq(1, pTalkName);
        break;
    case 1206:
        if (in_MotID == 9) pEntity->setTexAnmSeq(1, pTalkName);
        break;
    case 210:
    case 1205:
    case 1207:
        if (in_MotID == 7) pEntity->setTexAnmSeq(1, pTalkName);
        break;
    default:
        if (in_MotID == 4) {
            TRACE("ATEL:setTexAnmSeq:%d, %s\n", 1, pTalkName);
            setTexAnmSeq(1, pTalkName);
        }
        break;
    }
}
INT CAvatar::onPlayMotion(INT nCmd, void* pParam, INT nID) {
    AvatarMotionParam* p = (AvatarMotionParam*)pParam;
    if (!p) {
        changeMotion(nID, 0, FALSE, -1, 0, 0);
    } else {
        if (m_nMotInValidFlag) {
            changeMotion(p->_nMotionID, 0, p->_nFlag & 1, 0, 0, 0);
            m_pAnim->setSpeed(p->_fPlaySpeed);
            m_pAnim->clearFlag1(3);
            setAvatarState(0x4100, TRUE);
            m_pAnim->setZeroHokan(p->_nFlag & 4);
        } else {
            changeMotion(p->_nMotionID, 0, p->_nFlag & 1, -1, 0, 0);
            m_pAnim->setSpeed(p->_fPlaySpeed);
            setAvatarState(0x4100, TRUE);
            m_pAnim->setZeroHokan(p->_nFlag & 4);
        }
    }
    TexAnimTalk(p->_nMotionID);
    return -1;
}
INT CAvatar::onPlayMotionDx(INT nCmd, void* pParam, INT nID) {
    AvatarMotionParam* p = (AvatarMotionParam*)pParam;
    if (!p) {
        changeMotion(nID, 0, FALSE, -1, 0, 0);
    } else if (m_pAnim) {
        changeMotion(p->_nMotionID, 0, p->_nFlag & 1, p->_iChangeHokanRate * 50, 0, 0);
        m_pAnim->setSpeed(p->_fPlaySpeed);
        m_pAnim->clearFlag1(3);
        setAvatarState(0x4100, TRUE);
        m_pAnim->setZeroHokan(p->_nFlag & 4);
    }
    TexAnimTalk(p->_nMotionID);
    return -1;
}
INT CAvatar::onPlayMotionEx(INT nCmd, void* pParam, INT nID) {
    AvatarMotionParamEx2* p = (AvatarMotionParamEx2*)pParam;
    if (!p) {
        changeMotion(nID, 0, FALSE, -1, 0, 0);
    } else {
        changeMotionEx(p->_nRsrcID, p->_nMotionID, 0, p->_nInfo & 1, -1, 0, 0);
        m_pAnim->setSpeed(p->_fPlaySpeed);
        setAvatarState(0x4100, TRUE);
        m_pAnim->setZeroHokan(p->_nInfo & 4);
    }
    return -1;
}
INT CAvatar::onPlayMotionEfx(INT nCmd, void* pParam, INT nID) {
    AvatarMotionParamEfx* p = (AvatarMotionParamEfx*)pParam;
    if (!p) {
        changeMotion(nID, 0, FALSE, -1, 0, 0);
    } else {
        if (p->_nRsrcID) {
            changeMotionEx(p->_nRsrcID, p->_nMotionID, 0, p->_nFlag & 1, p->_nHokanFrame * 50, p->_nEfxNo, 0);
            m_pAnim->setSpeed(p->_fPlaySpeed);
            m_pAnim->setZeroHokan(p->_nFlag & 4);
            m_pAnim->setMotion(p->_StartFrame * 50, 0, 0);
            setAvatarState(0x4100, TRUE);
        } else {
            changeMotion(p->_nMotionID, 0, p->_nFlag & 1, p->_nHokanFrame * 50, p->_nEfxNo, 0);
            m_pAnim->setSpeed(p->_fPlaySpeed);
            m_pAnim->setZeroHokan(p->_nFlag & 4);
            m_pAnim->setMotion(p->_StartFrame * 50, 0, 0);
            setAvatarState(0x4100, TRUE);
        }
    }
    TexAnimTalk(p->_nMotionID);
    return -1;
}
INT CAvatar::onPlayMotionExDx(INT nCmd, void* pParam, INT nID) {
    AvatarMotionParamEx2* p = (AvatarMotionParamEx2*)pParam;
    if (!p) {
        changeMotion(nID, 0, FALSE, -1, 0, 0);
    } else {
        changeMotionEx(p->_nRsrcID, p->_nMotionID, 0, p->_nInfo & 1, p->_nHokanFrame * 50, 0, 0);
        m_pAnim->setSpeed(p->_fPlaySpeed);
        m_pAnim->clearFlag1(3);
        setAvatarState(0x4100, TRUE);
        m_pAnim->setZeroHokan(p->_nInfo & 4);
    }
    TexAnimTalk(p->_nMotionID);
    return -1;
}
INT CAvatar::onSetMotionFrame(INT nCmd, void* pParam, INT nID) {
    if (m_pAnim) {
        m_pAnim->setMotion(*(INT*)pParam * 50, 0, FALSE);
        m_pAnim->clearFlag1(3);
        setAvatarState(0x4100, TRUE);
    }
    return -1;
}
INT CAvatar::onSetMakeUpMotionInValid(INT nCmd, void* pParam, INT nID) {
    if (nID) m_nMotInValidFlag = 1;
    else m_nMotInValidFlag = 0;
    return -1;
}
INT CAvatar::onRemoveAvatar(INT nCmd, void* pParam, INT nID) {
    CRsrcDB::removeRsrc(getID());
    out();
    return -1;
}
INT CAvatar::onAvatarStop(INT nCmd, void* pParam, INT nID) {
    changeAvatarMainState(1);
    return -1;
}
INT CAvatar::onAvatarStart(INT nCmd, void* pParam, INT nID) {
    changeAvatarMainState(0);
    return -1;
}
INT CAvatar::onAvatarAttach(INT nCmd, void* pParam, INT nID) {
    CAvatar* pTarget = getAtel().getVSM()->getAvatar(nID);
    CCoord& crd = Crd(0);
    *(CCoord**)((BYTE*)&crd + 0x4c) = &pTarget->Crd(pTarget->getBoneIndex((const CHAR*)pParam, 0));
    return -1;
}
/* Parked: 4 lines - the original stores rot's zero quad (por/sq) first, before the callee saves; ours after
   the p->Type load. Init forms/orders and 6 min of permuter (8.8k candidates) did not change it. */
#if 0
INT CAvatar::onAvatarAttachEffect(INT nCmd, void* pParam, INT nID) {
    AvatarEffectAttach* p = (AvatarEffectAttach*)pParam;
    FVECTOR rot;
    rot.init();
    switch (p->Type) {
    case 0:
        break;
    case 1:
        rot.z = -1.5707964f;
        break;
    case 2:
        rot.set(0.0f, 4.712389f, 0.0f, 1.0f);
        Math3D::LimitPIVector(rot);
        break;
    }
    return CEffectMan::attachEffect(p->EffectName, p->EfxNo, (ObjType::eObjType)13, &Crd(getBoneIndex(p->BoneName, 0)), NULL, &rot, 0.0f);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onAvatarAttachEffect__Q24ATEL7CAvatariPvi);
#endif
INT CAvatar::onAvatarSetLookPosition(INT nCmd, void* pParam, INT nID) {
    AvatarSetLook* p = (AvatarSetLook*)pParam;
    FVECTOR v = _crdWorldPos(getAtel().getVSM()->getAvatar(p->TargetID)->Crd(0));
    if (!m_pLookStruct) m_pLookStruct = (AvatarLookStruct*)com_malloc_d(sizeof(AvatarLookStruct), __FILE__ "(2058)", RET_ADDR());
    m_pLookStruct->_nTargetID = p->TargetID;
    m_pLookStruct->_fOffsetY = p->OffsetY;
    v.y += p->OffsetY;
    m_pAnim->setLookPosition(v, p->LookMax, p->LookMin, getBoneIndex(p->BoneName, 0), p->Rate, 0);
    return -1;
}
INT CAvatar::onAvatarSetLookUpdatePosition(INT nCmd, void* pParam, INT nID) {
    AvatarSetLook* p = (AvatarSetLook*)pParam;
    FVECTOR v = _crdWorldPos(getAtel().getVSM()->getAvatar(p->TargetID)->Crd(0));
    if (!m_pLookStruct) m_pLookStruct = (AvatarLookStruct*)com_malloc_d(sizeof(AvatarLookStruct), __FILE__ "(2079)", RET_ADDR());
    m_pLookStruct->_nTargetID = p->TargetID;
    m_pLookStruct->_fOffsetY = p->OffsetY;
    v.y += p->OffsetY;
    m_pAnim->updateLookPosition(v, p->Rate, 0);
    return -1;
}
INT CAvatar::onAvatarSetLookAngle(INT nCmd, void* pParam, INT nID) {
    BYTE* p = (BYTE*)pParam;
    m_pAnim->setLookPosition(*(FVECTOR*)(p + 0x20), *(FVECTOR*)(p + 0x10), *(FVECTOR*)p, getBoneIndex(*(const CHAR**)(p + 0x34), 0), *(FLOAT*)(p + 0x30), 1);
    return -1;
}
INT CAvatar::onAvatarSetLookUpDateAngle(INT nCmd, void* pParam, INT nID) {
    m_pAnim->updateLookPosition(*(FVECTOR*)((BYTE*)pParam + 0x20), *(FLOAT*)((BYTE*)pParam + 0x30), 1);
    return -1;
}
INT CAvatar::onAvatarLookStart(INT nCmd, void* pParam, INT nID) {
    setAvatarState(0x10000, TRUE);
    return -1;
}
INT CAvatar::onAvatarLookStop(INT nCmd, void* pParam, INT nID) {
    setAvatarState(0x10000, FALSE);
    return -1;
}
INT CAvatar::onAvatarLookEnd(INT nCmd, void* pParam, INT nID) {
    m_pAnim->endLookPosition(*(FLOAT*)pParam);
    return -1;
}
INT CAvatar::onAvatarChangeTexture(INT nCmd, void* pParam, INT nID) {
    const CHAR** ppName = (const CHAR**)pParam;
    m_pModels->changeTexture(ppName[0], ppName[1]);
    return -1;
}
INT CAvatar::onSetTexAnimSeq(INT nCmd, void* pParam, INT nID) {
    setTexAnmSeq(nID, (const CHAR*)pParam);
    return -1;
}
INT CAvatar::onEnableNotSemiModel(INT nCmd, void* pParam, INT nID) {
    enableNotSemiModel(*(INT*)pParam, nID);
    return -1;
}
FLOAT CAvatar::getAvatarDirY2Target(const FVECTOR* pvTarget, const FMATRIX* pMatRide) {
    FVECTOR v;
    FVECTOR vTarget;
    getNowPosition(v);
    if (pMatRide && pMatRide == getAvatarRideMatrix()) {
        if (!pvTarget) {
            ApplyMatrix(vTarget, *pMatRide, getAvatarTargetPosLocal());
            pvTarget = &vTarget;
        }
    } else if (!pvTarget) {
        pvTarget = &getAvatarTargetPos();
    }
    SubVector(v, *pvTarget, v);
    return atan2f(v.x, v.z);
}
FVECTOR* CAvatar::getPathStartPos(const CHAR* pszName) const {
    FVECTOR* pPos = NULL;
    BYTE* pPTD = CRsrcDB::getRsrcPTD(pszName);
    if (pPTD) {
        if (*(USHORT*)(pPTD + 0xe)) {
            if (*(USHORT*)(pPTD + 0x10)) pPos = (FVECTOR*)(pPTD + 0x20);
        }
    }
    return pPos;
}
void CAvatar::disp() {
    CEntity::disp();
    if (DebugWork::g_dw.event.iAtelCoord) {
        lineX.CGPrim::disp();
        lineY.CGPrim::disp();
        lineZ.CGPrim::disp();
    }
}
AvatarMoveStruct::~AvatarMoveStruct() {
    delete _pPathData;
    delete _pHermiteHelper;
}
BOOL AvatarMoveStruct::setupPath(const CHAR* pszPath, USHORT nMotion) {
    delete _pPathData;
    _pPathData = new (com_malloc_d(sizeof(CPathData), __FILE__ "(2268)", RET_ADDR())) CPathData(pszPath);
    _nMoveMotion = 0;
    if (_pPathData) {
        if (!_pHermiteHelper) {
            _pHermiteHelper = new (com_malloc_d(sizeof(CHermitePath), __FILE__ "(2274)", RET_ADDR())) CHermitePath;
            if (!_pHermiteHelper) goto end;
        }
        {
            PATH_PARAM param;
            _pPathData->getPathParam(0, param);
            _pHermiteHelper->makePathFromPathVector(_pPathData->getPathVectorData(0));
        }
        _nMoveMotion = nMotion;
    } else {
        delete _pHermiteHelper;
        _pHermiteHelper = NULL;
    }
end:
    return _pHermiteHelper != NULL;
}
void AvatarMoveStruct::getPosOnPath(FVECTOR& vPos) {
    if (_pHermiteHelper) _pHermiteHelper->getNowPos(vPos);
}
BOOL AvatarMoveStruct::updatePosOnPath(FLOAT fTime) {
    BOOL bRet = TRUE;
    if (_pHermiteHelper) bRet = _pHermiteHelper->update(fTime);
    return bRet;
}
// SOURCE DATA: .rodata
