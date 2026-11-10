#define GAME_BULLET_BULLET_CPP /* MATCH: unit gate (its header gives this unit its own parts); without it the image differs (tested 2026-11-02) */
#include "System/Task.h"
#include "System/TaskMan.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in GimmickFountain); which header it
 * comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "Game/Entity.h"
#include "System/Fsm.h" // before Gprim.h: CState's vtable copy follows CVector<G_LINE>'s in .rodata
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Bullet/Bullet.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Collision/CollDisp.h"
#include "Game/Collision/CollAttack.h"
#include "Game/Collision/CollMap.h"
#include "Game/Movement.h"
#include "Game/LockOnManager.h"
#include "System/Coord.h"
#include "System/Debug.h"
#include "System/Math3D.h"
#include "Game/Debug/DebugWork.h"
#include "include_asm.h"
#include "System/HeapMan.h"




extern "C" {
float sinf(float x);
float cosf(float x);
float atan2f(float y, float x);
}
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1);




















static inline void _apply33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1) { ApplyMatrix33(v0, m0, v1); }

static inline BOOL _collDispRef() {
    BOOL& r = CCollDisp::m_fgDisp;
    return r;
}

static inline void _setNewState(CState*& p, CState& NewState) { p = &NewState; }

void BULLET_PARAM::init(INT inBulletID, INT inGroupID) {
    iBulletID = inBulletID;
    iGroupID = inGroupID;
    iMemberID = 0;
    CollKind = COLL_KIND_ESHOT;
    vPos.init();
    vRot.init();
    fSpeed = 0.0f;
    fMaxDist = 30.0f;
    tqExplodeTime = 0;
    tqHomingTime = 0;
    tqHomingTime2 = 0;
    hHomingTarget = 0;
    fHomingRadius = 30.0f;
    fHomingRate = 1.0f;
    vHomingPos.init();
    uiFlag = 0;
    AtkKind = 1;
    AtkAttr = 0;
    iAttackPoint = 0;
    uiAttackID = 0;
    fCollRadius = 0.5f;
    fShiftY = 0.0f;
    CollType = COLL_TYPE_SPHERE;
    fChangeRadius = 0.0f;
    fCollOffsetY = 0.0f;
    tqCollAtkTime = 0;
    fScale = 1.0f;
    tqFadeTime = 1500;
    hBoundTarget = 0;
    iBoundCount = -1;
    fEffectBulletSkipFrame = 0.0f;
    pEffectTbl = NULL;
    pOffsetTbl = NULL;
    pRotTbl = NULL;
    iExtraEffectNum = 0;
    pExtraEffectTbl = NULL;
    pExplodeEffectTbl = NULL;
    pShadow = NULL;
}

CBullet::CBullet(CTask* inParent)
    : CEntity(inParent, (ObjType::eObjType)5), m_hOwner(0), m_hEffectHandle(0), m_iFileID(-1), m_iSEID(-1),
      m_hSEHandle(0), m_hEffectShadow(0), m_iExtraEffectHandleNum(0), m_pExtraEffectHandle(NULL),
      m_hHitEnemyHandle(0), m_pAttack(NULL) {}

CBullet::~CBullet() {
    INT i;
    delete m_pAttack;
    m_pAttack = NULL;
    CEffectMan::killEffect(m_hEffectHandle, 0.0f);
    CEffectMan::killEffect(m_hEffectShadow, 0.0f);
    fadeoutSE(m_hSEHandle, m_iFileID, m_iSEID, 16, 0);
    if (m_iExtraEffectHandleNum > 0) {
        for (i = 0; i < m_iExtraEffectHandleNum; i++) {
            CEffectMan::killEffect(m_pExtraEffectHandle[i], 0.0f);
        }
        if (m_pExtraEffectHandle) delete m_pExtraEffectHandle;
        m_pExtraEffectHandle = NULL;
    }
    delete m_pCollLines;
    m_pCollLines = NULL;
    TRACE("class CBullet destructor\n");
}

INT CBullet::getTypeID() const { return 0x1007; }

