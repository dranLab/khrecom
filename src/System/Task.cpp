#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/CoordMan.h"
#include "System/Graphic/Gprim.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"






#define _ASSERT_STR(e) #e
#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, _ASSERT_STR(e), 0)



CTask::CTask(CTask* pParent, INT nProcPrio)
    : m_nProcPrio(nProcPrio), m_pParent(NULL), m_listChildren(CTaskMan::getSLCManager()) {
    _nZeroMember128 = 0;
    if (!pParent) {
        m_hThis = -1;
    } else {
        pParent->addChild(*this);
    }
    CMainApp::incObjCount();
    m_pszDebugInfo = CHeapMan::getDebugComment(this);
    if (!m_pszDebugInfo) {
        m_pszDebugInfo = "";
    }
}

CTask::~CTask() {
    deleteAllChildren();
    removeFromParent();
    if (isSysTask()) {
        removeFromSystem();
    }
    if (isMainTask()) {
        CTaskMan::removeMainTask(*this);
    }
    if (isCoordManaged() && m_pCrds) {
        m_pCrds->setSysFix(FALSE, CCoords::_PP_PRIMARY);
        delete m_pCrds;
    }
    CMainApp::decObjCount();
}

void CTask::invokeMain(TaskMainParam& param) {
    if ((param._nFlag & 1) && !(m_nInfo & TASK_INFO_IGNOREPAUSE)) {
        clearTimeLag();
        return;
    }
    TIMEQ nLag = isIgnorePlaySpeed() ? param._nSysLag : param._nLag;
    if (nLag <= 0) {
        clearTimeLag();
        return;
    }
    m_nLagStock += nLag;
    if (canProcMain(param)) {
        m_cFrameSkip = 0;
        m_nTimeLag = m_nLagStock;
        m_nLagStock = 0;
        main();
    } else {
        m_cFrameSkip++;
    }
}

void CTask::invokeDispAll(INT nMode) {
    if (isInvisible()) {
        return;
    }
    clearLastClip();
    disp();
    for (SLContainerT<CTask>* pContainer = m_listChildren.getHead(); pContainer; pContainer = pContainer->getNext()) {
        if (pContainer->getValue()) {
            pContainer->getValue()->invokeDispAll(nMode);
        }
    }
}

void CTask::invokeSweepChildren() {
    after();
    for (SLContainerT<CTask>* pContainer = m_listChildren.getHead(); pContainer; pContainer = pContainer->getNext()) {
        CTask* pTask = pContainer->getValue();
        if (pTask) {
            if (pTask->isOut()) {
                pTask->setParent(NULL);
                pContainer->setValue(NULL);
                delete pTask;
            } else {
                pTask->invokeSweepChildren();
            }
        }
    }
    m_listChildren.sweep();
}

BOOL CTask::canProcMain(TaskMainParam& param) {
    INT nPrio = m_nProcPrio;
    if (nPrio & PRI_BIT_ALWAYS) {
        return TRUE;
    }
    if (isForceProcNextMain()) {
        m_nInfo &= ~TASK_INFO_FORCEPROC;
        return TRUE;
    }
    if (nPrio & PRI_BIT_DEFAULT) {
        if (m_pParent) {
            return !m_pParent->m_cFrameSkip;
        }
        nPrio = PRI_BIT_SOMETIMES;
    }
    INT nInterval = 7;
    if (nPrio & PRI_BIT_SOMETIMES) {
        INT nLevel = nPrio & PRI_SOMETIMES_MASK;
        nInterval = nLevel + 2;
        if (isClippedLastTime() && !isIgnoreClipOut()) {
            nInterval = 15;
        } else {
            nInterval -= param._nProcLevel;
        }
    }
    if (m_cFrameSkip >= nInterval) {
        return TRUE;
    }
    if (param._nFlag & 2) {
        return FALSE;
    }
    INT nRnd = param._nRnd = (param._nRnd + 7) & 0xF;
    return nRnd * nInterval < 15;
}

TIMEQ CTask::getDispFrameRate() const {
    return CMainApp::getDispFrameRate();
}

void CTask::createCoords() {
    if (!m_pCrds || !isCoordManaged()) {
#line 214
        m_pCrds = new (__FILE__ "(214)") CCoords(isSecondaryObj() ? CCoords::_PP_SECONDARY : CCoords::_PP_PRIMARY);
        ASSERT(m_pCrds);
        manageCoord(TRUE);
    }
    if (isEnableCoordCalc()) {
        m_pCrds->setSysFix(TRUE, isSecondaryObj() ? CCoords::_PP_SECONDARY : CCoords::_PP_PRIMARY);
    }
}

void CTask::addCoord(UINT nNum, CCoord* pParent) {
    createCoords();
    m_pCrds->addCoord(nNum, pParent);
    manageCoord(TRUE);
}

void CTask::addCoord(UINT nNum) {
    CTask* pMap = CTaskMan::getSysTask(2);
    CCoord* pCoord = NULL;
    if (pMap && this != pMap) {
        CCoords* pCoords = pMap->getCoords();
        if (pCoords) {
            pCoord = &pCoords->Crd(0);
        }
    }
    addCoord(nNum, pCoord);
}

