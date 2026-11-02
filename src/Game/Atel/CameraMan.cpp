
#define CAMERAMAN_H_PATH "c:/KHCOM/MainApp/Game/Atel/CameraMan.h"
#define GPRIM_H_PATH "System/Graphic/Gprim.h"
#define PATHDATACAMERA_H_PATH "System/Graphic/PathDataCamera.h"
#include <string.h>
inline void* operator new(size_t, void* p) throw() { return p; }

/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "Game/Atel/CameraMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/PathDataCamera.h"

#include "Game/Atel/Avatar.h"

#include "Game/Atel/VSME.h"
namespace ATEL {

















} // namespace ATEL

#include "Game/Atel/AtelCamera.h"
#include "System/Graphic/PathCamera.h"
#include "System/Graphic/WorldEnv.h"
#include "System/MainApp.h"
#include <math.h>
#include "include_asm.h"

template <class T> static inline T& _ref(T& v) { return v; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}

extern "C" float atan2f(float y, float x);
extern "C" float powf(float x, float y);
extern "C" float tanf(float x); extern "C" void sceVu0RotMatrixX(FMATRIX& m0, const FMATRIX& m1, float rx); extern "C" void sceVu0RotMatrixY(FMATRIX& m0, const FMATRIX& m1, float ry);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2); void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
extern "C" float sceVu0InnerProduct(float* v0, float* v1);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m, const FVECTOR& v1);

#include "System/Graphic/PackMan.h"




static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

/* the parameter block of the camera commands (original name unknown) */
struct ATEL_CAMERA_PARAM {
    FVECTOR vCameraPos;      /* 0x00 */
    FVECTOR vTargetPos;      /* 0x10 */
    const CHAR* pszName;     /* 0x20 */
    FLOAT fFrame;            /* 0x24 */
    USHORT nChangeKind;      /* 0x28 */
    USHORT nChangeKind2;     /* 0x2a */
    USHORT nFlag;            /* 0x2c */
    SHORT nKind;             /* 0x2e */
    SHORT nTargetID;         /* 0x30 */
    SHORT nLocalID;          /* 0x32 */
    FLOAT fFullFrame;        /* 0x34 */
    UBYTE _m_0x38[4];
    FLOAT fStartEndFrame;    /* 0x3c */
    FLOAT fSide;             /* 0x40 */
    FLOAT fHeight;           /* 0x44 */
    FLOAT fFront;            /* 0x48 */
    ATEL::CVSM* pVsm;        /* 0x4c */
};

/* the map task (system task 2): only its fog data is used (Game/Map.h) */
struct _MAP_FOG {
    UBYTE _m_0x00[0x18];
    FLOAT screenz; /* 0x18 */
};
struct _MAP_TASK {
    UBYTE _m_0x0000[0x15e0];
    _MAP_FOG* m_pFog; /* 0x15e0 */
};