#line 148
BOOL CBullet::setup(const BULLET_PARAM& inParam) {
    FVECTOR pos;
    pos = inParam.vPos;
    m_uiFlag = inParam.uiFlag;
    if (inParam.iGroupID == 3) {
        m_pMove = new KH_AT("(149)") CMovement(this);
        if (!m_pMove) {
            return FALSE;
        }
        addCoord(1);
        m_pMove->setCollKind((COLL_KIND)inParam.CollKind);
        m_pMove->setRadius(inParam.fCollRadius);
        m_pMove->setShiftY(inParam.fShiftY);
        warp(pos);
        setStartPos(pos);
        setRot(inParam.vRot);
        updateCoord();
        coordFix();
        if (inParam.pShadow) {
            m_hEffectShadow = setEffect(inParam.pShadow->pEffectName, inParam.pShadow->iNo, &pos, NULL, 0.0f);
            updateShadow();
        }
        BULLET_EFFECT_INFO* info;
        if (inParam.pEffectTbl && (info = &inParam.pEffectTbl[3])->pEffectName) {
            FVECTOR* offset = inParam.pOffsetTbl ? &inParam.pOffsetTbl[3] : NULL;
            FVECTOR* rot = inParam.pRotTbl ? &inParam.pRotTbl[3] : NULL;
            m_hEffectHandle = attachEffect(info->pEffectName, info->iNo, &Crd(0), offset, rot, 0.0f);
            CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
            if (effect) {
                if (inParam.isAttachNoRot()) {
                    effect->setAttachNoRot(TRUE);
                }
                effect->setScale(inParam.fScale, inParam.fScale, inParam.fScale);
                effect->setSkipFrame(inParam.fEffectBulletSkipFrame);
            }
        }
    } else {
        if (!CEntity::initalize(inParam.iGroupID, inParam.iMemberID)) {
            return FALSE;
        }
        m_pMove->setCollKind((COLL_KIND)inParam.CollKind);
        m_pMove->setRadius(inParam.fCollRadius);
        m_pMove->setShiftY(inParam.fShiftY);
        warp(pos);
        setStartPos(pos);
        setRot(inParam.vRot);
        updateCoord();
        coordFix();
        if (inParam.pShadow) {
            m_hEffectShadow = setEffect(inParam.pShadow->pEffectName, inParam.pShadow->iNo, &pos, NULL, 0.0f);
            updateShadow();
        }
    }
    if (inParam.pExtraEffectTbl) {
        m_iExtraEffectHandleNum = inParam.iExtraEffectNum;
        m_pExtraEffectHandle =
            new KH_AT("(233)") HANDLE[inParam.iExtraEffectNum];
        if (!m_pExtraEffectHandle) {
            return FALSE;
        }
        if (isExtraFullAdjust()) {
            FVECTOR* offset = inParam.pOffsetTbl ? &inParam.pOffsetTbl[3] : NULL;
            FVECTOR* rot = inParam.pRotTbl ? &inParam.pRotTbl[3] : NULL;
            BULLET_EFFECT_INFO* info = &inParam.pExtraEffectTbl[3];
            if (info->pEffectName) {
                m_pExtraEffectHandle[0] = attachEffect(info->pEffectName, info->iNo, &Crd(0), offset, rot, 0.0f);
            }
        } else {
            FVECTOR pos;
            INT i;
            pos.init();
            for (i = 0; i < inParam.iExtraEffectNum; i++) {
                BULLET_EFFECT_INFO* info = &inParam.pExtraEffectTbl[i];
                if (info->pEffectName) {
                    m_pExtraEffectHandle[i] = attachEffect(info->pEffectName, info->iNo, &Crd(0), &pos, NULL, 0.0f);
                }
            }
        }
    }
    setTimer(inParam.tqHomingTime);
    m_pAttack = new KH_AT("(279)") CCollAttack;
    if (!m_pAttack) {
        return FALSE;
    }
    m_pAttack->init((COLL_KIND)inParam.CollKind, (COLL_TYPE)inParam.CollType, getHandle(), 0);
    m_pAttack->setAttackKind((ATK_KIND)inParam.AtkKind);
    m_pAttack->setAttackAttr((ATK_ATTR)inParam.AtkAttr);
    m_pAttack->setAttack(inParam.iAttackPoint, 0);
    m_pAttack->setRadius(inParam.fCollRadius);
    m_pAttack->setShiftY(inParam.fShiftY);
    m_pAttack->setStartPos(pos);
    m_pAttack->setEndPos(pos);
    m_pAttack->setEnemyPos(pos);
    m_pAttack->updateAttackID();
    m_pAttack->on();
    if (inParam.uiAttackID) {
        m_pAttack->setAttackID(inParam.uiAttackID);
    }
    if (inParam.pEffectTbl) {
        BULLET_EFFECT_INFO* info = &inParam.pEffectTbl[1];
        if (info->pEffectName) {
            m_pAttack->setHitEffectParam(info->pEffectName, info->iNo, (ObjType::eObjType)getEntityType());
        }
    }
    if (isStartCollAtkOff() && m_tqCollAtkTimer > 0) {
        m_pAttack->off();
    }
    m_pAttack->setEnemyHandle(0);
    m_fSpeed = inParam.fSpeed;
    m_fMaxDist = inParam.fMaxDist;
    m_pEffectTbl = inParam.pEffectTbl;
    m_pOffsetTbl = inParam.pOffsetTbl;
    m_pRotTbl = inParam.pRotTbl;
    m_pExtraEffectTbl = inParam.pExtraEffectTbl;
    makeEffect(0);
    playBulletSE_(0);
    m_hSEHandle = playBulletSE_(3);
    if (inParam.pEffectTbl) {
        m_iFileID = m_pEffectTbl[3].iFile;
        m_iSEID = m_pEffectTbl[3].iID;
    }
    m_hTarget = inParam.hHomingTarget;
    m_tqHomingTimer = inParam.tqHomingTime2;
    m_fChangeRadius = inParam.fChangeRadius;
    m_fCollOffsetY = inParam.fCollOffsetY;
    m_tqCollAtkTimer = inParam.tqCollAtkTime;
    m_tqFadeTime = inParam.tqFadeTime;
    setRate(inParam.fHomingRate);
    if (isGravity()) {
        m_pMove->enableGravity(TRUE);
        m_pMove->setShiftY(inParam.fShiftY - 0.005f);
    }
    if (!m_hTarget && isAutoLockOn()) {
        m_hTarget = LockOnManager::CLockOnManager::getLockOnObject(*this);
    }
    m_pMove->enableSliding(FALSE);
    m_State.set(this, &CBullet::beginMainState, &CBullet::mainState, &CBullet::endMainState);
    _setNewState(m_pNewState, m_State);
#line 352
    if (CCollDisp::m_fgDisp) {
        FLOAT deltaRingAngle;
        G_LINE line;
        FVECTOR ofs;
        FMATRIX m;
        m_pCollLines = new KH_AT("(357)") C3DLines;
        KH_DEBUG(if (!(m_pCollLines)) AssertApp(__FILE__, __LINE__, "m_pCollLines", 0);)
        m_pCollLines->m_pCrd = &Crd(0);
        {
            INT ring;
            INT segNum = 11;
            deltaRingAngle = 0.62831855f;
            ofs.set(0.0f, inParam.fShiftY, 0.0f);
            FMATRIX& w = Crd(0).getWorldMatrix();
            InverseMatrix(m, w);
            _apply33(ofs, m, ofs);
            line.C1.col = line.C0.col = 0xFF00FF00;
            for (ring = 0; ring < 5; ring++) {
                FLOAT r0 = sinf(ring * deltaRingAngle);
                FLOAT r1 = sinf((ring + 1) * deltaRingAngle);
                FLOAT y0 = cosf(ring * deltaRingAngle);
                FLOAT y1 = cosf((ring + 1) * deltaRingAngle);
                INT seg;
                for (seg = 0; seg < segNum; seg++) {
                    FLOAT theta = seg * deltaRingAngle;
                    FLOAT x0 = r0 * sinf(theta);
                    FLOAT z0 = r0 * cosf(theta);
                    FLOAT x1 = r1 * sinf(theta);
                    FLOAT z1 = r1 * cosf(theta);
                    line.P0.set(x0, y0, z0);
                    AddVectorXYZ(line.P0, line.P0, ofs);
                    ScaleVectorXYZ(line.P0, line.P0, inParam.fCollRadius);
                    line.P1.set(x1, y1, z1);
                    AddVectorXYZ(line.P1, line.P1, ofs);
                    ScaleVectorXYZ(line.P1, line.P1, inParam.fCollRadius);
                    m_pCollLines->m_vLINE.push_back(line);
                }
            }
        }
    } else {
        m_pCollLines = NULL;
    }
    return TRUE;
}

