
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h"
#include "System/Graphic/Gprim.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/Str.h"
#define DEBUGPATHDATA_H_PATH "Game/Debug/DebugPathData.h" /* MATCH: the original includes it under this name */
#include "Game/Debug/DebugPathData.h"
#include "System/Graphic/HermitePathCamera.h"
#include "System/Graphic/HermitePath.h"
#include "System/Graphic/PathData.h"
#include "System/Graphic/WorldEnv.h"
#include "Game/Collision/CollMapCamera.h"
#include "System/HeapMan.h"
#include "include_asm.h"
#include <string.h>

void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

static inline void _initVf0(FVECTOR& v) {
    asm volatile("sqc2 $vf0, 0x0(%0)\n" : : "r"(&v));
}

#line 28
CHermitePathCamera::CHermitePathCamera(CTask* p, const FVECTOR* pPath, INT iPathMax, const CTask* pTarget,
                                       const PATH_TARGET_PARAM& sTargetParam)
    : CCamera(p, -1) {
    m_iStatus = 1;
    m_pPath = new (__FILE__ "(32)") CPathVector(pPath, iPathMax);
    ASSERT(m_pPath);
    _init(pTarget, sTargetParam);
}

#line 38
CHermitePathCamera::CHermitePathCamera(CTask* p, const CPathVector* pPath, const CTask* pTarget,
                                       const CHermitePath* pHPath, INT iType, const CTask* pParent)
    : CCamera(p, -1) {
    m_iStatus = 0;
    m_pPath = (CPathVector*)pPath;
    ASSERT(m_pPath);
    PATH_TARGET_PARAM sTParam;
    memset(&sTParam, 0, sizeof(sTParam));
    PATH_PARAM sParam;
    m_pPath->getPathParam(sParam);
    sTParam.sPDParam = sParam;
    sTParam.pHPath = (CHermitePath*)pHPath;
    sTParam.iMoveMode = iType;
    sTParam.pParent = (CTask*)pParent;
    _init(pTarget, sTParam);
}

CHermitePathCamera::~CHermitePathCamera() {
    delete m_pMyHPath;
    m_pMyHPath = NULL;
    delete m_pCollMapCamera;
    m_pCollMapCamera = NULL;
    if (m_iStatus & 1) {
        delete m_pPath;
        m_pPath = NULL;
    }
}

