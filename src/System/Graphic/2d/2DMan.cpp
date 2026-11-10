

#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2Dman.h" /* MATCH: this unit names 2DMan.h so (header default "System/Graphic/2D/2DMan.h") */

#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/2d/2DSpr.h"
#include "System/Graphic/2d/2DSeq.h"
#include "System/Graphic/2d/2DLay.h"
#include "System/Graphic/2d/2DLayObj.h"
#include "Game/SceneEnv.h"
#include "Game/Debug/DebugWork.h"
#include <string.h>
#include "System/HeapMan.h"

#define SJ_FOREACH(T, pEntry, list) \
    for (T *pEntry = (list).getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry; \
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL)

extern INT SCREEN_HEIGHT;

template <class T> static inline T& _ref(T& v) { return v; }

namespace D2 {







C2DMan* C2DMan::m_pInstance = NULL;
C2DBaseList* C2DMan::m_p2DBaseList = NULL;
C2DOSLCManager* C2DMan::m_p2DOSLCManager = NULL;
TIMEQ C2DMan::m_qTimeD;
INT C2DMan::m_nLayer;
INT C2DMan::m_fg2DViewer;
C2DMan::D2MAN_LEVEL C2DMan::m_nLevel;
C2DMan::D2MAN_LEVEL C2DMan::m_nLevelReq;

C2DMan::C2DMan(CTask* p) : CTaskSecondary(p) {
#line 49
    m_p2DBaseList = new KH_AT("(49)") C2DBaseList;
    _ref(m_p2DOSLCManager) = new KH_AT("(50)") C2DOSLCManager;
    m_p2DOSLCManager->createContainer();
    m_nLayer = 11;
    m_nLevel = D2MAN_LEVEL0;
    m_nLevelReq = D2MAN_LEVEL0;
    m_fg2DViewer = 0;
    C2DSpr::initializeList();
    C2DBase::initialize();
    C2DLayObj::m_uiID = 0;
    C2DSeqObj::m_uiID = 0;
}

C2DMan::~C2DMan() {
    while (_ref(m_p2DBaseList)->getListSize()) {
        C2DBase* pEntry = m_p2DBaseList->getHead();
        m_p2DBaseList->remove(*pEntry);
        delete pEntry;
    }
    delete m_p2DBaseList;
    m_p2DBaseList = NULL;
    m_pInstance = NULL;
    C2DSpr::destroyAll();
    delete m_p2DOSLCManager;
    m_p2DOSLCManager = NULL;
}

void C2DMan::changeLevel(D2MAN_LEVEL nLevel) {
    if (m_pInstance) {
        m_nLevel = nLevel;
        C2DSpr::change2DSeqObjList(nLevel);
    }
}

C2DOSLCManager& C2DMan::get2DOSLCManager() {
    return *m_p2DOSLCManager;
}

void C2DMan::resetTexInfo() {}

HANDLE C2DMan::init(void* inFile, const CHAR* pBaseName) {
    C2DBase* p2DBase;
    char chrTmp[8];

    if (!m_pInstance) {
        TRACE("C2DMan: C2DMan\x96\xa2\x8b\x4e\x93\xae\n");
        return 0;
    }
    p2DBase = NULL;
    switch (*(UINT*)inFile) {
    case 0x44525053:
#line 171
        p2DBase = new KH_AT("(171)") C2DSpr;
        break;
    case 0x44514553:
#line 174
        p2DBase = new KH_AT("(174)") C2DSeq;
        break;
    case 0x4459414C:
#line 177
        p2DBase = new KH_AT("(177)") C2DLay;
        break;
    }
    if (!p2DBase) {
        TRACE("C2DMan: \x8d\xec\x90\xac\x82\xc5\x82\xab\x82\xb8\n");
        return 0;
    }
    if (!p2DBase->init(inFile, pBaseName)) {
        TRACE("C2DMan: \x8f\x89\x8a\xfa\x89\xbb\x83\x47\x83\x89\x81\x5b\n");
        delete p2DBase;
        return 0;
    }
    m_p2DBaseList->pushBack(*p2DBase);
    *(UINT*)chrTmp = *(UINT*)inFile;
    chrTmp[4] = 0;
    TRACE_COL((TtyColor)0x21, "C2DMan: %s,%s\x82\xf0\x93\x6f\x98\x5e %d\n", pBaseName, chrTmp,
              m_p2DBaseList->getListSize());
    return p2DBase->getBaseHandle();
}

HANDLE C2DMan::create(const CHAR* pBaseName, UINT inNo) {
    HANDLE handle;

    if (!m_pInstance) return 0;
    handle = 0;
    SJ_FOREACH(C2DBase, pEntry, *m_p2DBaseList) {
        if (!strcmp(pEntry->getBaseName(), pBaseName)) {
            handle = pEntry->create(inNo);
            break;
        }
    }
    return handle;
}

HANDLE C2DMan::create(const CHAR* pBaseName, UINT inNo, SeqControl* pSeqControl) {
    HANDLE handle;

    if (!m_pInstance) return 0;
    handle = 0;
    SJ_FOREACH(C2DBase, pEntry, *m_p2DBaseList) {
        if (!strcmp(pEntry->getBaseName(), pBaseName)) {
            handle = pEntry->create(inNo, pSeqControl);
            break;
        }
    }
    return handle;
}

HANDLE C2DMan::create(HANDLE hBaseHandle, UINT inNo) {
    HANDLE handle;

    if (!m_pInstance) return 0;
    handle = 0;
    SJ_FOREACH(C2DBase, pEntry, *m_p2DBaseList) {
        if (pEntry->getBaseHandle() == hBaseHandle) {
            handle = pEntry->create(inNo);
            break;
        }
    }
    return handle;
}

HANDLE C2DMan::createSeq(const CHAR* pBaseName, UINT inUnitNo, UINT inNo, SeqControl* pSeqControl) {
    HANDLE handle;

    if (!m_pInstance) return 0;
    handle = 0;
    SJ_FOREACH(C2DBase, pEntry, *m_p2DBaseList) {
        if (!strcmp(pEntry->getBaseName(), pBaseName)) {
            handle = pEntry->createSeq(inUnitNo, inNo, pSeqControl);
            break;
        }
    }
    return handle;
}

HANDLE C2DMan::createFnt(const CHAR* pBaseName, C2DFnt* p2DFnt, SeqControl* pSeqControl) {
    HANDLE handle;

    if (!m_pInstance) return 0;
    handle = 0;
    SJ_FOREACH(C2DBase, pEntry, *m_p2DBaseList) {
        if (!strcmp(pEntry->getBaseName(), pBaseName)) {
            handle = ((C2DSeq*)pEntry)->createFnt(p2DFnt, pSeqControl);
            break;
        }
    }
    return handle;
}

HANDLE C2DMan::replaceSeq(HANDLE hCurrent, const CHAR* pBaseName, UINT inNo, SeqControl* pSeqControl) {
    HANDLE handle;

    if (!m_pInstance) return 0;
    handle = 0;
    SJ_FOREACH(C2DBase, pEntry, *m_p2DBaseList) {
        if (!strcmp(pEntry->getBaseName(), pBaseName)) {
            handle = ((C2DSeq*)pEntry)->replaceSeq(hCurrent, inNo, pSeqControl);
            break;
        }
    }
    return handle;
}

HANDLE C2DMan::replaceFnt(HANDLE hCurrent, const CHAR* pBaseName, C2DFnt* p2DFnt, SeqControl* pSeqControl) {
    HANDLE handle;

    if (!m_pInstance) return 0;
    handle = 0;
    SJ_FOREACH(C2DBase, pEntry, *m_p2DBaseList) {
        if (!strcmp(pEntry->getBaseName(), pBaseName)) {
            handle = ((C2DSeq*)pEntry)->replaceFnt(hCurrent, p2DFnt, pSeqControl);
            break;
        }
    }
    return handle;
}

HANDLE C2DMan::createSpr(const CHAR* pBaseName, UINT inUnitNo, UINT inNo) {
    HANDLE handle;

    if (!m_pInstance) return 0;
    handle = 0;
    SJ_FOREACH(C2DBase, pEntry, *m_p2DBaseList) {
        if (!strcmp(pEntry->getBaseName(), pBaseName)) {
            handle = pEntry->createSpr(inUnitNo, inNo);
            break;
        }
    }
    return handle;
}

void C2DMan::setLayer(INT inLayer) {
    if (m_pInstance) m_nLayer = inLayer;
}

void C2DMan::destroy(HANDLE handle) {
    if (!m_pInstance || !handle) return;
    C2DO* p2DO = get2DOSLCManager().get2DO(handle);
    if (p2DO && C2DSpr::deleteList(*p2DO)) delete p2DO;
}

void C2DMan::destroyBase(HANDLE handleBase) {
    if (!m_pInstance || !handleBase) return;
    SJ_FOREACH(C2DBase, pEntry, *m_p2DBaseList) {
        if (pEntry->getBaseHandle() == handleBase) {
            m_p2DBaseList->remove(*pEntry);
            delete pEntry;
            return;
        }
    }
}

BOOL C2DMan::getLayPosition(const CHAR* pLadName, UINT inNo, UINT inIdx, FVECTOR& outPos, BOOL bLocal) {
    if (!m_pInstance) return FALSE;
    SJ_FOREACH(C2DBase, pEntry, *m_p2DBaseList) {
        if (!strcmp(pEntry->getBaseName(), pLadName)) {
            LAY_SCREEN* pScreen = (LAY_SCREEN*)((UINT)inIdx * sizeof(LAY_SCREEN) + (UINT)((C2DLay*)pEntry)->getLayScreen(inNo));
            outPos.x = pScreen->x;
            outPos.y = pScreen->y;
            outPos.w = 1.0f;
            outPos.z = 0.0f;
            if (bLocal) {
                outPos.x -= 256.0f;
                outPos.y -= _ref(SCREEN_HEIGHT) / 2;
            }
            return TRUE;
        }
    }
    return FALSE;
}

void C2DMan::main() {
    if (m_pInstance) {
        if (!(CSceneEnv::getStopTypeBit() & 0x10000)) {
            m_qTimeD = getFrameRate();
            C2DSpr::_main(m_nLevel);
        }
        m_nLayer = 11;
    }
}

void C2DMan::disp() {
    if (m_pInstance && DebugWork::g_dw.sys.m_iFukidasiDisp) C2DSpr::_disp(m_nLevel);
}

BOOL C2DMan::isEnd(HANDLE handle) {
    if (!m_pInstance) return FALSE;
    SLContainerT<C2DO>* pContainer = get2DOSLCManager().getContainer(handle);
    if (!pContainer) return FALSE;
    C2DBase* pBase = getBase(pContainer->getGeneral());
    if (!pBase) return FALSE;
    return pBase->isEnd(handle);
}

TIMEQ C2DMan::getTime(HANDLE handle) {
    if (!m_pInstance) return 0;
    SLContainerT<C2DO>* pContainer = get2DOSLCManager().getContainer(handle);
    if (!pContainer) return 0;
    C2DBase* pBase = getBase(pContainer->getGeneral());
    if (!pBase) return 0;
    return pBase->getTime(handle);
}

TIMEQ C2DMan::getTimeMax(HANDLE handle) {
    if (!m_pInstance) return 0;
    SLContainerT<C2DO>* pContainer = get2DOSLCManager().getContainer(handle);
    if (!pContainer) return 0;
    C2DBase* pBase = getBase(pContainer->getGeneral());
    if (!pBase) return 0;
    return pBase->getTimeMax(handle);
}

void C2DMan::enableDisp(HANDLE handle, BOOL bEnable) {
    if (!m_pInstance) return;
    SLContainerT<C2DO>* pContainer = get2DOSLCManager().getContainer(handle);
    if (!pContainer) return;
    C2DBase* pBase = getBase(pContainer->getGeneral());
    if (!pBase) return;
    pBase->enableDisp(handle, bEnable);
}

void C2DMan::changeSeqTime(HANDLE handle, UINT inTime) {
    if (!m_pInstance) return;
    C2DO* p2DO = get2DOSLCManager().get2DO(handle);
    if (p2DO) p2DO->changeTimeExt(inTime * 50);
}

void C2DMan::changeSeqTexture(HANDLE handle, INT inSprIdx, CHAR* pTexName, G_UV* pStart, G_UV* pEnd) {
    if (!m_pInstance) return;
    C2DO* p2DO = get2DOSLCManager().get2DO(handle);
    if (p2DO) p2DO->changeTexture(inSprIdx, pTexName, pStart, pEnd);
}

void C2DMan::changeSeqTexture(HANDLE handle, INT inSeqIdx, INT inSprIdx, CHAR* pTexName, G_UV* pStart, G_UV* pEnd) {
    if (!m_pInstance) return;
    C2DO* p2DO = get2DOSLCManager().get2DO(handle);
    if (p2DO) p2DO->changeTexture(inSeqIdx, inSprIdx, pTexName, pStart, pEnd);
}

void C2DMan::restartSeq(HANDLE handle, FLOAT fStart) {
    if (!m_pInstance) return;
    C2DO* p2DO = get2DOSLCManager().get2DO(handle);
    if (p2DO) p2DO->restart((INT)fStart);
}

void C2DMan::changeSeqLayer(HANDLE handle, INT nLayer) {
    if (!m_pInstance) return;
    C2DO* p2DO = get2DOSLCManager().get2DO(handle);
    if (p2DO) p2DO->changeLayer(nLayer);
}

C2DSeqObj* C2DMan::getSeqObj(HANDLE handle) {
    if (!m_pInstance) return NULL;
    SLContainerT<C2DO>* pContainer = get2DOSLCManager().getContainer(handle);
    return pContainer ? (C2DSeqObj*)pContainer->getValue() : NULL;
}

C2DBase* C2DMan::getBase(HANDLE handleBase) {
    if (!m_pInstance) return NULL;
    SJ_FOREACH(C2DBase, pEntry, *m_p2DBaseList) {
        if (pEntry->getBaseHandle() == handleBase) return pEntry;
    }
    return NULL;
}

INT C2DMan::getIdMax(const CHAR* pBaseName, UINT inType, UINT inUnitNo) {
    if (!m_pInstance) return 0;
    SJ_FOREACH(C2DBase, pEntry, *m_p2DBaseList) {
        if (!strcmp(pEntry->getBaseName(), pBaseName)) {
            INT nMax = 0;
            switch (inType) {
            case 0:
                nMax = pEntry->getIdMax();
                break;
            case 1:
                nMax = pEntry->getSeqIdMax(inUnitNo);
                break;
            case 2:
                nMax = pEntry->getSprIdMax(inUnitNo);
                break;
            case 3:
                nMax = pEntry->getUnitIdMax();
                break;
            }
            return nMax;
        }
    }
    return 0;
}

void C2DMan::report() {}

}  // namespace D2
// SOURCE DATA: .data .rodata