void CBullet::setBodyRadius(FLOAT inRadius, FLOAT inShiftY, INT inSliding, COLL_KIND inKind) {
    m_pMove->setRadius(inRadius);
    if (isGravity()) {
        m_pMove->setShiftY(inShiftY - 0.005f);
    } else {
        m_pMove->setShiftY(inShiftY);
    }
    m_pMove->enableSliding(inSliding);
    if (inKind != COLL_KIND_NONE) {
        m_pMove->setCollKind(inKind);
    }
}

void CBullet::setCollAttackPos(const FVECTOR& inPos) {
    FVECTOR pos = inPos;
    m_pAttack->setStartPos(pos);
    m_pAttack->setEndPos(pos);
    m_pAttack->setEnemyPos(pos);
}

#line 439
void CBullet::setAttackID(UINT inID) {
    KH_DEBUG(if (!(m_pAttack)) AssertApp(__FILE__, __LINE__, "m_pAttack", 0);)
    m_pAttack->setAttackID(inID);
}

#line 447
UINT CBullet::getAttackID() const {
    KH_DEBUG(if (!(m_pAttack)) AssertApp(__FILE__, __LINE__, "m_pAttack", 0);)
    return m_pAttack->getAttackID();
}

#line 455
void CBullet::updateAttackID() {
    KH_DEBUG(if (!(m_pAttack)) AssertApp(__FILE__, __LINE__, "m_pAttack", 0);)
    m_pAttack->updateAttackID();
}

void CBullet::disp() {
    CEntity::disp();
    if (CCollDisp::m_fgDisp && m_pCollLines) {
        m_pCollLines->disp();
    }
}

CEffect* CBullet::getAttachEffectBullet() { return CEffectMan::getEffect(m_hEffectHandle); }

void CBullet::kill() {
    if (isFadeKill()) {
        m_uiFlag |= 0x800000000000ULL;
        setupFade(1.0f, 0.0f, m_tqFadeTime, -1);
    } else {
        out();
    }
}

void CBullet::beginMainState() { m_pMove->calcVelocity(m_fSpeed); }

void CBullet::mainState() {
    FVECTOR pos;
    if (isHomingOn() && m_tqHomingTimer > 0) {
        m_tqHomingTimer -= getFrameRate();
        CTask* task = CTaskMan::getTask(m_hTarget);
        if (task) {
            ((CEntity*)task)->getLockOnPos(pos);
            if (isDegHoming()) {
                if (!isNoXRotHoming()) {
                    m_pMove->turnToTarget(pos, getRate() * 0.017453292f);
                } else {
                    m_pMove->turnToTargetY(pos, getRate() * 0.017453292f);
                }
                m_pMove->calcVelocity(m_fSpeed);
            } else if (!isNoXRotHoming()) {
                m_pMove->homingMove(pos, getRate(), m_fSpeed);
            } else {
                m_pMove->homingMoveXZ(pos, getRate(), m_fSpeed);
            }
        } else if (isAutoLockOn()) {
            m_hTarget = LockOnManager::CLockOnManager::getLockOnObject(*this);
        }
    }
    if (isChangeRadius()) {
        FLOAT r = m_pMove->getRadius() + getFrameRateF() * m_fChangeRadius;
        m_pMove->setRadius(r);
        m_pMove->setShiftY(0.0f);
        m_pAttack->setRadius(r);
        m_pAttack->setShiftY(0.0f);
    }
    if (isCollAtkTime()) {
        if (m_tqCollAtkTimer > 0) {
            m_tqCollAtkTimer -= getFrameRate();
            if (m_tqCollAtkTimer <= 0) {
                m_pAttack->off();
            }
        }
    } else if (isStartCollAtkOff() && m_tqCollAtkTimer > 0) {
        m_pAttack->off();
        m_tqCollAtkTimer -= getFrameRate();
        if (m_tqCollAtkTimer <= 0) {
            m_pAttack->on();
        }
    }
}

