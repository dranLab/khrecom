
#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" /* MATCH: this unit names CollUtil.h by its full original path */

#include "System/Task.h"
#include "System/TaskMan.h"
#include "Game/Collision/CollInfo.h"









#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "Game/Movement.h"
#include "Game/Entity.h"
#include "Game/Collision/CollMap.h"
#include "System/Math3D.h"
#include "System/Debug.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/HeapMan.h"
#include "include_asm.h"



#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

extern "C" float fabsf(float x);
extern "C" int sprintf(char* s, const char* fmt, ...);
extern "C" float sinf(float x);
extern "C" float cosf(float x);
extern "C" float atan2f(float y, float x);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m, const FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m, const FVECTOR& v1);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);

static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

static inline FLOAT _fabs(FLOAT inValue) {
    FLOAT ret;
    asm volatile("abs.s %0, %1" : "=f"(ret) : "f"(inValue));
    return ret;
}

/* No clobbers on purpose: a volatile asm whose pattern is a bare ASM_OPERANDS makes cse flush its tables (the
 * original reloads constants and member addresses after it, move_). */
static inline void _calcMove(FVECTOR& vOut, FVECTOR& vOutSpeed, const FVECTOR& inAcc, FLOAT fInPassage) {
    UINT nHalf = 0x3f000000;
    asm volatile("qmtc2.ni %3, $vf5\n"
                 "qmtc2.ni %4, $vf7\n"
                 "lqc2 $vf3, 0x0(%1)\n"
                 "vmul.x $vf7, $vf5, $vf7\n"
                 "lqc2 $vf4, 0x0(%2)\n"
                 "lqc2 $vf2, 0x0(%0)\n"
                 "vmove.xyzw $vf6, $vf3\n"
                 "vmulx.xyzw $vf4, $vf4, $vf5x\n"
                 "vadd.xyzw $vf3, $vf3, $vf4\n"
                 "vmulx.xyzw $vf6, $vf6, $vf5x\n"
                 "vmulx.xyzw $vf4, $vf4, $vf7x\n"
                 "vadd.xyzw $vf2, $vf2, $vf6\n"
                 "vadd.xyzw $vf2, $vf2, $vf4\n"
                 "sqc2 $vf3, 0x0(%1)\n"
                 "sqc2 $vf2, 0x0(%0)\n"
                 :
                 : "r"(&vOut), "r"(&vOutSpeed), "r"(&inAcc), "r"(fInPassage), "r"(nHalf));
}

