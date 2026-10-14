// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef SYSTEM_GRAPHIC_EFFECT_EFFECT_H
#define SYSTEM_GRAPHIC_EFFECT_EFFECT_H

#include "khtypes.h"
#include "System/MainApp.h"

/* Placeholder for the original header's inline function asserting isValid(): its strings
 * ("System/Graphic/Effect/effect.h", "isValid()") are in .rodata of every includer. The line number is not
 * known (it is not part of the data). */
#line 1 "System/Graphic/Effect/effect.h"
inline void _effectCheckValid(BOOL inValid) {
    if (!inValid) AssertApp(__FILE__, __LINE__, "isValid()", 0);
}

/* PARTIAL: added for Game/Player/SummonTink */
class CEffect {
    UBYTE _m_0x00[0x30]; /* 0x00 m_vScale .. 0x2c m_eType */
public:
    u_int m_iFlag;       /* 0x30 */
    UBYTE _m_0x34[0x4c]; /* 0x34 m_pData .. */
    void loopEnd();
};

#endif /* SYSTEM_GRAPHIC_EFFECT_EFFECT_H */
