// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef GAME_MOVEMENT_H
#define GAME_MOVEMENT_H

#include "khtypes.h"
#include "System/Vector.h" // PARTIAL: added for Game/Enemy/MapEnemyShadow (setScale)

class CEntity; // PARTIAL: added for Game/Enemy/MapEnemyShadow

class CMovement {
private:
    UBYTE _m_0x00[0x108] __attribute__((aligned(16))); /* 0x000 m_vOldPos .. 0x104 m_WallCollInfoO */
    UINT m_uiFlag;              /* 0x108 */
    UINT m_uiOldCollisionFlag;  /* 0x10c */
    UINT m_uiCollisionFlag;     /* 0x110 */
    UBYTE _m_0x114[0x4c];       /* 0x114 m_fMass .. 0x150 m_vLastHitPos */

public:
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
    UINT getCollisionFlag() { return m_uiCollisionFlag; } // PARTIAL: added for Game/Gimmick/GimmickHitFlying
    // PARTIAL: added for Game/Debug/KisEnemy (m_vPos 0x20, m_vRot 0x40, m_vVel 0x70)
    FVECTOR& getPos() { return *(FVECTOR*)&_m_0x00[0x20]; }
    FVECTOR& getRot() { return *(FVECTOR*)&_m_0x00[0x40]; }
    FVECTOR& getVel() { return *(FVECTOR*)&_m_0x00[0x70]; }
    // PARTIAL: added for Game/Enemy/MapEnemyShadow (m_vScale 0x60)
    void setScale(const FVECTOR& inScale) { *(FVECTOR*)&_m_0x00[0x60] = inScale; }
    void homingY(const CEntity* inTarget, FLOAT inRate);
    void homingMoveXZ(const FVECTOR& inTarget, FLOAT inRate, FLOAT inSpeed);
    void homingMoveXZ(const CEntity* inTarget, FLOAT inRate, FLOAT inSpeed);
    void calcVelocityXZ(FLOAT inSpeed); // PARTIAL: added for Game/Player/SummonDumbo
};

// PARTIAL: added for Game/Player/SummonDumbo
namespace Movement {
FLOAT Approach(FLOAT inA0, FLOAT inA1, FLOAT inRate, FLOAT inFrameRate);
}

#endif /* GAME_MOVEMENT_H */
