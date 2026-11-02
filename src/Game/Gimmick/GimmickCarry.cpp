
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Gimmick/GimmickCarry.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"

#include "Game/CardBattle.h"
#include "Game/Card.h"
#include "Game/Technic.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in GimmickTreasureBox); which header
 * it comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

inline void* operator new(size_t, void* p) throw() { return p; }
#include "Game/Atel/ChatMan.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"

#include "Game/Collision/CollObj.h"
#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Game.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Camera.h"
#include "System/Math3D.h"
#include "include_asm.h"

#include <math.h>

/* Inline in the original's collision header: the weak copies of its destructors and collideMap end this
 * object (the named copies are other objects'). */
class CPointCollision : public CCollision {
public:
    CPointCollision() { m_Type = COLL_TYPE_POINT; }
    virtual ~CPointCollision() {}
    virtual INT collideMap() { return CCollUtil::getInstance()->_detectMCD(*this); }
};

// System/Coord.h
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void MulMatrix(FMATRIX& m0, const FMATRIX& m1, const FMATRIX& m2);
extern "C" void sceVu0UnitMatrix(FMATRIX& m);
extern "C" void sceVu0RotMatrixX(FMATRIX& m0, FMATRIX& m1, float rx);
extern "C" void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
extern "C" void sceVu0CopyVector(float v0[4], const float v1[4]);

static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m);
}

static inline void _rotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry) {
    sceVu0RotMatrixY(m0, m1, ry);
}

static inline FLOAT _sqrtf(FLOAT h) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(h));
    return ret;
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

#define CARRY_GRAVITY 0.32666668f
#define CARRY_HALF_PI 1.5707964f
#define CARRY_PI 3.1415927f

CGimmickCarry::CGimmickCarry(CTask* inParent) : CBaseGimmick(inParent) {
    m_Stat = 0;
    m_PowY = 0.0f;
    m_FlagSetInfo = 0;
    m_StartCal = 0;
    m_AlphaStep = 0;
    m_HitObj = 0;
}

CGimmickCarry::~CGimmickCarry() {
    if (m_FlagSetInfo == 0) {
        ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
    }
}

BOOL CGimmickCarry::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    bodyCollOffBody(-1);
    bodyCollOff(-1);
    rideModelEnableAllCollision(FALSE);
    setGimmickFlag(4, TRUE);
    return TRUE;
}

void CGimmickCarry::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat == 0) rideModelEnableAllCollision(TRUE);
    if (getParam() == 5) rideModelChangeListR2();
    changeMotion(1, 0, FALSE, -1, 0, 0);
    if (m_pAttack) m_pAttack->setCollKind(0);
    setGimmickFlag(0x10, TRUE);
    bodyCollOff(-1);
    bodyCollOffBody(-1);
    if ((UINT)getID() % 10000 == 607) bodyCollOnBody(0);
    if (m_Stat != 0) {
        out();
    } else {
        enableFade2Path(TRUE, -1);
    }
}

BOOL CGimmickCarry::checkCarryPermission() {
    if (isRideOnPlayer()) return FALSE;
    if (g_pPlayer->getPos().y - getPos().y > 0.2f || g_pPlayer->getPos().y - getPos().y < -0.2f) return FALSE;
    rideModelEnableAllCollision(FALSE);
    setEntityFlag(0, 0x10000);
    setEntityFlag(0x1000, 0);
    bodyCollOffBody(-1);
    m_Stat = 1;
    saveStat(1);
    return TRUE;
}

INT CGimmickCarry::getTypeID() const {
    return 0x2003;
}

void CGimmickCarry::beginStateMain() {
    if (m_Stat == 3) m_pMove->setVel(m_throwDir);
}

