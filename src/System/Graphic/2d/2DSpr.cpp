
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h" /* MATCH: this unit names 2DMan.h so (header default "System/Graphic/2D/2DMan.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/2d/2DSpr.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "System/Task.h"
#include <string.h>

inline void* operator new(size_t, void* p) throw() { return p; }




#include "System/RsrcDB.h"
#include "System/HeapMan.h"




namespace D2 {










INT C2DSpr::m_n2DSeqObjListSelect = -1;
C2DOSLCList* C2DSpr::m_p2DOSLCList = NULL;
C2DOSLCList* C2DSpr::m_p2DOSLCL[4] = {NULL, NULL, NULL, NULL};

C2DSpr::C2DSpr() {
    makeBaseHandle(D2BASE_TYPE_SPR);
    m_pSprParts = NULL;
    m_pSprGroup = NULL;
    m_pSprSprite = NULL;
    m_pTexName[0] = '\0';
    m_pTexInfo = NULL;
}

C2DSpr::~C2DSpr() {}

#line 50
void C2DSpr::initializeList() {
    INT i;
    for (i = 0; i < 3; i++) {
        if (!m_p2DOSLCL[i]) {
            m_p2DOSLCL[i] = new (__FILE__ "(55)")
                C2DOSLCList(C2DMan::get2DOSLCManager());
        }
    }
    if (m_n2DSeqObjListSelect < 0) {
        m_n2DSeqObjListSelect = 0;
        m_p2DOSLCList = m_p2DOSLCL[0];
    }
}

void C2DSpr::destroyAll() {
    INT i;
    for (i = 0; i < 3; i++) {
        if (m_p2DOSLCL[i]) {
            TRACE_COL(_TTY_YELLOW, "C2DSpr::2DSeqObjList%d \x8d\xed\x8f\x9c\x81\x41\x93\x6f\x98\x5e\x90\x94 %d\n", i,
                      m_p2DOSLCL[i]->getSize());
            for (SLContainerT<C2DO>* __p = m_p2DOSLCL[i]->getHead(); __p; __p = __p->getNext()) {
                if (__p->getValue()) {
                    delete __p->getValue();
                    __p->setValue(NULL);
                }
            }
            m_p2DOSLCL[i]->removeAll();
            delete m_p2DOSLCL[i];
            m_p2DOSLCL[i] = NULL;
        }
    }
    m_n2DSeqObjListSelect = -1;
}

void C2DSpr::destroyAllForSceneChange() {
    INT i;
    for (i = 0; i < 3; i++) {
        if (m_p2DOSLCL[i]) {
            TRACE_COL(_TTY_YELLOW, "C2DSpr::2DSeqObjList%d \x8d\xed\x8f\x9c\x81\x41\x93\x6f\x98\x5e\x90\x94 %d\n", i,
                      m_p2DOSLCL[i]->getSize());
            for (SLContainerT<C2DO>* __p = m_p2DOSLCL[i]->getHead(); __p; __p = __p->getNext()) {
                if (__p->getValue()) {
                    delete __p->getValue();
                    __p->setValue(NULL);
                }
            }
            m_p2DOSLCL[i]->removeAll();
        }
    }
}

void C2DSpr::change2DSeqObjList(UINT nList) {
    if (nList < 3) {
        m_p2DOSLCList = m_p2DOSLCL[nList];
        m_n2DSeqObjListSelect = nList;
    }
}

#line 156
HANDLE C2DSpr::addList(C2DO& Obj) {
    SLContainerT<C2DO>* pNew;
    HANDLE h;
    if (Obj._getHandle()) return -1;
    if (!m_p2DOSLCList) AssertApp(__FILE__, __LINE__, "m_p2DOSLCList", 0);
    pNew = m_p2DOSLCList->pushBack(Obj);
    if (!pNew) {
        h = -1;
    } else {
        pNew->setGeneral(Obj.getBaseHandle());
        h = pNew->getHandle();
        Obj._setHandle(h);
    }
    return h;
}

#line 178
HANDLE C2DSpr::replaceList(HANDLE hCurrent, C2DO& Obj) {
    HANDLE h;
    SLContainerT<C2DO>* pNew;
    if (!hCurrent) return 0;
    if (!m_p2DOSLCList) AssertApp(__FILE__, __LINE__, "m_p2DOSLCList", 0);
    h = Obj._getHandle();
    if (h) m_p2DOSLCList->remove(h);
    pNew = m_p2DOSLCList->replace(hCurrent, Obj);
    if (!pNew) {
        h = -1;
    } else {
        pNew->setGeneral(Obj.getBaseHandle());
        h = pNew->getHandle();
        Obj._setHandle(h);
    }
    return h;
}

#line 205
BOOL C2DSpr::deleteList(C2DO& Obj) {
    HANDLE h = Obj._getHandle();
    BOOL bResult;
    if (!h) return FALSE;
    if (!m_p2DOSLCList) AssertApp(__FILE__, __LINE__, "m_p2DOSLCList", 0);
    bResult = m_p2DOSLCList->remove(h);
    if (bResult) Obj._setHandle(0);
    return bResult;
}

void C2DSpr::resetTexInfo() {
    if (m_pTexName[0]) {
        m_pTexInfo = CRsrcDB::getRsrcTex(m_pTexName, NAME_CASE_SENSITIVE);
        if (!m_pTexInfo) m_pTexInfo = CRsrcDB::getRsrcTex("error", NAME_CASE_SENSITIVE);
    }
}

void C2DSpr::_main(UINT nLevel) {
    if (m_p2DOSLCL[0] && nLevel < 3) {
        for (SLContainerT<C2DO>* pContainer = m_p2DOSLCL[nLevel]->getHead(); pContainer;
             pContainer = pContainer->getNext()) {
            if (pContainer->getValue()) pContainer->getValue()->_main();
        }
    }
}

void C2DSpr::_disp(UINT nLevel) {
    INT i;
    if (m_p2DOSLCL[0] && nLevel < 3) {
        for (i = 0; i <= nLevel; i++) {
            for (SLContainerT<C2DO>* pContainer = m_p2DOSLCL[i]->getHead(); pContainer;
                 pContainer = pContainer->getNext()) {
                if (pContainer->getValue()) pContainer->getValue()->_disp();
            }
        }
    }
}

BOOL C2DSpr::init(void* pSpd, const CHAR* pBaseName) {
    SPR_HEADER* pHeader = (SPR_HEADER*)pSpd;
    CHAR* pExt;
    INT idx;
    if (!pSpd) return FALSE;
    strcpy(m_pBaseName, pBaseName);
    pExt = strchr(m_pBaseName, '.');
    if (pExt) {
        idx = pExt - m_pBaseName;
        if (idx >= 16) return FALSE;
        strncpy(m_pTexName, m_pBaseName, idx);
        m_pTexName[idx] = '\0';
    } else if (strchr(m_pBaseName, ':')) {
        strcpy(m_pTexName, m_pBaseName);
    } else {
        return FALSE;
    }
    m_pSprParts = (SPR_PARTS*)((CHAR*)pSpd + pHeader->parts_ofs);
    m_pSprGroup = (SPR_GROUP*)((CHAR*)pSpd + pHeader->group_ofs);
    m_pSprSprite = (SPR_SPRITE*)((CHAR*)pSpd + pHeader->sprite_ofs);
    m_nPartsMax = pHeader->parts_max;
    m_nGroupMax = pHeader->group_max;
    m_nSpriteMax = pHeader->sprite_max;
    return TRUE;
}

#line 315
HANDLE C2DSpr::create(UINT inNo) {
    if (inNo >= (UINT)m_nSpriteMax) return 0;
    m_pTexInfo = CRsrcDB::getRsrcTex(m_pTexName, NAME_CASE_SENSITIVE);
    if (!m_pTexInfo) m_pTexInfo = CRsrcDB::getRsrcTex("error", NAME_CASE_SENSITIVE);
    SPR_SPRITE* pSprSprite = &m_pSprSprite[inNo];
    INT nIndex = pSprSprite->pos;
    INT nSprite = pSprSprite->max;
    C2DSeqObj* p2DSeqObj = new (__FILE__ "(324)") C2DSeqObj;
    return p2DSeqObj->create(m_hBase, nSprite, m_pTexInfo, &m_pSprGroup[nIndex], m_pSprParts);
}

#line 345
HANDLE C2DSpr::createSpr(UINT inUnitNo, UINT inNo) {
    if (inNo >= (UINT)m_nSpriteMax) return 0;
    m_pTexInfo = CRsrcDB::getRsrcTex(m_pTexName, NAME_CASE_SENSITIVE);
    if (!m_pTexInfo) m_pTexInfo = CRsrcDB::getRsrcTex("error", NAME_CASE_SENSITIVE);
    SPR_SPRITE* pSprSprite = &m_pSprSprite[inNo];
    INT nIndex = pSprSprite->pos;
    INT nSprite = pSprSprite->max;
    C2DSeqObj* p2DSeqObj = new (__FILE__ "(354)") C2DSeqObj;
    return p2DSeqObj->create(m_hBase, nSprite, m_pTexInfo, &m_pSprGroup[nIndex], m_pSprParts);
}

void C2DSpr::destroy(HANDLE handle) {
    C2DO* p2DO = C2DMan::get2DOSLCManager().get2DO(handle);
    if (p2DO) {
        m_p2DOSLCList->remove(handle);
        delete p2DO;
    }
}

C2DSeqObj* C2DSpr::getSeqObj(HANDLE handle) {
    return (C2DSeqObj*)C2DMan::get2DOSLCManager().get2DO(handle);
}

void C2DSpr::enableDisp(HANDLE handle, BOOL bEnable) {
    C2DO* p2DO = C2DMan::get2DOSLCManager().get2DO(handle);
    if (p2DO) p2DO->enableDisp(bEnable);
}

}  // namespace D2

// SOURCE DATA: .data .rodata
