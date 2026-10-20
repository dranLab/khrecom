// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef GAME_MOVEMENT_H
#define GAME_MOVEMENT_H

#include "khtypes.h"
#include "System/Vector.h" // PARTIAL: added for Game/Enemy/MapEnemyShadow (setScale)
#include "Game/Collision/CollObj.h" // PARTIAL: added for Game/Bullet/Bullet (COLL_KIND)

class CEntity; // PARTIAL: added for Game/Enemy/MapEnemyShadow
class CCollMap; // PARTIAL: added for Game/Player/SummonSinba

#ifdef MOVEMENT_H_FULL_CLASS
/* The complete class, for Game/Movement (its key unit, which emits every inline below as a weak copy, in this
 * order). Other units keep the partial class below. */
typedef FVECTOR QUATERNION;
class CMovement {
private:
    FVECTOR m_vOldPos;                 /* 0x000 */
    FVECTOR m_vOldPos2;                /* 0x010 */
    FVECTOR m_vPos;                    /* 0x020 */
    FVECTOR m_vOldRot;                 /* 0x030 */
    FVECTOR m_vRot;                    /* 0x040 */
    QUATERNION m_qQuat;                /* 0x050 */
    FVECTOR m_vScl;                    /* 0x060 */
    FVECTOR m_vVel;                    /* 0x070 */
    FVECTOR m_vAcc;                    /* 0x080 */
    FVECTOR m_vHitBackVel;             /* 0x090 */
    FVECTOR m_vDetectNormal;           /* 0x0a0 */
    FVECTOR m_vWallNormal;             /* 0x0b0 */
    FVECTOR m_vEscapeNormal;           /* 0x0c0 */
    FVECTOR m_vLadderPos;              /* 0x0d0 */
    FVECTOR m_vLadderNormal;           /* 0x0e0 */
    FLOAT m_fLadderHeight;             /* 0x0f0 */
    COLL_INFO_UNION m_JumpCollInfo;    /* 0x0f4 */
    COLL_INFO_UNION m_CollInfo;        /* 0x0f8 */
    COLL_INFO_UNION m_WallCollInfo;    /* 0x0fc */
    COLL_INFO_UNION m_CollInfoO;       /* 0x100 */
    COLL_INFO_UNION m_WallCollInfoO;   /* 0x104 */
    UINT m_uiFlag;                     /* 0x108 */
    UINT m_uiOldCollisionFlag;         /* 0x10c */
    UINT m_uiCollisionFlag;            /* 0x110 */
    FLOAT m_fMass;                     /* 0x114 */
    FLOAT m_fGravity;                  /* 0x118 */
    FLOAT m_fFootRadius;               /* 0x11c */
    FLOAT m_fWallRadius;               /* 0x120 */
    BOOL m_bExceptNowRide;             /* 0x124 */
    INT m_iCounter;                    /* 0x128 */
    FLOAT m_fPower;                    /* 0x12c */
    FLOAT m_fForce;                    /* 0x130 */
    FLOAT m_fLimitSpeedY;              /* 0x134 */
    TIMEQ m_tqWaterSurfaceTimer;       /* 0x138 */
    INT m_iWaterSurface;               /* 0x13c */
    CEntity* m_pOwner;                 /* 0x140 */
    CCollMap* m_pMapCollision;         /* 0x144 */
    FVECTOR m_vLastHitPos;             /* 0x150 */

public:
    CMovement(CEntity* inOwner);
    virtual ~CMovement();
    void setPos(const FVECTOR& inPos) { m_vPos = inPos; }
    void setOldPos(const FVECTOR& inPos) { m_vOldPos = inPos; }
    void setOldPos2(const FVECTOR& inPos) { m_vOldPos2 = inPos; }
    void setRot(const FVECTOR& inRot) { m_vRot = inRot; }
    void setQuaternion(const FVECTOR& inQuat) { m_qQuat = inQuat; }
    void setScl(const FVECTOR& inScl) { m_vScl = inScl; }
    void setVel(const FVECTOR& inVel) { m_vVel = inVel; }
    void setAcc(const FVECTOR& inAcc) { m_vAcc = inAcc; }
    void setHitBackVel(const FVECTOR& inVel) { m_vHitBackVel = inVel; }
    void setFlag(UINT inSet, UINT inMask) { m_uiFlag = (m_uiFlag | inSet) & ~inMask; }
    void setCollisionFlag(UINT inSet, UINT inMask) { m_uiCollisionFlag = (m_uiCollisionFlag | inSet) & ~inMask; }
    void setMass(FLOAT inMass) { m_fMass = inMass; }
    void setGravity(FLOAT inGravity) { m_fGravity = inGravity; }
    void setCollKind(COLL_KIND inKind);
    void setRadius(FLOAT inRadius);
    void setShiftY(FLOAT inShiftY);
    void setPower(FLOAT inPower) { m_fPower = inPower; }
    void setForce(FLOAT inForce) { m_fForce = inForce; }
    void setFootRadius(FLOAT inRadius) {
        m_fFootRadius = inRadius;
        setRadius(inRadius);
    }
    void setWallRadius(FLOAT inRadius) { m_fWallRadius = inRadius; }
    void setLimitSpeedY(FLOAT inLimit) { m_fLimitSpeedY = inLimit; }
    const FVECTOR& getPos() const { return m_vPos; }
    const FVECTOR& getOldPos() const { return m_vOldPos2; }
    const FVECTOR& getRot() const { return m_vRot; }
    const FVECTOR& getOldRot() const { return m_vOldRot; }
    FVECTOR& getRot() { return m_vRot; }
    QUATERNION& getQuaternion() { return m_qQuat; }
    const FVECTOR& getScl() const { return m_vScl; }
    const FVECTOR& getVel() const { return m_vVel; }
    const FVECTOR& getAcc() const { return m_vAcc; }
    const FVECTOR& getHitBackVel() const { return m_vHitBackVel; }
    FLOAT getSpeed() const;
    FLOAT getMass() const { return m_fMass; }
    FLOAT getGravity() const { return m_fGravity; }
    FLOAT getForce() const { return m_fForce; }
    UINT getFlag() const { return m_uiFlag; }
    UINT getCollisionFlag() const { return m_uiCollisionFlag; }
    const COLL_INFO_UNION& getWallCollInfo() const { return m_WallCollInfo; }
    const COLL_INFO_UNION& getJumpCollInfo() const { return m_JumpCollInfo; }
    const COLL_INFO_UNION& getCollInfo() const { return m_CollInfo; }
    const COLL_INFO_UNION& getCollInfoO() const { return m_CollInfoO; }
    const FVECTOR& getDetectNormal() const { return m_vDetectNormal; }
    const FVECTOR& getWallNormal() const { return m_vWallNormal; }
    const FVECTOR& getEscapeNormal() const { return m_vEscapeNormal; }
    const FVECTOR* getLadderPos() const {
        if (isLadder()) return &m_vLadderPos;
        return NULL;
    }
    const FVECTOR* getLadderNormal() const {
        if (isLadder()) return &m_vLadderNormal;
        return NULL;
    }
    FLOAT getLadderHeight() const { return isLadder() ? m_fLadderHeight : -1.0f; }
    FLOAT getRadius() const;
    FLOAT getShiftY() const;
    const CCollMap* getMapCollision() const { return m_pMapCollision; }
    CCollMap* getMapCollision() { return m_pMapCollision; }
    UINT isDisableCollision() const { return m_uiFlag & 1; }
    UINT isEnableLadder() const { return m_uiFlag & 2; }
    UINT isGravity() const { return m_uiFlag & 4; }
    UINT isDisableComputeVelocity() const { return m_uiFlag & 8; }
    UINT isWallEffectOn() const { return m_uiFlag & 0x10; }
    UINT isQuaternion() const { return m_uiFlag & 0x20; }
    UINT isWallCheck() const { return m_uiFlag & 0x40; }
    UINT isWaterSurfaceCheck() const { return m_uiFlag & 0x80; }
    UINT isWaterSurface() const { return m_uiFlag & 0x80000000; }
    UINT isSlipDownOn() const { return m_uiFlag & 0x100; }
    UINT isRideOff() const { return m_uiFlag & 0x200; }
    UINT isGround() const { return m_uiCollisionFlag & 5; }
    UINT isWall() const { return m_uiCollisionFlag & 0xa; }
    UINT isMapGround() const { return m_uiCollisionFlag & 1; }
    UINT isMapWall() const { return m_uiCollisionFlag & 2; }
    UINT isObjRide() const { return m_uiCollisionFlag & 4; }
    UINT isObjSide() const { return m_uiCollisionFlag & 8; }
    UINT isLadder() const { return m_uiCollisionFlag & 0x10; }
    void enableGravity(BOOL inEnable);
    void enableExceptNowRide(BOOL inEnable) { m_bExceptNowRide = inEnable; }
    void enableSliding(BOOL inEnable);
    void calcVelocity(FLOAT inSpeed);
    void calcVelocity(FLOAT inSpeed, const FVECTOR& inDir);
    void calcVelocityXZ(FLOAT inSpeed);
    void calcVelocityXZ(FLOAT inSpeed, const FVECTOR& inDir);
    FLOAT calcOrbitSpeedFromDistance(FLOAT inTheta, FLOAT inDist);
    void calcOrbitVelocityFromDistance(FLOAT inTheta, FLOAT inDist);
    FLOAT calcOrbitSpeedFromSpeedY(FLOAT inSpeedY, FLOAT inDist, FLOAT* outAngle);
    void calcOrbitVelocityFromSpeedY(FLOAT inSpeedY, FLOAT inDist);
    FLOAT calcOrbitSpeedFromHeight(FLOAT inTheta, FLOAT inHeight);
    void calcOrbitVelocityFromHeight(FLOAT inTheta, FLOAT inHeight);
    void homing(const CEntity& inTarget, FLOAT inRate);
    void homing(const CEntity* inTarget, FLOAT inRate);
    void homing(const FVECTOR& inTarget, FLOAT inRate);
    void homingY(const CEntity& inTarget, FLOAT inRate);
    void homingY(const CEntity* inTarget, FLOAT inRate);
    void homingY(const FVECTOR& inTarget, FLOAT inRate);
    void homingMove(const CEntity& inTarget, FLOAT inRate, FLOAT inSpeed);
    void homingMove(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed);
    void homingMove(const FVECTOR& inTarget, FLOAT inRate, FLOAT inSpeed);
    void homingMoveXZ(const CEntity& inTarget, FLOAT inRate, FLOAT inSpeed);
    void homingMoveXZ(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed);
    void homingMoveXZ(const FVECTOR& inTarget, FLOAT inRate, FLOAT inSpeed);
    BOOL turnToTarget(const CEntity& inTarget, FLOAT inLimit);
    BOOL turnToTarget(const CEntity* inTarget, FLOAT inLimit);
    BOOL turnToTarget(const FVECTOR& inTarget, FLOAT inLimit);
    BOOL turnToTargetY(const CEntity& inTarget, FLOAT inLimit);
    BOOL turnToTargetY(const CEntity* inTarget, FLOAT inLimit);
    BOOL turnToTargetY(const FVECTOR& inTarget, FLOAT inLimit);
    void chase(const CEntity& inTarget, FLOAT inRate, FLOAT inSpeed);
    void chase(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed);
    void evade(const CEntity& inTarget, FLOAT inRate, FLOAT inSpeed);
    void evade(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed);
    void intercept(const CEntity& inTarget, FLOAT inRate, FLOAT inSpeed);
    void intercept(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed);
    void update();
    void save();
    BOOL adjustRide();
    BOOL notifyRide();
    void land(FLOAT inDownLength, FLOAT inUpLength, BOOL inFirstHit);
    void updateVelocity();

private:
    void calcPos_();
    void move_(BOOL inGround);
    BOOL rideCollisionDetect_(FVECTOR& inStart, FVECTOR& inEnd, FVECTOR* outPos, FVECTOR* outNormal);
    BOOL mapCollisionDetect_(FVECTOR& inStart, FVECTOR& inEnd, FVECTOR* outPos, FVECTOR* outNormal);
    INT collideRide_();
    INT collideMap_(BOOL inCollInfoSave);
    void detect_();
    INT detectWall_();
    void setWallEffect_();
    BOOL setMapWallEffect_();
    BOOL setRideWallEffect_();
    void setWaterSurfaceEffect_();
    void calcVelocity_(FLOAT inSpeed);
    void calcVelocity_(FLOAT inSpeed, const FVECTOR& inDir);
    void calcVelocityXZ_(FLOAT inSpeed);
    void calcVelocityXZ_(FLOAT inSpeed, const FVECTOR& inDir);
};
#else
class CMovement {
private:
    UBYTE _m_0x00[0x108] __attribute__((aligned(16))); /* 0x000 m_vOldPos .. 0x104 m_WallCollInfoO */
    UINT m_uiFlag;              /* 0x108 */
    UINT m_uiOldCollisionFlag;  /* 0x10c */
    UINT m_uiCollisionFlag;     /* 0x110 */
    FLOAT m_fMass;              /* 0x114 */ // PARTIAL: split out for Game/Player/SummonCloud
    // PARTIAL: m_fGravity .. m_fWallRadius split out for Game/Gimmick/GimmickCarry
    FLOAT m_fGravity;           /* 0x118 */
    FLOAT m_fFootRadius;        /* 0x11c */
    FLOAT m_fWallRadius;        /* 0x120 */
    UBYTE _m_0x124[0x20];       /* 0x124 m_bExceptNowRide .. 0x140 m_pOwner */
    CCollMap* m_pMapCollision;  /* 0x144 */ // PARTIAL: added for Game/Player/SummonSinba
    UBYTE _m_0x148[0x18];       /* 0x148 .. 0x150 m_vLastHitPos */

public:
    CMovement(CEntity* inOwner); // PARTIAL: added for Game/Gimmick/BaseGimmick
    virtual ~CMovement();       /* 0x160 */