void CGimmickCarry::stateMain() {
    switch (m_Stat) {
    case 1:
        break;
    case 2:
        if (m_pAnim->getTotalTqCrd() - m_pAnim->getCurrentTqCrd() <= getFrameRate()) {
            CarryThrow();
            if (m_Stat == 3) m_pMove->setVel(m_throwDir);
        }
        break;
    case 3:
        if (moveThrow()) {
            switch ((UINT)getID() % 10000) {
            case 138:
            case 235:
            case 335:
            case 535:
            case 719:
            case 735:
            case 835:
            case 864:
            case 1135:
            case 1166:
            case 1235: {
                CPointCollision cColl;
                cColl.setStartPos(getPos());
                cColl.setEndPos(cColl.m_inWS);
                cColl.m_inWE.y -= 3.0f;
                if (cColl.collideMap()) warp(cColl.m_outDetect);
                break;
            }
            default:
                if (m_HitObj == 1) {
                    m_Stat = 9;
                    bodyCollOffBody(-1);
                    bodyCollOff(-1);
                    m_pMove->setFlag(1, 0);
                    m_Wait = 0.0f;
                }
                break;
            }
            if (m_Stat != 9) {
                FVECTOR v;
                v.x = 0.0f;
                v.w = 1.0f;
                v.y = 0.0f;
                v.z = 0.0f;
                m_pMove->setVel(v);
                changeMotion(2, 0, FALSE, 0, 0, 0);
                m_pMove->setFlag(1, 0);
                m_pAttack->setCollKind(0);
                m_Wait = 0.0f;
                m_Stat = 10;
            }
        }
        break;
    case 9:
        m_Wait += getFrameRateF();
        if (m_Wait >= 6.0f) {
            FVECTOR v;
            v.x = 0.0f;
            v.w = 1.0f;
            v.y = 0.0f;
            v.z = 0.0f;
            m_pMove->setVel(v);
            changeMotion(2, 0, FALSE, 0, 0, 0);
            m_pMove->setFlag(1, 0);
            m_pAttack->setCollKind(0);
            m_Wait = 0.0f;
            m_Stat = 10;
        }
        break;
    case 8: {
        CPointCollision cColl;
        cColl.setStartPos(getPos());
        cColl.setEndPos(cColl.m_inWS);
        cColl.m_inWE.y -= 3.0f;
        if (cColl.collideMap()) warp(cColl.m_outDetect);
        changeMotion(2, 0, FALSE, 0, 0, 0);
        m_pAttack->setCollKind(0);
        m_Stat = 10;
        m_Wait = 0.0f;
        break;
    }
    case 4:
        CarryliftStart();
        m_StartCal = 1;
        m_Stat = 1;
        break;
    case 5:
        CarryWalkStart();
        m_Stat = 1;
        break;
    case 6:
        CarryStandStart();
        m_Stat = 1;
        break;
    case 7:
        CarryThrowStart();
        m_Stat = 2;
        break;
    case 10:
        setEntityFlag(1, 0);
        out();
        break;
    }

    switch (m_AlphaStep) {
    case 0:
        break;
    case 2:
        for (INT i = 0; i < getModelCount(); i++) {
            FLOAT alpha = getModel()[i].getAlphaScale();
            alpha -= getFrameRateF() * 0.05f;
            if (alpha <= 0.3f) {
                alpha = 0.3f;
                m_AlphaStep = 0;
            }
            getModel(i)->setAlphaScale(alpha);
        }
        break;
    case 1:
        for (INT i = 0; i < getModelCount(); i++) {
            FLOAT alpha = getModel()[i].getAlphaScale();
            alpha += getFrameRateF() * 0.05f;
            if (alpha >= 1.0f) {
                alpha = 1.0f;
                m_AlphaStep = 0;
            }
            getModel(i)->setAlphaScale(alpha);
        }
        break;
    }
}

void CGimmickCarry::endStateMain() {}

void CGimmickCarry::lift() {
    m_Stat = 4;
}

void CGimmickCarry::CarryWalk() {
    m_Stat = 5;
}

void CGimmickCarry::CarryStand() {
    m_Stat = 6;
}

void CGimmickCarry::ThrowGimmick(const FVECTOR& in_TargetPos) {
    sceVu0CopyVector(m_targetPos.v, in_TargetPos.v);
    m_Stat = 7;
}

