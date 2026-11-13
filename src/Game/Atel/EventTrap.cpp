
#include "Game/Atel/EventTrap.h"
#include "Game/Collision/CollUtil.h"

void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1);














static inline void _resetCoord(CCoord& crd) {
    crd.m_Local.init();
    crd.touch();
    crd.fix();
}

namespace ATEL {

void CEventLine::setData(const LineTrapSrc& inData) {
    KH_DEBUG(G_LINE Line;)
    INT i;

    for (i = 0; i < 2; i++) {
        m_srcData._vP[i] = inData._vP[i]; /* the original: FVECTOR::set (Coord.h:731); .set() here moves a store past the asm below */
    }
    setCheckData(0, m_srcData._vP[0]);
    KH_DEBUG(_resetCoord(m_Crd);)
    KH_DEBUG(m_Shape.m_pCrd = &m_Crd;)

    KH_DEBUG(Line.P0.set(m_srcData._vP[0]);)
    KH_DEBUG(Line.P1.set(m_srcData._vP[0]);)
    KH_DEBUG(Line.P1.y = m_srcData._vP[1].y;)
    KH_DEBUG(Line.C0.set(0xFF, 0x40, 0x40, 0x80);)
    KH_DEBUG(Line.C1.set(0xFF, 0x40, 0x40, 0x80);)
    KH_DEBUG(m_Shape.m_vLINE.push_back(Line);)
    KH_DEBUG(Line.P0.set(m_srcData._vP[1]);)
    KH_DEBUG(m_Shape.m_vLINE.push_back(Line);)
    KH_DEBUG(Line.P1.set(m_srcData._vP[1]);)
    KH_DEBUG(Line.P1.y = m_srcData._vP[0].y;)
    KH_DEBUG(m_Shape.m_vLINE.push_back(Line);)
    KH_DEBUG(Line.P0.set(m_srcData._vP[0]);)
    KH_DEBUG(m_Shape.m_vLINE.push_back(Line);)
    KH_DEBUG(Line.P1.set(m_srcData._vP[1]);)
    KH_DEBUG(m_Shape.m_vLINE.push_back(Line);)
}

INT CEventLine::checkTrap(const ETMoveData& inData) {
    return CCollUtil::instance()._detectActorTrapXZ(inData, getCheckData());
}

KH_DEBUG(void CEventLine::dispShape() {)
    KH_DEBUG(m_Shape.disp();)
KH_DEBUG(})

void CEventBox::setData(const BoxTrapSrc& inData) {
    m_srcData = inData;
    setCheckData(1, m_matVert.v[0]);
    setupVertexes();
}

INT CEventBox::checkTrap(const ETMoveData& inData) {
    return CCollUtil::instance()._detectActorTrapXZ(inData, getCheckData());
}

void CEventBox::setupVertexes() {
    FLOAT fW = m_srcData._fWidth * 0.5f;
    FLOAT fD = m_srcData._fDepth * 0.5f;
    FMATRIX matV;
    FMATRIX mat;
    KH_DEBUG(G_LINE Line;)

    matV.v[0].set(-fW, 0.0f, -fD);
    matV.v[1].set(fW, m_srcData._fHeight, -fD);
    matV.v[2].set(fW, 0.0f, fD);
    matV.v[3].set(-fW, 0.0f, fD);
    mat.init();
    RotMatrixY(mat, mat, m_srcData._fRotY);
    mat.setTrans(m_srcData._vPos);
    MulMatrix(m_matVert, mat, matV);
    KH_DEBUG(_resetCoord(m_Crd);)

    KH_DEBUG(m_Shape[0].m_pCrd = &m_Crd;)
    KH_DEBUG(m_Shape[0].setSemiTrans(ALPHA_BLEND);)
    KH_DEBUG(m_Shape[0].m_C0.set(0xFF, 0x40, 0x40, 0x40);)
    KH_DEBUG(m_Shape[0].m_C1 = m_Shape[0].m_C0;)
    KH_DEBUG(m_Shape[0].m_C2 = m_Shape[0].m_C0;)
    KH_DEBUG(m_Shape[0].m_C3 = m_Shape[0].m_C0;)
    KH_DEBUG(m_Shape[0].m_P0.set(m_matVert.v[0]);)
    KH_DEBUG(m_Shape[0].m_P1.set(m_matVert.v[1]);)
    KH_DEBUG(m_Shape[0].m_P2.set(m_matVert.v[3]);)
    KH_DEBUG(m_Shape[0].m_P3.set(m_matVert.v[2]);)
    KH_DEBUG(m_Shape[0].m_P1.y = m_matVert.v[0].y;)
    KH_DEBUG(m_Shape[0].m_P2.y = m_matVert.v[0].y;)
    KH_DEBUG(m_Shape[0].m_P3.y = m_matVert.v[0].y;)

    KH_DEBUG(m_Shape[1].m_pCrd = &m_Crd;)
    KH_DEBUG(m_Shape[1].setSemiTrans(ALPHA_BLEND);)
    KH_DEBUG(m_Shape[1].m_C0 = m_Shape[0].m_C0;)
    KH_DEBUG(m_Shape[1].m_C1 = m_Shape[0].m_C0;)
    KH_DEBUG(m_Shape[1].m_C2 = m_Shape[0].m_C0;)
    KH_DEBUG(m_Shape[1].m_C3 = m_Shape[0].m_C0;)
    KH_DEBUG(m_Shape[1].m_P0.set(m_matVert.v[0]);)
    KH_DEBUG(m_Shape[1].m_P1.set(m_matVert.v[1]);)
    KH_DEBUG(m_Shape[1].m_P2.set(m_matVert.v[3]);)
    KH_DEBUG(m_Shape[1].m_P3.set(m_matVert.v[2]);)
    KH_DEBUG(m_Shape[1].m_P0.y = m_matVert.v[1].y;)
    KH_DEBUG(m_Shape[1].m_P2.y = m_matVert.v[1].y;)
    KH_DEBUG(m_Shape[1].m_P3.y = m_matVert.v[1].y;)

    KH_DEBUG(m_Lines.m_pCrd = &m_Crd;)
    KH_DEBUG(Line.C0 = m_Shape[0].m_C0;)
    KH_DEBUG(Line.C1 = m_Shape[0].m_C0;)
    KH_DEBUG(Line.P0.set(m_Shape[0].m_P0);)
    KH_DEBUG(Line.P1.set(m_Shape[1].m_P0);)
    KH_DEBUG(m_Lines.m_vLINE.push_back(Line);)
    KH_DEBUG(Line.P0.set(m_Shape[0].m_P1);)
    KH_DEBUG(Line.P1.set(m_Shape[1].m_P1);)
    KH_DEBUG(m_Lines.m_vLINE.push_back(Line);)
    KH_DEBUG(Line.P0.set(m_Shape[0].m_P2);)
    KH_DEBUG(Line.P1.set(m_Shape[1].m_P2);)
    KH_DEBUG(m_Lines.m_vLINE.push_back(Line);)
    KH_DEBUG(Line.P0.set(m_Shape[0].m_P3);)
    KH_DEBUG(Line.P1.set(m_Shape[1].m_P3);)
    KH_DEBUG(m_Lines.m_vLINE.push_back(Line);)
}

KH_DEBUG(void CEventBox::dispShape() {)
    KH_DEBUG(m_Shape[0].disp();)
    KH_DEBUG(m_Shape[1].disp();)
    KH_DEBUG(m_Lines.disp();)
KH_DEBUG(})

} // namespace ATEL
// SOURCE DATA: .rodata
