// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef SYSTEM_GRAPHIC_2D_2DMAN_H
#define SYSTEM_GRAPHIC_2D_2DMAN_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"

/* Placeholders for the original header's inline functions: C2DMan's singleton constructor/creation
 * ("System/Graphic/2D/2DMan.h", "__null == m_pInstance", "System/Graphic/2D/2DMan.h(49)") and the 2D
 * container pool overflow message ("2D用コンテナが MAX に到達しました\n", Shift-JIS), in this order in
 * .rodata of every includer. Only line 49 is known (it is part of the allocation message). */
#line 1 "System/Graphic/2D/2DMan.h"
inline void _2dManCheckInstance(void* inInstance) {
    if (!(__null == inInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
}
inline void* _2dManCreate() {
    return com_malloc_d(4, __FILE__ "(49)", (UINT)-1);
}
inline void _2dSLCOverflow() {
    TRACE("2D\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}

#endif /* SYSTEM_GRAPHIC_2D_2DMAN_H */
