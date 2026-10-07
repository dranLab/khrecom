// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef GAME_MOVEMENT_H
#define GAME_MOVEMENT_H

#include "khtypes.h"

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
};

#endif /* GAME_MOVEMENT_H */
