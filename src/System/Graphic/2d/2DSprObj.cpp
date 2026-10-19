#define GPRIM_H_PATH "System/Graphic/Gprim.h"
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/2d/2DSprObj.h"
#include "include_asm.h"

#include "System/Graphic/2d/2DSpr.h"
#include "System/Graphic/2d/2DSeqObj.h"

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

extern "C" char D_004672C0[], D_004672F0[]; /* PARTIAL */
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", init__Q22D29C2DSprObjUi);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x322F6432\n.word 0x72705344\n.word 0x2E6A624F\n.word 0x28707063\n.word 0x29343531\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", initSprite__Q22D29C2DSprObjiPQ22D29SPR_GROUPPQ22D29SPR_PARTS);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x322F6432\n.word 0x72705344\n.word 0x2E6A624F\n.word 0x00707063\n.word 0x754E6E69\n.word 0x3D3E206D\n.word 0x00003020\n.word 0x00000000\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x322F6432\n.word 0x72705344\n.word 0x2E6A624F\n.word 0x28707063\n.word 0x29343931\n.word 0x00000000\n.text\n");
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", _main__Q22D29C2DSprObji);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", _disp__Q22D29C2DSprObj);
void C2DSprObj::changeTexture(INT inNum, const TexInfo* pTexInfo, G_UV* pStart, G_UV* pEnd) {
    if (!m_pSprPrim) return;
    if (inNum >= m_nPoly) return;
    if (!(inNum >= 0)) AssertApp(D_004672C0, 731, D_004672F0, 0); /* PARTIAL: strings owned by INCLUDE_ASM'd initSprite */
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
