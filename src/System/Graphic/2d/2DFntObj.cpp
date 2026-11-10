#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h" /* MATCH: this unit names 2DMan.h so (header default "System/Graphic/2D/2DMan.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/2d/2DFntObj.h"
#include "System/Graphic/2d/2DFnt.h"
#include "System/Graphic/2d/2DSeqCtrl.h"

void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT t);
void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1);
void MulVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void FTOI0Vector(IVECTOR& v0, const FVECTOR& v1);

namespace D2 {






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

C2DFntObj::C2DFntObj(SeqControl* pSeqControl) : C2DObj(pSeqControl) {
    m_p2DFnt = NULL;
    m_vPos.init();
    m_vCol.set(1.0f, 1.0f, 1.0f, 1.0f);
}

C2DFntObj::~C2DFntObj() {
    if (m_p2DFnt) delete m_p2DFnt;
    m_p2DFnt = NULL;
}

TIMEQ C2DFntObj::init(C2DFnt* p2DFnt) {
    m_p2DFnt = p2DFnt;
    /* read through a reference: keeps the load after the m_p2DFnt store (scheduling) */
    const USHORT& nLayer = C2DMan::m_nLayer;
    m_p2DFnt->setLayerFG(nLayer);
    m_fgDisp = 1;
    return -1;
}

void C2DFntObj::move() {
    FVECTOR fVec;
    FMATRIX mat;
    FMATRIX mat2;
    FVECTOR vCol;
    IVECTOR iVec;
    G_RGBA rgba;

    if (m_p2DFnt) {
        _unitMatrixVu0(mat);
        mat.setTrans(m_vPos);
        ScaleVector(vCol, m_vCol, 128.0f);
        if (m_pSeqControl) {
            MulMatrix(mat, m_pSeqControl->controlMatrix, mat);
            if (m_pSeqControl->fgFntScale) {
                m_p2DFnt->m_Size = (FIX4)(m_p2DFnt->m_seqFntSize * mat.m[0][0]);
            }
            MulVector(vCol, vCol, m_pSeqControl->controlColor);
        }
        ClampVector(vCol, vCol, 0.0f, 256.0f);
        m_p2DFnt->setPositionF(mat.m[3][0] * 16.0f, mat.m[3][1] * 16.0f);
        FTOI0Vector(iVec, vCol);
        rgba.set(iVec.x, iVec.y, iVec.z, iVec.w);
        m_p2DFnt->setBrightness(rgba);
    }
}

void C2DFntObj::_main(TIMEQ qTime) {
    move();
}

INT C2DFntObj::_disp() {
    if (m_p2DFnt && m_fgDisp) {
        m_p2DFnt->disp();
    }
    return 0;
}

void C2DFntObj::changeLayer(INT nLayer) {
    if (m_p2DFnt) {
        m_p2DFnt->setLayer(3, nLayer);
    }
}

void C2DFntObj::shiftBase(FLOAT fX, FLOAT fY) {
    if (m_p2DFnt) {
        m_vPos.x = fX;
        m_vPos.y = fY;
    }
}

void C2DFntObj::shiftColor(FLOAT fR, FLOAT fG, FLOAT fB, FLOAT fA) {
    if (m_p2DFnt) {
        m_vCol.x = fA;
        m_vCol.y = fG;
        m_vCol.z = fB;
        m_vCol.w = fA;
    }
}

}  // namespace D2

// SOURCE DATA: .rodata
