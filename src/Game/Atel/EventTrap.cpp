
#include "Game/Atel/EventTrap.h"
#include "Game/Collision/CollUtil.h"

extern "C" void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
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

static inline void _resetCoord(CCoord& crd) {
    _unitMatrixVu0(crd.m_Local);
    crd.m_Flag = 1;
    crd.fix();
}

namespace ATEL {

void CEventLine::setData(const LineTrapSrc& inData) {
    G_LINE Line;
    INT i;

    for (i = 0; i < 2; i++) {
        m_srcData._vP[i] = inData._vP[i];
    }
    setCheckData(0, m_srcData._vP[0]);
    _resetCoord(m_Crd);
    m_Shape.m_pCrd = &m_Crd;

    Line.P0 = m_srcData._vP[0];
    Line.P1 = m_srcData._vP[0];
    Line.P1.y = m_srcData._vP[1].y;
    Line.C0.set(0xFF, 0x40, 0x40, 0x80);
    Line.C1.set(0xFF, 0x40, 0x40, 0x80);
    m_Shape.m_vLINE.push_back(Line);
    Line.P0 = m_srcData._vP[1];
    m_Shape.m_vLINE.push_back(Line);
    Line.P1 = m_srcData._vP[1];
    Line.P1.y = m_srcData._vP[0].y;
    m_Shape.m_vLINE.push_back(Line);
    Line.P0 = m_srcData._vP[0];
    m_Shape.m_vLINE.push_back(Line);
    Line.P1 = m_srcData._vP[1];
    m_Shape.m_vLINE.push_back(Line);
}

INT CEventLine::checkTrap(const ETMoveData& inData) {
    return CCollUtil::getInstance()->_detectActorTrapXZ(inData, getCheckData());
}

void CEventLine::dispShape() {
    m_Shape.disp();
}

void CEventBox::setData(const BoxTrapSrc& inData) {
    m_srcData = inData;
    setCheckData(1, m_matVert.v[0]);
    setupVertexes();
}

INT CEventBox::checkTrap(const ETMoveData& inData) {
    return CCollUtil::getInstance()->_detectActorTrapXZ(inData, getCheckData());
}

void CEventBox::setupVertexes() {
    FLOAT fW = m_srcData._fWidth * 0.5f;
    FLOAT fD = m_srcData._fDepth * 0.5f;
    FMATRIX matV;
    FMATRIX mat;
    G_LINE Line;

    matV.v[0].set(-fW, 0.0f, -fD);
    matV.v[1].set(fW, m_srcData._fHeight, -fD);
    matV.v[2].set(fW, 0.0f, fD);
    matV.v[3].set(-fW, 0.0f, fD);
    _unitMatrixVu0(mat);
    sceVu0RotMatrixY(mat, mat, m_srcData._fRotY);
    mat.setTrans(m_srcData._vPos);
    MulMatrix(m_matVert, mat, matV);
    _resetCoord(m_Crd);

    m_Shape[0].m_pCrd = &m_Crd;
    m_Shape[0].setSemiTrans(ALPHA_BLEND);
    m_Shape[0].m_C0.set(0xFF, 0x40, 0x40, 0x40);
    m_Shape[0].m_C1 = m_Shape[0].m_C0;
    m_Shape[0].m_C2 = m_Shape[0].m_C0;
    m_Shape[0].m_C3 = m_Shape[0].m_C0;
    m_Shape[0].m_P0 = m_matVert.v[0];
    m_Shape[0].m_P1 = m_matVert.v[1];
    m_Shape[0].m_P2 = m_matVert.v[3];
    m_Shape[0].m_P3 = m_matVert.v[2];
    m_Shape[0].m_P1.y = m_matVert.v[0].y;
    m_Shape[0].m_P2.y = m_matVert.v[0].y;
    m_Shape[0].m_P3.y = m_matVert.v[0].y;

    m_Shape[1].m_pCrd = &m_Crd;
    m_Shape[1].setSemiTrans(ALPHA_BLEND);
    m_Shape[1].m_C0 = m_Shape[0].m_C0;
    m_Shape[1].m_C1 = m_Shape[0].m_C0;
    m_Shape[1].m_C2 = m_Shape[0].m_C0;
    m_Shape[1].m_C3 = m_Shape[0].m_C0;
    m_Shape[1].m_P0 = m_matVert.v[0];
    m_Shape[1].m_P1 = m_matVert.v[1];
    m_Shape[1].m_P2 = m_matVert.v[3];
    m_Shape[1].m_P3 = m_matVert.v[2];
    m_Shape[1].m_P0.y = m_matVert.v[1].y;
    m_Shape[1].m_P2.y = m_matVert.v[1].y;
    m_Shape[1].m_P3.y = m_matVert.v[1].y;

    m_Lines.m_pCrd = &m_Crd;
    Line.C0 = m_Shape[0].m_C0;
    Line.C1 = m_Shape[0].m_C0;
    Line.P0 = m_Shape[0].m_P0;
    Line.P1 = m_Shape[1].m_P0;
    m_Lines.m_vLINE.push_back(Line);
    Line.P0 = m_Shape[0].m_P1;
    Line.P1 = m_Shape[1].m_P1;
    m_Lines.m_vLINE.push_back(Line);
    Line.P0 = m_Shape[0].m_P2;
    Line.P1 = m_Shape[1].m_P2;
    m_Lines.m_vLINE.push_back(Line);
    Line.P0 = m_Shape[0].m_P3;
    Line.P1 = m_Shape[1].m_P3;
    m_Lines.m_vLINE.push_back(Line);
}

void CEventBox::dispShape() {
    m_Shape[0].disp();
    m_Shape[1].disp();
    m_Lines.disp();
}

} // namespace ATEL
// SOURCE DATA: .rodata