    void setFlag(UINT inFlag, BOOL inOn) {
        if (inOn) m_uiFlag |= inFlag;
        else m_uiFlag &= ~inFlag;
    }
    void setCollisionFlag(UINT inFlag, BOOL inOn) {
        if (inOn) m_uiCollisionFlag |= inFlag;
        else m_uiCollisionFlag &= ~inFlag;
    }
    void enableGravity(BOOL inOn);
    // PARTIAL: added for Game/Enemy/Heartless
    FVECTOR& getVelocity() { return *(FVECTOR*)&_m_0x00[0x70]; }
    void evade(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed);
    UINT getCollisionFlag() { return m_uiCollisionFlag; } // PARTIAL: added for Game/Gimmick/GimmickHitFlying
    // PARTIAL: added for Game/Debug/KisEnemy (m_vPos 0x20, m_vRot 0x40, m_vVel 0x70)
    FVECTOR& getPos() { return *(FVECTOR*)&_m_0x00[0x20]; }
    FVECTOR& getRot() { return *(FVECTOR*)&_m_0x00[0x40]; }
    FVECTOR& getVel() { return *(FVECTOR*)&_m_0x00[0x70]; }
    // PARTIAL: added for Game/Enemy/EnemyBluerhapsody
    void setVel(const FVECTOR& inVel) { getVel() = inVel; }
    UINT isGround() { return m_uiCollisionFlag & 5; }
    UINT isGravity() const { return m_uiFlag & 4; } // PARTIAL: added for Game/Atel/Avatar
    UINT isWall() { return m_uiCollisionFlag & 0xa; } // PARTIAL: added for Game/Enemy/EnemyYellowopera
    // PARTIAL: added for Game/Enemy/MapEnemyShadow (m_vScale 0x60)
    void setScale(const FVECTOR& inScale) { *(FVECTOR*)&_m_0x00[0x60] = inScale; }
    void homingY(const CEntity* inTarget, FLOAT inRate);
    void homingY(const FVECTOR& inTarget, FLOAT inRate); // PARTIAL: added for Game/Player/FriendAriel
    void homingMoveXZ(const FVECTOR& inTarget, FLOAT inRate, FLOAT inSpeed);
    void homingMoveXZ(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed);
    void chase(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed); // PARTIAL: added for Game/Enemy/MapEnemyGround
    // PARTIAL: added for Game/Entity
    void chase(const CEntity& inTarget, FLOAT inRate, FLOAT inSpeed);
    void evade(const CEntity& inTarget, FLOAT inRate, FLOAT inSpeed);
    void intercept(const CEntity& inTarget, FLOAT inRate, FLOAT inSpeed);
    void calcVelocityXZ(FLOAT inSpeed); // PARTIAL: added for Game/Player/SummonDumbo
    // PARTIAL: added for Game/Player/FriendPan
    void homing(const CEntity* inTarget, FLOAT inRate);
    void calcVelocity(FLOAT inSpeed);
    void calcOrbitVelocityFromDistance(FLOAT inTheta, FLOAT inDist); // PARTIAL: added for Game/Enemy/EnemyPirate
    void calcOrbitVelocityFromSpeedY(FLOAT inSpeedY, FLOAT inDist); // PARTIAL: added for Game/Enemy/EnemyWightnight
    // PARTIAL: added for Game/Player/SummonBambi
    void calcVelocity(FLOAT inSpeed, const FVECTOR& inDir);
    FLOAT calcOrbitSpeedFromDistance(FLOAT inTheta, FLOAT inDist);
    INT& getJumpCollInfo() { return *(INT*)&_m_0x00[0xf4]; } // PARTIAL: added for Game/SEMotion (COLL_INFO_UNION)
    CCollMap* getMapCollision() { return m_pMapCollision; } // PARTIAL: added for Game/Player/SummonSinba
    // PARTIAL: added for Game/Gimmick/GimmickSearchLaser
    void setRadius(FLOAT inRadius);
    void land(FLOAT inDownLength, FLOAT inUpLength, BOOL inFirstHit);
    void setMass(FLOAT inMass) { m_fMass = inMass; } // PARTIAL: added for Game/Player/SummonCloud
    // PARTIAL: added for Game/Gimmick/GimmickCarry
    void setFootRadius(FLOAT inRadius) {
        m_fFootRadius = inRadius;
        setRadius(inRadius);
    }
    void setWallRadius(FLOAT inRadius) { m_fWallRadius = inRadius; }
    BOOL turnToTargetY(const FVECTOR& inTarget, FLOAT inRate); // PARTIAL: added for Game/Player/Summon
    void turnToTargetY(const CEntity* inTarget, FLOAT inRate); // PARTIAL: added for Game/Enemy/EnemyWightnight
    // PARTIAL: added for Game/Bullet/Bullet (m_vDetectNormal 0xa0)
    void turnToTarget(const FVECTOR& inTarget, FLOAT inRate);
    void homingMove(const FVECTOR& inTarget, FLOAT inRate, FLOAT inSpeed);
    void homingMove(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed); // PARTIAL: added for Game/Enemy/MapEnemyAir
    void setShiftY(FLOAT inShiftY);
    FLOAT getRadius() const;
    FLOAT getSpeed() const;
    void enableSliding(INT inFlag);
    void setCollKind(COLL_KIND inKind);
    FVECTOR& getDetectNormal() { return *(FVECTOR*)&_m_0x00[0xa0]; }
    void update(); // PARTIAL: added for Game/Prize
};
#endif

// PARTIAL: added for Game/Player/SummonDumbo
namespace Movement {
FLOAT Approach(FLOAT inA0, FLOAT inA1, FLOAT inRate, FLOAT inFrameRate);
FLOAT Homing(FLOAT inA0, FLOAT inA1, FLOAT inRate, FLOAT inFrameRate); // PARTIAL: added for Game/Player/Summon (parameter names guessed)
BOOL TurnToDir(FLOAT& outDir, FLOAT inA0, FLOAT inA1, FLOAT inLimit);
void AdjustMoveDelta(FVECTOR& outVec);
void CalcPlaneMatrix(FMATRIX& outMat, const FVECTOR& inNormal, const FVECTOR& inPos);
extern CHAR* PlayerWaterSurfaceEffectName;
extern CHAR* PooWaterSurfaceEffectName;
extern CHAR* WallEffectName;
extern CHAR* EscapeEffectName;
}

#endif /* GAME_MOVEMENT_H */