namespace ATEL {

CCameraMan* CCameraMan::m_pInstance = NULL;
CCameraDtList* CCameraMan::m_pCCameraDtList = NULL;

CCameraMan::CCameraMan(CTask* pParent) : CTask(pParent, 0x8000) {
    m_pActiveCameraDt = NULL;
    m_timerIgnorePause = 0;
    m_fgWipe = 0;
    m_frameEffKindSave = 0;
    m_fgGakaku = 0;
    m_currentGakaku = 0.0f;
    ignorePause(TRUE);
    ignorePlaySpeed(TRUE);
    TRACE("CCameraMan Create\n");
}

CCameraMan::~CCameraMan() {
    dispose();
    m_pInstance = NULL;
    TRACE("CCameraMan Delete\n");
}

#line 65
void CCameraMan::setup() {
    if (!m_pCCameraDtList) m_pCCameraDtList = new (__FILE__ "(66)") CCameraDtList;
}

void CCameraMan::dispose() {
    TRACE("CCameraMan dispose\n");
    delete m_pCCameraDtList;
    m_pCCameraDtList = NULL;
}

CCameraDt::~CCameraDt() {
    TRACE("CCameraDt Delete\n");
    if (m_pCamera) m_pCamera->out();
}

static inline FVECTOR operator+(const FVECTOR& v0, const FVECTOR& inV) {
    FVECTOR vec = v0;
    vec.x += inV.x;
    vec.y += inV.y;
    vec.z += inV.z;
    vec.w += inV.w;
    return vec;
}
static inline void _unitMatrixVu0(FMATRIX& m) {
    asm volatile("vmr32.xyzw $vf1, $vf0\n"
                 "sqc2 $vf0, 0x30(%0)\n"
                 "vmr32.xyzw $vf2, $vf1\n"
                 "sqc2 $vf1, 0x20(%0)\n"
                 "vmr32.xyzw $vf3, $vf2\n"
                 "sqc2 $vf2, 0x10(%0)\n"
                 "sqc2 $vf3, 0x0(%0)\n"
                 :
                 : "r"(&m)
                 : "memory");
}

void CCameraMan::main() {
    CCameraDt* pDt = m_pActiveCameraDt;
    if (pDt) {
        FLOAT fRate0 = getFrameRateF();
        pDt->m_fFrameCnt += fRate0;
        ICamera* pCam = pDt->m_pCamera;
        FVECTOR vec;
        FVECTOR objTrans;
        if (pDt->m_kind == 5) objTrans = pDt->m_pTargetObj->Crd(0).getWorldMatrix().getTrans();
        if (pDt->m_kind < 2) objTrans = pDt->m_pTargetObj->Crd(0).getWorldMatrix().getTrans();
        switch (pDt->m_kind) {
        case 1:
            if (pDt->m_changeKind == 16) {
                pDt->m_cameraPos = objTrans + pDt->m_relativePos;
            } else {
                pDt->m_currentCameraPos = objTrans + pDt->m_relativePos;
            }
        case 0:
        case 5:
            if (pDt->m_changeKind == 16) {
                pDt->m_targetPos = objTrans;
                pDt->m_targetPos.y += pDt->m_targetHeight;
            } else {
                pDt->m_currentTargetPos = objTrans;
                pDt->m_currentTargetPos.y += pDt->m_targetHeight;
            }
        case 2:
            if (pDt->m_changeKind == 16) {
                if (pDt->m_fFrameCnt < pDt->m_fTargetFrame) {
                    FLOAT r = 1.0f - powf(pDt->m_fChangeRate, fRate0);
                    if (pDt->m_fTargetFrame * 0.5f < pDt->m_fFrameCnt) {
                        FLOAT f = pDt->m_fFrameCnt / pDt->m_fTargetFrame - 0.5f;
                        f += f;
                        if (r < f) r = f;
                    }
                    FVECTOR vec0;
                    FVECTOR vec1;
                    FMATRIX mat0;
                    SubVector(vec, pDt->m_targetPos, pDt->m_currentTargetPos);
                    ScaleVector(vec, vec, r);
                    AddVectorXYZ(pDt->m_currentTargetPos, pDt->m_currentTargetPos, vec);
                    makeRot(vec0, pDt->m_cameraPos, pDt->m_targetPos);
                    makeRot(vec1, pDt->m_currentCameraPos, pDt->m_currentTargetPos);
                    SubVector(vec, vec0, vec1);
                    if (vec.y > 3.141592653589793) {
                        vec.y = vec.y - 6.283185307179586;
                    } else if (vec.y < -3.141592653589793) {
                        vec.y = vec.y + 6.283185307179586;
                    }
                    r *= 1.125f;
                    if (1.0f < r) r = 1.0f;
                    ScaleVector(vec, vec, r);
                    AddVectorXYZ(vec1, vec1, vec);
                    FLOAT fR0 = vec1.y;
                    if (fR0 < -3.1415927f) {
                        fR0 += 6.2831855f;
                    } else if (3.1415927f < fR0) {
                        fR0 -= 6.2831855f;
                    }
                    vec1.y = fR0;
                    _unitMatrixVu0(mat0);
                    sceVu0RotMatrixX(mat0, mat0, vec1.x);
                    sceVu0RotMatrixY(mat0, mat0, vec1.y);
                    mat0.v[3] = pDt->m_currentTargetPos;
                    vec0.set(0.0f, 0.0f, vec1.z);
                    ApplyMatrix(vec0, mat0, vec0);
                    pDt->m_currentCameraPos = vec0;
                } else {
                    pDt->m_currentCameraPos = pDt->m_cameraPos;
                    pDt->m_currentTargetPos = pDt->m_targetPos;
                }
            }
            vec = pDt->m_currentCameraPos;
            vec.w = 1.0f;
            pCam->setPos(vec);
            pDt->m_currentTargetPos.w = 1.0f;
            pCam->lookAt(pDt->m_currentTargetPos, NULL);
            break;
        case 6:
            {
                FVECTOR vec;
                if (m_pActiveCameraDt->m_fFrameCnt <= m_pActiveCameraDt->m_fStartEndFrame + 2.0f) {
                    ScaleVector(vec, m_pActiveCameraDt->m_camStEdAddSpeed, fRate0);
                    AddVectorXYZ(m_pActiveCameraDt->m_camStEdSpeed, m_pActiveCameraDt->m_camStEdSpeed, vec);
                    AddVectorXYZ(pDt->m_currentTargetPos, pDt->m_currentTargetPos, m_pActiveCameraDt->m_camStEdSpeed);
                } else if (m_pActiveCameraDt->m_fFrameCnt <=
                           m_pActiveCameraDt->m_fMidFrame + m_pActiveCameraDt->m_fStartEndFrame + 1.0f) {
                    ScaleVector(vec, m_pActiveCameraDt->m_camMidSpeed, fRate0);
                    AddVectorXYZ(pDt->m_currentTargetPos, pDt->m_currentTargetPos, vec);
                    m_pActiveCameraDt->m_camStEdSpeed = m_pActiveCameraDt->m_camMidSpeed;
                } else if (m_pActiveCameraDt->m_fFrameCnt <= m_pActiveCameraDt->m_fMidFrame + m_pActiveCameraDt->m_fStartEndFrame +
                                                                  m_pActiveCameraDt->m_fStartEndFrame + 1.0f) {
                    ScaleVector(vec, m_pActiveCameraDt->m_camStEdAddSpeed, fRate0);
                    SubVectorXYZ(m_pActiveCameraDt->m_camStEdSpeed, m_pActiveCameraDt->m_camStEdSpeed, vec);
                    AddVectorXYZ(pDt->m_currentTargetPos, pDt->m_currentTargetPos, m_pActiveCameraDt->m_camStEdSpeed);
                }
                pDt->m_currentTargetPos.w = 1.0f;
                pCam->lookAt(pDt->m_currentTargetPos, NULL);
            }
            break;
        }
    }
    if (m_timerIgnorePause) {
        if (--m_timerIgnorePause == 0) {
            CPackMan::setFrameEffect(0, 0, 0, 0);
            CMainApp::setSysPause(FALSE);
            if (m_pActiveCameraDt) m_pActiveCameraDt->m_pCamera->ignorePause(FALSE);
        }
    }
    if (m_fgGakaku) {
        FLOAT fDist = m_targetGakaku - m_currentGakaku;
        if (fDist <= 1.0 && fDist >= -1.0) {
            m_currentGakaku = m_targetGakaku;
            m_fgGakaku = 0;
        } else {
            m_currentGakaku = m_currentGakaku + getFrameRateF() * (fDist * 0.1f);
            if ((m_targetGakaku - m_currentGakaku) * fDist < 0.0f) m_currentGakaku = m_targetGakaku;
        }
        CCamera* pCam = CWorldEnv::getActiveCamera();
        if (pCam) {
            FLOAT scrz = tanf((90.0f - m_currentGakaku * 0.5f) * 6.283185307179586 / 360.0) * 256.0f - 512.0f;
            pCam->setProjectionDist(scrz);
        }
    }
}

INT CCameraMan::createCameraFix(void* pParam, INT nID) {
    ATEL_CAMERA_PARAM* p = (ATEL_CAMERA_PARAM*)pParam;
    FVECTOR oldCameraPos;
    FVECTOR oldTargetPos;
    if (m_pCCameraDtList->getListSize() >= 8) return -1;
    if (searchCameraDt(nID)) return -1;
    if (m_currentGakaku == 0.0f) {
        if (!getDefaultGakaku(m_currentGakaku)) m_currentGakaku = 50.0f;
    }
    if (CWorldEnv::getActiveCamera()) {
        oldCameraPos = CWorldEnv::getActiveCamera()->Crd(0).m_World.v[3];
        oldTargetPos = *CWorldEnv::getActiveCamera()->getLook();
    } else {
        oldCameraPos.x = 0.0f;
        oldCameraPos.y = 0.0f;
        oldCameraPos.z = 0.0f;
        oldTargetPos.x = 0.0f;
        oldTargetPos.y = 0.0f;
        oldTargetPos.z = 0.0f;
    }
#line 351
    CAtelCamera* pAtelCamera = new (__FILE__ "(351)") CAtelCamera(this, -1);
    pAtelCamera->setGakaku(m_currentGakaku);
#line 354
    CCameraDt* pCameraDt = new (__FILE__ "(354)") CCameraDt;
    pCameraDt->m_nID = nID;
    pCameraDt->m_pCamera = pAtelCamera;
    pCameraDt->m_changeKind = p->nChangeKind;
    pCameraDt->m_changeKind2 = p->nChangeKind2;
    pCameraDt->m_status = 0;
    pCameraDt->m_cameraPos = p->vCameraPos;
    pCameraDt->m_targetPos = p->vTargetPos;
    CTask* pTargetObj = getObjFromId(p->nTargetID, p->pVsm);
    if (!pTargetObj) pTargetObj = getSysTask(3);
    pCameraDt->m_pTargetObj = pTargetObj;
    if (p->nFlag & 1) pCameraDt->m_cameraPos = oldCameraPos;
    if (p->nTargetID) {
        pCameraDt->m_targetPos = pTargetObj->Crd(0).m_World.v[3];
        pCameraDt->m_targetPos.y += p->fHeight;
    }
    pCameraDt->m_fFrameCnt = 0.0f;
    if (p->fFrame > 1000.0f) {
        pCameraDt->m_fTargetFrame = p->fFrame - 1000.0f;
    } else {
        pCameraDt->m_fTargetFrame = p->fFrame * 60.0f;
    }
    pCameraDt->m_fChangeRate = powf(0.001f, 1.0f / pCameraDt->m_fTargetFrame);
    if (pCameraDt->m_changeKind != 16) {
        pCameraDt->m_currentCameraPos = pCameraDt->m_cameraPos;
        pCameraDt->m_currentTargetPos = pCameraDt->m_targetPos;
    } else {
        pCameraDt->m_currentTargetPos = oldTargetPos;
        pCameraDt->m_currentCameraPos = oldCameraPos;
    }
    _ref(m_pCCameraDtList)->pushBack(*pCameraDt);
    if (m_pActiveCameraDt) m_pActiveCameraDt->m_pCamera->stop();
    m_pActiveCameraDt = pCameraDt;
    wipeSet(pCameraDt->m_changeKind);
    return 1;
}

INT CCameraMan::onCreateCameraFix(INT nCmd, void* pParam, INT nID) {
    INT ret = createCameraFix(pParam, nID);
    if (ret != 1) return ret;
    m_pActiveCameraDt->m_kind = 0;
    m_pActiveCameraDt->m_targetHeight = 1.5f;
    main();
    return 1;
}

INT CCameraMan::onCreateCameraRelative(INT nCmd, void* pParam, INT nID) {
    INT ret = createCameraFix(pParam, nID);
    if (ret != 1) return ret;
    m_pActiveCameraDt->m_kind = 1;
    m_pActiveCameraDt->m_targetHeight = ((ATEL_CAMERA_PARAM*)pParam)->fHeight;
    m_pActiveCameraDt->m_relativePos = ((ATEL_CAMERA_PARAM*)pParam)->vCameraPos;
    main();
    return 1;
}

INT CCameraMan::onCreateCameraLookAt(INT nCmd, void* pParam, INT nID) {
    INT ret = createCameraFix(pParam, nID);
    if (ret != 1) return ret;
    m_pActiveCameraDt->m_kind = 5;
    m_pActiveCameraDt->m_targetHeight = ((ATEL_CAMERA_PARAM*)pParam)->fHeight;
    m_pActiveCameraDt->m_relativePos = ((ATEL_CAMERA_PARAM*)pParam)->vCameraPos;
    main();
    return 1;
}

INT CCameraMan::onCreateCameraFixTarget(INT nCmd, void* pParam, INT nID) {
    INT ret = createCameraFix(pParam, nID);
    if (ret != 1) return ret;
    m_pActiveCameraDt->m_kind = 2;
    main();
    return 1;
}

INT CCameraMan::onCreateCameraPath(INT nCmd, void* pParam, INT nID) {
    ATEL_CAMERA_PARAM* p = (ATEL_CAMERA_PARAM*)pParam;
    if (m_pCCameraDtList->getListSize() >= 8) return -1;
    if (searchCameraDt(nID)) return -1;
#line 476
    CPathCamera* pPathCamera = new (__FILE__ "(476)") CPathCamera(this);
    if (!pPathCamera->init(p->pszName, TRUE, FALSE, 0x80)) {
        delete pPathCamera;
        return -1;
    }
    pPathCamera->run();
#line 484
    CCameraDt* pCameraDt = new (__FILE__ "(484)") CCameraDt;
    pCameraDt->m_pCamera = pPathCamera;
    pCameraDt->m_nID = nID;
    pCameraDt->m_kind = 3;
    pCameraDt->m_status = 0;
    pCameraDt->m_changeKind = p->nChangeKind;
    pCameraDt->m_changeKind2 = p->nChangeKind2;
    _ref(m_pCCameraDtList)->pushBack(*pCameraDt);
    if (m_pActiveCameraDt) m_pActiveCameraDt->m_pCamera->stop();
    m_pActiveCameraDt = pCameraDt;
    wipeSet(pCameraDt->m_changeKind);
    return 1;
}

INT CCameraMan::onCreateCameraPathData(INT nCmd, void* pParam, INT nID) {
    ATEL_CAMERA_PARAM* p = (ATEL_CAMERA_PARAM*)pParam;
    if (m_pCCameraDtList->getListSize() >= 8) return -1;
    if (searchCameraDt(nID)) return -1;
    CTask* pTargetObj = getObjFromId(p->nTargetID, p->pVsm);
    CTask* pLocalObj = getObjFromId(p->nLocalID, p->pVsm);
    if (!pTargetObj) return -1;
#line 519
    CPathDataCamera* pPathDataCamera = new (__FILE__ "(519)") CPathDataCamera(this, p->pszName, p->nKind, pTargetObj, 0, pLocalObj);
#line 521
    CCameraDt* pCameraDt = new (__FILE__ "(521)") CCameraDt;
    pCameraDt->m_pCamera = pPathDataCamera;
    pCameraDt->m_nID = nID;
    pCameraDt->m_kind = 4;
    pCameraDt->m_status = 0;
    pCameraDt->m_changeKind = p->nChangeKind;
    pCameraDt->m_changeKind2 = p->nChangeKind2;
    _ref(m_pCCameraDtList)->pushBack(*pCameraDt);
    if (m_pActiveCameraDt) m_pActiveCameraDt->m_pCamera->stop();
    m_pActiveCameraDt = pCameraDt;
    wipeSet(pCameraDt->m_changeKind);
    return 1;
}

INT CCameraMan::onDestroyCamera(INT nCmd, void* pParam, INT nID) {
    CCameraDt* pCameraDt = searchCameraDt(nID);
    if (!pCameraDt) return -1;
    m_pCCameraDtList->remove(*pCameraDt);
    if (pCameraDt == m_pActiveCameraDt) {
        if (m_pCCameraDtList->getListSize() == 0) {
            m_pActiveCameraDt = NULL;
        } else {
            m_pActiveCameraDt = m_pCCameraDtList->getTail();
            m_pActiveCameraDt->m_pCamera->run();
            main();
        }
    }
    if (pCameraDt->m_changeKind) {
        if (pCameraDt->m_changeKind == 14) {
            wipeSet(13);
        } else if (pCameraDt->m_changeKind2 != 16) {
            wipeSet(pCameraDt->m_changeKind2);
        }
    }
    delete pCameraDt;
    ignorePause(TRUE);
    ignorePlaySpeed(TRUE);
    return 1;
}

INT CCameraMan::onDestroyCameraAll(INT nCmd, void* pParam, INT nID) {
    if (m_pActiveCameraDt) {
        if (m_pActiveCameraDt->m_changeKind2 != 16) wipeSet(m_pActiveCameraDt->m_changeKind2);
        for (CCameraDt *__p = m_pCCameraDtList->getHead(), *__pn = __p ? __p->getNext() : NULL; __p;
             __p = __pn, __pn = __pn ? __pn->getNext() : NULL) {
            delete __p;
        }
        m_pCCameraDtList->resetParam();
        m_pActiveCameraDt = NULL;
        ignorePause(TRUE);
        ignorePlaySpeed(TRUE);
        return 1;
    }
    return -1;
}

INT CCameraMan::onSetActiveCamera(INT nCmd, void* pParam, INT nID) {
    CCameraDt* pCameraDt = searchCameraDt(nID);
    if (!pCameraDt) return -1;
    pCameraDt->m_pCamera->setEnable(FALSE);
    pCameraDt->m_pCamera->setEnable(TRUE);
    m_pActiveCameraDt = pCameraDt;
    pCameraDt->m_pCamera->run();
    main();
    return 1;
}

INT CCameraMan::onChaeckExistCamera(INT nCmd, void* pParam, INT nID) {
    return searchCameraDt(nID) ? 1 : -1;
}

INT CCameraMan::onChaeckEndCamera(INT nCmd, void* pParam, INT nID) {
    CCameraDt* pCameraDt = searchCameraDt(nID);
    if (!pCameraDt) return -1;
    *(INT*)pParam = pCameraDt->m_pCamera->isEnd();
    return 1;
}

INT CCameraMan::onSetPlayerCamera(INT nCmd, void* pParam, INT nID) {
    return -1;
}

INT CCameraMan::onSetPlayerCameraNormal(INT nCmd, void* pParam, INT nID) {
    return -1;
}

INT CCameraMan::onResetPlayerCamera(INT nCmd, void* pParam, INT nID) {
    return -1;
}

INT CCameraMan::onChangeGakaku(INT nCmd, void* pParam, INT nID) {
    if (!getDefaultGakaku(m_currentGakaku)) return -1;
    FLOAT gakaku = *(FLOAT*)pParam;
    if (gakaku < 0.0f) {
        gakaku = -gakaku;
        m_currentGakaku = gakaku;
    }
    m_targetGakaku = gakaku;
    m_fgGakaku = 1;
    return 1;
}

INT CCameraMan::onResetGakaku(INT nCmd, void* pParam, INT nID) {
    if (!getDefaultGakaku(m_targetGakaku)) return -1;
    m_fgGakaku = 1;
    return 1;
}

INT CCameraMan::onCreateCameraChat(INT nCmd, void* pParam, INT nID) {
    return -1;
}

#line 714
INT CCameraMan::onAddCameraQuakeV(INT nCmd, void* pParam, INT nID) {
    CCamera* pCam = CWorldEnv::getActiveCamera();
    new (__FILE__ "(715)") CCameraQuakeV(*pCam, (CameraQuakeParam*)pParam, 0.0f);
    return 1;
}

#line 722
INT CCameraMan::onAddCameraQuakeH(INT nCmd, void* pParam, INT nID) {
    CCamera* pCam = CWorldEnv::getActiveCamera();
    new (__FILE__ "(723)") CCameraQuakeH(*pCam, (CameraQuakeParam*)pParam);
    return 1;
}

INT CCameraMan::onSetOffsetY(INT nCmd, void* pParam, INT nID) {
    CCameraDt* pCameraDt = searchCameraDt(nID);
    if (!pCameraDt) return -1;
    pCameraDt->m_targetHeight = *(FLOAT*)pParam;
    return 1;
}

INT CCameraMan::onIgnorePause(INT nCmd, void* pParam, INT nID) {
    CCameraDt* pCameraDt = searchCameraDt(nID);
    if (!pCameraDt) return -1;
    INT fg = *(INT*)pParam & 1;
    pCameraDt->m_pCamera->ignorePause(fg);
    pCameraDt->m_status = (pCameraDt->m_status & ~1) | fg;
    ignorePause(fg);
    return 1;
}

INT CCameraMan::onIgnorePlaySpeed(INT nCmd, void* pParam, INT nID) {
    CCameraDt* pCameraDt = searchCameraDt(nID);
    if (!pCameraDt) return -1;
    INT fg = *(INT*)pParam & 1;
    pCameraDt->m_pCamera->ignorePlaySpeed(fg);
    pCameraDt->m_status = (pCameraDt->m_status & ~2) | (fg << 1);
    ignorePlaySpeed(fg);
    return 1;
}

INT CCameraMan::onChangeCamTarget(INT nCmd, void* pParam, INT nID) {
    ATEL_CAMERA_PARAM* p = (ATEL_CAMERA_PARAM*)pParam;
    FVECTOR v;
    FVECTOR v2;
    FVECTOR sted;
    FVECTOR mid;
    FVECTOR len;
    FVECTOR lenMid;
    m_pActiveCameraDt->m_targetHeight = p->fHeight;
    m_pActiveCameraDt->m_targetSide = p->fSide;
    m_pActiveCameraDt->m_targetFront = p->fFront;
    m_pActiveCameraDt->m_fFullFrame = p->fFullFrame;
    m_pActiveCameraDt->m_fStartEndFrame = p->fStartEndFrame;
    m_pActiveCameraDt->m_kind = 6;
    m_pActiveCameraDt->m_fMidFrame = 0.0f;
    v.x = m_pActiveCameraDt->m_targetSide;
    v.y = m_pActiveCameraDt->m_targetHeight;
    v.z = m_pActiveCameraDt->m_targetFront;
    ApplyMatrixXYZ(v, getObjFromId(p->nTargetID, p->pVsm)->Crd(0).getWorldMatrix(), v);
    SubVector(len, v, m_pActiveCameraDt->m_currentTargetPos);
    Normalize(v, len);
    if (sceVu0InnerProduct(len.v, v.v) < 5.0f) ScaleVector(v, v, 0.01f);
    ScaleVector(v2, v, 2.0f);
    SubVector(lenMid, len, v2);
    ScaleVector(mid, lenMid, 1.0f / (m_pActiveCameraDt->m_fFullFrame - m_pActiveCameraDt->m_fStartEndFrame));
    m_pActiveCameraDt->m_camMidSpeed = mid;
    ScaleVector(sted, mid, 1.0f / m_pActiveCameraDt->m_fStartEndFrame);
    m_pActiveCameraDt->m_camStEdAddSpeed = sted;
    ScaleVector(v, sted, m_pActiveCameraDt->m_fStartEndFrame * 0.5f);
    AddVector(v, sted, v);
    ScaleVector(v, v, m_pActiveCameraDt->m_fStartEndFrame * 0.5f * 0.5f);
    ScaleVector(v, v, 2.0f);
    ScaleVector(v2, v, 2.0f);
    SubVector(lenMid, len, v2);
    if (lenMid.x != 0.0f && mid.x != 0.0f) {
        FLOAT f = lenMid.x / mid.x;
        if (m_pActiveCameraDt->m_fMidFrame < f) m_pActiveCameraDt->m_fMidFrame = f;
    }
    if (lenMid.y != 0.0f && mid.y != 0.0f) {
        FLOAT f = lenMid.y / mid.y;
        if (m_pActiveCameraDt->m_fMidFrame < f) m_pActiveCameraDt->m_fMidFrame = f;
    }
    if (lenMid.z != 0.0f && mid.z != 0.0f) {
        FLOAT f = lenMid.z / mid.z;
        if (m_pActiveCameraDt->m_fMidFrame < f) m_pActiveCameraDt->m_fMidFrame = f;
    }
    m_pActiveCameraDt->m_camStEdSpeed._q = 0;
    m_pActiveCameraDt->m_fFrameCnt = 0.0f;
    return 1;
}

INT CCameraMan::onStop(INT nCmd, void* pParam, INT nID) {
    if (!m_pActiveCameraDt) return -1;
    m_pActiveCameraDt->m_pCamera->stop();
    return 1;
}

INT CCameraMan::onRun(INT nCmd, void* pParam, INT nID) {
    if (!m_pActiveCameraDt) return -1;
    m_pActiveCameraDt->m_pCamera->run();
    return 1;
}

INT CCameraMan::onCamEffect(INT nCmd, void* pParam, INT nID) {
    wipeSet(nID);
    return 1;
}

INT CCameraMan::getDefaultGakaku(FLOAT& outGakaku) {
    _MAP_TASK* pMap = (_MAP_TASK*)getSysTask(2);
    if (!pMap) return 0;
    FLOAT scrz = pMap->m_pFog->screenz;
    outGakaku = atan2f(256.0f, scrz + 512.0f) * 114.59155902616465;
    return 1;
}

void CCameraMan::wipeSet(INT kind) {
    switch (kind & 0xF) {
    case 0:
        CPackMan::setFrameEffect(0, 0, 0, 0);
        break;
    case 6:
        CPackMan::setFrameEffect(1, 0, 0, 0);
        m_timerIgnorePause = 10;
        CMainApp::setSysPause(TRUE);
        break;
    case 7:
        CPackMan::setFrameEffect(1, 0, 0, 0);
        break;
    case 8:
        CPackMan::setFrameEffect(2, 0, 0, 0);
        break;
    }
}

void CCameraMan::wipeReset() {
    if (m_frameEffKindSave != 1) CPackMan::setFrameEffect(0, 0, 0, 0);
}

void CCameraMan::makeRot(FVECTOR& outRot, FVECTOR& inCameraPos, FVECTOR& inTargetPos) {
    FVECTOR vec0;
    FVECTOR rot0;
    SubVector(vec0, inCameraPos, inTargetPos);
    rot0.y = atan2f(vec0.x, vec0.z);
    FLOAT xyzLen = vec0.x * vec0.x + vec0.z * vec0.z;
    rot0.x = -atan2f(vec0.y, _sqrtf(xyzLen));
    xyzLen += vec0.y * vec0.y;
    rot0.z = _sqrtf(xyzLen);
    rot0.w = 0.0f;
    outRot = rot0;
}

CTask* CCameraMan::getObjFromId(INT id, CVSM* pVsm) {
    if (!pVsm) return NULL;
    if (id == -2) return getSysTask(2);
    if (id == -1) return getSysTask(3);
    return pVsm->getAvatar(id);
}

CCameraDt* CCameraMan::searchCameraDt(INT nID) {
    for (CCameraDt *pEntry = m_pCCameraDtList->getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry;
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL) {
        if (pEntry->m_nID == nID) return pEntry;
    }
    return NULL;
}

} // namespace ATEL

// SOURCE DATA: .data .rodata
