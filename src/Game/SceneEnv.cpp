#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "Game/Game.h"
#include "Game/SceneEnv.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"



#define CH_MAX 4

static inline void setV(FVECTOR& v, FLOAT x, FLOAT y, FLOAT z, FLOAT w) {
    v.w = w;
    v.z = z;
    v.y = y;
    v.x = x;
}

struct CSceneEnvColor {
private:
    FVECTOR m_vOrg;
    FVECTOR m_vAim;
public:
    FVECTOR m_vColor;
    ObjType::eObjBit m_iColBit;
private:
    FLOAT m_fPar;
    FLOAT m_fParAdd;
    UINT m_iStatus;

public:
    CSceneEnvColor() {
        setV(m_vColor, 1.0f, 1.0f, 1.0f, 1.0f);
        setV(m_vOrg, 1.0f, 1.0f, 1.0f, 1.0f);
        setV(m_vAim, 1.0f, 1.0f, 1.0f, 1.0f);
        m_fPar = 1.0f;
        m_fParAdd = 1.0f;
        m_iColBit = ObjType::OBJ_BIT_CLEAR;
        m_iStatus = 0;
    }
    ~CSceneEnvColor() {}
    void update(FLOAT fr);
    void setColor(ObjType::eObjBit iBit, FLOAT fFrame, const FVECTOR& vAim, const FVECTOR* pvOrg);
    BOOL isColorEnd();
};

struct CSceneEnvTask : public CTask {
    CSceneEnvTask(CTask* p);
    virtual ~CSceneEnvTask();
    virtual void main();
    virtual void disp();
};

CSceneEnvTask* CSceneEnv::m_pTask = NULL;
CSceneEnvColor* CSceneEnv::m_pCol = NULL;
ObjType::eObjBit CSceneEnv::m_iStopBit = ObjType::OBJ_BIT_CLEAR;
ObjType::eObjBit CSceneEnv::m_iStopBitReq = ObjType::OBJ_BIT_CLEAR;
BOOL CSceneEnv::m_iStopGauge = 0;
BOOL CSceneEnv::m_iStopGaugeReq = 0;
UINT CSceneEnv::m_iStatus = 0;
FVECTOR CSceneEnv::m_vOutColor;

CSceneEnvTask::CSceneEnvTask(CTask* p) : CTask(p, 0x4000) {
    declareSysTask(0x1C);
    CSceneEnv::m_pTask = this;
}

CSceneEnvTask::~CSceneEnvTask() {
    CSceneEnvTask*& r = CSceneEnv::m_pTask;
    if (r == this) {
        r = NULL;
    }
}

void CSceneEnvTask::main() {
    if (!CGame::isSuspended()) {
        CSceneEnv::_update(getFrameRateF());
    }
}

void CSceneEnvTask::disp() {}

#line 188
void CSceneEnv::initialize(CTask* p) {
    m_pCol = new (__FILE__ "(189)") CSceneEnvColor[CH_MAX];
    if (!(m_pCol)) AssertApp(__FILE__, __LINE__, "m_pCol", 0);
    m_iStopBit = ObjType::OBJ_BIT_CLEAR;
    m_iStopBitReq = ObjType::OBJ_BIT_CLEAR;
    m_iStopGauge = 0;
    m_iStopGaugeReq = 0;
    m_iStatus = 0;
    if (m_pTask == NULL) {
        new (__FILE__ "(198)") CSceneEnvTask(p);
    }
}

void CSceneEnv::finalize() {
    if (m_pCol) {
        delete[] m_pCol;
    }
    m_pCol = NULL;
    m_iStopBit = ObjType::OBJ_BIT_CLEAR;
    m_iStopBitReq = ObjType::OBJ_BIT_CLEAR;
    m_iStopGauge = 0;
    m_iStopGaugeReq = 0;
    m_iStatus = 0;
    if (m_pTask) {
        m_pTask->out();
    }
}

#line 225
void CSceneEnv::setColor(ObjType::eObjBit iBit, FLOAT fFrame, const FVECTOR& vAim, const FVECTOR* pvOrg, INT nCh) {
    if (!(!((UINT)nCh >= CH_MAX || nCh < 0 || m_pCol == NULL))) AssertApp(__FILE__, __LINE__, "! (nCh >= CH_MAX || nCh < 0 || m_pCol == __null)", 0);
    m_pCol[nCh].setColor(iBit, fFrame, vAim, pvOrg);
}

void CSceneEnvColor::setColor(ObjType::eObjBit iBit, FLOAT fFrame, const FVECTOR& vAim, const FVECTOR* pvOrg) {
    if (pvOrg) {
        m_vColor = *pvOrg;
    }
    m_vOrg = m_vColor;
    if (fFrame <= 0.0f) {
        m_fPar = 0.0f;
        m_fParAdd = 1.0f;
        m_vAim = vAim;
        m_vColor = vAim;
    } else {
        m_fPar = 0.0f;
        m_fParAdd = 1.0f / fFrame;
        m_vAim = vAim;
    }
    m_iColBit = iBit;
    m_iStatus |= 1;
}

#line 260
BOOL CSceneEnv::isColorEnd(INT nCh) {
    if (!(!((UINT)nCh >= CH_MAX || nCh < 0 || m_pCol == NULL))) AssertApp(__FILE__, __LINE__, "! (nCh >= CH_MAX || nCh < 0 || m_pCol == __null)", 0);
    return m_pCol[nCh].isColorEnd();
}

