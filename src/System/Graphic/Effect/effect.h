// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef SYSTEM_GRAPHIC_EFFECT_EFFECT_H
#define SYSTEM_GRAPHIC_EFFECT_EFFECT_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/Vector.h" // PARTIAL: added for Game/Gimmick/GimmickTutorial

/* Placeholder for the original header's inline function asserting isValid(): its strings
 * ("System/Graphic/Effect/effect.h", "isValid()") are in .rodata of every includer. The line number is not
 * known (it is not part of the data). */
#ifndef EFFECT_H_PATH // PARTIAL: path override added for Game/Enemy/EnemyBarrelspider ("System/Graphic/Effect/Effect.h")
#define EFFECT_H_PATH "System/Graphic/Effect/effect.h"
#endif
#line 1 EFFECT_H_PATH
inline void _effectCheckValid(BOOL inValid) {
    if (!inValid) AssertApp(__FILE__, __LINE__, "isValid()", 0);
}

class CEntity; // PARTIAL: added for Game/Player/SummonGenie
class CCoords; // PARTIAL: added for Game/Bullet/Bullet

/* PARTIAL: added for Game/Player/SummonTink */
class CEffect {
    FVECTOR m_vScale;    /* 0x00 */ // PARTIAL: split out for Game/Gimmick/GimmickTutorial
    FVECTOR m_vColor;    /* 0x10 */ // PARTIAL: split out for Game/EffectMotion
    // PARTIAL: split out for Game/Bullet/Bullet
    HANDLE m_hHandle;    /* 0x20 */
    CCoords* m_pCrds;    /* 0x24 */
    UBYTE _m_0x28[0x8];  /* 0x28 m_pVecLst, m_eType */
public:
    u_int m_iFlag;       /* 0x30 */
    UBYTE _m_0x34[0x4c]; /* 0x34 m_pData .. */
    void loopEnd();
    INT getTotalFrame(); // PARTIAL: added for Game/Enemy/EnemyBarrelspider
    FLOAT getNowFrame(); // PARTIAL: added for Game/Enemy/EnemyCreapplant
    void kill(); // PARTIAL: added for Game/Gimmick/BaseGimmick
    // PARTIAL: added for Game/Gimmick/GimmickTutorial
    void setScale(FLOAT fScale) {
        m_vScale.x = m_vScale.y = m_vScale.z = fScale;
    }
    // PARTIAL: added for Game/Player/SummonGenie
    FVECTOR& scale() { return m_vScale; }
    // PARTIAL: added for Game/Player/FriendDonald
    void setScale(FLOAT _x, FLOAT _y, FLOAT _z) { m_vScale.set(_x, _y, _z); }
    void setAttachNoRot(BOOL bVal) {
        if (bVal) {
            m_iFlag |= 4;
        } else {
            m_iFlag &= ~4;
        }
    }
    void setZSortEntity(CEntity* pEnt, INT nEntZOfs, BOOL bAlpha);
    // PARTIAL: added for Game/EffectMotion
    FVECTOR& color() { return m_vColor; }
    void setAttachReverseTrs(BOOL bVal) {
        if (bVal) {
            m_iFlag |= 8;
        } else {
            m_iFlag &= ~8;
        }
    }
    void setAttachNoScale(BOOL bVal) {
        if (bVal) {
            m_iFlag |= 0x80;
        } else {
            m_iFlag &= ~0x80;
        }
    }
    void setLightCalc(BOOL bVal);
    void setSkipFrame(FLOAT fFrame);
    CCoords* getCoords() { return m_pCrds; } // PARTIAL: added for Game/Bullet/Bullet
    void fadeout(FLOAT fTime);
    void setViewclip(BOOL bVal);
    BOOL isLoop();
    void detachCoord();
};

#endif /* SYSTEM_GRAPHIC_EFFECT_EFFECT_H */
