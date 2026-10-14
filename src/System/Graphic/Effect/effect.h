// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef SYSTEM_GRAPHIC_EFFECT_EFFECT_H
#define SYSTEM_GRAPHIC_EFFECT_EFFECT_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/Vector.h" // PARTIAL: added for Game/Gimmick/GimmickTutorial

/* Placeholder for the original header's inline function asserting isValid(): its strings
 * ("System/Graphic/Effect/effect.h", "isValid()") are in .rodata of every includer. The line number is not
 * known (it is not part of the data). */
#line 1 "System/Graphic/Effect/effect.h"
inline void _effectCheckValid(BOOL inValid) {
    if (!inValid) AssertApp(__FILE__, __LINE__, "isValid()", 0);
}

/* PARTIAL: added for Game/Player/SummonTink */
class CEffect {
    FVECTOR m_vScale;    /* 0x00 */ // PARTIAL: split out for Game/Gimmick/GimmickTutorial
    UBYTE _m_0x10[0x20]; /* 0x10 .. 0x2c m_eType */
public:
    u_int m_iFlag;       /* 0x30 */
    UBYTE _m_0x34[0x4c]; /* 0x34 m_pData .. */
    void loopEnd();
    // PARTIAL: added for Game/Gimmick/GimmickTutorial
    void setScale(FLOAT fScale) {
        m_vScale.x = m_vScale.y = m_vScale.z = fScale;
    }
};

#endif /* SYSTEM_GRAPHIC_EFFECT_EFFECT_H */
