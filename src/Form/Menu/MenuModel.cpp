#define TASK_H_NO_SLC_OVERFLOW_MSG
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/Anim.h"
#include "Form/Menu/MenuModel.h"
#include "System/HeapMan.h"
#include "System/Graphic/Camera.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void* operator new[](size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void operator delete[](void* mem) { com_free(mem); }

#include "System/Graphic/TexAnm.h"
#line 22

#line 31

class CRsrcDB {
public:
    static INT getModelCount(INT nRsrcID);
    static MODELDATA** getRsrcMDL(INT nRsrcID);
    static TEXANM_DATA* getRsrcTXA(INT nRsrcID);
};

extern FLOAT ASPECT_RATE_X;
extern FLOAT ASPECT_RATE_Y;

extern "C" {
void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
void sceVu0InversMatrix(FMATRIX& m0, FMATRIX& m1);
void sceVu0ViewScreenMatrix(FMATRIX& m, float scrz, float ax, float ay, float cx, float cy, float zmin, float zmax,
                            float nearz, float farz);
void sceVu0InterVector(FVECTOR& v0, FVECTOR& v1, FVECTOR& v2, float t);
}
void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1);

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

static inline void _setParent(CCoord& crd, CCoord* pParent) { crd.m_pParent = pParent; }
static inline void _touch(CCoord& crd) { crd.m_Flag = 1; }

static inline void _viewScreenMatrix(FMATRIX& matOut, FLOAT fInAx, FLOAT fInAy) {
    sceVu0ViewScreenMatrix(matOut, 512.0f, ASPECT_RATE_X, ASPECT_RATE_Y, fInAx, fInAy, 1.0f, 16777000.0f, 0.1f, 100.0f);
}

CMenuModel::CMenuModel(CTask* p, INT nRsrcID, CCoord* pParentCoord, INT iPosX, INT iPosY) : CTask(p, 0x4000) {
    m_nRsrcID = nRsrcID;
    m_vPos.init();
    m_pParentCoord = pParentCoord;
    m_pModels = NULL;
    m_pAnim = NULL;
    m_pTexAnm = NULL;
    m_nStep = 0;
    m_nTexAnmNum = 0;
    m_bDisp = TRUE;
    m_fFadeTime = -1.0f;
    m_nModelCount = CRsrcDB::getModelCount(m_nRsrcID);
    if (m_nModelCount == 0) {
        return;
    }
    MODELDATA** pModelTbl = CRsrcDB::getRsrcMDL(m_nRsrcID);
    if (pModelTbl == NULL) {
        return;
    }
#line 46
    m_pModels = new (__FILE__ "(46)") CModel[m_nModelCount];
    if (!m_pModels) AssertApp(__FILE__, __LINE__, "m_pModels", 0);

    TEXANM_DATA* pTXAData = CRsrcDB::getRsrcTXA(m_nRsrcID);
    if (pTXAData) {
        m_nTexAnmNum = pTXAData->getDataNum();
        if (m_nTexAnmNum > 0) {
#line 56
            m_pTexAnm = new (__FILE__ "(56)") CTexAnm[m_nTexAnmNum];
            if (m_pTexAnm) {
                for (INT i = 0; i < m_nTexAnmNum; i++) {
                    m_pTexAnm[i].setup(*pTXAData, i);
                }
            }
        }
    }
    for (INT i = 0; i < m_nModelCount; i++) {
        m_pModels[i].link(this, pModelTbl[i]);
        m_pModels[i].linkTexAnm(m_pTexAnm, m_nTexAnmNum);
        m_pModels[i].m_nFlag |= 0x20;
        if (m_pParentCoord) {
            _setParent(m_pModels[i].getCrd(0), m_pParentCoord);
        }
    }
#line 77
    m_pAnim = new (__FILE__ "(77)") CAnim;
    m_pAnim->link(this, m_nRsrcID);
    m_pAnim->init(0, 0);

    CCoord& crd = Crd(0);
    _unitMatrixVu0(crd.m_Local);
    if (!m_pParentCoord) {
        crd.m_Local.m[2][2] = 0.1f;
        crd.m_Local.m[1][1] = 0.1f;
        crd.m_Local.m[0][0] = 0.1f;
    }
    crd.m_Local.m[3][0] = m_vPos.x;
    crd.m_Local.m[3][1] = m_vPos.y;
    crd.m_Local.m[3][2] = m_vPos.z;
    m_vPosWSC.set(0.0f, 0.125f, 0.405f, 1.0f);
    m_PosScreen.x = iPosX;
    m_PosScreen.y = iPosY;
    changeLocalWS();
    _touch(crd);
}

CMenuModel::~CMenuModel() {
    if (m_pModels) {
        delete[] m_pModels;
    }
    m_pModels = NULL;
    if (m_pAnim) {
        delete m_pAnim;
    }
    m_pAnim = NULL;
    if (m_pTexAnm) {
        delete[] m_pTexAnm;
    }
    m_pTexAnm = NULL;
}