void CGimmickCarry::CarryliftStart() {
    FVECTOR v;
    FLOAT rotY = calcDirection(g_pPlayer);
    rotY -= getRot().y;
    if (CGame::isSoraGame()) {
        rotY -= CARRY_HALF_PI;
    } else {
        rotY -= CARRY_PI;
    }
    rotY = Math3D::LimitPI(rotY);
    v.x = 0.0f;
    v.y = 1.0f;
    v.z = 0.0f;
    v.w = 1.0f;
    Quaternion::QuaternionNormalize(m_rotQtn, m_rotQtn);
    Quaternion::QuaternionRotationAxis(m_rotQtn, v, -rotY);
    setDispOffFlag(2, 0);
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = 0.0f;
    v.w = 1.0f;
    setRot(v);
    setPos(v);
    warp(v);
    if (CGame::isSoraGame()) {
        Crd(0).setParent(&g_pPlayer->Crd(g_pPlayer->getBoneIndex("WeaponR", 0)));
    } else {
        Crd(0).setParent(&g_pPlayer->Crd(g_pPlayer->getBoneIndex("Wep0", 0)));
    }
    if (CGame::isSoraGame()) {
        changeMotionEx(0x1ADCE, 3, g_pPlayer->getMotionCurrentTime() + getFrameRate(), FALSE, 0, 0, 0);
    } else {
        changeMotionEx(0x1ADCF, 3, g_pPlayer->getMotionCurrentTime() + getFrameRate(), FALSE, 0, 0, 0);
    }
    Crd(0).m_Flag = 1;
    updateCoord();
    coordFix();
    g_pPlayer->cancelLockon();
}

void CGimmickCarry::CarryWalkStart() {
    if (CGame::isSoraGame()) {
        changeMotionEx(0x1ADCE, 5, g_pPlayer->getMotionCurrentTime(), FALSE, -1, 0, 0);
    } else {
        changeMotionEx(0x1ADCF, 5, g_pPlayer->getMotionCurrentTime(), FALSE, -1, 0, 0);
    }
}

void CGimmickCarry::CarryStandStart() {
    if (CGame::isSoraGame()) {
        changeMotionEx(0x1ADCE, 4, g_pPlayer->getMotionCurrentTime(), FALSE, -1, 0, 0);
    } else {
        changeMotionEx(0x1ADCF, 4, g_pPlayer->getMotionCurrentTime(), FALSE, -1, 0, 0);
    }
}

void CGimmickCarry::CarryThrowStart() {
    if (CGame::isSoraGame()) {
        changeMotionEx(0x1ADCE, 6, g_pPlayer->getMotionCurrentTime() + getFrameRate(), FALSE, -1, 0, 0);
    } else {
        changeMotionEx(0x1ADCF, 6, g_pPlayer->getMotionCurrentTime() + getFrameRate(), FALSE, -1, 0, 0);
    }
}