void CBullet::endMainState() {}

void CBullet::lastUpdate() {
    setSEPanAndVol(m_hSEHandle, m_iFileID, m_iSEID, 0, NULL, 0);
    updateShadow();
    if (updateFade()) {
        if (!isAlwaysFadeOut() && (m_pMove->getCollisionFlag() & 0xF) && !isTouchAny()) {
            makeEffect(2);
            playBulletSE_(2);
            if (!isNoHitOut() && !isNoHitMapOut()) {
                out();
            } else {
                if (isClearSpeedOnGround() && m_pMove->getVel().y < 0.0f && (m_pMove->getCollisionFlag() & 5)) {
                    m_uiFlag |= 0x1000000000000ULL;
                    clearSpeed();
                }
                if (isClearSpeedTouchWall() && (m_pMove->getCollisionFlag() & 0xA)) {
                    m_uiFlag |= 0x1000000000000ULL;
                    clearSpeed();
                }
            }
        }
        return;
    }
    updateAttackCollision();
    if (m_pAttack->collideAttack()) {
        playBulletSE_(1);
        m_hHitEnemyHandle = m_pAttack->getEnemyHandle();
        if (isClearSpeedOnObj()) {
            clearSpeed();
        }
        if (isAlwaysFadeOut()) {
            m_uiFlag |= 0x800000000000ULL;
            setupFade(1.0f, 0.0f, m_tqFadeTime, -1);
            return;
        }
        if (!isNoHitOut() && !isNoHitObjOut()) {
            out();
            return;
        }
    } else if ((m_pMove->getCollisionFlag() & 0xF) && !isTouchAny()) {
        makeEffect(2);
        playBulletSE_(2);
        if (isClearSpeedOnGround() && m_pMove->getVel().y < 0.0f && (m_pMove->getCollisionFlag() & 5)) {
            m_uiFlag |= 0x1000000000000ULL;
            clearSpeed();
        }
        if (isClearSpeedTouchWall() && (m_pMove->getCollisionFlag() & 0xA)) {
            m_uiFlag |= 0x1000000000000ULL;
            clearSpeed();
        }
        if (isAlwaysFadeOut()) {
            m_uiFlag |= 0x800000000000ULL;
            setupFade(1.0f, 0.0f, m_tqFadeTime, -1);
            return;
        }
        if (!isNoHitOut() && !isNoHitMapOut()) {
            out();
            return;
        }
    } else if (calcDistanceSq(getStartPos()) > m_fMaxDist * m_fMaxDist) {
        if (isMaxDistSetEffect()) {
            BULLET_EFFECT_INFO* info;
            if (m_pEffectTbl && (info = &m_pEffectTbl[1])->pEffectName) {
                setEffect(info->pEffectName, info->iNo, &getPos(), NULL, 0.0f);
            }
            playBulletSE_(1);
            out();
            return;
        }
        if (isMaxDistClearSpeed()) {
            clearSpeed();
            return;
        }
        m_uiFlag |= 0x800000000000ULL;
        setupFade(1.0f, 0.0f, m_tqFadeTime, -1);
        return;
    }
    if (isTimeOut() && getTimer() <= 0) {
        m_uiFlag |= 0x800000000000ULL;
        setupFade(1.0f, 0.0f, m_tqFadeTime, -1);
        return;
    }
    if (m_hEffectHandle && !getAttachEffectBullet()) {
        out();
    }
}

HANDLE CBullet::makeEffect(INT inID) {
    BULLET_EFFECT_INFO* info;
    FVECTOR* rot;
    FVECTOR pos;
    HANDLE hEffect;
    BULLET_EFFECT_INFO* infoEx;

    if (!m_pEffectTbl || !(info = &m_pEffectTbl[inID])->pEffectName) {
        return 0;
    }
    rot = m_pRotTbl ? &m_pRotTbl[inID] : NULL;
    if (m_pOffsetTbl) {
        ApplyMatrix(pos, Crd(0).getWorldMatrix(), m_pOffsetTbl[inID]);
    } else {
        pos = getPos();
    }
    infoEx = isExtraFullAdjust() ? &m_pExtraEffectTbl[inID] : NULL;
    if (inID == 1) {
        hEffect = setEffect(info->pEffectName, info->iNo, m_pAttack->getDetectPos(), NULL, 0.0f);
        if (infoEx && infoEx->pEffectName) {
            setEffect(infoEx->pEffectName, infoEx->iNo, m_pAttack->getDetectPos(), NULL, 0.0f);
        }
    } else if (inID == 2 && !isUnrevisedHitMapOut()) {
        FVECTOR vecRight;
        FVECTOR vecUp;
        FVECTOR normal;
        FMATRIX m;
        vecUp._q = 0;
        vecUp.y = 1.0f;
        normal = m_pMove->getDetectNormal();
        if (-1e-05f < normal.x && normal.x < 1e-05f && -1e-05f < normal.z && normal.z < 1e-05f) {
            vecRight.set(-normal.y, 0.0f, 0.0f, 0.0f);



        } else {
            OuterProduct(vecRight, vecUp, normal);
        }
        Normalize(vecRight, vecRight);
        OuterProduct(vecUp, normal, vecRight);
        Normalize(vecUp, vecUp);
        m.init();
        m.v[0] = vecRight;
        m.v[1] = vecUp;
        m.v[2] = normal;
        if (rot) {
            RotMatrixX(m, m, rot->x);
            RotMatrixY(m, m, rot->y);
            RotMatrixZ(m, m, rot->z);
        }
        ScaleVector(normal, normal, -(m_pMove->getRadius() - 0.001f));
        AddVectorXYZ(pos, pos, normal);
        pos.w = 1.0f;
        m.setTrans(pos);
        hEffect = setEffect(info->pEffectName, info->iNo, &m, 0.0f);
        if (infoEx && infoEx->pEffectName) {
            setEffect(infoEx->pEffectName, infoEx->iNo, &m, 0.0f);
        }
        return hEffect;
    } else {
        FMATRIX m;
        FMATRIX mat;
        m = Crd(0).getWorldMatrix();
        if (rot) {
            mat.init();
            RotMatrixX(mat, mat, rot->x);
            RotMatrixY(mat, mat, rot->y);
            RotMatrixZ(mat, mat, rot->z);
            MulMatrix(m, m, mat);
        }
        pos.w = 1.0f;
        m.setTrans(pos);
        hEffect = setEffect(info->pEffectName, info->iNo, &m, 0.0f);
        if (infoEx && infoEx->pEffectName) {
            setEffect(infoEx->pEffectName, infoEx->iNo, &m, 0.0f);
        }
    }
    return hEffect;
}

