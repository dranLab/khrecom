
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/PathDataCamera.h"


#include "System/TaskMan.h"



#include "System/Graphic/Str.h"
#define DEBUGPATHDATA_H_PATH "Game/Debug/DebugPathData.h" /* MATCH: the original includes it under this name */
#include "Game/Debug/DebugPathData.h"
#include "System/Graphic/PathData.h"
#include "System/Graphic/HermitePath.h"
#include "System/TaskMan.h"
#include "System/HeapMan.h"





















#line 30
CPathDataCamera::CPathDataCamera(CTask* p, const CHAR* pszInName, INT iKind, const CTask* pTarget, INT iNum,
                                 const CTask* pParent) : CTask(p, 0x4000) {
    m_pPathData = new KH_AT("(32)") CPathData(pszInName);
    KH_DEBUG(if (!m_pPathData) AssertApp(__FILE__, __LINE__, "m_pPathData", 0);)
    m_iStatus = 1;
    _init(iKind, (CTask*)pTarget, iNum, (CTask*)pParent);
}

#line 41
CPathDataCamera::CPathDataCamera(CTask* p, const PATH_DATA* pPathData, INT iKind, const CTask* pTarget, INT iNum,
                                 const CTask* pParent) : CTask(p, 0x4000) {
    m_pPathData = new KH_AT("(43)") CPathData(pPathData);
    KH_DEBUG(if (!m_pPathData) AssertApp(__FILE__, __LINE__, "m_pPathData", 0);)
    m_iStatus = 1;
    _init(iKind, (CTask*)pTarget, iNum, (CTask*)pParent);
}

CPathDataCamera::CPathDataCamera(CTask* p, const CPathData* pCPathData, INT iKind, const CTask* pTarget, INT iNum,
                                 const CTask* pParent) : CTask(p, 0x4000) {
    m_pPathData = (CPathData*)pCPathData;
    m_iStatus = 0;
    _init(iKind, (CTask*)pTarget, iNum, (CTask*)pParent);
}

CPathDataCamera::~CPathDataCamera() {
    TRACE("CPathDataCamera::Deathtruct -> This:%08x\n", this);
    if (m_pHPC) m_pHPC->out();
    if (m_iStatus & 1) {
        if (m_pPathData) delete m_pPathData;
        m_pPathData = NULL;
    }
    if (m_pHPM) delete m_pHPM;
    m_pHPM = NULL;
    if (m_DDPathCamArrow) delete m_DDPathCamArrow;
    m_DDPathCamArrow = NULL;
}

#line 85
void CPathDataCamera::_init(INT iKind, CTask* pTarget, INT iNum, CTask* pParent) {
    TRACE("CPathDataCamera::Construct -> This:%08x Kind:%d Target:%08x Num:%d\n", this, iKind, pTarget, iNum);
    CTask* pObj = CTaskMan::getSysTask(1);
    if (!pObj) pObj = this;
    setPriority(0x8000);
    m_pTarget = pTarget;
    m_iKind = iKind;
    m_hHPC = 0;
    m_pHPC = NULL;
    m_pHPM = NULL;
    m_iNum = iNum;
    m_iCNum = iNum;
    CTask::ignorePlaySpeed(TRUE);
    m_DDPathCamArrow = NULL;
    switch (iKind) {
    case 2:
#line 109
        m_pHPC = new KH_AT("(109)") CHermitePathCamera(pObj, m_pPathData->getPathVectorData(m_iNum), m_pTarget, NULL, 1, pParent);
        KH_DEBUG(if (!m_pHPC) AssertApp(__FILE__, __LINE__, "m_pHPC", 0);)
        break;
    case 1:
#line 115
        m_pHPC = new KH_AT("(115)") CHermitePathCamera(pObj, m_pPathData->getPathVectorData(m_iNum), m_pTarget, NULL, 0, pParent);
        KH_DEBUG(if (!m_pHPC) AssertApp(__FILE__, __LINE__, "m_pHPC", 0);)
        break;
    case 0:
    case 3:
    case 4: {
        INT iIdx = -1;
        INT iType;
        for (; m_iNum < m_pPathData->getPathNum(); m_iNum++) {
            iIdx = m_pPathData->getLinkChild(m_iNum, -1);
            if (iIdx >= 0) break;
        }
        if (iIdx < 0) {
#line 135
            KH_DEBUG(if (!(!("Child(Camera)Path Nothing!"))) AssertApp(__FILE__, __LINE__, "! (\"Child(Camera)Path Nothing!\")", 0);)
        }
        m_iCNum = iIdx;
        m_pTarget = this;
        addCoord(1);
        vu0UnitMatrix(Crd(0).m_Local);
        if (pParent && pParent->getCoordNum() > 0) Crd(0).setParent(&pParent->Crd(0));
        if (m_pPathData->getPathVectorNum(m_iNum) >= 2) {
#line 149
            m_pHPM = new KH_AT("(149)") CHermitePath;
            KH_DEBUG(if (!m_pHPM) AssertApp(__FILE__, __LINE__, "m_pHPM", 0);)
            m_pHPM->makePathFromPathVector(m_pPathData->getPathVectorData(m_iNum));
            iType = 1;
            if (m_iKind) iType = (m_iKind == 4) ? 2 : 0;
            update(0.0f, FALSE);
        } else {
            FVECTOR vPos = m_pPathData->getPathPosition(m_iNum, 0);
            vPos.w = 1.0f;
            iType = 0;
            Crd(0).m_Local.setTrans(vPos);
            m_iKind = 1;
        }
        Crd(0).touch();
        Crd(0).fix(0);
#line 192
        m_pHPC = new KH_AT("(192)") CHermitePathCamera(pObj, m_pPathData->getPathVectorData(iIdx), m_pTarget, m_pHPM, iType, pParent);
        KH_DEBUG(if (!m_pHPC) AssertApp(__FILE__, __LINE__, "m_pHPC", 0);)
        break;
    }
    default:
#line 198
        KH_DEBUG(if (!(!("Kind Unknown!"))) AssertApp(__FILE__, __LINE__, "! (\"Kind Unknown!\")", 0);)
        break;
    }
    if (m_pHPC) m_hHPC = m_pHPC->getHandle();
}