void CTask::setCoord(CCoords& crds) {
    if (isCoordManaged() && m_pCrds) {
        delete[] m_pCrds;
    }
    m_pCrds = &crds;
    manageCoord(FALSE);
}

INT CTask::getCoordNum() const {
    if (m_pCrds) {
        return m_pCrds->getCoordNum();
    }
    return 0;
}

void CTask::delCoord() {
    if (isCoordManaged()) {
        delete m_pCrds;
        m_pCrds = NULL;
        manageCoord(FALSE);
    }
}

CCoord& CTask::Crd(UINT nIndex) {
    if (m_pCrds) {
        return m_pCrds->Crd(nIndex);
    }
    return CCoordMan::getExceptCoord(TRUE);
}

const CCoord& CTask::Crd(UINT nIndex) const {
    if (m_pCrds) {
        return m_pCrds->Crd(nIndex);
    }
    return CCoordMan::getExceptCoord(TRUE);
}

void CTask::calcCoord(TIMEQ nTQ) {
    if (m_pCrds && isEnableCoordCalc()) {
        m_pCrds->update(nTQ);
    }
}

FMATRIX& CTask::getLocal(UINT nIndex) {
    return Crd(nIndex).m_Local;
}

const FMATRIX& CTask::getWorld(UINT nIndex) const {
    return Crd(nIndex).m_World;
}

void CTask::setLastClip(INT c) {
    if (c < 0) {
        if (!m_cLastClip) {
            m_cLastClip = -1;
        }
    } else {
        m_cLastClip = 1;
    }
}

void CTask::enableCoordCalc(BOOL bFlag) {
    if (!bFlag) {
        m_nInfo |= TASK_INFO_NOCOORDCALC;
    } else {
        m_nInfo &= ~TASK_INFO_NOCOORDCALC;
    }
    if (m_pCrds) {
        m_pCrds->setSysFix(bFlag, isSecondaryObj() ? CCoords::_PP_SECONDARY : CCoords::_PP_PRIMARY);
    }
}

void CTask::calcCoordAll(TIMEQ nTQ) {
    if (isOut()) {
        return;
    }
    calcCoord(nTQ);
    for (SLContainerT<CTask>* pContainer = m_listChildren.getHead(); pContainer; pContainer = pContainer->getNext()) {
        if (pContainer->getValue()) {
            pContainer->getValue()->calcCoordAll(nTQ);
        }
    }
}

void CTask::adjustCoord() {
    if (m_pCrds) {
        m_pCrds->touch();
    }
    calcCoordAll(CMainApp::getTickCount());
}

void CTask::adjustSelfCoord() {
    if (m_pCrds) {
        m_pCrds->touch();
    }
    calcCoord(CMainApp::getTickCount());
}

BOOL CTask::addToParent(CTask& inParent) {
    return inParent.addChild(*this);
}

HANDLE CTask::addChild(CTask& inTask) {
    SLContainerT<CTask>* pNew;
    HANDLE hTask;
    if (inTask.getParent()) {
        return -1;
    }
    pNew = m_listChildren.pushBack(inTask);
    if (pNew) {
        inTask.setParent(this);
        hTask = pNew->getHandle();
        inTask.setHandle(hTask);
        return hTask;
    }
    return -1;
}

BOOL CTask::removeChild(HANDLE hTask) {
    CTask* pTask = CTaskMan::getTask(hTask);
    return pTask == NULL ? FALSE : removeChild(*pTask);
}

static inline BOOL _clearChild(CTaskSLCList& list, HANDLE hTask) {
    for (SLContainerT<CTask>* pContainer = list.getHead(); pContainer; pContainer = pContainer->getNext()) {
        if (hTask == pContainer->getHandle()) {
            pContainer->setValue(NULL);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CTask::removeChild(CTask& inTask) {
    BOOL bResult = _clearChild(m_listChildren, inTask.getHandle());
    if (bResult) {
        inTask.setParent(NULL);
    }
    return bResult;
}

BOOL CTask::removeFromParent() {
    if (m_pParent) {
        return m_pParent->removeChild(*this);
    }
    return FALSE;
}

BOOL CTask::deleteChild(HANDLE hTask) {
    CTask* pTask = CTaskMan::getTask(hTask);
    return pTask == NULL ? FALSE : deleteChild(*pTask);
}

BOOL CTask::deleteChild(CTask& inTask) {
    BOOL bResult = removeChild(inTask);
    if (bResult) {
        delete &inTask;
    }
    return bResult;
}

void CTask::deleteAllChildren() {
    for (SLContainerT<CTask>* __p = m_listChildren.getHead(); __p; __p = __p->getNext()) {
        if (__p->getValue()) {
            delete __p->getValue();
            __p->setValue(NULL);
        }
    }
    m_listChildren.removeAll();
}

BOOL CTask::declareSysTask(UINT nID) {
    return CTaskMan::registerSysTask(nID, *this);
}

BOOL CTask::removeFromSystem() {
    return CTaskMan::unregisterSysTask(*this);
}

CTask* CTask::getSysTask(UINT nID) {
    return CTaskMan::getSysTask(nID);
}

void CTask::removeMainTask() {
    CTaskMan::removeMainTask(*this);
}
// SOURCE DATA: .rodata
