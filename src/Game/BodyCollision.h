// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef GAME_BODYCOLLISION_H
#define GAME_BODYCOLLISION_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "Game/CollParam.h"

union FVECTOR; // PARTIAL: added for Game/Gimmick/BaseGimmick

// PARTIAL: added for Game/Gimmick/GimmickGuillotine (layouts partial)
class CCollBody {
private:
    UBYTE _m_0x00[0x70]; /* 0x000 CCollObj, m_vEnemyPos .. m_vHitEffectRot */
    ATK_KIND m_DamageKind; /* 0x070 */
public:
    ATK_KIND getDamageKind() { return m_DamageKind; }
    HANDLE setHitEffect(); // PARTIAL: added for Game/Gimmick/BaseGimmick
};

class CBodyCollision {
private:
    UINT m_uiFlag;       /* 0x00 */ // PARTIAL: split out for Game/Gimmick/BaseGimmick
    UBYTE _m_0x04[0x2c]; /* 0x04 m_uiSysBodyFlag .. m_iHitIndex */
    CCollBody* m_pHit;   /* 0x30 */
public:
    // PARTIAL: added for Game/Gimmick/BaseGimmick
    INT isHit() { return m_uiFlag & 4; }
    void update();
    void detect();
    BOOL collideBody(FVECTOR* outPos, FVECTOR* outNormal, FVECTOR* outDetectPos);
    CCollBody* getHitCollBody() { return m_pHit; }
};

/* Placeholder for the original header's inline function checking a collision kind: its strings
 * ("Game/BodyCollision.h", "0 <= inKind && inKind < COLLISION_KIND_MAX") are in .rodata of every includer.
 * The line number is not known (it is not part of the data). */
#line 1 "Game/BodyCollision.h"
inline void _bodyCollCheckKind(INT inKind) {
    if (!(0 <= inKind && inKind < 2)) AssertApp(__FILE__, __LINE__, "0 <= inKind && inKind < COLLISION_KIND_MAX", 0);
}

#endif /* GAME_BODYCOLLISION_H */
