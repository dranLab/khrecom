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
    UBYTE _m_0x00[0x0c];   /* 0x000 CCollObj: CSJointT, m_Enable */
    UBYTE* m_pCollision;   /* 0x00c CCollision* */ // PARTIAL: split out for Game/Enemy/EnemyWhitemushroom
    UBYTE _m_0x10[0x60];   /* 0x010 m_hTask, m_vEnemyPos .. m_vHitEffectRot */
    ATK_KIND m_DamageKind; /* 0x070 */
    INT m_DamageAttr;      /* 0x074 ATK_ATTR */ // PARTIAL: added for Game/Enemy/EnemyWhitemushroom
    UBYTE _m_0x78[0x88];   /* 0x078 .. */
    FLOAT m_fBadStatusParam; /* 0x100 */ // PARTIAL: added for Game/Enemy/EnemyCreapplant (name unknown)
public:
    FLOAT getBadStatusParam() { return m_fBadStatusParam; } // PARTIAL: added for Game/Enemy/EnemyCreapplant (name unknown)
    ATK_KIND getDamageKind() { return m_DamageKind; }
    INT getDamageAttr() { return m_DamageAttr; } // PARTIAL: added for Game/Enemy/EnemyWhitemushroom (name unknown)
    /* CCollObj::getDetectPos (m_pCollision->m_vDetectPos at 0x40) */
    FVECTOR* getDetectPos() { return m_pCollision ? (FVECTOR*)(m_pCollision + 0x40) : NULL; }
    HANDLE setHitEffect(); // PARTIAL: added for Game/Gimmick/BaseGimmick
};

class CBodyCollision {
private:
    UINT m_uiFlag;       /* 0x00 */ // PARTIAL: split out for Game/Gimmick/BaseGimmick
    UBYTE _m_0x04[0x1c]; /* 0x04 m_uiSysBodyFlag .. */
    INT m_iDamage;       /* 0x20 */ // PARTIAL: split out for Game/Enemy/EnemyWhitemushroom (name unknown)
    UBYTE _m_0x24[0xc];  /* 0x24 .. m_iHitIndex */
    CCollBody* m_pHit;   /* 0x30 */
public:
    INT getDamage() { return m_iDamage; } // PARTIAL: added for Game/Enemy/EnemyWhitemushroom (name unknown)
    // PARTIAL: added for Game/Gimmick/BaseGimmick
    INT isHit() { return m_uiFlag & 4; }
    void update();
    void detect();
    BOOL collideBody(FVECTOR* outPos, FVECTOR* outNormal, FVECTOR* outDetectPos);
    CCollBody* getHitCollBody() { return m_pHit; }
    void setMuteki(INT inIndex);       // PARTIAL: added for Game/Enemy/EnemyBlackfungus
    void setMutekiDir(FLOAT inDir);
};

/* Placeholder for the original header's inline function checking a collision kind: its strings
 * ("Game/BodyCollision.h", "0 <= inKind && inKind < COLLISION_KIND_MAX") are in .rodata of every includer.
 * The line number is not known (it is not part of the data). */
#line 1 "Game/BodyCollision.h"
inline void _bodyCollCheckKind(INT inKind) {
    if (!(0 <= inKind && inKind < 2)) AssertApp(__FILE__, __LINE__, "0 <= inKind && inKind < COLLISION_KIND_MAX", 0);
}

#endif /* GAME_BODYCOLLISION_H */