void CGimmickCarry::CarryThrow() {
    FMATRIX mat;
    FVECTOR vec;
    FVECTOR rot;
    CGimmickManager* pCGimMan;
    FMATRIX m;
    FVECTOR v;
    FVECTOR v2;
    FLOAT height;
    FLOAT t;
    FLOAT g;
    FLOAT si;
    FLOAT r;
    FLOAT len;
    FLOAT ang;
    mat = Crd(0).getWorldMatrix();
    if (CGame::isSoraGame()) {
        warp(Crd(1).getWorldMatrix().v[3]);
    } else {
        warp(Crd(0).getWorldMatrix().v[3]);
    }
    vec.x = mat.v[2].x;
    vec.y = mat.v[2].y;
    vec.z = mat.v[2].z;
    if (CGame::isSoraGame()) {
        rot.y = g_pPlayer->getRot().y;
    } else {
        FLOAT ofs = -CARRY_HALF_PI;
        rot.y = g_pPlayer->getRot().y + ofs;
        rot.y = Math3D::LimitPI(rot.y);
    }
    rot.x = 0.0f;
    rot.z = 0.0f;
    setRot(rot);
    Crd(0).setParent(NULL);
    Crd(0).m_Flag = 1;
    if (CGame::isSoraGame()) {
        changeMotionEx(0x1ADCE, 7, 0, FALSE, 0, 0, 0);
    } else {
        changeMotionEx(0x1ADCF, 7, 0, FALSE, 0, 0, 0);
    }
    pCGimMan = (CGimmickManager*)getSysTask(17);
    g = CARRY_GRAVITY;
    m_Stat = 3;
    height = m_targetPos.y - getPos().y;
    si = sinf(pCGimMan->getThrowMaxAngle());
    if (height == 0.0f) {
        t = pCGimMan->getThrowDirPow() * 2.0f * si / g;
    } else if (height < 0.0f) {
        FLOAT h = -height + pCGimMan->getThrowDirPow() * pCGimMan->getThrowDirPow() * (si * si) / (2.0f * g);
        t = pCGimMan->getThrowDirPow() * sinf(pCGimMan->getThrowMaxAngle()) / g + _sqrtf(2.0f * h / g);
    } else {
        t = pCGimMan->getThrowDirPow() * si / g +
            _sqrtf(2.0f * (height + pCGimMan->getThrowDirPow() * pCGimMan->getThrowDirPow() * (si * si) / (2.0f * g) - height) / g);
    }
    r = pCGimMan->getThrowDirPow() * (t * 60.0f) * cosf(pCGimMan->getThrowMaxAngle());
    v = m_targetPos;
    v2 = getPos();
    v.y = 0.0f;
    v2.y = 0.0f;
    SubVector(v2, m_targetPos, getPos());
    len = Math3D::VectorLength(v2);
    if (len < r) {
        ang = pCGimMan->getThrowMaxAngle() * (len / r);
    } else {
        ang = pCGimMan->getThrowMaxAngle();
    }
    _unitMatrixVu0(m);
    sceVu0RotMatrixX(m, m, -ang * 0.017453292f);
    sceVu0RotMatrixY(m, m, g_pPlayer->getRot().y);
    v.set(0.0f, 0.0f, pCGimMan->getThrowDirPow(), 0.0f);
    ApplyMatrix(m_throwDir, m, v);
    if ((UINT)getID() % 10000 == 607) {
        bodyCollOnBody(1);
    } else {
        bodyCollOnBody(-1);
    }
    setGimmickFlag(0x10, FALSE);
    bodyCollOn(-1);
    m_pMove->enableGravity(TRUE);
    m_pMove->setFlag(0, 1);
    m_pMove->setFlag(0x40, 0);
    m_pMove->setFlag(0x10, 0);
    m_pMove->setFootRadius(0.25f);
    m_pMove->setWallRadius(0.5f);
    CPointCollision cColl;
    cColl.setStartPos(getPos());
    cColl.setEndPos(g_pPlayer->getPos());
    cColl.m_inWE.y = getPos().y;
    SubVector(v, cColl.m_inWS, cColl.m_inWE);
    Normalize(v, v);
    ScaleVector(v, v, 2.0f);
    AddVector(cColl.m_inWE, cColl.m_inWS, v);
    if (CCollUtil::getInstance()->_detectMCD(cColl)) {
        /* "ギミック：投げられていきなり外にでた\n" */
        TRACE("\x83\x4d\x83\x7e\x83\x62\x83\x4e\x81\x46\x93\x8a\x82\xb0\x82\xe7\x82\xea\x82\xc4\x82\xa2\x82\xab\x82\xc8"
              "\x82\xe8\x8a\x4f\x82\xc9\x82\xc5\x82\xbd\n");
        ScaleVector(v, v, 0.1f);
        SubVector(v, cColl.m_outWE, v);
        warp(v);
        v.set(0.0f, 0.0f, 0.0f);
        m_pMove->setVel(v);
        setEntityFlag(1, 0);
        m_pMove->enableGravity(FALSE);
        m_Stat = 8;
    }
    moveThrow();
    updateCoord();
    coordFix();
}
void CGimmickCarry::CancelCarry() {
    FMATRIX mat;
    FVECTOR vec;
    FVECTOR rot;
    FMATRIX m;
    FVECTOR v;
    mat =Crd(0).getWorldMatrix();
    m_Stat = 3;
    if (CGame::isSoraGame()) {
        warp(Crd(1).getWorldMatrix().v[3]);
    } else {
        warp(Crd(0).getWorldMatrix().v[3]);
    }
    if (CGame::isSoraGame()) {
        rot.y = g_pPlayer->getRot().y;
    } else {
        FLOAT ofs = -CARRY_HALF_PI;
        rot.y = g_pPlayer->getRot().y + ofs;
        rot.y = Math3D::LimitPI(rot.y);
    }
    rot.x = 0.0f;
    rot.z = 0.0f;
    setRot(rot);
    Crd(0).setParent(NULL);
    Crd(0).m_Flag = 1;
    if (CGame::isSoraGame()) {
        changeMotionEx(0x1ADCE, 7, 0, FALSE, 0, 0, 0);
    } else {
        changeMotionEx(0x1ADCF, 7, 0, FALSE, 0, 0, 0);
    }
    _unitMatrix(m);
    _rotMatrixY(m, m, getRot().y);
    v.set(0.0f, 0.0f, 0.005f, 0.0f);
    ApplyMatrix(m_throwDir, m, v);
    if ((UINT)getID() % 10000 == 607) {
        bodyCollOnBody(1);
    } else {
        bodyCollOnBody(-1);
    }
    setGimmickFlag(0x10, FALSE);
    m_pMove->enableGravity(TRUE);
    m_pMove->setFlag(0, 1);
    m_pMove->setFlag(0x40, 0);
    m_pMove->setFlag(0x10, 0);
    m_pMove->setFootRadius(0.25f);
    m_pMove->setWallRadius(0.5f);
    CPointCollision cColl;
    cColl.setStartPos(getPos());
    cColl.setEndPos(g_pPlayer->getPos());
    cColl.m_inWE.y = getPos().y;
    SubVector(v, cColl.m_inWS, cColl.m_inWE);
    Normalize(v, v);
    ScaleVector(v, v, 2.0f);
    AddVector(cColl.m_inWE, cColl.m_inWS, v);
    if (CCollUtil::getInstance()->_detectMCD(cColl)) {
        /* "ギミック：投げられていきなり外にでた\n" */
        TRACE("\x83\x4d\x83\x7e\x83\x62\x83\x4e\x81\x46\x93\x8a\x82\xb0\x82\xe7\x82\xea\x82\xc4\x82\xa2\x82\xab\x82\xc8"
              "\x82\xe8\x8a\x4f\x82\xc9\x82\xc5\x82\xbd\n");
        ScaleVector(v, v, 0.1f);
        SubVector(v, cColl.m_outWE, v);
        warp(v);
        v.set(0.0f, 0.0f, 0.0f);
        m_pMove->setVel(v);
        m_pMove->enableGravity(FALSE);
        m_Stat = 8;
    }
    updateCoord();
    coordFix();
    changeGimmickState(0);
}

