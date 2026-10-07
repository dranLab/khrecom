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

#endif /* SYSTEM_GRAPHIC_EFFECT_EFFECT_H */