BOOL CBullet::updateFade() {
    CEffect* effect;
    if (!isBulletFade()) {
        return FALSE;
    }
    if ((effect = getAttachEffectBullet())) {
        effect->color().w = getFadeAlpha();
    }
    if ((effect = CEffectMan::getEffect(m_hEffectShadow))) {
        effect->color().w = getFadeAlpha();
    }
    if (isExtraFullAdjust() && (effect = CEffectMan::getEffect(m_pExtraEffectHandle[0]))) {
        effect->color().w = getFadeAlpha();
    }
    if (!isFade()) {
        out();
    }
    return TRUE;
}

void CBullet::updateAttackCollision() {
    FVECTOR pos;
    m_pAttack->setStartPos(*m_pAttack->getEndPos());
    pos = getPos();
    pos.y += m_fCollOffsetY;
    m_pAttack->setEndPos(pos);
    m_pAttack->setEnemyPos(pos);
}

#line 920
void CBullet::updateShadow() {
    CEffect* effect;
    CCollMap* coll;
    FVECTOR pos;
    if (!m_hEffectShadow || !(effect = CEffectMan::getEffect(m_hEffectShadow))) {
        return;
    }
    coll = m_pMove->getMapCollision();
    KH_DEBUG(if (!(coll)) AssertApp(__FILE__, __LINE__, "coll", 0);)
    pos = getPos();
    pos.y += 0.1f;
    coll->setStartPos(pos);
    pos.y -= 10.1f;
    coll->setEndPos(pos);
    if (coll->collideRide(0) || coll->collideMap()) {
        FVECTOR vecRight;
        FVECTOR vecUp;
        FVECTOR normal;
        FMATRIX m;
        vecUp._q = 0;
        vecUp.y = 1.0f;
        normal = *coll->getDetectNormal();
        if (-1e-05f < normal.x && normal.x < 1e-05f && -1e-05f < normal.z && normal.z < 1e-05f) {
            vecRight.set(normal.y, 0.0f, 0.0f, 0.0f);



        } else {
            OuterProduct(vecRight, vecUp, normal);
        }
        Normalize(vecRight, vecRight);
        OuterProduct(vecUp, normal, vecRight);
        Normalize(vecUp, vecUp);
        m.init();
        m.v[0] = vecRight;
        m.v[1] = vecUp;
        m.v[2] = normal;
        RotMatrixX(m, m, 1.5707964f);
        if (!isAttachNoRot()) {
            RotMatrixY(m, m, getRot().y);
        }
        ScaleVector(normal, normal, -0.005f);
        AddVectorXYZ(pos, *coll->getOutPos(), normal);
        pos.w = 1.0f;
        m.setTrans(pos);
        effect->getCoords()->Crd(0).getLocalMatrix() = m;
        effect->getCoords()->Crd(0).touch();
    }
}

HANDLE CBullet::playBulletSE_(INT inID) {
    BULLET_EFFECT_INFO* info;
    if (!m_pEffectTbl) {
        return 0;
    }
    info = &m_pEffectTbl[inID];
    if (info->iFile < 0) {
        return 0;
    }
    return playSEPanAndVol(info->iFile, info->iID & 0x7FFFFFFF, 0, NULL, (UINT)info->iID >> 31);
}

CHomingBullet::CHomingBullet(CTask* inParent) : CBullet(inParent) {}

CHomingBullet::~CHomingBullet() {}

