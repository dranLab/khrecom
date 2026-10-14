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
/* The load of m_pInstance is not a "fixed scalar" to gcc's alias analysis (it stays ordered after stores through
 * pointers, Game/Gimmick/GimmickSearchLaser's CGimmickSearchLaserCircle::stateMain): a reference to it does that. */
#line 192
inline CCollUtil* CCollUtil::getInstance() {
    CCollUtil*& instance = m_pInstance;
    if (!instance) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    return instance;
}
#endif

#endif /* GAME_COLLISION_COLLUTIL_H */