static inline FLOAT _getLadderPosition(FVECTOR& outPos, FVECTOR& outNorm) {
    return CCollUtil::instance()._getLadderPosition(outPos, outNorm);
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

namespace Movement {
CHAR* PlayerWaterSurfaceEffectName = "rec_move_efB";
CHAR* PooWaterSurfaceEffectName = "rec_move_efP";
CHAR* WallEffectName = "rec_wall";
CHAR* EscapeEffectName = "rec_escape";
} // namespace Movement

FLOAT Movement::Homing(FLOAT inA0, FLOAT inA1, FLOAT inRate, FLOAT inFrameRate) {
    FLOAT a0 = Math3D::LimitPI(inA0);
    FLOAT a1 = Math3D::LimitPI(inA1);
    if (a0 == a1) return a1;
    FLOAT t = inRate * inFrameRate;
    if (t <= 0.0f) return a0;
    if (1.0f <= t) return a1;
    FLOAT f = Math3D::LimitPI(a1 - a0) * t;
    if (_fabs(f) < t * 0.001f) return a1;
    return Math3D::LimitPI(a0 + f);
}

FLOAT Movement::Approach(FLOAT inA0, FLOAT inA1, FLOAT inRate, FLOAT inFrameRate) {
    if (inA0 == inA1) return inA1;
    FLOAT t = inRate * inFrameRate;
    FLOAT f = inA1 - inA0;
    if (t < 1.0f) f *= t;
    if (fabsf(f) < 0.0003f) return inA1;
    return inA0 + f;
}

BOOL Movement::TurnToDir(FLOAT& outDir, FLOAT inA0, FLOAT inA1, FLOAT inLimit) {
    BOOL isTurn = TRUE;
    FLOAT rot = Math3D::LimitPI(inA1 - inA0);
    if (inLimit < rot) {
        outDir = Math3D::LimitPI(inA0 + inLimit);
    } else if (rot < -inLimit) {
        outDir = Math3D::LimitPI(inA0 - inLimit);
    } else {
        outDir = inA1;
        isTurn = FALSE;
    }
    return isTurn;
}

void Movement::AdjustMoveDelta(FVECTOR& outVec) {
    FLOAT len0 = Math3D::VectorLength(outVec);
    if (len0 == 0.0f) return;
    FLOAT d = len0 / 0.64278764f;
    outVec.y -= d;
    FLOAT len1 = Math3D::VectorLength(outVec);
    ScaleVectorXYZ(outVec, outVec, len0 / len1);
}

void Movement::CalcPlaneMatrix(FMATRIX& outMat, const FVECTOR& inNormal, const FVECTOR& inPos) {
    FVECTOR vecRight;
    FVECTOR vecUp;
    vecUp._q = 0;
    vecUp.y = 1.0f;
    if (-1e-05f < inNormal.x && inNormal.x < 1e-05f && -1e-05f < inNormal.z && inNormal.z < 1e-05f) {
        vecRight.set(-inNormal.y, 0.0f, 0.0f, 0.0f);
    } else {
        OuterProduct(vecRight, vecUp, inNormal);
    }
    Normalize(vecRight, vecRight);
    OuterProduct(vecUp, inNormal, vecRight);
    Normalize(vecUp, vecUp);
    _unitMatrixVu0(outMat);
    outMat.v[0] = vecRight;
    outMat.v[1] = vecUp;
    outMat.v[2] = inNormal;
    outMat.v[3] = inPos;
}

CMovement::CMovement(CEntity* inOwner) {
    struct _V1 { FVECTOR v[1]; };
#define vZero (((const _V1){{{{0.0f, 0.0f, 0.0f, 1.0f}}}}).v[0])
#define vOne (((const _V1){{{{1.0f, 1.0f, 1.0f, 1.0f}}}}).v[0])
    /* MATCH: the two constants sit in TImode locals: both lq come first and the vtable store stays where the original has it;
       the stores are written out (not init(), no chains) in the order that gives the original's issue order; ro keeps o in memory */
    FVECTOR o;
    FVECTOR& ro = o;
    u_long128 qZero = vZero._q;
    u_long128 qOne = vOne._q;
    m_uiFlag = 0;
    m_uiCollisionFlag = 0;
    m_fMass = 1.0f;
    m_fGravity = -0.008166667f;
    m_fFootRadius = 0.5f;
    m_fWallRadius = 0.5f;
    m_bExceptNowRide = FALSE;
    m_iCounter = 0;
    m_fPower = 1.0f;
    m_fForce = 1.0f;
    m_vOldPos._q = qZero;
    m_vPos._q = qZero;
    m_vOldRot._q = qZero;
    m_vRot._q = qZero;
    m_vVel._q = qZero;
    ro._q = qOne;
    m_vLastHitPos._q = 0;
    m_vHitBackVel._q = 0;
    m_qQuat._q = 0;
    m_vLastHitPos.w = 1.0f;
    m_vHitBackVel.w = 1.0f;
    m_qQuat.w = 1.0f;
    m_vWallNormal._q = 0;
    m_fLimitSpeedY = -0.5f;
    m_vAcc._q = qZero;
    m_tqWaterSurfaceTimer = 0;
    m_pOwner = inOwner;
    m_JumpCollInfo.info = 0;
    m_vScl._q = qOne;
    m_vWallNormal.w = 1.0f;
    m_CollInfo.info = 0;
#line 211
    m_pMapCollision = new (__FILE__ "(211)") CCollMap;
    ASSERT(m_pMapCollision);
    COLL_KIND coll_kind;
    switch (inOwner->getEntityType()) {
    case 1:
    case 0x11:
        coll_kind = (COLL_KIND)0;
        break;
    case 2:
    case 3:
        coll_kind = (COLL_KIND)2;
        break;
    default:
        coll_kind = (COLL_KIND)2;
        break;
    }
    m_pMapCollision->init(coll_kind, (COLL_TYPE)2, 1, inOwner->getHandle());
#undef vZero
#undef vOne
}

CMovement::~CMovement() {
    if (m_pMapCollision) delete m_pMapCollision;
    m_pMapCollision = NULL;
}

void CMovement::setCollKind(COLL_KIND inCollKind) {
#line 246
    ASSERT(m_pMapCollision);
    m_pMapCollision->setKind(inCollKind);
}

void CMovement::setRadius(FLOAT inRadius) {
#line 254
    ASSERT(m_pMapCollision);
    m_pMapCollision->setRadius(inRadius);
}

void CMovement::setShiftY(FLOAT inShiftY) {
#line 262
    ASSERT(m_pMapCollision);
    m_pMapCollision->setShiftY(inShiftY);
}

FLOAT CMovement::getSpeed() const { return _sqrtf(InnerProduct(m_vVel, m_vVel)); }

FLOAT CMovement::getRadius() const {
#line 277
    ASSERT(m_pMapCollision);
    return m_pMapCollision->getRadius();
}

FLOAT CMovement::getShiftY() const {
#line 285
    ASSERT(m_pMapCollision);
    return m_pMapCollision->getShiftY();
}

void CMovement::enableGravity(BOOL inEnable) {
    if (inEnable) {
        setFlag(4, 0);
    } else {
        setFlag(0, 4);
    }
}

void CMovement::enableSliding(BOOL inEnable) {
#line 318
    ASSERT(m_pMapCollision);
    m_pMapCollision->enableSliding(inEnable);
}

void CMovement::calcVelocity(FLOAT inSpeed) { calcVelocity_(inSpeed); }

void CMovement::calcVelocity(FLOAT inSpeed, const FVECTOR& inDir) { calcVelocity_(inSpeed, inDir); }

void CMovement::calcVelocityXZ(FLOAT inSpeed) { calcVelocityXZ_(inSpeed * m_fPower); }

void CMovement::calcVelocityXZ(FLOAT inSpeed, const FVECTOR& inDir) { calcVelocityXZ_(inSpeed * m_fPower, inDir); }

FLOAT CMovement::calcOrbitSpeedFromDistance(FLOAT inTheta, FLOAT inDist) {
    return _sqrtf(m_fMass * -m_fGravity * inDist * m_fForce / sinf(inTheta * 2.0f));
}

void CMovement::calcOrbitVelocityFromDistance(FLOAT inTheta, FLOAT inDist) {
    FLOAT speed = calcOrbitSpeedFromDistance(inTheta, inDist);
    FVECTOR vel;
    vel.set(0.0f, sinf(inTheta), cosf(inTheta), 1.0f);
    calcVelocity_(speed, vel);
}

FLOAT CMovement::calcOrbitSpeedFromSpeedY(FLOAT inSpeedY, FLOAT inDist, FLOAT* outAngle) {
    FLOAT speedXZ = inDist * -m_fGravity * m_fMass * m_fForce / (inSpeedY * 2.0f);
    if (outAngle) *outAngle = atan2f(inSpeedY, speedXZ);
    return _sqrtf(speedXZ * speedXZ + inSpeedY * inSpeedY);
}

void CMovement::calcOrbitVelocityFromSpeedY(FLOAT inSpeedY, FLOAT inDist) {
    FLOAT angle;
    FLOAT speed = calcOrbitSpeedFromSpeedY(inSpeedY, inDist, &angle);
    FVECTOR vel;
    vel.set(0.0f, sinf(angle), cosf(angle), 1.0f);
    calcVelocity_(speed, vel);
}

FLOAT CMovement::calcOrbitSpeedFromHeight(FLOAT inTheta, FLOAT inHeight) {
    FLOAT s = sinf(inTheta);
    return _sqrtf(m_fMass * 2.0f * -m_fGravity * inHeight * m_fForce / (s * s));
}

void CMovement::calcOrbitVelocityFromHeight(FLOAT inTheta, FLOAT inHeight) {
    FLOAT speed = calcOrbitSpeedFromHeight(inTheta, inHeight);
    FVECTOR vel;
    vel.set(0.0f, sinf(inTheta), cosf(inTheta), 1.0f);
    calcVelocity_(speed, vel);
}

void CMovement::homing(const CEntity& inTarget, FLOAT inRate) { homing(inTarget.getPos(), inRate); }

void CMovement::homing(const CEntity* inTarget, FLOAT inRate) {
    if (inTarget) homing(inTarget->getPos(), inRate);
}

void CMovement::homing(const FVECTOR& inTarget, FLOAT inRate) {
    FVECTOR dir;
#line 436
    ASSERT(m_pOwner);
    m_pOwner->calcDirection(dir, inTarget);
    FLOAT frameRate = m_pOwner->getFrameRateF();
    m_vRot.x = Movement::Homing(m_vRot.x, dir.x, inRate, frameRate);
    m_vRot.y = Movement::Homing(m_vRot.y, dir.y, inRate, frameRate);
}

void CMovement::homingY(const CEntity& inTarget, FLOAT inRate) { homingY(inTarget.getPos(), inRate); }

void CMovement::homingY(const CEntity* inTarget, FLOAT inRate) {
    if (inTarget) homingY(inTarget->getPos(), inRate);
}

void CMovement::homingY(const FVECTOR& inTarget, FLOAT inRate) {
#line 462
    ASSERT(m_pOwner);
    m_vRot.y = Movement::Homing(m_vRot.y, m_pOwner->calcDirection(inTarget), inRate, m_pOwner->getFrameRateF());
}

void CMovement::homingMove(const CEntity& inTarget, FLOAT inRate, FLOAT inSpeed) {
    homing(inTarget, inRate);
    calcVelocity_(inSpeed);
}

void CMovement::homingMove(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed) {
    if (inTarget) {
        homing(inTarget, inRate);
        calcVelocity_(inSpeed);
    }
}

void CMovement::homingMove(const FVECTOR& inTarget, FLOAT inRate, FLOAT inSpeed) {
    homing(inTarget, inRate);
    calcVelocity_(inSpeed);
}

void CMovement::homingMoveXZ(const CEntity& inTarget, FLOAT inRate, FLOAT inSpeed) {
    homingY(inTarget, inRate);
    calcVelocityXZ_(inSpeed * m_fPower);
}

void CMovement::homingMoveXZ(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed) {
    if (inTarget) {
        homingY(inTarget, inRate);
        calcVelocityXZ_(inSpeed * m_fPower);
    }
}

void CMovement::homingMoveXZ(const FVECTOR& inTarget, FLOAT inRate, FLOAT inSpeed) {
    homingY(inTarget, inRate);
    calcVelocityXZ_(inSpeed * m_fPower);
}

BOOL CMovement::turnToTarget(const CEntity& inTarget, FLOAT inLimit) { return turnToTarget(inTarget.getPos(), inLimit); }

BOOL CMovement::turnToTarget(const CEntity* inTarget, FLOAT inLimit) {
    if (inTarget) return turnToTarget(inTarget->getPos(), inLimit);
    return FALSE;
}

BOOL CMovement::turnToTarget(const FVECTOR& inTarget, FLOAT inLimit) {
    BOOL isTurn;
    FVECTOR dir;
    m_pOwner->calcDirection(dir, inTarget);
    FLOAT limit = inLimit * m_pOwner->getFrameRateF();
    isTurn = Movement::TurnToDir(m_vRot.x, m_vRot.x, dir.x, limit);
    if (Movement::TurnToDir(m_vRot.y, m_vRot.y, dir.y, limit)) isTurn = TRUE;
    return isTurn;
}

BOOL CMovement::turnToTargetY(const CEntity& inTarget, FLOAT inLimit) { return turnToTargetY(inTarget.getPos(), inLimit); }

BOOL CMovement::turnToTargetY(const CEntity* inTarget, FLOAT inLimit) {
    if (inTarget) return turnToTargetY(inTarget->getPos(), inLimit);
    return FALSE;
}

BOOL CMovement::turnToTargetY(const FVECTOR& inTarget, FLOAT inLimit) {
    return Movement::TurnToDir(m_vRot.y, m_vRot.y, m_pOwner->calcDirection(inTarget), inLimit * m_pOwner->getFrameRateF());
}

void CMovement::chase(const CEntity& inTarget, FLOAT inRate, FLOAT inSpeed) {
    FVECTOR u;
    SubVector(u, inTarget.m_pMove->getPos(), getPos());
    Normalize(u, u);
    m_vRot.y = Movement::Homing(m_vRot.y, atan2f(u.x, u.z), inRate, m_pOwner->getFrameRateF());
    calcVelocityXZ_(inSpeed * m_fPower);
}

void CMovement::chase(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed) {
    if (inTarget) chase(*inTarget, inRate, inSpeed);
}

void CMovement::evade(const CEntity& inTarget, FLOAT inRate, FLOAT inSpeed) {
    FVECTOR u;
    SubVector(u, getPos(), inTarget.m_pMove->getPos());
    Normalize(u, u);
    m_vRot.y = Movement::Homing(m_vRot.y, atan2f(u.x, u.z), inRate, m_pOwner->getFrameRateF());
    calcVelocityXZ_(inSpeed * m_fPower);
}

void CMovement::evade(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed) {
    if (inTarget) evade(*inTarget, inRate, inSpeed);
}

void CMovement::intercept(const CEntity& inTarget, FLOAT inRate, FLOAT inSpeed) {
    FVECTOR u;
    FVECTOR v;
    FVECTOR vr;
    FVECTOR sr;
    FVECTOR st;
    SubVector(vr, inTarget.m_pMove->getVel(), getVel());
    SubVector(sr, inTarget.m_pMove->getPos(), getPos());
    FLOAT vrLen = _sqrtf(InnerProduct(vr, vr));
    if (vrLen != 0.0f) {
        FLOAT t = _sqrtf(InnerProduct(sr, sr)) / vrLen;
        ScaleVector(v, inTarget.m_pMove->getVel(), t);
        AddVector(st, v, inTarget.m_pMove->getPos());
        SubVector(u, st, getPos());
        Normalize(u, u);
        m_vRot.y = Movement::Homing(m_vRot.y, atan2f(u.x, u.z), inRate, m_pOwner->getFrameRateF());
        calcVelocityXZ_(inSpeed);
    } else {
        chase(inTarget, inRate, inSpeed * m_fPower);
    }
}

void CMovement::intercept(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed) {
    if (inTarget) intercept(*inTarget, inRate, inSpeed);
}
void CMovement::update() {
    m_vOldPos2 = m_vPos;
    m_uiOldCollisionFlag = m_uiCollisionFlag;
    if (m_iCounter > 0) m_iCounter -= m_pOwner->getFrameRate();
    calcPos_();
    m_uiCollisionFlag = 0;
    if (!isDisableCollision()) {
        detect_();
        if (isWallCheck() && detectWall_()) {
            FVECTOR result;
            FVECTOR normal;
            if (rideCollisionDetect_(m_vOldPos, m_vPos, &result, &normal)) {
                m_vPos = result;
                if (normal.y >= 0.64278764f && !isObjRide()) {
                    setCollisionFlag(4, 0);
                    m_pMapCollision->registerLastRide(m_pOwner->getHandle());
                }
            }
            if (mapCollisionDetect_(m_vOldPos, m_vPos, &result, &normal)) {
                m_vPos = result;
                if (normal.y >= 0.64278764f) setCollisionFlag(1, 0);
            }
            if (isMapGround() && isObjRide()) {
                setCollisionFlag(0, 4);
                m_pMapCollision->removeRide();
            }
        }
        setWallEffect_();
        setWaterSurfaceEffect_();
    }
    if (isSlipDownOn() && isWall()) {
        FVECTOR v;
        SubVectorXYZ(v, m_vPos, m_vOldPos);
        FLOAT spd = Math3D::VectorLength(v);
        if (spd < getSpeed()) {
            Normalize(m_vVel, m_vVel);
            ScaleVectorXYZ(m_vVel, m_vVel, spd);
        }
    }
    m_vPos.w = 1.0f;
    m_vOldPos = m_vPos;
    if (isGround()) m_vLastHitPos = m_vPos;
    if (m_vPos.y <= -75.0f) {
        char buff[128];
        sprintf(buff, "\x93\x96\x82\xbd\x82\xe8\x94\xb2\x82\xaf ... last position = (%.4f, %.4f, %.4f), ID = %d",
                m_vLastHitPos.x, m_vLastHitPos.y, m_vLastHitPos.z, m_pOwner->getID());
        INT type = m_pOwner->getEntityType();
        if (type == 1 || type == 0x11) {
            m_vOldPos = m_vLastHitPos;
            m_vPos = m_vLastHitPos;
            m_vOldPos.y += 0.25f;
            TRACE("%s\n", buff);
        } else if (isGravity()) {
            TRACE("%s\n", buff);
        }
    }
}
void CMovement::save() { m_vOldRot = m_vRot; }
BOOL CMovement::adjustRide() {
    if (isObjRide()) {
        m_vOldPos = m_vPos;
        if (m_pMapCollision->getRidePosition(m_vRot, m_vPos)) {
            FVECTOR result = m_vPos;
            m_pMapCollision->setStartPos(m_vOldPos);
            m_pMapCollision->setEndPos(m_vPos);
            if (m_pMapCollision->collideRide(FALSE)) result = *m_pMapCollision->getOutPos();
            m_pMapCollision->setStartPos(m_vOldPos);
            m_pMapCollision->setEndPos(result);
            if (m_pMapCollision->collideMap()) result = *m_pMapCollision->getOutPos();
            m_vPos = result;
            m_vOldPos = result;
        } else {
            setCollisionFlag(0, 4);
            m_pMapCollision->removeRide();
        }
        return TRUE;
    }
    return FALSE;
}

BOOL CMovement::notifyRide() {
    if (isObjRide()) {
        m_pMapCollision->setRidePosition(m_vRot, m_vPos);
        return TRUE;
    }
    return FALSE;
}

void CMovement::land(FLOAT inDownLength, FLOAT inUpLength, BOOL inFirstHit) {
    FVECTOR old_pos = m_vOldPos;
    FVECTOR pos = m_vPos;
    FVECTOR ridePos;
    FVECTOR mapPos;
    m_vOldPos = m_vPos;
    m_vPos.y -= inDownLength;
    m_vOldPos.y += inUpLength;
    if (inFirstHit) {
        if (rideCollisionDetect_(m_vOldPos, m_vPos, &ridePos, NULL)) {
            if (mapCollisionDetect_(m_vOldPos, m_vPos, &mapPos, NULL) &&
                _fabs(pos.y - ridePos.y) > _fabs(pos.y - mapPos.y)) {
                collideMap_(TRUE);
            } else {
                collideRide_();
            }
        } else {
            collideMap_(TRUE);
        }
    } else {
        detect_();
    }
    if (!(m_uiCollisionFlag & 0xf)) {
        m_vOldPos = old_pos;
        m_vPos = pos;
    } else {
        m_vOldPos = m_vPos;
        notifyRide();
    }
}

void CMovement::updateVelocity() {
    FVECTOR acc = m_vAcc;
    if (isGravity()) acc.y += m_fGravity * m_fMass * m_fForce;
    if (m_vVel.y < m_fLimitSpeedY) {
        m_vVel.y = m_fLimitSpeedY;
        acc.y = 0.0f;
    }
    FVECTOR pos;
    pos.init();
    _calcMove(pos, m_vVel, acc, m_pOwner->getFrameRateF());
}
void CMovement::calcPos_() {
#line 1135
    ASSERT(m_pOwner);
    if (isDisableComputeVelocity()) return;
    if (isDisableCollision()) {
        move_(FALSE);
        return;
    }
    if (m_pOwner->getEntityType() != 4) {
        FVECTOR result;
        if (rideCollisionDetect_(m_vOldPos, m_vPos, &result, NULL)) m_vPos = result;
        if (mapCollisionDetect_(m_vOldPos, m_vPos, &result, NULL)) m_vPos = result;
        m_vOldPos = m_vPos;
        FVECTOR pos = m_vPos;
        FVECTOR vel = m_vVel;
        if (0.0f < m_vVel.y || !isGround()) {
            move_(FALSE);
            return;
        }
        FVECTOR acc = m_vAcc;
        if (isSlipDownOn()) {
            FVECTOR a;
            FMATRIX m;
            FLOAT theta = atan2f(_sqrtf(m_vDetectNormal.z * m_vDetectNormal.z + m_vDetectNormal.x * m_vDetectNormal.x),
                                 m_vDetectNormal.y);
            a.set(0.0f, 0.0f, m_fMass * 0.008166667f * sinf(Math3D::LimitPI(theta)), 1.0f);
            _unitMatrixVu0(m);
            RotMatrixY(m, m, atan2f(m_vDetectNormal.x, m_vDetectNormal.z));
            ApplyMatrix(a, m, a);
            AddVector(acc, acc, a);
        }
        FVECTOR acc2 = acc;
        if (acc2.x != 0.0f || acc2.z != 0.0f) {
            acc2.y = 0.0f;
            Movement::AdjustMoveDelta(acc2);
        }
        AddVectorXYZ(vel, vel, m_vHitBackVel);
        if (vel.x != 0.0f || vel.z != 0.0f) {
            vel.y = 0.0f;
            Movement::AdjustMoveDelta(vel);
        } else {
            vel.y = -0.05f;
        }
        _calcMove(m_vPos, vel, acc2, m_pOwner->getFrameRateF());
        ScaleVectorXYZ(acc, acc, m_pOwner->getFrameRateF());
        AddVectorXYZ(m_vVel, m_vVel, acc);
    } else {
        FVECTOR pos = m_vPos;
        FVECTOR vel = m_vVel;
        if (0.0f < m_vVel.y) {
            move_(FALSE);
            return;
        }
        if ((vel.x != 0.0f || vel.z != 0.0f) && isGround()) {
            vel.y = 0.0f;
            FLOAT speed = _sqrtf(InnerProduct(vel, vel));
            Normalize(vel, vel);
            ScaleVector(vel, vel, speed * cosf(0.87266463f));
            vel.y = -speed * sinf(0.87266463f);
            FVECTOR acc = m_vAcc;
            _calcMove(pos, vel, acc, m_pOwner->getFrameRateF());
            ScaleVectorXYZ(acc, acc, m_pOwner->getFrameRateF());
            AddVectorXYZ(vel, m_vVel, acc);
        } else {
            move_(TRUE);
            return;
        }
        FVECTOR start = m_vOldPos;
        FVECTOR end = pos;
        FVECTOR norm;
        if (start.y == end.y) end.y -= 0.1f;
        if (mapCollisionDetect_(start, end, NULL, &norm) && norm.y >= 0.64278764f) { /* MATCH: else-if with the arm duplicated (cross-jumped), not one || condition */
            m_vPos = pos;
            m_vVel = vel;
        } else if (rideCollisionDetect_(start, end, NULL, &norm) && norm.y >= 0.64278764f) {
            m_vPos = pos;
            m_vVel = vel;
        } else {
            move_(TRUE);
        }
    }
}
void CMovement::move_(BOOL inGround) {
    FVECTOR acc = m_vAcc;
    FVECTOR hitback;
    if (isGravity()) acc.y += m_fGravity * m_fMass * m_fForce;
    if (m_vVel.y < m_fLimitSpeedY) {
        m_vVel.y = m_fLimitSpeedY;
        acc.y = 0.0f;
    }
    if (inGround && isGround()) {
        m_vVel.y = 0.0f;
        acc.y = 0.0f;
    }
    _calcMove(m_vPos, m_vVel, acc, m_pOwner->getFrameRateF());
    ScaleVectorXYZ(hitback, m_vHitBackVel, m_pOwner->getFrameRateF());
    AddVectorXYZ(m_vPos, m_vPos, hitback);
}
BOOL CMovement::rideCollisionDetect_(FVECTOR& inStart, FVECTOR& inEnd, FVECTOR* outPos, FVECTOR* outNormal) {
    if (isRideOff()) return FALSE;
    m_pMapCollision->setStartPos(inStart);
    m_pMapCollision->setEndPos(inEnd);
    if (m_pMapCollision->collideRide(m_bExceptNowRide)) {
        if (outNormal) *outNormal = *m_pMapCollision->getDetectNormal();
        if (outPos) *outPos = *m_pMapCollision->getOutPos();
        return TRUE;
    }
    return FALSE;
}

BOOL CMovement::mapCollisionDetect_(FVECTOR& inStart, FVECTOR& inEnd, FVECTOR* outPos, FVECTOR* outNormal) {
    m_pMapCollision->setStartPos(inStart);
    m_pMapCollision->setEndPos(inEnd);
    if (m_pMapCollision->collideMap()) {
        if (outNormal) *outNormal = *m_pMapCollision->getDetectNormal();
        if (outPos) *outPos = *m_pMapCollision->getOutPos();
        return TRUE;
    }
    return FALSE;
}
INT CMovement::collideRide_() {
    FVECTOR start = m_vOldPos;
    FVECTOR end = m_vPos;
    FVECTOR pos;
    FVECTOR normal;
    INT result;
    if (m_pOwner->getEntityType() == 4 && start.y == end.y) end.y -= 0.02f;
    result = rideCollisionDetect_(start, end, &pos, &normal);
    m_CollInfoO = m_pMapCollision->getDetectInfoO();
    if (!isWallCheck()) {
        m_WallCollInfoO = m_CollInfoO;
        if ((UINT)(m_WallCollInfoO.infoATTR - 4) < 2) m_vEscapeNormal = normal;
    }
    if (result) {
        m_vPos = pos;
        m_vDetectNormal = normal;
        if (normal.y < 0.64278764f) {
            m_vWallNormal = normal;
            setCollisionFlag(8, 4);
            m_pMapCollision->removeRide();
        } else {
            m_JumpCollInfo = m_pMapCollision->getDetectInfoO();
            setCollisionFlag(4, 0);
            if (m_pMapCollision->getOutStatus() == 1) setCollisionFlag(8, 0);
            m_pMapCollision->registerLastRide(m_pOwner->getHandle());
        }
        return TRUE;
    }
    m_pMapCollision->removeRide();
    setCollisionFlag(0, 4);
    return FALSE;
}

INT CMovement::collideMap_(BOOL inCollInfoSave) {
    FVECTOR start = m_vOldPos;
    FVECTOR end = m_vPos;
    FVECTOR pos;
    FVECTOR normal;
    INT result;
    if (m_pOwner->getEntityType() == 4 && start.y == end.y) end.y -= 0.02f;
    result = mapCollisionDetect_(start, end, &pos, &normal);
    m_CollInfo = m_pMapCollision->getDetectInfo();
    if (!isWallCheck() && inCollInfoSave) {
        m_WallCollInfo = m_CollInfo;
        if ((UINT)(m_WallCollInfo.infoATTR - 4) < 2) m_vEscapeNormal = normal;
        if (isEnableLadder() && m_CollInfo.infoATTR == 2) {
            m_fLadderHeight = _getLadderPosition(m_vLadderPos, m_vLadderNormal);
            setCollisionFlag(0x10, 0);
        }
    }
    if (result) {
        m_vPos = pos;
        m_vDetectNormal = normal;
        if (normal.y < 0.64278764f) {
            m_vWallNormal = normal;
            setCollisionFlag(2, 0);
        } else {
            m_JumpCollInfo = m_pMapCollision->getDetectInfo();
            setCollisionFlag(1, 0);
            if (m_pMapCollision->getOutStatus() == 1) setCollisionFlag(2, 0);
        }
        return TRUE;
    }
    return FALSE;
}
void CMovement::detect_() {
    collideRide_();
    if (collideMap_(TRUE)) {
        UINT backup_flag = m_uiCollisionFlag;
        if (collideRide_()) {
            if (collideMap_(TRUE)) m_vPos = m_vOldPos;
        } else if (backup_flag & 4) {
            m_pMapCollision->registerLastRide(m_pOwner->getHandle());
            setCollisionFlag(4, 0);
        }
        if (isMapGround() && isObjRide()) {
            setCollisionFlag(0, 4);
            m_pMapCollision->removeRide();
        }
    }
    INT type = m_pOwner->getEntityType();
    if ((type == 1 || type == 0x11) && !isGround()) {
        FVECTOR oldPos = m_vOldPos;
        FVECTOR pos = m_vPos;
        m_vOldPos = m_vPos;
        CTask* pMap = CTaskMan::getSysTask(2);
        if (pMap && *(INT*)((BYTE*)pMap + 0x50) == 0x7f35a) {
            m_vOldPos.y += 0.5f;
        } else {
            m_vOldPos.y += 0.075f;
        }
        m_vPos.y -= 0.05f;
        collideRide_();
        if (collideMap_(FALSE)) {
            UINT backup_flag = m_uiCollisionFlag;
            if (collideRide_()) {
                if (collideMap_(FALSE)) m_vPos = m_vOldPos;
            } else if (backup_flag & 4) {
                m_pMapCollision->registerLastRide(m_pOwner->getHandle());
                setCollisionFlag(4, 0);
            }
            if (isMapGround() && isObjRide()) {
                setCollisionFlag(0, 4);
                m_pMapCollision->removeRide();
            }
        }
        if (!isGround()) {
            m_vOldPos = oldPos;
            m_vPos = pos;
        }
    }
}
INT CMovement::detectWall_() {
    UINT ret;
    FVECTOR v;
    v.set(0.0f, 0.0f, m_fWallRadius - m_fFootRadius, 1.0f);
    m_WallCollInfo.info = 0;
    m_WallCollInfoO.info = 0;
    ret = 0;
    FLOAT theta;
    for (theta = 0.0f; theta < 360.0f; theta += 20.0f) {
        FMATRIX m;
        FVECTOR end;
        FVECTOR pos;
        FVECTOR normal;
        FVECTOR diff;
        FLOAT rad = Math3D::LimitPI(theta * 0.017453292f + m_vRot.y);
        _unitMatrixVu0(m);
        RotMatrixX(m, m, m_vRot.x);
        RotMatrixY(m, m, rad);
        RotMatrixZ(m, m, m_vRot.z);
        TransMatrix(m, m, m_vPos);
        ApplyMatrix(end, m, v);
        if (rideCollisionDetect_(m_vPos, end, &pos, &normal)) {
            COLL_INFO_UNION info = m_pMapCollision->getDetectInfoO();
            if (info.info) {
                m_WallCollInfoO = info;
                if ((UINT)(info.infoATTR - 4) < 2) m_vEscapeNormal = normal;
            }
            if (normal.y < 0.64278764f || m_pMapCollision->getOutStatus() == 1) {
                m_vWallNormal = normal;
                setCollisionFlag(8, 0);
                SubVectorXYZ(diff, pos, end);
                diff.y = 0.0f;
                AddVectorXYZ(m_vPos, m_vPos, diff);
                ret = 1;
            }
        }
        if (mapCollisionDetect_(m_vPos, end, &pos, &normal)) {
            COLL_INFO_UNION info = m_pMapCollision->getDetectInfo();
            if (info.info) {
                m_WallCollInfo = info;
                if ((UINT)(info.infoATTR - 4) < 2) m_vEscapeNormal = normal;
            }
            if (isEnableLadder() && m_WallCollInfo.infoATTR == 2) {
                m_fLadderHeight = _getLadderPosition(m_vLadderPos, m_vLadderNormal);
                setCollisionFlag(0x10, 0);
            }
            if (normal.y < 0.64278764f || m_pMapCollision->getOutStatus() == 1) {
                ret = 1;
                m_vWallNormal = normal;
                setCollisionFlag(2, 0);
                SubVectorXYZ(diff, pos, end);
                diff.y = 0.0f;
                AddVectorXYZ(m_vPos, m_vPos, diff);
            }
        }
    }
    for (theta = 0.0f; theta < 360.0f; theta += 20.0f) {
        FMATRIX m;
        FVECTOR end;
        FVECTOR pos;
        FVECTOR normal;
        FVECTOR diff;
        FLOAT rad = Math3D::LimitPI(theta * 0.017453292f + m_vRot.y);
        _unitMatrixVu0(m);
        RotMatrixX(m, m, m_vRot.x);
        RotMatrixY(m, m, rad);
        RotMatrixZ(m, m, m_vRot.z);
        TransMatrix(m, m, m_vPos);
        ApplyMatrix(end, m, v);
        if (rideCollisionDetect_(end, m_vPos, &pos, &normal)) {
            COLL_INFO_UNION info = m_pMapCollision->getDetectInfoO();
            if (info.info) {
                m_WallCollInfoO = info;
                if ((UINT)(info.infoATTR - 4) < 2) m_vEscapeNormal = normal;
            }
            if (normal.y < 0.64278764f || m_pMapCollision->getOutStatus() == 1) {
                m_vWallNormal = normal;
                setCollisionFlag(8, 0);
                ret = 1;
                SubVectorXYZ(diff, pos, m_vPos);
                diff.y = 0.0f;
                AddVectorXYZ(m_vPos, m_vPos, diff);
            }
        }
        if (mapCollisionDetect_(end, m_vPos, &pos, &normal)) {
            COLL_INFO_UNION info = m_pMapCollision->getDetectInfo();
            if (info.info) {
                m_WallCollInfo = info;
                if ((UINT)(info.infoATTR - 4) < 2) m_vEscapeNormal = normal;
            }
            if (isEnableLadder() && m_WallCollInfo.infoATTR == 2) {
                m_fLadderHeight = _getLadderPosition(m_vLadderPos, m_vLadderNormal);
                setCollisionFlag(0x10, 0);
            }
            if (normal.y < 0.64278764f || m_pMapCollision->getOutStatus() == 1) {
                m_vWallNormal = normal;
                setCollisionFlag(2, 0);
                SubVectorXYZ(diff, pos, m_vPos);
                ret = 1;
                diff.y = 0.0f;
                AddVectorXYZ(m_vPos, m_vPos, diff);
            }
        }
    }
    {
        FVECTOR start = m_vPos;
        start.y += 1.0f;
        FVECTOR pos;
        FVECTOR normal;
        FVECTOR diff;
        if (rideCollisionDetect_(m_vPos, start, &pos, &normal)) {
            COLL_INFO_UNION info = m_pMapCollision->getDetectInfoO();
            if (info.info) m_WallCollInfoO = info;
            if (normal.y < 0.64278764f || m_pMapCollision->getOutStatus() == 1) {
                m_vWallNormal = normal;
                setCollisionFlag(8, 0);
                ret = 1;
                SubVectorXYZ(diff, pos, start);
                AddVectorXYZ(m_vPos, m_vPos, diff);
            }
        }
        if (mapCollisionDetect_(m_vPos, start, &pos, &normal)) {
            COLL_INFO_UNION info = m_pMapCollision->getDetectInfo();
            if (info.info) m_WallCollInfo = info;
            if (isEnableLadder() && m_WallCollInfo.infoATTR == 2) {
                m_fLadderHeight = _getLadderPosition(m_vLadderPos, m_vLadderNormal);
                setCollisionFlag(0x10, 0);
            }
            if (normal.y < 0.64278764f || m_pMapCollision->getOutStatus() == 1) {
                m_vWallNormal = normal;
                ret = 1;
                setCollisionFlag(2, 0);
                SubVectorXYZ(diff, pos, start);
                AddVectorXYZ(m_vPos, m_vPos, diff);
            }
        }
    }
    if (!ret) m_WallCollInfo.info = 0;
    return ret;
}
void CMovement::setWallEffect_() {
    if (isWallEffectOn() && m_iCounter <= 0 && !setRideWallEffect_()) setMapWallEffect_();
}
BOOL CMovement::setMapWallEffect_() {
    FVECTOR start;
    FVECTOR v;
    FVECTOR end;
    COLL_INFO_UNION collInfo;
    FVECTOR normal;
    FMATRIX m;
    FVECTOR pos;
    HANDLE handle;
    CEffect* effect;
    if ((UINT)(m_WallCollInfo.infoATTR - 4) >= 2) return FALSE;
    m_pOwner->getLockOnPos(start);
    v = m_vVel;
    start.x = m_vPos.x;
    start.z = m_vPos.z;
    v.y = 0.0f;
    if (Math3D::VectorLengthSq(v) <= 0.0f) {
        v.set(0.0f, 0.0f, 1.0f, 1.0f);
        ApplyMatrix33(v, m_pOwner->Crd(0).m_World, v);
    } else {
        Normalize(v, v);
    }
    AddVectorXYZ(end, start, v);
    m_pMapCollision->setStartPos(start);
    m_pMapCollision->setEndPos(end);
    if (!m_pMapCollision->collideMap()) return FALSE;
    collInfo = m_pMapCollision->getDetectInfo();
    if ((UINT)(collInfo.infoATTR - 4) >= 2) return FALSE;
    if (m_pMapCollision->getDetectNormal()->y >= 0.64278764f) return FALSE;
    normal = *m_pMapCollision->getDetectNormal();
    pos = *m_pMapCollision->getOutPos();
    ScaleVectorXYZ(v, normal, -0.3f);
    AddVector(pos, pos, v);
    pos.w = 1.0f;
    Movement::CalcPlaneMatrix(m, normal, pos);
    if (m_WallCollInfo.infoATTR == 4) {
        handle = m_pOwner->setEffect(Movement::WallEffectName, 0, &m, 0.0f);
    } else {
        handle = m_pOwner->setEffect(Movement::EscapeEffectName, 0, &m, 0.0f);
    }
    effect = CEffectMan::getEffect(handle);
    if (effect) m_iCounter = effect->getTotalFrame() * 50;
    return TRUE;
}

BOOL CMovement::setRideWallEffect_() {
    FVECTOR start;
    FVECTOR v;
    FVECTOR end;
    COLL_INFO_UNION collInfo;
    FVECTOR normal;
    FMATRIX m;
    FVECTOR pos;
    HANDLE handle;
    CEffect* effect;
    if ((UINT)(m_WallCollInfoO.infoATTR - 4) >= 2) return FALSE;
    m_pOwner->getLockOnPos(start);
    v = m_vVel;
    start.x = m_vPos.x;
    start.z = m_vPos.z;
    v.y = 0.0f;
    if (Math3D::VectorLengthSq(v) <= 0.0f) {
        v.set(0.0f, 0.0f, 1.0f, 1.0f);
        ApplyMatrix33(v, m_pOwner->Crd(0).m_World, v);
    } else {
        Normalize(v, v);
    }
    AddVectorXYZ(end, start, v);
    m_pMapCollision->setStartPos(start);
    m_pMapCollision->setEndPos(end);
    if (!m_pMapCollision->collideRide(FALSE)) return FALSE;
    collInfo = m_pMapCollision->getDetectInfoO();
    if ((UINT)(collInfo.infoATTR - 4) >= 2) return FALSE;
    if (m_pMapCollision->getDetectNormal()->y >= 0.64278764f) return FALSE;
    normal = *m_pMapCollision->getDetectNormal();
    pos = *m_pMapCollision->getOutPos();
    ScaleVectorXYZ(v, normal, -0.3f);
    AddVector(pos, pos, v);
    pos.w = 1.0f;
    Movement::CalcPlaneMatrix(m, normal, pos);
    if (m_WallCollInfoO.infoATTR == 4) {
        handle = m_pOwner->setEffect(Movement::WallEffectName, 0, &m, 0.0f);
    } else {
        handle = m_pOwner->setEffect(Movement::EscapeEffectName, 0, &m, 0.0f);
    }
    effect = CEffectMan::getEffect(handle);
    if (effect) m_iCounter = effect->getTotalFrame() * 50;
    return TRUE;
}

void CMovement::setWaterSurfaceEffect_() {
    if (!isWaterSurfaceCheck()) return;
    CHAR* effectName = Movement::PlayerWaterSurfaceEffectName;
    UINT id = m_pOwner->getID();
    if (id / 10000 == 10 && id % 10000 == 1005) effectName = Movement::PooWaterSurfaceEffectName;
    FVECTOR pos;
    if (!isWaterSurface()) {
        pos = m_vPos;
        if (m_pMapCollision->checkWaterSurface(pos, 1.0f)) {
            FVECTOR rot = m_vRot;
            rot.x = 0.0f;
            rot.z = 0.0f;
            setFlag(0x80000000, 0);
            CEffectMan::setEffect(effectName, 4, (ObjType::eObjType)0xb, &pos, &rot, 0.0f);
            m_iWaterSurface = 0;
            m_tqWaterSurfaceTimer = 1;
        }
    } else {
        pos = m_vPos;
        INT ret = m_pMapCollision->checkWaterSurface(pos, 1.0f);
        switch (ret) {
        case 0: {
            setFlag(0, 0x80000000);
            pos = m_vOldPos;
            m_pMapCollision->checkWaterSurface(pos, _fabs(m_vPos.y - m_vOldPos.y) + 0.1f);
            FVECTOR rot = m_vRot;
            rot.x = 0.0f;
            rot.z = 0.0f;
            CEffectMan::setEffect(effectName, 4, (ObjType::eObjType)0xb, &pos, &rot, 0.0f);
            break;
        }
        case 1:
            if (m_tqWaterSurfaceTimer > 0) m_tqWaterSurfaceTimer -= m_pOwner->getFrameRate();
            if (m_pOwner->isFirstStandsGroundIK()) {
                FVECTOR vel;
                SubVectorXYZ(vel, m_vPos, m_vOldPos2);
                FLOAT speed = Math3D::VectorLength(vel);
                FVECTOR rot = m_vRot;
                rot.x = 0.0f;
                rot.z = 0.0f;
                if (speed <= 0.0f) {
                    if (m_tqWaterSurfaceTimer <= 0) {
                        CEffectMan::setEffect(effectName, 0, (ObjType::eObjType)0xb, &pos, &rot, 0.0f);
                        m_tqWaterSurfaceTimer = 6000;
                    }
                } else if (speed <= 0.05f) {
                    CEffectMan::setEffect(effectName, 1, (ObjType::eObjType)0xb, &pos, &rot, 0.0f);
                } else {
                    CEffectMan::setEffect(effectName, 2, (ObjType::eObjType)0xb, &pos, &rot, 0.0f);
                }
            }
            break;
        }
    }
}
void CMovement::calcVelocity_(FLOAT inSpeed) {
    static FVECTOR g_vUnitVec = {{0.0f, 0.0f, 1.0f, 1.0f}};
    calcVelocity_(inSpeed, g_vUnitVec);
}

void CMovement::calcVelocity_(FLOAT inSpeed, const FVECTOR& inDir) {
    FMATRIX m;
    _unitMatrixVu0(m);
    RotMatrixX(m, m, m_vRot.x);
    RotMatrixY(m, m, m_vRot.y);
    RotMatrixZ(m, m, m_vRot.z);
    ApplyMatrix33(m_vVel, m, inDir);
    ScaleVector(m_vVel, m_vVel, inSpeed);
}

void CMovement::calcVelocityXZ_(FLOAT inSpeed) {
    static FVECTOR g_vUnitVec = {{0.0f, 0.0f, 1.0f, 1.0f}};
    calcVelocityXZ_(inSpeed, g_vUnitVec);
}

void CMovement::calcVelocityXZ_(FLOAT inSpeed, const FVECTOR& inDir) {
    FMATRIX m;
    FVECTOR v;
    _unitMatrixVu0(m);
    RotMatrixY(m, m, m_vRot.y);
    ApplyMatrix33(v, m, inDir);
    ScaleVector(v, v, inSpeed);
    m_vVel.x = v.x;
    m_vVel.z = v.z;
}
// SOURCE DATA: .data .rodata