BOOL CHomingBullet::setup(const BULLET_PARAM& inParam) {
    BOOL result = CBullet::setup(inParam);
    m_hHomingTarget = inParam.hHomingTarget;
    m_fHomingRadius = inParam.fHomingRadius;
    setRate(inParam.fHomingRate);
    m_vHomingPos = inParam.vHomingPos;
    m_pExplodeEffectTbl = inParam.pExplodeEffectTbl;
    m_tqExplodeTime = inParam.tqExplodeTime;
    m_fExplodeRadius = inParam.vPos.w;
    m_fSpreadScale = m_fExplodeRadius / inParam.fCollRadius;
    if (isSpread()) {
        CEffect* effect = getAttachEffectBullet();
        if (effect) {
            effect->scale().x = m_fSpreadScale;
            effect->scale().y = m_fSpreadScale;
            effect->scale().z = m_fSpreadScale;
        }
        m_pAttack->setRadius(m_fExplodeRadius);
        m_pAttack->setShiftY(0.0f);
        exeSpread();
    }
    m_hBoundTarget = inParam.hBoundTarget;
    m_iBoundCount = inParam.iBoundCount;
    m_vTwist.x = -0.008726646f;
    m_vTwist.y = 0.008726646f;
    m_vTwist.z = -0.08726646f;
    m_vTwist.w = 1.0f;
    if (DebugWork::g_dw.player.iTemp == 3) {
        m_vTwist.w = DebugWork::g_dw.player.vTemp.w;
        m_vTwist.x = DebugWork::g_dw.player.vTemp.x * 10.0f * 0.017453292f;
        m_vTwist.y = DebugWork::g_dw.player.vTemp.y * 10.0f * 0.017453292f;
        m_vTwist.z = DebugWork::g_dw.player.vTemp.z * 10.0f * 0.017453292f;
    }
    m_iStep = 0;
    if (isHitExplodeOnly()) {
        m_pAttack->off();
    }
    if (!m_hHomingTarget && isAutoLockOn()) {
        m_hHomingTarget = LockOnManager::CLockOnManager::getLockOnObject(*this);
    }
    return result;
}

void CHomingBullet::mainState() {
    FVECTOR vec;
    CEntity* pTarget;
    if (m_iStep) {
        return;
    }
    if (isTwistMoveOn()) {
        exeTwist();
        return;
    }
    if (isHomingPosOn()) {
        vec = m_vHomingPos;
    } else {
        if (m_hHomingTarget <= 0) {
            if (isAutoLockOn()) {
                m_hTarget = LockOnManager::CLockOnManager::getLockOnObject(*this);
            }
            return;
        }
        pTarget = (CEntity*)CTaskMan::getTask(m_hHomingTarget);
        if (!pTarget) {
            m_hHomingTarget = 0;
            return;
        }
        if (isStartHomingOff() && calcDistanceSq(getStartPos()) < m_fExplodeRadius * m_fExplodeRadius) {
            return;
        }
        pTarget->getLockOnPos(vec);
    }
    if (isClearSpeedOnTarget()) {
        if (calcDistance(vec) < m_fHomingRadius) {
            clearSpeed();
        } else if (isDegHoming()) {
            m_pMove->turnToTarget(vec, getRate() * 0.017453292f);
            m_pMove->calcVelocity(m_fSpeed);
        } else {
            m_pMove->homingMove(vec, getRate(), m_fSpeed);
        }
    } else if (calcDistance(vec) < m_fHomingRadius) {
        if (isTwistMoveOn()) {
            setRate(getRate() * 1.1f);
        }
        if (isDegHoming()) {
            m_pMove->turnToTarget(vec, getRate() * 0.017453292f);
            m_pMove->calcVelocity(m_fSpeed);
        } else {
            m_pMove->homingMove(vec, getRate(), m_fSpeed);
        }
    }
    if (getTimer() <= 0) {
        m_hHomingTarget = 0;
        m_uiFlag &= ~0x20000000ULL;
    }
}

void CHomingBullet::lastUpdate() {
    CEffect* effect;
    BULLET_EFFECT_INFO* info;
    if (m_iStep == 0) {
        setSEPanAndVol(m_hSEHandle, m_iFileID, m_iSEID, 0, NULL, 0);
        updateShadow();
        if (updateFade()) {
            if (isHitExplodeOnly()) {
                if (!m_pAttack->isEnable()) {
                    m_pAttack->on();
                }
                m_pAttack->collideAttack();
            }
            return;
        }
        if (isSpread()) {
            exeSpread();
        }
        updateAttackCollision();
        if (m_pAttack->collideAttack()) {
            if (!initExplode()) {
                playBulletSE_(1);
                m_hHitEnemyHandle = m_pAttack->getEnemyHandle();
                if (isClearSpeedOnObj()) {
                    clearSpeed();
                    m_fSpeed = 0.0f;
                }
                if (isAlwaysFadeOut()) {
                    m_uiFlag |= 0x800000000000ULL;
                    setupFade(1.0f, 0.0f, m_tqFadeTime, -1);
                    return;
                }
                if (!isNoHitOut() && !isNoHitObjOut()) {
                    out();
                    return;
                }
            }
        } else if ((m_pMove->getCollisionFlag() & 0xF) && !isTouchAny()) {
            if (bound_()) {
                return;
            }
            if (!initExplode()) {
                if (isSpread()) {
                    effect = CEffectMan::getEffect(CBullet::makeEffect(2));
                    if (effect) {
                        CEffect* attach = getAttachEffectBullet();
                        effect->scale().x = attach->scale().x;
                        effect->scale().y = attach->scale().y;
                        effect->scale().z = attach->scale().z;
                    }
                } else {
                    CBullet::makeEffect(2);
                }
                playBulletSE_(2);
                if (isClearSpeedOnGround() && m_pMove->getVel().y < 0.0f && (m_pMove->getCollisionFlag() & 5)) {
                    m_uiFlag |= 0x1000000000000ULL;
                    clearSpeed();
                    m_fSpeed = 0.0f;
                }
                if (isClearSpeedTouchWall() && (m_pMove->getCollisionFlag() & 0xA)) {
                    m_uiFlag |= 0x1000000000000ULL;
                    clearSpeed();
                }
                if (isAlwaysFadeOut()) {
                    m_uiFlag |= 0x800000000000ULL;
                    setupFade(1.0f, 0.0f, m_tqFadeTime, -1);
                    m_fSpeed = 0.0f;
                    return;
                }
                if (!isNoHitOut() && !isNoHitMapOut()) {
                    out();
                    return;
                }
            }
        } else if (calcDistanceSq(getStartPos()) > m_fMaxDist * m_fMaxDist) {
            if (!initExplode()) {
                if (isMaxDistClearSpeed()) {
                    clearSpeed();
                    m_fSpeed = 0.0f;
                    if (!isAlwaysFadeOut()) {
                        if (isTimeOut() && getTimer() <= 0) {
                            m_uiFlag |= 0x800000000000ULL;
                            setupFade(1.0f, 0.0f, m_tqFadeTime, -1);
                        }
                        return;
                    }
                }
                if (isMaxDistSetEffect()) {
                    if (m_pEffectTbl && (info = &m_pEffectTbl[1])->pEffectName) {
                        setEffect(info->pEffectName, info->iNo, &getPos(), NULL, 0.0f);
                    }
                    playBulletSE_(1);
                    out();
                    return;
                }
                m_uiFlag |= 0x800000000000ULL;
                setupFade(1.0f, 0.0f, m_tqFadeTime, -1);
                return;
            }
        }
        if (isTimeOut() && getTimer() <= 0) {
            m_uiFlag |= 0x800000000000ULL;
            setupFade(1.0f, 0.0f, m_tqFadeTime, -1);
            return;
        }
    } else {
        if (isExplodeCollSpread()) {
            exeSpread();
        }
        if (getTimer() > 0) {
            m_pAttack->collideAttack();
        } else {
            delete m_pCollLines;
            m_pCollLines = NULL;
        }
    }
    if (m_hEffectHandle && !getAttachEffectBullet()) {
        out();
    }
}

