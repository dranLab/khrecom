
#include "System/CoordMan.h"
#include "System/FifoFpl.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/Debug.h"

#define _ASSERT_STR(e) #e
#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, _ASSERT_STR(e), 0)




class CCoordsSLCManager : public ISLCManagerT<CCoords>, private CFifoFpl {
public:
    CCoordsSLCManager() { CFifoFpl::initialize(sizeof(SLContainerT<CCoords>), 0x400, NULL); }
    virtual SLContainerT<CCoords>* createContainer() {
        SLContainerT<CCoords>* pContainer = (SLContainerT<CCoords>*)allocBlock();
        if (!pContainer) {
            TRACE_COL(_TTY_RED, "CCoords \x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
        }
        return pContainer;
    }
    virtual void releaseContainer(SLContainerT<CCoords>& inContainer) { releaseBlock(&inContainer); }
    virtual SLContainerT<CCoords>* getContainer(HANDLE hID) { return (SLContainerT<CCoords>*)getValidActiveBlock(hID); }
};

typedef CSLCListT<CCoords> CCoordsList;

namespace {
CCoordsSLCManager* s_pSLCManager = NULL;
CCoordsList* s_pCoordsList1 = NULL;
CCoordsList* s_pCoordsList2 = NULL;
CCoords* s_pExceptCoords = NULL;
}

void CCoordMan::initialize() {
#line 109
    ASSERT((NULL == s_pSLCManager) && (NULL == s_pCoordsList1) && (NULL == s_pCoordsList2) && (NULL == s_pExceptCoords));

    s_pSLCManager = new (__FILE__ "(111)") CCoordsSLCManager;
    ASSERT(s_pSLCManager);

    s_pCoordsList1 = new (__FILE__ "(114)") CCoordsList(*s_pSLCManager);
    s_pCoordsList2 = new (__FILE__ "(115)") CCoordsList(*s_pSLCManager);
    s_pExceptCoords = new (__FILE__ "(116)") CCoords(CCoords::_PP_PRIMARY);
    ASSERT(s_pCoordsList1 && s_pCoordsList2 && s_pExceptCoords);
    s_pExceptCoords->addCoord(1, NULL);
    s_pCoordsList1->pushBack(*s_pExceptCoords);
}

void CCoordMan::finalize() {
    delete s_pCoordsList1;
    s_pCoordsList1 = NULL;
    delete s_pCoordsList2;
    s_pCoordsList2 = NULL;
    delete s_pExceptCoords;
    s_pExceptCoords = NULL;
    delete s_pSLCManager;
    s_pSLCManager = NULL;
}

void CCoordMan::update(TIMEQ nTQ, CCoords::_PROC_PRIO pp) {
    SLContainerT<CCoords>* pContainer;
    if (pp & CCoords::_PP_PRIMARY) {
#line 141
        ASSERT(s_pCoordsList1);
        for (pContainer = s_pCoordsList1->getHead(); pContainer; pContainer = pContainer->getNext()) {
            CCoords* p = pContainer->getValue();
            if (p && p->isSysUpdate(CCoords::_PP_PRIMARY)) {
                p->update(nTQ);
            }
        }
    }
    if (pp & CCoords::_PP_SECONDARY) {
#line 155
        ASSERT(s_pCoordsList2);
        for (pContainer = s_pCoordsList2->getHead(); pContainer; pContainer = pContainer->getNext()) {
            CCoords* p = pContainer->getValue();
            if (p && p->isSysUpdate(CCoords::_PP_SECONDARY)) {
                p->update(nTQ);
            }
        }
    }
}

HANDLE CCoordMan::add(CCoords& crds, CCoords::_PROC_PRIO pp) {
    CCoordsList* pList;
    SLContainerT<CCoords>* pNew;
    HANDLE h;

    if (crds.getSysHandle()) {
        return -1;
    }
    pList = (pp & CCoords::_PP_PRIMARY) ? s_pCoordsList1 : s_pCoordsList2;
#line 178
    ASSERT(pList);
    pNew = pList->pushBack(crds);
    h = -1;
    if (pNew) {
        h = pNew->getHandle();
        crds.setSysHandle(h);
    }
    return h;
}

BOOL CCoordMan::del(CCoords& crds) {
    HANDLE h;
    BOOL bResult;

    h = crds.getSysHandle();
    if (!h) {
        return FALSE;
    }
#line 199
    ASSERT(s_pCoordsList1 && s_pCoordsList2);
    bResult = s_pCoordsList1->remove(h);
    if (!bResult) {
        bResult = s_pCoordsList2->remove(h);
    }
    if (bResult) {
        crds.setSysHandle(0);
    }
    return bResult;
}

BOOL CCoordMan::del(HANDLE hCoords) {
    CCoords* pCrds = getCoords(hCoords);
    if (pCrds) {
        return del(*pCrds);
    }
    return FALSE;
}

CCoords* CCoordMan::getCoords(HANDLE hCoords) {
    if (s_pSLCManager && hCoords) {
        SLContainerT<CCoords>* pContainer = s_pSLCManager->getContainer(hCoords);
        if (pContainer) {
            return pContainer->getValue();
        }
    }
    return NULL;
}

void CCoordMan::resetTimeStamp() {
    SLContainerT<CCoords>* pContainer;
#line 231
    ASSERT(s_pCoordsList1);
    for (pContainer = s_pCoordsList1->getHead(); pContainer; pContainer = pContainer->getNext()) {
        if (pContainer->getValue()) {
            pContainer->getValue()->resetTimeStamp();
        }
    }
#line 239
    ASSERT(s_pCoordsList2);
    for (pContainer = s_pCoordsList2->getHead(); pContainer; pContainer = pContainer->getNext()) {
        if (pContainer->getValue()) {
            pContainer->getValue()->resetTimeStamp();
        }
    }
}

CCoord& CCoordMan::getExceptCoord(BOOL bVerbose) {
#line 254
    ASSERT(s_pExceptCoords);
    if (bVerbose) {
        TRACE("CCoordMan::getExceptCoord() ***\x8e\x77\x92\xe8\x82\xb3\x82\xea\x82\xbd" "Coord\x82\xcd\x91\xb6\x8d\xdd\x82\xb5\x82\xc8\x82\xa2\x82\xbd\x82\xdf\x97\xe1\x8a\x4f" "Coord\x82\xf0\x95\xd4\x82\xb5\x82\xdc\x82\xb7***\n");
    }
    return s_pExceptCoords->Crd(0);
}
// SOURCE DATA: .data .rodata
