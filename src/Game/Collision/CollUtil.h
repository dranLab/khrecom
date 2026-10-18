// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef GAME_COLLISION_COLLUTIL_H
#define GAME_COLLISION_COLLUTIL_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

/* Placeholders for CCollUtil's inline singleton constructor/creation: their strings
 * ("Game/Collision/CollUtil.h", "__null == m_pInstance", "Game/Collision/CollUtil.h(185)", "m_pInstance")
 * are in .rodata of every includer. Only line 185 is known (it is part of the allocation message). */
/* PARTIAL: path override added for Game/Collision/CollMapCamera ("c:/KHCOM/MainApp/...") */
#ifndef COLLUTIL_H_PATH
#define COLLUTIL_H_PATH "Game/Collision/CollUtil.h"
#endif
#line 1 COLLUTIL_H_PATH
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
// PARTIAL: added for Game/Collision/CollMapCamera (original lines unknown)
inline INT CCollUtil::detectMCD(CCollision& collision) {
    return getInstance()->_detectMCD(collision);
}
// PARTIAL: added for Game/Collision/CollBody (original lines unknown)
inline INT CCollUtil::detectColumnColumnEz(CColumnCollision& coll0, CColumnCollision& coll1) {
    return getInstance()->_detectColumnColumnEz(coll0, coll1);
}
inline INT CCollUtil::detectColumnColumn(CColumnCollision& coll0, CColumnCollision& coll1) {
    return getInstance()->_detectColumnColumn(coll0, coll1);
}
inline INT CCollUtil::detectOCD(CCollision& collision, MCD_INFO* pMCDInfo, OCD_WORK* pOCDWork) {
    return getInstance()->_detectOCD(collision, pMCDInfo, pOCDWork, 1);
}
// PARTIAL: added for Game/Collision/Ride (original line unknown)
inline void CCollUtil::initializeData(void* pData, MCD_INFO* pMCDInfo) {
    getInstance()->_initializeData(pData, pMCDInfo);
}
// PARTIAL: added for Game/Collision/CollAttack (original lines unknown)
inline INT CCollUtil::detectSpherePoint(CSphereCollision& coll0, CPointCollision& coll1) {
    return getInstance()->_detectSpherePoint(coll0, coll1);
}
inline INT CCollUtil::detectSphereSphere(CSphereCollision& coll0, CSphereCollision& coll1) {
    return getInstance()->_detectSphereSphere(coll0, coll1);
}
inline INT CCollUtil::detectSpherePoly3(CSphereCollision& coll0, CPoly3Collision& coll1) {
    return getInstance()->_detectSpherePoly3(coll0, coll1);
}
inline INT CCollUtil::detectSphereCircle(CSphereCollision& coll0, CCircleCollision& coll1) {
    return getInstance()->_detectSphereCircle(coll0, coll1);
}
inline INT CCollUtil::detectColumnPoint(CColumnCollision& coll0, CPointCollision& coll1) {
    return getInstance()->_detectColumnPoint(coll0, coll1);
}
inline INT CCollUtil::detectColumnSphere(CColumnCollision& coll0, CSphereCollision& coll1, INT inFlag) {
    return getInstance()->_detectColumnSphere(coll0, coll1, inFlag);
}
inline INT CCollUtil::detectColumnPoly3(CColumnCollision& coll0, CPoly3Collision& coll1) {
    return getInstance()->_detectColumnPoly3(coll0, coll1);
}
#endif

#endif /* GAME_COLLISION_COLLUTIL_H */