HANDLE CHomingBullet::makeEffect(INT inID) {
    BULLET_EFFECT_INFO* info;
    INT id;
    FVECTOR* rot;
    FVECTOR pos;
    if (!m_pEffectTbl) {
        return 0;
    }
    id = inID;
    if (id == 4) {
        if (m_pExplodeEffectTbl) {
            info = m_pExplodeEffectTbl;
        } else {
            id = 1;
            info = &m_pEffectTbl[id];
        }
    } else {
        info = &m_pEffectTbl[id];
    }
    if (!info->pEffectName) {
        return 0;
    }
    rot = NULL;
    if (m_pRotTbl) {
        rot = &m_pRotTbl[id];
    }
    if (m_pOffsetTbl) {
        ApplyMatrix(pos, Crd(0).getWorldMatrix(), m_pOffsetTbl[id]);
    } else {
        pos = getPos();
    }
    return setEffect(info->pEffectName, info->iNo, &pos, rot, 0.0f);
}

#line 1500
INT CHomingBullet::initExplode() {
    CEffect* effect;
    if (!m_tqExplodeTime) {
        return 0;
    }
    effect = CEffectMan::getEffect(m_hEffectHandle);
    if (effect) {
        effect->kill();
    }
    fadeoutSE(m_hSEHandle, m_iFileID, m_iSEID, 16, 0);
    if (isExplodeCollSpread()) {
        m_uiFlag |= 0x80;
        m_pAttack->setRadius(m_fExplodeRadius);
        m_pAttack->setShiftY(0.0f);
        exeSpread();
    } else {
        m_pAttack->setRadius(m_fExplodeRadius);
    }
    clearSpeed();
    m_fSpeed = 0.0f;
    m_pMove->enableGravity(FALSE);
    setTimer(m_tqExplodeTime);
    m_hEffectHandle = makeEffect(4);
    playBulletSE_(1);
    m_iStep++;
    delete m_pCollLines;
    m_pCollLines = NULL;
#line 1524
    if (_collDispRef()) {
        FLOAT deltaRingAngle;
        G_LINE line;
        m_pCollLines = new KH_AT("(1527)") C3DLines;
        KH_DEBUG(if (!(m_pCollLines)) AssertApp(__FILE__, __LINE__, "m_pCollLines", 0);)
        m_pCollLines->m_pCrd = &Crd(0);
        {
            INT ring;
            INT segNum = 11;
            deltaRingAngle = 0.62831855f;
            line.C1.col = line.C0.col = 0xFF00FF00;
            for (ring = 0; ring < 5; ring++) {
                FLOAT r0 = sinf(ring * deltaRingAngle);
                FLOAT r1 = sinf((ring + 1) * deltaRingAngle);
                FLOAT y0 = cosf(ring * deltaRingAngle);
                FLOAT y1 = cosf((ring + 1) * deltaRingAngle);
                INT seg;
                for (seg = 0; seg < segNum; seg++) {
                    FLOAT theta = seg * deltaRingAngle;
                    FLOAT x0 = r0 * sinf(theta);
                    FLOAT z0 = r0 * cosf(theta);
                    FLOAT x1 = r1 * sinf(theta);
                    FLOAT z1 = r1 * cosf(theta);
                    line.P0.set(x0, y0, z0);
                    ScaleVectorXYZ(line.P0, line.P0, m_fExplodeRadius);
                    line.P1.set(x1, y1, z1);
                    ScaleVectorXYZ(line.P1, line.P1, m_fExplodeRadius);
                    m_pCollLines->m_vLINE.push_back(line);
                }
            }
        }
    }
    return 1;
}

