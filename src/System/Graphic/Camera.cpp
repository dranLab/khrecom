#define TASK_H_NO_SLC_OVERFLOW_MSG
#include "System/HeapMan.h"
#include "System/Graphic/Camera.h"
#include "System/Math3D.h"
extern "C" float tanf(float);
extern "C" float sinf(float);
extern "C" float atan2f(float, float);

extern INT VSYNC_TIMEQ;

class CWorldEnv {
public:
    static void pushCamera(CCamera& inCamera);
    static void popCamera(CCamera& inCamera);
    static FLOAT getScreenZ();
};

void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1);
extern "C" void sceVu0RotMatrixZ(FMATRIX& m0, FMATRIX& m1, float rz);

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

CCamera::CCamera(CTask* pParent, INT nPri) : CTaskSecondary(pParent) {
    init(nPri);
}

CCamera::~CCamera() {
    setEnable(FALSE);
}

void CCamera::init(INT nPri) {
    setPriority(0x4000);
    addCoord(1);
    _unitMatrixVu0(Crd(0).m_Local);
    Crd(0).touch();
    setCameraPriority(nPri);
    FLOAT fDist = CWorldEnv::getScreenZ();
    m_vOffset._q = 0;
    m_vLook._q = 0;
    m_vOffset.w = 0.0f;
    setProjectionDist(fDist);
    ignorePlaySpeed(TRUE);
    setEnable(TRUE);
}

void CCamera::setAnchor(CCoord* pParentCrd) {
    Crd(0).setParent(pParentCrd);
}

BOOL CCamera::setEnable(BOOL bFlag) {
    if (!bFlag) {
        CWorldEnv::popCamera(*this);
    } else {
        CWorldEnv::pushCamera(*this);
    }
}

void CCamera::lookAt(const FVECTOR& vTarget, const FVECTOR* pvUp) {
    FVECTOR vUp;
    FVECTOR vForward;
    FVECTOR vRight;

    adjustSelfCoord();
    FMATRIX& mat = Crd(0).getWorldMatrix();
    if (pvUp) {
        vUp = *pvUp;
    } else {
        vUp.set(0.0f, 1.0f, 0.0f, 0.0f);
    }
    setLook(vTarget);
    vForward = vTarget;
    SubVector(vForward, vForward, mat.v[3]);
    Normalize(vForward, vForward);
    if (vForward.x < 1e-5f && vForward.x > -1e-5f && vForward.z < 1e-5f && vForward.z > -1e-5f) {
        vUp.set(-vForward.y, 0.0f, 0.0f, 0.0f);
    }
    OuterProduct(vRight, vUp, vForward);
    Normalize(vRight, vRight);
    OuterProduct(vUp, vForward, vRight);
    mat.v[0] = vRight;
    mat.v[1] = vUp;
    mat.v[2] = vForward;
    Crd(0).recalcLocalMatrix();
}

void CCamera::lookAt(const FVECTOR& vTarget, FLOAT fBank) {
    FMATRIX matLocal;
    lookAt(vTarget, (const FVECTOR*)NULL);
    FMATRIX& mat = Crd(0).getWorldMatrix();
    _unitMatrixVu0(matLocal);
    sceVu0RotMatrixZ(matLocal, matLocal, fBank);
    MulMatrix(mat, mat, matLocal);
    Crd(0).recalcLocalMatrix();
}

void CCamera::setGakaku(FLOAT fGakaku) {
    FLOAT scrz = (90.0f - fGakaku * 0.5f) * 6.2831855f;
    scrz = tanf(scrz / 360.0f) * 256.0f - 512.0f;
    setProjectionDist(scrz);
}

FLOAT ViewAngleToProjection(FLOAT fViewAngle) {
    FLOAT fProj = 256.0f / tanf(fViewAngle * 0.008726646f);
    return fProj - 512.0f;
}

FLOAT ProjectionToViewAngle(FLOAT fProjection) {
    FLOAT fProj = atan2f(256.0f, fProjection + 512.0f) * 114.59155f;
    return fProj;
}

CCameraQuake::CCameraQuake(const CameraQuakeParam* pParam, FLOAT fGeta) {
    m_nTime = 0;
    if (pParam) {
        setup(*pParam, fGeta);
    }
}

void CCameraQuake::setup(const CameraQuakeParam& inParam, FLOAT fGeta) {
    INT nTime = inParam._nTime * VSYNC_TIMEQ;
    if (nTime == 0) {
        return;
    }
    if (nTime < 0) {
        nTime = -nTime;
        m_nTime = 0;
    } else {
        m_nTime = nTime;
    }
    FLOAT fTime = nTime;
    m_fGeta = fGeta;
    if (fGeta > 0.0f) {
        m_fStep = -1.5707964f;
    } else if (fGeta < 0.0f) {
        m_fStep = 1.5707964f;
    } else {
        m_fStep = 0.0f;
    }
    m_fPower = inParam._fPower;
    m_fSpeed = inParam._fCycle * 3.1415927f / fTime;
    m_fAbsorb = m_fPower / fTime;
    m_fGetaPer = m_fGeta / fTime;
}

BOOL CCameraQuake::updateParam(TIMEQ nTQ) {
    FLOAT fTQ = nTQ;
    m_fStep = Math3D::LimitPI(m_fStep + m_fSpeed * fTQ);
    m_fOffset = sinf(m_fStep) * m_fPower + m_fGeta;
    if (m_nTime > 0) {
        m_fPower -= m_fAbsorb * fTQ;
        m_fGeta -= m_fGetaPer * fTQ;
        m_nTime -= nTQ;
        if (m_nTime == 0) {
            m_nTime = -1;
        }
    }
    return isEnd();
}

void CCameraQuakeV::main() {
    CCamera* pCamera = (CCamera*)getParent();
    if (isEnd()) {
        if (pCamera) {
            pCamera->setOffsetY(0.0f);
        }
        out();
    } else {
        updateParam(getFrameRate());
        if (pCamera) {
            pCamera->setOffsetY(getOffset());
        }
    }
}

void CCameraQuakeH::main() {
    CCamera* pCamera = (CCamera*)getParent();
    if (isEnd()) {
        if (pCamera) {
            pCamera->setOffsetX(0.0f);
        }
        out();
    } else {
        updateParam(getFrameRate());
        if (pCamera) {
            pCamera->setOffsetX(getOffset());
        }
    }
}
// SOURCE DATA: .rodata