#line 71
void CHermitePathCamera::_init(const CTask* pTarget, const PATH_TARGET_PARAM& sTargetParam) {
    setPriority(PRI_BIT_ALWAYS);
    enableCoordCalc(FALSE);
    m_pCollMapCamera = NULL;
    m_pTarget = pTarget;
    ASSERT(m_pTarget);
    m_fDistance = sTargetParam.sPDParam.fTargetDistance;
    m_fOffsetY = sTargetParam.sPDParam.fTargetOffsetY;
    m_fLookDelay = sTargetParam.sPDParam.fLookDelay;
    if (m_fLookDelay <= 0.0f || m_fLookDelay > 1.0f) {
        m_fLookDelay = 1.0f;
    }
    m_fLearpIn = sTargetParam.sPDParam.fLearpIn;
    if (m_fLearpIn <= 0.0f) {
        m_fLearpIn = 1.0f;
    } else {
        m_fLearpIn = 1.0f / m_fLearpIn;
    }
    m_fLearpOut = sTargetParam.sPDParam.fLearpOut;
    if (m_fLearpOut <= 0.0f) {
        m_fLearpOut = 1.0f;
    } else {
        m_fLearpOut = 1.0f / m_fLearpOut;
    }
    m_iPathFlags = sTargetParam.sPDParam.iFlags;
    m_pHPath = sTargetParam.pHPath;
    m_pMyHPath = NULL;
    _initVf0(m_vPos);
    _initVf0(m_vWPos);
    m_fStep = 0.0f;
    m_fStartProj = sTargetParam.sPDParam.fProj;
    m_fEndProj = sTargetParam.sPDParam.fProj2;
    setProjectionDist(m_fStartProj);
    TRACE("Proj %f %f\x93x\n", sTargetParam.sPDParam.fProj + 512.0f,
          ProjectionToViewAngle(sTargetParam.sPDParam.fProj));
    if (sTargetParam.pParent && sTargetParam.pParent->getCoordNum() > 0) {
        Crd(0).setParent(&sTargetParam.pParent->Crd(0));
    }
    if (m_iPathFlags & 0x180) {
        setCameraPriority(0x81);
        setEnable(FALSE);
        setEnable(TRUE);
    }
    if (!m_pHPath) {
        switch (sTargetParam.iMoveMode) {
        case 0:
        default:
            m_iKind = 1;
            _setMyCamPath(sTargetParam);
            break;
        case 1:
            m_iKind = 2;
            break;
        }
    } else {
        switch (sTargetParam.iMoveMode) {
        case 0:
        default:
            m_iKind = 3;
            if (m_iPathFlags & 0x20000000) {
                _setMyCamPath(sTargetParam);
            }
            break;
        case 1:
            m_iKind = 0;
            _setMyCamPath(sTargetParam);
            break;
        case 2:
            m_iKind = 4;
            _setMyCamPath(sTargetParam);
            break;
        }
    }
    m_vTargetPos = ((CCoord&)m_pTarget->Crd(0)).getWorldMatrix().getTrans();
    m_vTargetPos.y += m_fOffsetY;
    m_iStep = 1;
    FVECTOR vPos;
    FVECTOR vLook;
    updatePos(vPos, vLook, 0.0f, FALSE);
    if (m_iPathFlags & 0x80) {
        CCamera* pCam = CWorldEnv::getOldCamera();
        if (pCam && pCam->getLook()) {
            m_vLearpAimLook = m_vTargetPos;
            m_vLearpOldLook = *pCam->getLook();
            m_vLearpAimPos = m_vWPos;
            m_vLearpOldPos = pCam->Crd(0).getWorldMatrix().getTrans();
            m_fLearpAimProj = getProjectionDist();
            m_fLearpOldProj = pCam->getProjectionDist();
            m_iStep = 0;
        }
    }
    updateMatrix(vPos, vLook, updatePos(vPos, vLook, 0.0f, FALSE));
    FVECTOR _vPos = Crd(0).getWorldMatrix().getTrans();
    TRACE("START P:%f %f %f %f\n", _vPos.x, _vPos.y, _vPos.z, _vPos.w);
    if (m_pTarget) {
        FVECTOR _vLook = ((CCoord&)m_pTarget->Crd(0)).getWorldMatrix().getTrans();
        TRACE("START L:%f %f %f %f\n", _vLook.x, _vLook.y, _vLook.z, _vLook.w);
    }
    m_DDPathCamArrow.getCoord().setParent(&Crd(0));
    FVECTOR vRot;
    _initVf0(vRot);
    vRot.w = 5.0f;
    m_DDPathCamArrow.setCoord(NULL, &vRot);
}

#line 208
void CHermitePathCamera::_setMyCamPath(const PATH_TARGET_PARAM& sTargetParam) {
    m_pMyHPath = new (__FILE__ "(209)") CHermitePath;
    ASSERT(m_pMyHPath);
    m_pMyHPath->makePathFromPathVector(m_pPath);
    if (sTargetParam.sPDParam.iFlags & 1) {
        if (sTargetParam.sPDParam.fTime != 0.0f) {
            m_pMyHPath->setSpeed(m_pMyHPath->getHermiteDistanceAll() / (sTargetParam.sPDParam.fTime * 60.0f));
            m_pMyHPath->setDataSpeed();
        }
    } else if (sTargetParam.sPDParam.iFlags & 2) {
        m_pMyHPath->setSpeed(sTargetParam.sPDParam.fSpeed * 0.0046296297f);
        m_pMyHPath->setDataSpeed();
    }
    if (m_pMyHPath->getSpeed() >= 0.0f) {
        m_pMyHPath->goStartPos();
    } else {
        m_pMyHPath->goEndPos();
    }
}

