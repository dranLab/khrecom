
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h" /* MATCH: this unit names 2DMan.h so (header default "System/Graphic/2D/2DMan.h") */
#include "System/TaskMan.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/2d/2DSprObj.h"
#include "include_asm.h"
#include "System/Graphic/2d/2DSpr.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DLay.h"
#include "System/Math3D.h"
#include "System/Graphic/WorldEnv.h"









using namespace D2;

#include "System/Graphic/TexInfo.h"
#include "System/HeapMan.h"




















#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)





/* PARTIAL: the 2D viewer's SeqControl is not linked into the game: lui/addiu of address 0 (name unknown) */
extern SeqControl s_ViewerSeqControl;
/* Ps2EeAs makes undefined weak symbols global: define it as weak absolute 0 (as Game/Enemy/CreateEnemy does) */
__asm__(".weak s_ViewerSeqControl\ns_ViewerSeqControl = 0\n");

extern "C" float sinf(float x);
extern "C" float cosf(float x);
extern INT SCREEN_HEIGHT;
void RotTransPersClipW(IVECTOR& iVec, const FVECTOR& fVec);
void ITOF4Vector(FVECTOR& fVec, const IVECTOR& iVec);


void ITOF0Vector(FVECTOR& fVec, const IVECTOR& iVec);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, float s);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

static inline void _itof0(FVECTOR& fVec, const IVECTOR& iVec) {
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vitof0.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&fVec), "r"(&iVec)
                 : "memory");
}


INT C2DSprObj::m_fgParent;