#line 1570
void CHomingBullet::exeSpread() {
    FLOAT radius;
    if (!isExplodeCollSpread()) {
        CEffect* effect = getAttachEffectBullet();
        if (effect) {
            effect->scale().x += m_fSpreadScale;
            effect->scale().y += m_fSpreadScale;
            effect->scale().z += m_fSpreadScale;
        }
    }
    radius = m_pAttack->getRadius() + m_fExplodeRadius;
    m_pAttack->setRadius(radius);
    m_pAttack->setShiftY(0.0f);
    delete m_pCollLines;
    m_pCollLines = NULL;
#line 1590
    if (_collDispRef()) {
        FLOAT deltaRingAngle;
        G_LINE line;
        m_pCollLines = new KH_AT("(1593)") C3DLines;
        KH_DEBUG(if (!(m_pCollLines)) AssertApp(__FILE__, __LINE__, "m_pCollLines", 0);)
        m_pCollLines->m_pCrd = &Crd(0);
        {
            INT ring;
            INT segNum = 11;
            deltaRingAngle = 0.62831855f;
            line.C1.col = line.C0.col = 0xFF00FF00;
            for (ring = 0; ring < 5; ring++) {
                FLOAT r0 = sinf(ring * deltaRingAngle);
                FLOAT r1 = sinf((ring + 1) * deltaRingAngle);
                FLOAT y0 = cosf(ring * deltaRingAngle);
                FLOAT y1 = cosf((ring + 1) * deltaRingAngle);
                INT seg;
                for (seg = 0; seg < segNum; seg++) {
                    FLOAT theta = seg * deltaRingAngle;
                    FLOAT x0 = r0 * sinf(theta);
                    FLOAT z0 = r0 * cosf(theta);
                    FLOAT x1 = r1 * sinf(theta);
                    FLOAT z1 = r1 * cosf(theta);
                    line.P0.set(x0, y0, z0);
                    ScaleVectorXYZ(line.P0, line.P0, radius);
                    line.P1.set(x1, y1, z1);
                    ScaleVectorXYZ(line.P1, line.P1, radius);
                    m_pCollLines->m_vLINE.push_back(line);
                }
            }
        }
    }
}

static inline FLOAT _roundRad(FLOAT in) {
    FLOAT r;
    if (in < -3.1415927f) {
        r = in + 6.2831855f;
    } else {
        r = in;
        if (r > 3.1415927f) {
            r -= 6.2831855f;
        }
    }
    return r;
}

void CHomingBullet::exeTwist() {
    FVECTOR rot;
    FVECTOR vel;
    FVECTOR dir;
    CEntity* pTarget;
    rot = getRot();
    vel = getVel();
    Normalize(dir, vel);
    rot.x = -atan2f(dir.y, _sqrtf(dir.z * dir.z + dir.x * dir.x));
    rot.y = atan2f(dir.x, dir.z);
    rot.z = 0.0f;
    rot.x = _roundRad(rot.x + m_vTwist.x);
    rot.y = _roundRad(rot.y + m_vTwist.y);
    rot.z = _roundRad(rot.z + m_vTwist.z);
    setRot(rot);
    m_pMove->calcVelocity(m_fSpeed);
    m_vTwist.z = _roundRad(m_vTwist.z * m_vTwist.w);
    if (m_tqHomingTimer > 0) {
        m_tqHomingTimer -= getFrameRate();
        if (m_tqHomingTimer <= 0) {
            m_tqHomingTimer = 0;
            m_hHomingTarget = 0;
        }
    }
    if (m_hHomingTarget <= 0) {
        m_hHomingTarget = LockOnManager::CLockOnManager::getLockOnObject(*this);
        m_pMove->enableGravity(TRUE);
        return;
    }
    pTarget = (CEntity*)CTaskMan::getTask(m_hHomingTarget);
    if (!pTarget) {
        m_hHomingTarget = 0;
        return;
    }
    if (isStartHomingOff() && calcDistanceSq(getStartPos()) < m_fExplodeRadius * m_fExplodeRadius) {
        return;
    }
    if (m_tqHomingTimer > 0) {
        FVECTOR vec;
        pTarget->getLockOnPos(vec);
        m_pMove->turnToTarget(vec, getRate() * 0.017453292f);
    }
    m_pMove->calcVelocity(m_fSpeed);
    if (getTimer() <= 0) {
        m_hHomingTarget = 0;
    }
}

BOOL CHomingBullet::bound_() {
    FLOAT speed;
    FVECTOR reflect;
    if (!isBoundOn()) {
        return FALSE;
    }
    speed = m_pMove->getSpeed() * 0.5f;
    Math3D::CalcReflectionVector(reflect, m_pMove->getVel(), m_pMove->getDetectNormal());
    Normalize(reflect, reflect);
    ScaleVectorXYZ(reflect, reflect, speed);
    if (m_hBoundTarget > 0) {
        CEntity* target = (CEntity*)CTaskMan::getTask(m_hBoundTarget);
        if (target) {
            FLOAT dir = calcDirection(target);
            FMATRIX m;
            m.init();
            RotMatrixY(m, m, dir);
            ApplyMatrix33(reflect, m, reflect);
        }
    }
    m_pMove->setVel(reflect);
    if (m_pMove->getDetectNormal().y < 0.64278764f) {
        return TRUE;
    }
    if (speed > 0.05f) {
        return TRUE;
    }
    m_uiFlag ^= 0x400000;
    return FALSE;
}

// SOURCE DATA: .rodata