void CHermitePathCamera::out() {
    FVECTOR _vPos = Crd(0).getWorldMatrix().getTrans();
    TRACE("END P:%f %f %f %f\n", _vPos.x, _vPos.y, _vPos.z, _vPos.w);
    if (m_pTarget) {
        FVECTOR _vLook = ((CCoord&)m_pTarget->Crd(0)).getWorldMatrix().getTrans();
        TRACE("END L:%f %f %f %f\n", _vLook.x, _vLook.y, _vLook.z, _vLook.w);
    }
    FLOAT fProj = getProjectionDist();
    TRACE("END Proj %f %f\x93x\n", fProj + 512.0f, ProjectionToViewAngle(fProj));
    CCamera* pCam = CWorldEnv::getOldCamera();
    if (pCam && pCam->getLook()) {
        FVECTOR _vPos = pCam->Crd(0).getWorldMatrix().getTrans();
        TRACE("END OP:%f %f %f %f\n", _vPos.x, _vPos.y, _vPos.z, _vPos.w);
        FVECTOR _vLook = *pCam->getLook();
        TRACE("END OL:%f %f %f %f\n", _vLook.x, _vLook.y, _vLook.z, _vLook.w);
        FLOAT fOProj = pCam->getProjectionDist();
        TRACE("END OProj %f %f\x93x\n", fOProj + 512.0f, ProjectionToViewAngle(fOProj));
    }
    if (m_iPathFlags & 0x100) {
        m_iStep = 2;
    } else {
        CTask::out();
    }
}

void CHermitePathCamera::main() {
    if (m_iStatus & 2) {
        return;
    }
    FLOAT fr = getFrameRateF();
    if (m_iStep == 2) {
        if (CWorldEnv::getActiveCamera() == this) {
            CCamera* pCam = CWorldEnv::getOldCamera();
            if (pCam && pCam->getLook()) {
                m_vLearpAimLook = *pCam->getLook();
                m_vLearpOldLook = *getLook();
                m_vLearpAimPos = pCam->Crd(0).getWorldMatrix().getTrans();
                m_vLearpOldPos = ((CCoord&)((const CTask*)this)->Crd(0)).getWorldMatrix().getTrans();
                m_fLearpAimProj = pCam->getProjectionDist();
                m_fLearpOldProj = getProjectionDist();
                m_iStep = 3;
            }
        }
        if (m_iStep != 3) {
            CTask::out();
            return;
        }
    }
    FVECTOR vPos;
    FVECTOR vLook;
    updateMatrix(vPos, vLook, updatePos(vPos, vLook, fr, TRUE));
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/HermitePathCamera", updatePos__18CHermitePathCameraR7FVECTORT1fi);
/* placeholder for updatePos's jump table (labels of the included assembly) */
__asm__(
    ".rdata\n"
    ".align 3\n"
    ".word .L0012B348\n"
    ".word .L0012B2A8\n"
    ".word .L0012B230\n"
    ".word .L0012B4D8\n"
    ".word .L0012B2A8\n"
    ".text\n");

void CHermitePathCamera::updateMatrix(FVECTOR& vPos, FVECTOR& vLook, FLOAT fRoll) {
    setPos(vPos);
    Crd(0).fix(0);
    if (m_iStatus & 1) {
        lookAt(vLook, (const FVECTOR*)NULL);
    } else {
        lookAt(vLook, fRoll);
    }
}

void CHermitePathCamera::disp() {
    if (CDebugPathData::isDebugPathData() && this != CWorldEnv::getActiveCamera() &&
        (CDebugPathData::m_iCtrlFlg & 2)) {
        m_pPath->dispDebugLine(0);
        m_DDPathCamArrow.disp();
    }
}

void CHermitePathCamera::noLearpOut() {
    m_iPathFlags &= ~0x100;
    if ((UINT)(m_iStep - 2) < 2) {
        CTask::out();
    }
}

#line 732
void CHermitePathCamera::initBGCheck(BOOL fgTransparency) {
    if (fgTransparency) {
        m_iStatus |= 4;
    } else {
        m_iStatus &= ~4;
    }
    if (!m_pCollMapCamera) {
        m_pCollMapCamera = new (__FILE__ "(734)") CCollMapCamera;
    }
}

BOOL CHermitePathCamera::isEnd() const {
    BOOL bRet = FALSE;
    if (m_pPath->getPathVector() && m_pPath->getPathVectorNum() > 0) {
        switch (m_iKind) {
        case 0:
        case 1:
            bRet = m_pMyHPath->isEnd();
            break;
        case 2:
            break;
        case 3:
            bRet = m_pHPath->isEnd();
            break;
        case 4:
            if (m_pMyHPath->isEnd()) {
                bRet = m_pHPath->isEnd() != 0;
            }
            break;
        }
    }
    return bRet;
}

// SOURCE DATA: .rodata
