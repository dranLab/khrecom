// PARTIAL: added for Form/Menu/MonstroGauge
#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

/* Placeholder for CStatus's inline singleton creation: its strings ("Game/Status.h",
 * "__null == m_pInstance", "Game/Status.h(38)") are in .rodata of every includer.
 * Only line 38 is known (it is part of the allocation message). */
#line 1 "Game/Status.h"
inline void* _statusCreate(void* inInstance) {
    if (!(__null == inInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
    return com_malloc_d(4, __FILE__ "(38)", (UINT)-1);
}

#endif /* GAME_STATUS_H */
