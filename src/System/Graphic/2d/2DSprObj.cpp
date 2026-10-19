#define GPRIM_H_PATH "System/Graphic/Gprim.h"
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h"
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

namespace D2 {
class C2DMan { /* System/Graphic/2d/2DMan.h */
public:
    static INT m_nLayer;
    static INT m_fg2DViewer;
};
}

using namespace D2;

/* System/Graphic/modeldata.h (its inline functions' strings are not in this object) */
struct VTransInfo {
    UINT _nDistPos;
    USHORT _nFormat;
    USHORT _nDistBufW;
    ULONGLONG* _pImage;
    UINT _qwImage;
    USHORT _nDistX;
    USHORT _nDistY;
    USHORT _nDistW;
    USHORT _nDistH;
    USHORT _nTw;
    USHORT _nTh;
    UINT _nTransSize;
};
struct TexInfo {
    ULONGLONG _nTPage;
    VTransInfo _TexInfo;
    VTransInfo _ClutInfo;
};

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new[](size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}

/* PARTIAL: the 2D viewer's SeqControl is not linked into the game: lui/addiu of address 0 (name unknown) */
extern SeqControl s_ViewerSeqControl;
/* Ps2EeAs makes undefined weak symbols global: define it as weak absolute 0 (as Game/Enemy/CreateEnemy does) */
__asm__(".weak s_ViewerSeqControl\ns_ViewerSeqControl = 0\n");

extern "C" float sinf(float x);
extern "C" float cosf(float x);
extern "C" void sceVu0InterVector(float* v0, float* v1, float* v2, float t);
extern INT SCREEN_HEIGHT;
void RotTransPersClipW(IVECTOR& iVec, const FVECTOR& fVec);
void ITOF4Vector(FVECTOR& fVec, const IVECTOR& iVec);

static inline void _loadRotTransPersMatrix(const FMATRIX& m) {
    asm volatile("vsub.xyzw $vf3, $vf0, $vf0\n"
                 "lqc2 $vf4, 0x0(%0)\n"
                 "lqc2 $vf5, 0x10(%0)\n"
                 "lqc2 $vf6, 0x20(%0)\n"
                 "lqc2 $vf7, 0x30(%0)\n"
                 :
                 : "r"(&m));
}

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

inline void operator delete[](void* pv) { com_free(pv); }

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
    m_vRot._q = 0;
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
        m_pScissoring = new (com_malloc_d(sizeof(CScissoring), __FILE__ "(194)", RET_ADDR())) CScissoring;
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", initForSeq__Q22D29C2DSprObjPQ22D213SEQ_ANIMATIONPQ22D210LAY_SCREEN);
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
            sceVu0InterVector((float*)&m_vRot, (float*)&m_vRotate[1], (float*)&m_vRotate[0], fRate);
            Math3D::LimitPIVector(m_vRot);
        }
        if (!m_status.scale_disable) InterVector2(m_vScl, m_vScale[1], m_vScale[0], fRate);
        if (!m_status.color_disable) sceVu0InterVector((float*)&m_vCol, (float*)&m_vColor[1], (float*)&m_vColor[0], fRate);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", move__Q22D29C2DSprObj);
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
            _loadRotTransPersMatrix(CWorldEnv::getWorldScreenMatrix());
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
