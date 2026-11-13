
#ifndef GAME_COLLISION_COLLUTIL_H
#define GAME_COLLISION_COLLUTIL_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

/* Placeholders for CCollUtil's inline singleton constructor/creation: their strings
 * ("Game/Collision/CollUtil.h", "__null == m_pInstance", "Game/Collision/CollUtil.h(185)", "m_pInstance")
 * are in .rodata of every includer. Only line 185 is known (it is part of the allocation message). */
/* MATCH: path override for Game/Collision/CollMapCamera ("c:/KHCOM/MainApp/...") */
#ifndef COLLUTIL_H_PATH
#define COLLUTIL_H_PATH "Game/Collision/CollUtil.h"
#endif
#line 1 COLLUTIL_H_PATH
inline void _collUtilCheckInstance(void* inInstance) {
    KH_DEBUG(if (!(__null == inInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);)
}
inline void* _collUtilCreate() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    void* p = com_malloc_d(4, __FILE__ "(185)", (UINT)-1);
    KH_DEBUG(if (!p) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);)
    return p;
}

/* The singleton accessor (line 194), for includers that have the class
 * (Game/Collision/CollUtilA.h, included before this header). */
#ifdef GAME_COLLISION_COLLUTILA_H /* MATCH: the accessor only for includers that have the class (see above) */
/* The load of m_pInstance is not a "fixed scalar" to gcc's alias analysis (it stays ordered after stores through
 * pointers, Game/Gimmick/GimmickSearchLaser's CGimmickSearchLaserCircle::stateMain): a reference to it does that. */
#line 192
inline CCollUtil& CCollUtil::instance() {
    CCollUtil*& p = m_pInstance;
    KH_DEBUG(if (!p) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);)
    return *p;
}

inline INT CCollUtil::detectMCD(CCollision& collision) {
    return instance()._detectMCD(collision);
}

inline INT CCollUtil::detectColumnColumnEz(CColumnCollision& coll0, CColumnCollision& coll1) {
    return instance()._detectColumnColumnEz(coll0, coll1);
}
inline INT CCollUtil::detectColumnColumn(CColumnCollision& coll0, CColumnCollision& coll1) {
    return instance()._detectColumnColumn(coll0, coll1);
}
inline INT CCollUtil::detectOCD(CCollision& collision, MCD_INFO* pMCDInfo, OCD_WORK* pOCDWork) {
    return instance()._detectOCD(collision, pMCDInfo, pOCDWork, 1);
}

inline INT CCollUtil::detectOCD(CCollision& collision, MCD_INFO* pMCDInfo, OCD_WORK* pOCDWork, INT fgLocal) {
    return instance()._detectOCD(collision, pMCDInfo, pOCDWork, fgLocal);
}
inline INT CCollUtil::checkWaterSurface(FVECTOR& inPos) {
    return instance()._checkWaterSurface(inPos);
}

inline void CCollUtil::initializeData(void* pData, MCD_INFO* pMCDInfo) {
    instance()._initializeData(pData, pMCDInfo);
}

inline INT CCollUtil::detectSpherePoint(CSphereCollision& coll0, CPointCollision& coll1) {
    return instance()._detectSpherePoint(coll0, coll1);
}
inline INT CCollUtil::detectSphereSphere(CSphereCollision& coll0, CSphereCollision& coll1) {
    return instance()._detectSphereSphere(coll0, coll1);
}
inline INT CCollUtil::detectSpherePoly3(CSphereCollision& coll0, CPoly3Collision& coll1) {
    return instance()._detectSpherePoly3(coll0, coll1);
}
inline INT CCollUtil::detectSphereCircle(CSphereCollision& coll0, CCircleCollision& coll1) {
    return instance()._detectSphereCircle(coll0, coll1);
}
inline INT CCollUtil::detectColumnPoint(CColumnCollision& coll0, CPointCollision& coll1) {
    return instance()._detectColumnPoint(coll0, coll1);
}
inline INT CCollUtil::detectColumnSphere(CColumnCollision& coll0, CSphereCollision& coll1, INT inFlag) {
    return instance()._detectColumnSphere(coll0, coll1, inFlag);
}
inline INT CCollUtil::detectColumnPoly3(CColumnCollision& coll0, CPoly3Collision& coll1) {
    return instance()._detectColumnPoly3(coll0, coll1);
}

#ifdef GAME_COLLISION_COLLOBJ_H /* MATCH: the collision classes' collideMap, when Game/Collision/CollObj.h came first (see there) */
inline INT CPointCollision::collideMap() { return CCollUtil::instance()._detectMCD(*this); }
inline INT CSphereCollision::collideMap() { return CCollUtil::instance()._detectMCD(*this); }
#endif
#endif

#endif /* GAME_COLLISION_COLLUTIL_H */