BOOL CGimmickCarry::moveThrow() {
    CPointCollision cColl;
    cColl.setStartPos(getPos());
    cColl.setEndPos(cColl.m_inWS);
    cColl.m_inWS.y += 1.0f;
    cColl.m_inWE.y -= 0.2f;
    if (cColl.collideMap()) return TRUE;
    if (m_pMove->getCollisionFlag() & 0xF) return TRUE;
    if (m_pAttack->test()) return TRUE;
    if ((getGimmickFlag() & 1) || (getGimmickFlag() & 2)) {
        m_HitObj = 1;
        return TRUE;
    }
    return FALSE;
}

void CGimmickCarry::lastUpdate() {
    if (m_StartCal == 1) {
        FMATRIX mat;
        Quaternion::QuaternionToMatrix(mat, m_rotQtn);
        if (CGame::isSoraGame()) {
            MulMatrix(Crd(2).getLocalMatrix(), Crd(2).getLocalMatrix(), mat);
            Crd(2).recalcWorldMatrix();
        } else {
            MulMatrix(Crd(0).getLocalMatrix(), Crd(0).getLocalMatrix(), mat);
            Crd(0).recalcWorldMatrix();
        }
        FVECTOR v = g_pPlayer->getPlayerCamera()->getNowCamEye();
        FVECTOR v2 = Crd(3).getWorldMatrix().v[3];
        SubVector(v, v, v2);
        if (InnerProduct(v, v) >= 1.5f) {
            m_AlphaStep = 1;
        } else {
            m_AlphaStep = 2;
        }
    }
    CBaseGimmick::lastUpdate();
}

void CGimmickCarry::setFlagSetInfo(INT in_flag) {
    m_FlagSetInfo = in_flag;
}

// SOURCE DATA: .rodata
