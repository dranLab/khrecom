// PARTIAL: added for Game/Debug/KisEnemy
#ifndef GAME_COLLISION_COLLDISP_H
#define GAME_COLLISION_COLLDISP_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

/* Placeholders for CCollDisp's inline singleton constructor/creation: their strings
 * ("Game/Collision/CollDisp.h", "__null == m_pInstance", "Game/Collision/CollDisp.h(49)", "m_pInstance")
 * are in .rodata of every includer. Only line 49 is known (it is part of the allocation message). */
#line 1 "Game/Collision/CollDisp.h"
inline void _collDispCheckInstance(void* inInstance) {
    if (!(__null == inInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
}
inline void* _collDispCreate() {
    void* p = com_malloc_d(4, __FILE__ "(49)", (UINT)-1);
    if (!p) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    return p;
}

#endif /* GAME_COLLISION_COLLDISP_H */
