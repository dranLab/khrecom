// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef GAME_MOVEMENT_H
#define GAME_MOVEMENT_H

#include "khtypes.h"
#include "System/Vector.h" // PARTIAL: added for Game/Enemy/MapEnemyShadow (setScale)
#include "Game/Collision/CollObj.h" // PARTIAL: added for Game/Bullet/Bullet (COLL_KIND)

class CEntity; // PARTIAL: added for Game/Enemy/MapEnemyShadow
class CCollMap; // PARTIAL: added for Game/Player/SummonSinba

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
    UINT isWall() { return m_uiCollisionFlag & 0xa; } // PARTIAL: added for Game/Enemy/EnemyYellowopera
    // PARTIAL: added for Game/Enemy/MapEnemyShadow (m_vScale 0x60)
    void setScale(const FVECTOR& inScale) { *(FVECTOR*)&_m_0x00[0x60] = inScale; }
    void homingY(const CEntity* inTarget, FLOAT inRate);
    void homingY(const FVECTOR& inTarget, FLOAT inRate); // PARTIAL: added for Game/Player/FriendAriel
    void homingMoveXZ(const FVECTOR& inTarget, FLOAT inRate, FLOAT inSpeed);
    void homingMoveXZ(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed);
    void chase(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed); // PARTIAL: added for Game/Enemy/MapEnemyGround
    void calcVelocityXZ(FLOAT inSpeed); // PARTIAL: added for Game/Player/SummonDumbo
    // PARTIAL: added for Game/Player/FriendPan
    void homing(const CEntity* inTarget, FLOAT inRate);
    void calcVelocity(FLOAT inSpeed);
    void calcOrbitVelocityFromDistance(FLOAT inTheta, FLOAT inDist); // PARTIAL: added for Game/Enemy/EnemyPirate
    void calcOrbitVelocityFromSpeedY(FLOAT inSpeedY, FLOAT inDist); // PARTIAL: added for Game/Enemy/EnemyWightnight
    // PARTIAL: added for Game/Player/SummonBambi
    void calcVelocity(FLOAT inSpeed, const FVECTOR& inDir);
    FLOAT calcOrbitSpeedFromDistance(FLOAT inTheta, FLOAT inDist);
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
};

// PARTIAL: added for Game/Player/SummonDumbo
namespace Movement {
FLOAT Approach(FLOAT inA0, FLOAT inA1, FLOAT inRate, FLOAT inFrameRate);
FLOAT Homing(FLOAT inA0, FLOAT inA1, FLOAT inRate, FLOAT inFrameRate); // PARTIAL: added for Game/Player/Summon (parameter names guessed)
}

#endif /* GAME_MOVEMENT_H */