void ApplyMatrix2(FVECTOR& v0, const FMATRIX& m0, const FVECTOR2& v2) {
    asm volatile("ld $8, 0x0(%2)\n"
                 "lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x10(%1)\n"
                 "lqc2 $vf6, 0x20(%1)\n"
                 "lqc2 $vf7, 0x30(%1)\n"
                 "qmtc2.ni $8, $vf8\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf0z\n"
                 "vmaddw.xyzw $vf4, $vf7, $vf0w\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&v0), "r"(&m0), "r"(&v2)
                 : "$8", "memory");
}
void AddVector2(FVECTOR2& v0, const FVECTOR2& v1, const FVECTOR2& v2) {
    asm volatile("ld $8, 0x0(%1)\n"
                 "ld $9, 0x0(%2)\n"
                 "qmtc2.ni $8, $vf4\n"
                 "qmtc2.ni $9, $vf5\n"
                 "vadd.xy $vf4, $vf4, $vf5\n"
                 "qmfc2.ni $8, $vf4\n"
                 "sd $8, 0x0(%0)\n"
                 :
                 : "r"(&v0), "r"(&v1), "r"(&v2)
                 : "$8", "$9", "memory");
}
void AddVector2(FVECTOR& v0, const FVECTOR2& v1, const FVECTOR2& v2) {
    asm volatile("ld $8, 0x0(%1)\n"
                 "ld $9, 0x0(%2)\n"
                 "qmtc2.ni $8, $vf4\n"
                 "qmtc2.ni $9, $vf5\n"
                 "vaddx.zw $vf4, $vf0, $vf0x\n"
                 "vadd.xy $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&v0), "r"(&v1), "r"(&v2)
                 : "$8", "$9", "memory");
}
void InterVector2(FVECTOR2& v0, const FVECTOR2& v1, const FVECTOR2& v2, float t) {
    asm volatile("ld $8, 0x0(%1)\n"
                 "ld $9, 0x0(%2)\n"
                 "qmtc2.ni $8, $vf4\n"
                 "qmtc2.ni $9, $vf5\n"
                 "mfc1 $8, %3\n"
                 "qmtc2.ni $8, $vf6\n"
                 "vsub.xy $vf4, $vf4, $vf5\n"
                 "vaddax.xy $ACC, $vf5, $vf0x\n"
                 "vmaddx.xy $vf4, $vf4, $vf6x\n"
                 "qmfc2.ni $8, $vf4\n"
                 "sd $8, 0x0(%0)\n"
                 :
                 : "r"(&v0), "r"(&v1), "r"(&v2), "f"(t)
                 : "$8", "$9", "memory");
}
void InterVector2(FVECTOR& v0, const FVECTOR2& v1, const FVECTOR2& v2, float t) {
    asm volatile("ld $8, 0x0(%1)\n"
                 "ld $9, 0x0(%2)\n"
                 "qmtc2.ni $8, $vf4\n"
                 "qmtc2.ni $9, $vf5\n"
                 "vaddx.zw $vf4, $vf0, $vf0x\n"
                 "mfc1 $8, %3\n"
                 "qmtc2.ni $8, $vf6\n"
                 "vsub.xy $vf4, $vf4, $vf5\n"
                 "vaddax.xy $ACC, $vf5, $vf0x\n"
                 "vmaddx.xy $vf4, $vf4, $vf6x\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&v0), "r"(&v1), "r"(&v2), "f"(t)
                 : "$8", "$9", "memory");
}
C2DSprObj::C2DSprObj(SeqControl* pSeqControl, const TexInfo* pTexInfo) {
    m_pSprPrim = NULL;
    m_vRot.zero();
    m_nPoly = 0;
    m_fgDisp = 0;
    m_Flg = 0;
    m_vPos._u64 = 0;
    m_vOfs._u64 = 0;
    m_vSwy._u64 = 0;
    m_vScl.x = 1.0f;
    m_vScl.y = 1.0f;
    m_vCol.set(1.0f, 1.0f, 1.0f, 1.0f);
    m_pSeqControl = pSeqControl;
    m_pTexInfo = pTexInfo;
    m_fgRotTransPers = -1;
    m_iStep = 0;
    m_pScissoring = NULL;
}
C2DSprObj::~C2DSprObj() {
    if (m_pSprPrim) delete[] m_pSprPrim;
    m_pSprPrim = NULL;
    if (m_pScissoring) delete m_pScissoring;
    m_pScissoring = NULL;
}
void C2DSprObj::init(UINT nSpr) {
    m_nPoly = nSpr;
#line 154
    m_pSprPrim = new (__FILE__ "(154)") SprPrim[nSpr];
    INT& rViewer = C2DMan::m_fg2DViewer;
    if (rViewer) m_pSeqControl = &s_ViewerSeqControl;
}
void C2DSprObj::initSprite(INT inNum, SPR_GROUP* pSprGroupe, SPR_PARTS* pSprParts) {
    if (!m_pSprPrim) return;
    if (inNum >= m_nPoly) return;
#line 168
    ASSERT(inNum >= 0);
    INT nLayerFG = C2DMan::m_nLayer;
    SprPrim* pSprPrim = &m_pSprPrim[inNum];
    pSprPrim->polyGT4.setLayerFG(nLayerFG);
    pSprPrim->polyGT4.setTPage((TexInfo*)m_pTexInfo);
    pSprPrim->polyGT4.setSemiTrans(ALPHA_BLEND);
    pSprPrim->pSprGroupe = pSprGroupe;
    pSprPrim->pSprParts = pSprParts;
    const VTransInfo& transInfo = m_pTexInfo->_TexInfo;
    FLOAT fW;
    FLOAT fH;
    USHORT nW = transInfo._nDistW;
    if (transInfo._nFormat == 0) {
        fW = nW * 2;
    } else {
        fW = nW;
    }
    USHORT nH = transInfo._nDistH;
    if (transInfo._nFormat == 0) {
        fH = nH * 2;
    } else {
        fH = nH;
    }
    FLOAT fU0 = pSprParts->u0 / fW;
    FLOAT fU1 = pSprParts->u1 / fW;
    FLOAT fV0 = pSprParts->v0 / fH;
    FLOAT fV1 = pSprParts->v1 / fH;
    pSprPrim->polyGT4.m_T0.set(fU0, fV0);
    pSprPrim->polyGT4.m_T1.set(fU1, fV0);
    pSprPrim->polyGT4.m_T2.set(fU0, fV1);
    pSprPrim->polyGT4.m_T3.set(fU1, fV1);
    if (pSprParts->u0 == 0 && pSprParts->v0 == 0 && pSprParts->u1 == 0 && pSprParts->v1 == 0 && m_nPoly == 1) {
#line 194
        m_pScissoring = new (__FILE__ "(194)") CScissoring;
        m_pScissoring->setLayerFG(nLayerFG);
        m_pScissoring->setSemiTrans(ALPHA_BLEND);
    }
    pSprPrim->polyGT4.m_P0.x = 0;
    pSprPrim->polyGT4.m_P0.y = 0;
    pSprPrim->polyGT4.m_P1.x = 0;
    pSprPrim->polyGT4.m_P1.y = 0;
    pSprPrim->polyGT4.m_P2.x = 0;
    pSprPrim->polyGT4.m_P2.y = 0;
    pSprPrim->polyGT4.m_P3.x = 0;
    pSprPrim->polyGT4.m_P3.y = 0;
    pSprPrim->polyGT4.m_C0.col = 0;
    pSprPrim->polyGT4.m_C1.col = 0;
    pSprPrim->polyGT4.m_C2.col = 0;
    pSprPrim->polyGT4.m_C3.col = 0;
}
void C2DSprObj::setFukidasiSize(FLOAT fSizeX, FLOAT fSizeY) {
    FVECTOR vS;
    FVECTOR vDiff;
    FVECTOR2 vPos[4];
    if (m_pSprPrim && m_nPoly == 9) {
        getRect(vPos, m_pSprPrim[4]);
        vS.set(-(fSizeX * 0.5f), -(fSizeY * 0.5f), fSizeX * 0.5f, fSizeY * 0.5f);
        vDiff.set(vPos[0].x, vPos[0].y, vPos[3].x, vPos[3].y);
        SubVector(m_vDiff, vS, vDiff);
        m_Flg |= 8;
    }
}
void C2DSprObj::getFukidasiRect(FVECTOR2 (*vOut)[4]) {
    if (m_pSprPrim && m_nPoly == 9) {
        for (INT i = 0; i < 4; i++) {
            vOut[0][i].x += m_vDiff.x;
            vOut[0][i].y += m_vDiff.y;
            vOut[8][i].x += m_vDiff.z;
            vOut[8][i].y += m_vDiff.w;
        }
        for (INT i = 0; i < 4; i++) {
            vOut[3][i].x += m_vDiff.x;
            vOut[6][i].x += m_vDiff.x;
            vOut[2][i].x += m_vDiff.z;
            vOut[5][i].x += m_vDiff.z;
            vOut[1][i].y += m_vDiff.y;
            vOut[2][i].y += m_vDiff.y;
            vOut[6][i].y += m_vDiff.w;
            vOut[7][i].y += m_vDiff.w;
        }
        for (INT i = 0; i < 4; i++) {
            if (!(i & 1)) {
                vOut[1][i].x += m_vDiff.x;
                vOut[4][i].x += m_vDiff.x;
                vOut[7][i].x += m_vDiff.x;
            } else {
                vOut[1][i].x += m_vDiff.z;
                vOut[4][i].x += m_vDiff.z;
                vOut[7][i].x += m_vDiff.z;
            }
        }
        for (INT i = 0; i < 4; i++) {
            if (i < 2) {
                vOut[3][i].y += m_vDiff.y;
                vOut[4][i].y += m_vDiff.y;
                vOut[5][i].y += m_vDiff.y;
            } else {
                vOut[3][i].y += m_vDiff.w;
                vOut[4][i].y += m_vDiff.w;
                vOut[5][i].y += m_vDiff.w;
            }
        }
    }
}
TIMEQ C2DSprObj::initForSeq(SEQ_ANIMATION* pSeqAnim, LAY_SCREEN* pLayScreen) {
    IVECTOR iVec;
    FVECTOR fVec;
    INT i;
    m_iStep = 1;
    m_status = pSeqAnim->status;
    m_status.base_disable |= m_status.base_stop;
    m_status.ofs_disable |= m_status.ofs_stop;
    m_status.rotate_disable |= m_status.rotate_stop;
    m_status.color_disable |= m_status.color_stop;
    m_status.scale_disable |= m_status.scale_stop;
    FVECTOR fVec2 = { 0.25f, 0.25f, 0.25f, 0.25f };
    INT iAlphaBlend = pSeqAnim->alpha_blend;
    if (iAlphaBlend) {
        INT a = iAlphaBlend == 1 ? ALPHA_PLUS : ALPHA_MINUS;
        for (i = 0; i < m_nPoly; i++) {
            m_pSprPrim[i].polyGT4.setSemiTrans(a);
        }
    }
    m_qTime[0] = pSeqAnim->time[0] * 50;
    m_qTime[1] = pSeqAnim->time[1] * 50;
    if (pLayScreen) {
        m_qTime[0] += pLayScreen->time * 50;
        m_qTime[1] += pLayScreen->time * 50;
    }
    iVec.set(pSeqAnim->x_base[0], pSeqAnim->y_base[0], pSeqAnim->x_base[1], pSeqAnim->y_base[1]);
    if (pLayScreen) {
        iVec.x += pLayScreen->x - 256;
        iVec.z += pLayScreen->x - 256;
        iVec.y += pLayScreen->y - SCREEN_HEIGHT / 2;
        iVec.w += pLayScreen->y - SCREEN_HEIGHT / 2;
    }
    ITOF0Vector(fVec, iVec);
    AddVector(fVec, fVec, fVec2);
    m_vBase[0].set(fVec.x, fVec.y);
    m_vBase[1].set(fVec.z, fVec.w);
    m_vPos = m_vBase[0];
    iVec.set(pSeqAnim->x_ofs[0], pSeqAnim->y_ofs[0], pSeqAnim->x_ofs[1], pSeqAnim->y_ofs[1]);
    ITOF0Vector(fVec, iVec);
    m_vOffset[0].set(fVec.x, fVec.y);
    m_vOffset[1].set(fVec.z, fVec.w);
    m_vOfs = m_vOffset[0];
    m_vRotate[0].set(pSeqAnim->x_rotate[0], pSeqAnim->y_rotate[0], -pSeqAnim->z_rotate[0], 0.0f);
    m_vRotate[1].set(pSeqAnim->x_rotate[1], pSeqAnim->y_rotate[1], -pSeqAnim->z_rotate[1], 0.0f);
    m_vRot = m_vRotate[0];
    Math3D::LimitPIVector(m_vRot);
    m_vScale[0].set(pSeqAnim->scale[0] * pSeqAnim->scale_x[0], pSeqAnim->scale[0] * pSeqAnim->scale_y[0]);
    m_vScale[1].set(pSeqAnim->scale[1] * pSeqAnim->scale_x[1], pSeqAnim->scale[1] * pSeqAnim->scale_y[1]);
    m_vScl = m_vScale[0];
    iVec.set(pSeqAnim->color[0].r, pSeqAnim->color[0].g, pSeqAnim->color[0].b, pSeqAnim->color[0].a);
    ITOF0Vector(fVec, iVec);
    ScaleVector(m_vColor[0], fVec, 0.0078125f);
    iVec.set(pSeqAnim->color[1].r, pSeqAnim->color[1].g, pSeqAnim->color[1].b, pSeqAnim->color[1].a);
    ITOF0Vector(fVec, iVec);
    ScaleVector(m_vColor[1], fVec, 0.0078125f);
    m_vCol = m_vColor[0];
    if (!m_status.curve_disable) {
        m_vCurve.set(pSeqAnim->curve[0][0], pSeqAnim->curve[0][1], pSeqAnim->curve[1][0], pSeqAnim->curve[1][1]);
    }
    if (!m_status.sway_disable) {
        fVec.set(pSeqAnim->x_sway[0], pSeqAnim->y_sway[0], pSeqAnim->x_sway[1], pSeqAnim->y_sway[1]);
        m_vSway[0].set(fVec.x, fVec.y);
        m_vRotate[0].w = (FLOAT)pSeqAnim->x_sway_count;
        m_vSway[1].set(fVec.z, fVec.w);
        m_vRotate[1].w = (FLOAT)pSeqAnim->y_sway_count;
    }
    return m_qTime[1] + (m_status.last_cut ? 0 : 50);
}
void C2DSprObj::_main(TIMEQ qTime) {
    FLOAT fRate;
    FVECTOR vec0;
    switch (m_iStep) {
    case 0:
        break;
    case 1:
        if (qTime < m_qTime[0]) break;
        m_fgDisp = 1;
        m_iStep = 2;
    case 2:
        if (qTime >= m_qTime[1] + (m_status.last_cut ? 0 : 50)) {
            m_iStep = 4;
            m_fgDisp = 0;
            if (!(m_Flg & 4)) break;
        }
        if (qTime >= m_qTime[1]) {
            fRate = 1.0f;
        } else {
            fRate = (FLOAT)(qTime - m_qTime[0]) / (FLOAT)(m_qTime[1] - m_qTime[0]);
        }
        if (!m_status.curve_disable) {
            float t2 = fRate * fRate;
            float t3 = t2 * fRate;
            float r = 1.0f - fRate;
            float r2 = r * r;
            float r3 = r2 * r;
            fRate = 1.0f - (r3 + 3.0f * r2 * fRate * (1.0f - m_vCurve.y) + 3.0f * r * t2 * (1.0f - m_vCurve.z) + t3 * 0.0f);
        }
        if (!m_status.base_disable) InterVector2(m_vPos, m_vBase[1], m_vBase[0], fRate);
        if (!m_status.ofs_disable) InterVector2(m_vOfs, m_vOffset[1], m_vOffset[0], fRate);
        if (!m_status.rotate_disable) {
            InterVector(m_vRot, m_vRotate[1], m_vRotate[0], fRate);
            Math3D::LimitPIVector(m_vRot);
        }
        if (!m_status.scale_disable) InterVector2(m_vScl, m_vScale[1], m_vScale[0], fRate);
        if (!m_status.color_disable) InterVector(m_vCol, m_vColor[1], m_vColor[0], fRate);
        if (!m_status.sway_disable) {
            InterVector2(m_vSwy, m_vSway[1], m_vSway[0], fRate);
            if (!m_status.sway_lag) {
                FLOAT f = fRate * 3.1415927f;
                m_vSwy.x *= sinf(f * m_vRotate[0].w);
                m_vSwy.y *= sinf(f * m_vRotate[1].w);
            } else {
                FLOAT f = fRate * 3.1415927f;
                m_vSwy.x *= cosf(f * m_vRotate[0].w);
                m_vSwy.y *= cosf(f * m_vRotate[1].w);
            }
        }
        m_Flg |= 1;
        break;
    }
    if (!(m_Flg & 1)) return;
    {
        if (!m_pSeqControl || !m_pSeqControl->fg3D) {
            move();
        } else if (m_fgParent == 0) {
            m_fgRotTransPers = 0;
            m_pSeqControl->fg3D = 1;
            if (!m_fgDisp) move();
            m_fgParent = 1;
        } else {
            if (m_fgParent > 0) {
                m_fgRotTransPers = 1;
            } else {
                m_fgRotTransPers = 2;
            }
        }
        m_Flg &= ~1;
    }
}
void MulMatrix(FMATRIX& m0, const FMATRIX& m1, const FMATRIX& m2);
void MulVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void FTOI0Vector(IVECTOR& v0, const FVECTOR& v1);

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
static inline void _applyMatrix2(FVECTOR& v0, const FMATRIX& m0, const FVECTOR2& v2) {
    /* MATCH: the original passes the FVECTOR2 to qmtc2 in t0 ($8); an unpinned temp gets another register */
    register ULONGLONG xy asm("$8") = v2._u64;
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x10(%1)\n"
                 "lqc2 $vf6, 0x20(%1)\n"
                 "lqc2 $vf7, 0x30(%1)\n"
                 "qmtc2.ni %2, $vf8\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf0z\n"
                 "vmaddw.xyzw $vf4, $vf7, $vf0w\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&v0), "r"(&m0), "r"(xy)
                 : "memory");
}
static inline void _ftoi4(IVECTOR& iVec, const FVECTOR& fVec) {
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vftoi4.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&iVec), "r"(&fVec)
                 : "memory");
}
void C2DSprObj::move() {
    FVECTOR fVec;
    IVECTOR iVec;
    FMATRIX mat;
    FMATRIX mat2;
    FVECTOR vCol;
    CHAR fgBilinear;
    u_long128 lCol;
    BOOL bBilinear;
    FVECTOR2 primPosBuf[9][4];
    fgBilinear = 0;
    setBilinear(m_status.bilinear_disable);
    _unitMatrixVu0(mat);
    mat.m[0][0] = m_vScl.x;
    mat.m[1][1] = m_vScl.y;
    RotMatrix(mat, mat, m_vRot);
    AddVector2(fVec, m_vPos, m_vSwy);
    mat.v[3] = fVec;
    _unitMatrixVu0(mat2);
    *(FVECTOR2*)&fVec = m_vOfs;
    mat2.v[3] = fVec;
    MulMatrix(mat, mat, mat2);
    vCol = m_vCol;
    if (m_pSeqControl) {
        if (m_fgParent == 0) {
            MulMatrix(mat, m_pSeqControl->matrix, mat);
            m_pSeqControl->parentMatrix = mat;
            if (m_pSeqControl->fgColor) {
                MulVector(vCol, m_pSeqControl->vCol, vCol);
                m_pSeqControl->parentColor = vCol;
                if (m_pSeqControl->pParentSeqControl) {
                    MulMatrix(mat, m_pSeqControl->pParentSeqControl->controlMatrix, mat);
                    MulVector(vCol, m_pSeqControl->pParentSeqControl->controlColor, vCol);
                    fgBilinear = m_pSeqControl->pParentSeqControl->fgBilinear;
                }
                m_pSeqControl->controlMatrix = mat;
                m_pSeqControl->controlColor = vCol;
            } else {
                if (m_pSeqControl->pParentSeqControl) {
                    MulMatrix(mat, m_pSeqControl->pParentSeqControl->controlMatrix, mat);
                    fgBilinear = m_pSeqControl->pParentSeqControl->fgBilinear;
                }
                m_pSeqControl->controlMatrix = mat;
            }
            m_fgParent = 1;
            if (mat.m[0][0] != 1.0f || mat.m[1][1] != 1.0f || mat.m[2][2] != 1.0f) setBilinear(TRUE);
            if (fgBilinear) setBilinear(TRUE);
            m_pSeqControl->fgBilinear = isBilinear();
        } else {
            if (m_pSeqControl->fgColor) {
                if (m_fgParent < 0 && m_pSeqControl->parentMatrix.m[3][3] != 0.0f) {
                    MulMatrix(mat, m_pSeqControl->parentMatrix, mat);
                    MulVector(vCol, m_pSeqControl->parentColor, vCol);
                } else {
                    MulMatrix(mat, m_pSeqControl->matrix, mat);
                    MulVector(vCol, m_pSeqControl->vCol, vCol);
                }
                if (m_pSeqControl->pParentSeqControl) {
                    MulMatrix(mat, m_pSeqControl->pParentSeqControl->controlMatrix, mat);
                    MulVector(vCol, m_pSeqControl->pParentSeqControl->controlColor, vCol);
                }
            } else {
                if (m_fgParent < 0 && m_pSeqControl->parentMatrix.m[3][3] != 0.0f) {
                    MulMatrix(mat, m_pSeqControl->parentMatrix, mat);
                } else {
                    MulMatrix(mat, m_pSeqControl->matrix, mat);
                }
                if (m_pSeqControl->pParentSeqControl) {
                    MulMatrix(mat, m_pSeqControl->pParentSeqControl->controlMatrix, mat);
                }
            }
            setBilinear(m_pSeqControl->fgBilinear);
            if (mat.m[0][0] != 1.0f || mat.m[1][1] != 1.0f || mat.m[2][2] != 1.0f) setBilinear(TRUE);
        }
    } else {
        if (mat.m[0][0] != 1.0f || mat.m[1][1] != 1.0f || mat.m[2][2] != 1.0f) setBilinear(TRUE);
    }
    ScaleVector(vCol, vCol, 128.0f);
    ClampVector(vCol, vCol, 0.0f, 255.0f);
    FTOI0Vector(*(IVECTOR*)&lCol, vCol);
    bBilinear = isBilinear();
    if (m_Flg & 8) {
        for (INT i = 0; i < m_nPoly; i++) {
            getRect(primPosBuf[i], m_pSprPrim[i]);
        }
        getFukidasiRect(primPosBuf);
    }
    for (INT i = 0; i < m_nPoly; i++) {
        SprPrim* pSprPrim = &m_pSprPrim[i];
        FVECTOR2* primPos;
        if (m_Flg & 8) {
            primPos = primPosBuf[i];
        } else {
            getRect(primPosBuf[0], *pSprPrim);
            primPos = primPosBuf[0];
        }
        G_RGBA* primCol = (G_RGBA*)((BYTE*)pSprPrim->pSprParts + 0x1C);
        G_XY* pXY = &pSprPrim->polyGT4.m_P0;
        G_RGBA* pRGBA = &pSprPrim->polyGT4.m_C0;
        pSprPrim->polyGT4.setTexNearest(!bBilinear);
        for (INT j = 0; j < 4; j++) {
            FVECTOR& v0 = fVec;
            _applyMatrix2(v0, mat, primPos[j]);
            _ftoi4(iVec, fVec);
            if (!bBilinear) {
                iVec.x = (iVec.x & 0xFFFFFFF0) | 8;
                iVec.y = (iVec.y & 0xFFFFFFF0) | 8;
            }
            pXY->set(iVec.x, iVec.y);
            pXY++;
            pRGBA->col = _vecToCol(_mulCol(_colToVec(primCol[j].col), lCol));
            pRGBA++;
        }
    }
}
void C2DSprObj::getRect(FVECTOR2* pvOut4, SprPrim& rSprPrim) {
    FVECTOR fVec;
    IVECTOR iVec;
    SPR_GROUP* pGroup = rSprPrim.pSprGroupe;
    iVec.set(pGroup->x0, pGroup->y0, pGroup->x1, pGroup->y1);
    _itof0(fVec, iVec);
    pvOut4[0].set(fVec.x, fVec.y);
    pvOut4[1].set(fVec.z, fVec.y);
    pvOut4[2].set(fVec.x, fVec.w);
    pvOut4[3].set(fVec.z, fVec.w);
}
INT C2DSprObj::_disp() {
    IVECTOR iVec;
    FVECTOR fVec;
    if (!m_fgDisp) return 0;
    if (m_fgRotTransPers >= 0) {
        if (m_fgRotTransPers == 0) {
            SetRotTransPersMatrix(CWorldEnv::getWorldScreenMatrix());
            RotTransPersClipW(iVec, m_pSeqControl->vPos);
            if (iVec.w != 0 || iVec.x < 0x5000 || iVec.x > 0xb000 || iVec.y < 0x5000 || iVec.y > 0xb000) {
                m_pSeqControl->fg3D = -1;
                return 0;
            }
            ITOF4Vector(fVec, iVec);
            m_pSeqControl->matrix.v[3].set(fVec.x - 2048.0f, fVec.y - 2048.0f, 0.0f, 1.0f);
            m_fgParent = 0;
            move();
        } else {
            if (m_pSeqControl->fg3D < 0) return 0;
            m_fgParent = (m_fgRotTransPers == 1) ? 1 : -1;
            move();
        }
    }
    if (!m_pScissoring) {
        for (INT i = 0; i < m_nPoly; i++) {
            SprPrim* pSprPrim = &m_pSprPrim[i];
            CPolyGT4ST* pPrim = &pSprPrim->polyGT4;
            INT w0;
            INT w1;
            if (pPrim->m_P0.x < pPrim->m_P1.x) {
                w0 = pPrim->m_P0.x;
                w1 = pPrim->m_P1.x;
            } else {
                w1 = pPrim->m_P0.x;
                w0 = pPrim->m_P1.x;
            }
            if (pPrim->m_P2.x < pPrim->m_P3.x) {
                if (pPrim->m_P2.x < w0) w0 = pPrim->m_P2.x;
                if (w1 < pPrim->m_P3.x) w1 = pPrim->m_P3.x;
            } else {
                if (pPrim->m_P3.x < w0) w0 = pPrim->m_P3.x;
                if (w1 < pPrim->m_P2.x) w1 = pPrim->m_P2.x;
            }
            if (w1 < -0x1000 || w0 > 0x1000) continue;
            if (pPrim->m_P0.y < pPrim->m_P1.y) {
                w0 = pPrim->m_P0.y;
                w1 = pPrim->m_P1.y;
            } else {
                w1 = pPrim->m_P0.y;
                w0 = pPrim->m_P1.y;
            }
            if (pPrim->m_P2.y < pPrim->m_P3.y) {
                if (pPrim->m_P2.y < w0) w0 = pPrim->m_P2.y;
                if (w1 < pPrim->m_P3.y) w1 = pPrim->m_P3.y;
            } else {
                if (pPrim->m_P3.y < w0) w0 = pPrim->m_P3.y;
                if (w1 < pPrim->m_P2.y) w1 = pPrim->m_P2.y;
            }
            if (w1 < -((SCREEN_HEIGHT >> 1) << 4) || w0 > ((SCREEN_HEIGHT >> 1) << 4)) continue;
            pSprPrim->polyGT4.CGPrim::disp();
        }
        return 0;
    }
    SprPrim* pSprPrim = m_pSprPrim;
    G_XY p0;
    G_XY p1;
    p0.set((pSprPrim->polyGT4.m_P0.x >> 4) + 256, (pSprPrim->polyGT4.m_P0.y >> 4) + SCREEN_HEIGHT / 2);
    p1.set((pSprPrim->polyGT4.m_P3.x >> 4) + 256, (pSprPrim->polyGT4.m_P3.y >> 4) + SCREEN_HEIGHT / 2);
    if (p0.x < 0) p0.x = 0;
    if (p0.x >= 0x200) p0.x = 0x1ff;
    if (p0.y < 0) p0.y = 0;
    if (p0.y > SCREEN_HEIGHT - 1) p0.y = SCREEN_HEIGHT - 1;
    if (p1.x < 0) p1.x = 0;
    if (p1.x >= 0x200) p1.x = 0x1ff;
    if (p1.y < 0) p1.y = 0;
    if (p1.y > SCREEN_HEIGHT - 1) p1.y = SCREEN_HEIGHT - 1;
    m_pScissoring->setRect(p0, p1);
    m_pScissoring->disp();
    return m_pScissoring->getLayerDepth();
}
void C2DSprObj::changeTexture(INT inNum, const TexInfo* pTexInfo, G_UV* pStart, G_UV* pEnd) {
    if (!m_pSprPrim) return;
    if (inNum >= m_nPoly) return;
#line 731
    ASSERT(inNum >= 0);
    SprPrim* pSprPrim = &m_pSprPrim[inNum];
    const VTransInfo* pTransInfo;
    if (pTexInfo) {
        pSprPrim->polyGT4.setTPage((TexInfo*)pTexInfo);
        pTransInfo = &pTexInfo->_TexInfo;
    } else {
        pTransInfo = &m_pTexInfo->_TexInfo;
    }
    FLOAT fW;
    FLOAT fH;
    USHORT nW = pTransInfo->_nDistW;
    if (pTransInfo->_nFormat == 0) {
        fW = nW * 2;
    } else {
        fW = nW;
    }
    USHORT nH = pTransInfo->_nDistH;
    if (pTransInfo->_nFormat == 0) {
        fH = nH * 2;
    } else {
        fH = nH;
    }
    FLOAT fU0 = pStart->u / fW;
    FLOAT fU1 = pEnd->u / fW;
    FLOAT fV0 = pStart->v / fH;
    FLOAT fV1 = pEnd->v / fH;
    pSprPrim->polyGT4.m_T0.set(fU0, fV0);
    pSprPrim->polyGT4.m_T1.set(fU1, fV0);
    pSprPrim->polyGT4.m_T2.set(fU0, fV1);
    pSprPrim->polyGT4.m_T3.set(fU1, fV1);
}
void C2DSprObj::changeLayer(INT nLayer) {
    for (INT i = 0; i < m_nPoly; i++) {
        SprPrim* pSprPrim = &m_pSprPrim[i];
        pSprPrim->polyGT4.setLayerFG(nLayer);
    }
    if (m_pScissoring) m_pScissoring->setLayerFG(nLayer);
}
void C2DSprObj::shiftBase(FLOAT fX, FLOAT fY) {
    m_vBase[0].x += fX;
    m_vBase[0].y += fY;
    m_vBase[1].x += fX;
    m_vBase[1].y += fY;
    m_vPos.x = m_vBase[0].x;
    m_vPos.y = m_vBase[0].y;
}
void C2DSprObj::restart() {
    m_fgDisp = 0;
    m_iStep = 1;
}

// SOURCE DATA: .rodata
