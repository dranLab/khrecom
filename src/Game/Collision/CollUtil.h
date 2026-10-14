// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef GAME_COLLISION_COLLUTIL_H
#define GAME_COLLISION_COLLUTIL_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

/* Placeholders for CCollUtil's inline singleton constructor/creation: their strings
 * ("Game/Collision/CollUtil.h", "__null == m_pInstance", "Game/Collision/CollUtil.h(185)", "m_pInstance")
 * are in .rodata of every includer. Only line 185 is known (it is part of the allocation message). */
#line 1 "Game/Collision/CollUtil.h"
inline void _collUtilCheckInstance(void* inInstance) {
    if (!(__null == inInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
}
inline void* _collUtilCreate() {
    void* p = com_malloc_d(4, __FILE__ "(185)", (UINT)-1);
    if (!p) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    return p;
}

/* PARTIAL: added for Game/Atel/EventTrap - the singleton accessor (line 194), for includers that have the class
 * (Game/Collision/CollUtilA.h, included before this header). */
#ifdef GAME_COLLISION_COLLUTILA_H
#line 193
inline CCollUtil* CCollUtil::getInstance() {
    if (!m_pInstance) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    return m_pInstance;
}
#endif

#endif /* GAME_COLLISION_COLLUTIL_H */