void CMenuModel::main() {
    if (m_nStep != 0) {
        return;
    }
    if (m_pAnim) {
        m_pAnim->play(getFrameRate(), 0);
    }
    if (m_pTexAnm) {
        for (INT i = 0; i < m_nTexAnmNum; i++) {
            m_pTexAnm[i].update(getFrameRateF());
        }
    }
}

CCoord* CMenuModel::getCoord(const CHAR* pBoneName) {
    INT index = m_pModels->getBoneIndex(pBoneName);
    if (index < 0) {
        return NULL;
    }
    return &m_pModels->getCrd(index);
}

void CMenuModel::changeAnim(INT nID, BOOL bHokan) {
    if (m_pAnim) {
        if (bHokan) {
            m_pAnim->change(nID, 0, -1);
        } else {
            m_pAnim->change(nID, 0, 0);
        }
    }
}

void CMenuModel::changeAnimEx(INT nRsrcID, INT nID, BOOL bHokan) {
    if (m_pAnim) {
        if (bHokan) {
            m_pAnim->changeEx(nRsrcID, nID, 0, -1);
        } else {
            m_pAnim->changeEx(nRsrcID, nID, 0, 0);
        }
    }
}

BOOL CMenuModel::isAnimEnd() {
    if (m_pAnim) {
        if (m_pAnim->isMotionEnd()) {
            return TRUE;
        }
    }
    return FALSE;
}

void CMenuModel::changeLocalWS() {
    FMATRIX mVS;
    FMATRIX mWV;
    _unitMatrixVu0(mWV);
    sceVu0RotMatrixY(mWV, mWV, 3.1415927f);
    mWV.v[3] = m_vPosWSC;
    sceVu0InversMatrix(mWV, mWV);
    mWV.m[0][0] = -mWV.m[0][0];
    mWV.m[1][0] = -mWV.m[1][0];
    mWV.m[2][0] = -mWV.m[2][0];
    mWV.m[3][0] = -mWV.m[3][0];
    mWV.m[0][1] = -mWV.m[0][1];
    mWV.m[1][1] = -mWV.m[1][1];
    mWV.m[2][1] = -mWV.m[2][1];
    mWV.m[3][1] = -mWV.m[3][1];
    _viewScreenMatrix(mVS, m_PosScreen.x + 0x800, 0x800 - m_PosScreen.y);
    MulMatrix(m_mWS, mVS, mWV);
    for (INT i = 0; i < m_nModelCount; i++) {
        m_pModels[i].setWorldScreenMatrix(&m_mWS);
        m_pModels[i].changeLayerExt(1, 13);
    }
}

void CMenuModel::setAlphaScale(FLOAT fAlphaScale) {
    for (INT i = 0; i < m_nModelCount; i++) {
        m_pModels[i].setAlphaScale(fAlphaScale);
    }
}

void CMenuModel::setColorScale(FVECTOR& vColor) {
    for (INT i = 0; i < m_nModelCount; i++) {
        m_pModels[i].setColorScale(vColor);
    }
}

void CMenuModel::setColorFade(FVECTOR& vColor, FVECTOR& vColor2, FLOAT fFadeTime) {
    m_colorOld = m_colorCurrent = vColor;
    m_colorTarget = vColor2;
    m_bDisp = TRUE;
    m_fFadeTime = fFadeTime;
    m_fTime = 0.0f;
}

void CMenuModel::setPosScreen(INT iPosX, INT iPosY) {
    CCoord& crd = Crd(0);
    m_PosScreen.x = iPosX;
    m_PosScreen.y = iPosY;
    changeLocalWS();
    _touch(crd);
}

void CMenuModel::setScale(FLOAT fScale) {
    CCoord& crd = Crd(0);
    _unitMatrixVu0(crd.m_Local);
    if (!m_pParentCoord) {
        crd.m_Local.m[2][2] = fScale;
        crd.m_Local.m[1][1] = fScale;
        crd.m_Local.m[0][0] = fScale;
    }
    crd.m_Local.m[3][0] = m_vPos.x;
    crd.m_Local.m[3][1] = m_vPos.y;
    crd.m_Local.m[3][2] = m_vPos.z;
    changeLocalWS();
    _touch(crd);
}

void CMenuModel::setSpeed(FLOAT fSpeed) {
    if (m_pAnim) {
        m_pAnim->setSpeed(fSpeed);
    }
}

void CMenuModel::disp() {
    if (!m_pModels) {
        return;
    }
    if (!m_bDisp) {
        return;
    }
    if (m_fFadeTime >= 0.0f) {
        FLOAT fRate = m_fTime / m_fFadeTime;
        m_fTime += getFrameRateF();
        if (m_fTime > m_fFadeTime) {
            m_colorCurrent = m_colorTarget;
            m_fFadeTime = -1.0f;
            if (m_colorCurrent.w == 0.0f) {
                m_bDisp = FALSE;
            }
        } else {
            sceVu0InterVector(m_colorCurrent, m_colorTarget, m_colorOld, fRate);
        }
        for (INT i = 0; i < m_nModelCount; i++) {
            m_pModels[i].setColorScale(m_colorCurrent);
        }
    }
    for (INT i = 0; i < m_nModelCount; i++) {
        m_pModels[i].disp();
    }
}

// SOURCE DATA: .rodata