void CPathDataCamera::main() {
    if (!(m_iStatus & 2)) update(getFrameRateF(), TRUE);
}

void CPathDataCamera::update(FLOAT fr, BOOL bUpdate) {
    FVECTOR vPos;
    switch (m_iKind) {
    case 3:
    case 4:
        if (bUpdate) {
            PATH_PARAM sParam;
            CPathVector* p = m_pPathData->getPathVectorData(m_iNum);
            p->getPathParam(sParam);
            if (sParam.iFlags & 0x4000000) {
                INT iBlock0 = m_pHPM->getPathBlock();
                INT iBlock1 = iBlock0 + 1;
                FLOAT fVSpeed0 = p->getPathSpeed(iBlock0);
                FLOAT fVSpeed1 = p->getPathSpeed(iBlock1);
                FLOAT fSpeed = (fVSpeed1 - fVSpeed0) * m_pHPM->getHermiteCoefficient(iBlock0, m_pHPM->getPathProgress()) + fVSpeed0;
                m_pHPM->setSpeed(fSpeed * 0.0046296297f);
            }
            m_pHPM->update(fr);
        }
    case 0:
        if (m_DDPathCamArrow) {
            FVECTOR vRot;
            vRot.init0();
            m_pHPM->getNowRot(vRot, 1.0f);
            vRot.z = m_pHPM->getNowRoll();
            vRot.w = 5.0f;
            m_DDPathCamArrow->setCoord(NULL, &vRot);
        }
        if (m_iKind == 0 && bUpdate) {
            PATH_PARAM sParam;
            m_pPathData->getPathParam(m_iCNum, sParam);
            if (!(sParam.iFlags & 0x20000000)) break;
        }
        m_pHPM->getNowPos(vPos);
        Crd(0).m_Local.setTrans(vPos);
        Crd(0).touch();
        break;
    case 1:
    case 2:
        break;
    }
}

void CPathDataCamera::disp() {
    if (CDebugPathData::isDebugPathData() && (CDebugPathData::m_iCtrlFlg & 2)) {
        if (m_pHPM && m_pHPM->getPathVector()) m_pHPM->getPathVector()->dispDebugLine(0);
        if (m_DDPathCamArrow) m_DDPathCamArrow->disp();
    }
}

BOOL CPathDataCamera::isEnd() {
    return m_pHPC->isEnd();
}

#line 309
void CPathDataCamera::entryDebugArrow() {
    if (!m_DDPathCamArrow && getCoordNum() > 0) {
        FVECTOR vRot;
#line 314
        m_DDPathCamArrow = new KH_AT("(314)") CDbgArrow;
        KH_DEBUG(if (!m_DDPathCamArrow) AssertApp(__FILE__, __LINE__, "m_DDPathCamArrow", 0);)
        m_DDPathCamArrow->getCoord().setParent(&Crd(0));
        vRot.init0();
        vRot.w = 5.0f;
        m_DDPathCamArrow->setCoord(NULL, &vRot);
    }
}

// SOURCE DATA: .rodata