BOOL CSceneEnv::isColorAllEnd() {
    INT i;
    if (m_pCol) {
        for (i = 0; i < CH_MAX; i++) {
            if (!m_pCol[i].isColorEnd()) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

BOOL CSceneEnvColor::isColorEnd() {
    if (m_iStatus & 1) {
        return FALSE;
    }
    return TRUE;
}

static inline void mulVector(FVECTOR& v0, const FVECTOR& v1) {
    __asm__ volatile(
        "lqc2 $vf4, 0x0(%0)\n"
        "lqc2 $vf5, 0x0(%1)\n"
        "vmul.xyzw $vf4, $vf4, $vf5\n"
        "sqc2 $vf4, 0x0(%2)\n"
        : : "r"(&v0), "r"(&v1), "r"(&v0) : "memory");
}

FVECTOR* CSceneEnv::getTypeColor(ObjType::eObjType eType) {
    FVECTOR* p = NULL;
    if (m_pCol) {
        ObjType::eObjBit eBit = (ObjType::eObjBit)(1 << eType);
        INT i;
        for (i = 0; i < CH_MAX; i++) {
            if (m_pCol[i].m_iColBit & eBit) {
                FVECTOR& v2 = m_pCol[i].m_vColor;
                if (p == NULL) {
                    p = &m_vOutColor;
                    *p = v2;
                } else {
                    mulVector(m_vOutColor, v2);
                }
            }
        }
    }
    return p;
}

#line 316
FVECTOR& CSceneEnv::getColor(INT nCh) {
    if (!(!((UINT)nCh >= CH_MAX || nCh < 0 || m_pCol == NULL))) AssertApp(__FILE__, __LINE__, "! (nCh >= CH_MAX || nCh < 0 || m_pCol == __null)", 0);
    return m_pCol[nCh].m_vColor;
}

#line 325
ObjType::eObjBit CSceneEnv::getColorTypeBit(INT nCh) {
    if (!(!((UINT)nCh >= CH_MAX || nCh < 0 || m_pCol == NULL))) AssertApp(__FILE__, __LINE__, "! (nCh >= CH_MAX || nCh < 0 || m_pCol == __null)", 0);
    return m_pCol[nCh].m_iColBit;
}

void CSceneEnv::setStop(ObjType::eObjBit iBit) {
    if ((m_iStatus & 1) && m_iStopBitReq != iBit) {
        TRACE("CSceneEnv::setStop() \x93\xaf\x83\x74\x83\x8c\x81\x5b\x83\x80\x82\xc5\x95\xca\x82\xcc\x90\xdd\x92\xe8\x82\xc5\x8c\xc4\x82\xce\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81\x42\x82\xc7\x82\xa4\x82\xb5\x82\xe6\x82\xa4\x81\x42\x8f\xe3\x8f\x91\x82\xab\x82\xb5\x82\xdc\x82\xb7\x81\x42%08x %08x\n", m_iStopBitReq, iBit);
    }
    m_iStopBitReq = iBit;
    m_iStatus |= 1;
}

ObjType::eObjBit CSceneEnv::getStopTypeBit() {
    return m_iStopBit;
}

ObjType::eObjBit CSceneEnv::getStopAndRequestTypeBit() {
    if (m_iStatus & 1) {
        return m_iStopBitReq;
    }
    return m_iStopBit;
}

BOOL CSceneEnv::getStopRequestTypeBit(ObjType::eObjBit* pOutStopBit) {
    if (m_iStatus & 1) {
        if (pOutStopBit) {
            *pOutStopBit = m_iStopBitReq;
        }
        return TRUE;
    }
    return FALSE;
}

void CSceneEnv::setStopGauge(BOOL bStop) {
    if ((m_iStatus & 2) && m_iStopGaugeReq != bStop) {
        TRACE("CSceneEnv::setStopGauge() \x93\xaf\x83\x74\x83\x8c\x81\x5b\x83\x80\x82\xc5\x95\xca\x82\xcc\x90\xdd\x92\xe8\x82\xc5\x8c\xc4\x82\xce\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81\x42\x82\xc7\x82\xa4\x82\xb5\x82\xe6\x82\xa4\x81\x42\x8f\xe3\x8f\x91\x82\xab\x82\xb5\x82\xdc\x82\xb7\x81\x42%08x %08x\n", m_iStopGaugeReq, bStop);
    }
    m_iStopGaugeReq = bStop;
    m_iStatus |= 2;
}

BOOL CSceneEnv::isStopGauge() {
    return m_iStopGauge;
}

BOOL CSceneEnv::isStopAndRequestGauge() {
    if (m_iStatus & 2) {
        return m_iStopGaugeReq;
    }
    return m_iStopGauge;
}

void CSceneEnv::updateSys() {
    m_iStopBit = m_iStopBitReq;
    m_iStopGauge = m_iStopGaugeReq;
    m_iStatus &= ~3;
}

void CSceneEnv::_update(FLOAT fr) {
    INT i;
    if (m_pCol) {
        for (i = 0; i < CH_MAX; i++) {
            m_pCol[i].update(fr);
        }
    }
}

void CSceneEnvColor::update(FLOAT fr) {
    if (m_fPar < 1.0f) {
        m_fPar += m_fParAdd * fr;
        if (m_fPar >= 1.0f) {
            m_fPar = 1.0f;
            m_vColor = m_vAim;
        } else {
            sceVu0InterVector(m_vColor.v, m_vAim.v, m_vOrg.v, m_fPar);
        }
    } else if (m_iStatus & 1) {
        m_iStatus &= ~1;
        if (m_vColor.x == 1.0f && m_vColor.y == 1.0f && m_vColor.z == 1.0f && m_vColor.w == 1.0f) {
            m_iColBit = ObjType::OBJ_BIT_CLEAR;
        }
    }
}
// SOURCE DATA: .data .rodata
