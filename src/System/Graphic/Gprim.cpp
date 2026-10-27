#define SYSTEM_GRAPHIC_GPRIM_CPP
#include "khtypes.h"
#include "System/Graphic/Gprim.h"
#include "System/Coord.h"
#include "include_asm.h"

extern INT SCREEN_HEIGHT;
enum NAME_CASE { NAME_CASE_SENSITIVE = 0 }; /* PARTIAL: System/RsrcData.h */
class CRsrcDB { /* PARTIAL: System/RsrcDB.h does not compile on its own here */
public:
    static struct TexInfo* getRsrcTex(const CHAR* pszName, NAME_CASE nc);
};
ULONGLONG* _makePkt_TexLine(ULONGLONG* p64, G_STRIP* it, G_STRIP* end);
ULONGLONG* _makePkt_Line(ULONGLONG* p64, G_STRIP* it, G_STRIP* end);
ULONGLONG* _makePkt_TexLine(ULONGLONG* p64, G_STRIP* it, G_STRIP* end, CCoord* pCrd);
ULONGLONG* _makePkt_Line(ULONGLONG* p64, G_STRIP* it, G_STRIP* end, CCoord* pCrd);
ULONGLONG* _makePkt_TexTriangle(ULONGLONG* p64, G_STRIP* it, G_STRIP* end, BOOL Back);
ULONGLONG* _makePkt_Triangle(ULONGLONG* p64, G_STRIP* it, G_STRIP* end, BOOL Back);
QWdata* _makePkt_TexLineStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end);
QWdata* _makePkt_LineStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end);
ULONGLONG* _makePkt_TexTriangle(ULONGLONG* p64, G_STRIP* it, G_STRIP* end, CCoord* pCrd, BOOL Back);
QWdata* _makePkt_TexLineStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end, CCoord* pCrd);
QWdata* _makePkt_TexTriangleStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end, BOOL Back);
QWdata* _makePkt_TexTriangleStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end, CCoord* pCrd, BOOL Back);
QWdata* _makePkt_TexTriangleFan(QWdata* pQw, G_STRIP* it, G_STRIP* end, BOOL Back);
QWdata* _makePkt_TexTriangleFan(QWdata* pQw, G_STRIP* it, G_STRIP* end, CCoord* pCrd, BOOL Back);
ULONGLONG* _makePkt_Triangle(ULONGLONG* p64, G_STRIP* it, G_STRIP* end, CCoord* pCrd, BOOL Back);
QWdata* _makePkt_LineStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end, CCoord* pCrd);
QWdata* _makePkt_TriangleStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end, BOOL Back);
QWdata* _makePkt_TriangleStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end, CCoord* pCrd, BOOL Back);
QWdata* _makePkt_TriangleFan(QWdata* pQw, G_STRIP* it, G_STRIP* end, BOOL Back);
QWdata* _makePkt_TriangleFan(QWdata* pQw, G_STRIP* it, G_STRIP* end, CCoord* pCrd, BOOL Back);
template <class T> static inline T& _ref(T& v) { return v; }
class CWorldEnv { /* PARTIAL: System/Graphic/WorldEnv.h */
public:
    static FMATRIX& getWorldScreenMatrix();
};
class CPackMan { /* PARTIAL: System/Graphic/PackMan.cpp */
public:
    static u_long128* pkOpenGifTag(u_long128 tag);
    static void pkAddGsAD(UINT addr, u_long data);
    static void pkCloseGifTag();
};
class CModelMan { /* PARTIAL: System/Graphic/ModelMan.h */
public:
    static void add2DispList(CDrawable& inPrim, FIX4 inZ);
};

void CGPrim::disp() {
    FIX4 z = calcZ();
    if (z != (FIX4)0xF0000000) {
        CModelMan::add2DispList(*this, z);
    }
}
void CGTexPrim::setTPage(const CHAR* pTexName) {
    m_pTexInfo = CRsrcDB::getRsrcTex(pTexName, NAME_CASE_SENSITIVE);
}
void CGTexPrim2D::setTPage(const CHAR* pTexName) {
    m_pTexInfo = CRsrcDB::getRsrcTex(pTexName, NAME_CASE_SENSITIVE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setTexture__9CGTexPrimPC7TexInfoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setTexture__11CGTexPrim2DPC7TexInfoi);
UINT CGTexPrim::getPacketSize() const {
    if (m_pTexInfo && !m_pTexInfo->isFix()) {
        return 26;
    }
    return 0;
}
UINT CGTexPrim2D::getPacketSize() const {
    if (m_pTexInfo && !m_pTexInfo->isFix()) {
        return 26;
    }
    return 0;
}
void CScissoring::makePkt() {
    QWdata giftag;
    giftag.ul64[0] = 0x1000800000008000UL;
    giftag.ul64[1] = 0xE;
    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsAD(0x41, (u_long)m_P0.x | ((u_long)m_P1.x << 16) | ((u_long)m_P0.y << 32) |
                                  ((u_long)m_P1.y << 48));
    CPackMan::pkCloseGifTag();
}
void CScissoring::setRect(G_XY& p0, G_XY& p1) {
    m_P0 = p0;
    m_P1 = p1;
}
void CScissoring::setDefault() {
    G_XY p0;
    G_XY p1;
    p0.x = 0;
    p0.y = 0;
    p1.x = 511;
    p1.y = SCREEN_HEIGHT - 1;
    setRect(p0, p1);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__6CZFill);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__5CTile);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__5CSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__8CPolyGT4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__10CPolyGT4ST);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__7CLineF2);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__7CPolyG4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__7CCircle);
CScreenColoredMask::CScreenColoredMask() {
    setLayerFG(1);
    m_P0.set(-0x1000, -(_ref(SCREEN_HEIGHT) * 8));
    m_P1.set(0x1000, _ref(SCREEN_HEIGHT) * 8);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__8CRotSprt);
static BOOL __isRect(const FVECTOR& v, const FVECTOR* vRect) {
    FVECTOR __CCTPrim_vDefRect;
    INT iRet;
    if (vRect == NULL) {
        __CCTPrim_vDefRect.x = -256.0f;
        __CCTPrim_vDefRect.y = -SCREEN_HEIGHT / 2;
        __CCTPrim_vDefRect.z = 512.0f;
        __CCTPrim_vDefRect.w = SCREEN_HEIGHT;
        vRect = &__CCTPrim_vDefRect;
    }
    __asm__ volatile(
        "lqc2 $vf4, 0x0(%1)\n"
        "lqc2 $vf5, 0x0(%2)\n"
        "vaddz.x $vf6, $vf5, $vf5z\n"
        "vaddw.y $vf6, $vf5, $vf5w\n"
        "vnop\n"
        "vnop\n"
        "ctc2.ni $0, $vi16\n"
        "vsub.xy $vf0, $vf4, $vf5\n"
        "vsub.xy $vf0, $vf6, $vf4\n"
        "vnop\n"
        "vnop\n"
        "vnop\n"
        "vnop\n"
        "vnop\n"
        "cfc2.ni %0, $vi16\n"
        : "=r"(iRet)
        : "r"(&v), "r"(vRect));
    return (iRet & 0xC0) == 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__6CCPrim);
BOOL CCPrim::isRect(INT i, const FVECTOR* vRect) {
    return __isRect(*(FVECTOR*)&m_V[i], vRect);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__6CTPrim);
BOOL CTPrim::isRect(INT i, const FVECTOR* vRect) {
    return __isRect(*(FVECTOR*)m_V[i].fP, vRect);
}
FLOAT RotTransPers(FVECTOR& out_v, const FMATRIX& in_m, const FVECTOR& in_v) {
    __asm__ volatile(
        "lqc2 $vf8, 0x0($6)\n"
        "lqc2 $vf4, 0x0($5)\n"
        "lqc2 $vf5, 0x10($5)\n"
        "lqc2 $vf6, 0x20($5)\n"
        "lqc2 $vf7, 0x30($5)\n"
        "vmulax.xyzw $ACC, $vf4, $vf8x\n"
        "vmadday.xyzw $ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
        "vdiv $Q, $vf0w, $vf8w\n"
        "vwaitq\n"
        "vmulq.xyz $vf8, $vf8, $Q\n"
        "cfc2.ni $2, $vi22\n"
        "mtc1 $2, $f0\n"
        "sqc2 $vf8, 0x0($4)\n"
    );
}
FLOAT RotTransPers(IVECTOR& out_v, const FMATRIX& in_m, const FVECTOR& in_v) {
    __asm__ volatile(
        "lqc2 $vf8, 0x0($6)\n"
        "lqc2 $vf4, 0x0($5)\n"
        "lqc2 $vf5, 0x10($5)\n"
        "lqc2 $vf6, 0x20($5)\n"
        "lqc2 $vf7, 0x30($5)\n"
        "vmulax.xyzw $ACC, $vf4, $vf8x\n"
        "vmadday.xyzw $ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
        "vdiv $Q, $vf0w, $vf8w\n"
        "vwaitq\n"
        "vmulq.xyz $vf8, $vf8, $Q\n"
        "cfc2.ni $2, $vi22\n"
        "mtc1 $2, $f0\n"
        "vftoi4.xyzw $vf8, $vf8\n"
        "sqc2 $vf8, 0x0($4)\n"
    );
}
u_long128 RotTransPersW(const FMATRIX& in_m, const FVECTOR& in_v) {
    __asm__ volatile(
        "lqc2 $vf8, 0x0($5)\n"
        "lqc2 $vf4, 0x0($4)\n"
        "lqc2 $vf5, 0x10($4)\n"
        "lqc2 $vf6, 0x20($4)\n"
        "lqc2 $vf7, 0x30($4)\n"
        "vmulax.xyzw $ACC, $vf4, $vf8x\n"
        "vmadday.xyzw $ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
        "vdiv $Q, $vf0w, $vf8w\n"
        "vmax.w $vf8, $vf0, $vf0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf8, $vf8, $Q\n"
        "vftoi4.xyz $vf8, $vf8\n"
        "qmfc2.ni $2, $vf8\n"
    );
}
FLOAT RotTransPersClipW(IVECTOR& out_v, const FVECTOR& in_v) {
    __asm__ volatile(
        "lqc2 $vf8, 0x0($5)\n"
        "vmulax.xyzw $ACC, $vf4, $vf8x\n"
        "vmadday.xyzw $ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
        "vdiv $Q, $vf0w, $vf8w\n"
        "vmulaw.xyz $ACC, $vf3, $vf8w\n"
        "vmaddw.xyz $vf8, $vf8, $vf0w\n"
        "vsub.xyzw $vf10, $vf0, $vf0\n"
        "lui $2, (0x45800000 >> 16)\n"
        "vwaitq\n"
        "vmulq.xyz $vf8, $vf8, $Q\n"
        "pextlw $2, $2, $2\n"
        "qmtc2.ni $2, $vf11\n"
        "ctc2.ni $0, $vi16\n"
        "vsub.xyzw $vf0, $vf8, $vf10\n"
        "vsub.xy $vf0, $vf11, $vf8\n"
        "vftoi4.xyzw $vf9, $vf8\n"
        "vnop\n"
        "cfc2.ni $2, $vi22\n"
        "mtc1 $2, $f0\n"
        "sqc2 $vf9, 0x0($4)\n"
        "cfc2.ni $2, $vi16\n"
        "andi $2, $2, 0xC0\n"
        ".set noreorder\n"
        "bnel $2, $0, 0f\n"
        "ori $2, $0, 0x8000\n"
        ".set reorder\n"
        "0:\n"
        "sw $2, 0xC($4)\n"
    );
}
FLOAT RotTransPersClipW(IVECTOR& out_v, FVECTOR& out_fv, const FVECTOR& in_v) {
    __asm__ volatile(
        "lqc2 $vf8, 0x0($6)\n"
        "vmulax.xyzw $ACC, $vf4, $vf8x\n"
        "vmadday.xyzw $ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
        "vdiv $Q, $vf0w, $vf8w\n"
        "vmulaw.xyz $ACC, $vf3, $vf8w\n"
        "vmaddw.xyz $vf8, $vf8, $vf0w\n"
        "vsub.xyzw $vf10, $vf0, $vf0\n"
        "lui $2, (0x45800000 >> 16)\n"
        "vwaitq\n"
        "vmulq.xyz $vf8, $vf8, $Q\n"
        "pextlw $2, $2, $2\n"
        "qmtc2.ni $2, $vf11\n"
        "ctc2.ni $0, $vi16\n"
        "vsub.xyzw $vf0, $vf8, $vf10\n"
        "vsub.xy $vf0, $vf11, $vf8\n"
        "vftoi4.xyzw $vf9, $vf8\n"
        "sqc2 $vf8, 0x0($5)\n"
        "cfc2.ni $2, $vi22\n"
        "mtc1 $2, $f0\n"
        "sqc2 $vf9, 0x0($4)\n"
        "cfc2.ni $2, $vi16\n"
        "andi $2, $2, 0xC0\n"
        ".set noreorder\n"
        "bnel $2, $0, 0f\n"
        "ori $2, $0, 0x8000\n"
        ".set reorder\n"
        "0:\n"
        "sw $2, 0xC($4)\n"
    );
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", BackClip__FPC7FVECTORi);
static inline void loadMatrix_VF04_07(FMATRIX& m) {
    __asm__ volatile(
        "lqc2 $vf4, 0x0(%0)\n"
        "lqc2 $vf5, 0x10(%0)\n"
        "lqc2 $vf6, 0x20(%0)\n"
        "lqc2 $vf7, 0x30(%0)\n"
        :
        : "r"(&m)
        : "memory");
}
static inline void setRotTransPersMatrix(FMATRIX& m) {
    __asm__ volatile(
        "vsub.xyzw $vf3, $vf0, $vf0\n"
        "lqc2 $vf4, 0x0(%0)\n"
        "lqc2 $vf5, 0x10(%0)\n"
        "lqc2 $vf6, 0x20(%0)\n"
        "lqc2 $vf7, 0x30(%0)\n"
        :
        : "r"(&m));
}
FIX4 C3DPolyFT4::calcZ() {
    FVECTOR c;
    FMATRIX rts;
    FVECTOR* fv;
    IVECTOR* iv;
    FLOAT fW;
    INT max;
    INT i;

    m_pCrd->getRotTransPersMatrix(rts);
    fv = &m_P0;
    iv = &p0;
    max = 0;
    setRotTransPersMatrix(rts);
    for (i = 0; i < 4; i++) {
        fW = RotTransPersClipW(iv[i], fv[i]);
        if (iv[i].w) {
            return (FIX4)0xF0000000;
        }
        *(FLOAT*)&iv[i].w = fW;
        if (max < iv[i].z) {
            max = iv[i].z;
        }
    }
    return max;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__10C3DPolyFT4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__10C3DPolyGT4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__10C3DPolyGT4);
FIX4 C3DPolyFT3::calcZ() {
    FMATRIX rts;
    FVECTOR* fv;
    IVECTOR* iv;
    FLOAT fW;
    INT max;
    INT i;

    m_pCrd->getRotTransPersMatrix(rts);
    fv = &m_P0;
    iv = &p0;
    max = 0;
    setRotTransPersMatrix(rts);
    for (i = 0; i < 3; i++) {
        fW = RotTransPersClipW(iv[i], fv[i]);
        if (iv[i].w) {
            return (FIX4)0xF0000000;
        }
        *(FLOAT*)&iv[i].w = fW;
        if (max < iv[i].z) {
            max = iv[i].z;
        }
    }
    return max;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__10C3DPolyFT3);
FIX4 C3DPolyGT3::calcZ() {
    FMATRIX rts;
    FVECTOR* fv;
    IVECTOR* iv;
    FLOAT fW;
    INT max;
    INT i;

    m_pCrd->getRotTransPersMatrix(rts);
    fv = &m_P0;
    iv = &p0;
    max = 0;
    setRotTransPersMatrix(rts);
    for (i = 0; i < 3; i++) {
        fW = RotTransPersClipW(iv[i], fv[i]);
        if (iv[i].w) {
            return (FIX4)0xF0000000;
        }
        *(FLOAT*)&iv[i].w = fW;
        if (max < iv[i].z) {
            max = iv[i].z;
        }
    }
    return max;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__10C3DPolyGT3);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__7C3DTile);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__7C3DTile);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__7C3DSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__7C3DSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__10C3DRotSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__10C3DRotSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__9C3DCircle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__9C3DCircle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__13C3DPointLight);
FIX4 C3DPointLight2::calcZ() {
    FIX4 nRet = ((CGPrim&)m_Cl[0]).calcZ();
    if (m_nType == 1) {
        ((CGPrim&)m_Cl[1]).calcZ();
    }
    return nRet;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__14C3DPointLight2);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePktSub__14C3DPointLight2i);
FIX4 C3DPolyG4::calcZ() {
    FVECTOR c;
    FMATRIX rts;
    FVECTOR* fv;
    IVECTOR* iv;
    INT max;
    INT i;

    m_pCrd->getRotTransPersMatrix(rts);
    fv = &m_P0;
    iv = &p0;
    max = 0;
    setRotTransPersMatrix(rts);
    for (i = 0; i < 4; i++) {
        RotTransPersClipW(iv[i], fv[i]);
        if (iv[i].w) {
            return (FIX4)0xF0000000;
        }
        if (max < iv[i].z) {
            max = iv[i].z;
        }
    }
    return max;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__9C3DPolyG4);
FIX4 C3DPolyG3::calcZ() {
    FVECTOR c;
    FMATRIX rts;
    FVECTOR* fv;
    IVECTOR* iv;
    INT max;
    INT i;

    m_pCrd->getRotTransPersMatrix(rts);
    fv = &m_P0;
    iv = &p0;
    max = 0;
    setRotTransPersMatrix(rts);
    for (i = 0; i < 3; i++) {
        RotTransPersClipW(iv[i], fv[i]);
        if (iv[i].w) {
            return (FIX4)0xF0000000;
        }
        if (max < iv[i].z) {
            max = iv[i].z;
        }
    }
    return max;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__9C3DPolyG3);
FIX4 C3DLine::calcZ() {
    FMATRIX mat0;
    m_pCrd->getRotTransPersMatrix(mat0);
    setRotTransPersMatrix(mat0);
    RotTransPersClipW(p0, m_P0);
    RotTransPersClipW(p1, m_P1);
    return p0.z;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__7C3DLine);
FIX4 C3DLineCoord2::calcZ() {
    FMATRIX mat0;
    FMATRIX mat1;
    m_pCrd0->getRotTransPersMatrix(mat0);
    m_pCrd1->getRotTransPersMatrix(mat1);
    setRotTransPersMatrix(mat0);
    RotTransPersClipW(p0, m_P0);
    setRotTransPersMatrix(mat1);
    RotTransPersClipW(p1, m_P1);
    return p0.z;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__13C3DLineCoord2);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__8C3DLines);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__8C3DLines);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexLine__FPUlP7G_STRIPT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexLine__FPUlP7G_STRIPT1P6CCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexTriangle__FPUlP7G_STRIPT1i);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexTriangle__FPUlP7G_STRIPT1P6CCoordi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_Line__FPUlP7G_STRIPT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_Line__FPUlP7G_STRIPT1P6CCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_Triangle__FPUlP7G_STRIPT1i);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_Triangle__FPUlP7G_STRIPT1P6CCoordi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexLineStrip__FP6QWdataP7G_STRIPT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexLineStrip__FP6QWdataP7G_STRIPT1P6CCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexTriangleStrip__FP6QWdataP7G_STRIPT1i);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexTriangleStrip__FP6QWdataP7G_STRIPT1P6CCoordi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexTriangleFan__FP6QWdataP7G_STRIPT1i);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexTriangleFan__FP6QWdataP7G_STRIPT1P6CCoordi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_LineStrip__FP6QWdataP7G_STRIPT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_LineStrip__FP6QWdataP7G_STRIPT1P6CCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TriangleStrip__FP6QWdataP7G_STRIPT1i);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TriangleStrip__FP6QWdataP7G_STRIPT1P6CCoordi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TriangleFan__FP6QWdataP7G_STRIPT1i);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TriangleFan__FP6QWdataP7G_STRIPT1P6CCoordi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPrimGetaZ__FiRC7FVECTORPC7FMATRIXf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__12C3DStripBase);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__12C3DStripBase);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__15C3DTexStripBase);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__15C3DTexStripBase);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePktSub__13C3DLineTStripP6QWdata);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePktSub__18C3DLineTStripCoordP6QWdata);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePktSub__13C3DPolyTStripP6QWdata);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePktSub__18C3DPolyTStripCoordP6QWdata);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePktSub__11C3DPolyTFanP6QWdata);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePktSub__16C3DPolyTFanCoordP6QWdata);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePktSub__12C3DLineStripP6QWdata);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePktSub__17C3DLineStripCoordP6QWdata);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePktSub__12C3DPolyStripP6QWdata);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePktSub__17C3DPolyStripCoordP6QWdata);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePktSub__10C3DPolyFanP6QWdata);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePktSub__15C3DPolyFanCoordP6QWdata);
void C3DCPrim::_init() {
    m_PRIM = 0;
    m_TEST = (UINT)-1;
    m_ALPHA = (UINT)-1;
    m_ZBUF = (UINT)-1;
    m_CLAMP = (ULONGLONG)-1;
    m_SCISSOR = (ULONGLONG)-1;
    m_pCrd = NULL;
    m_pObj = NULL;
    m_V = NULL;
    m_fGeta = 0.0f;
    m_iRep = 0;
    m_iMax = 0;
    m_iZ = -1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _create__8C3DCPrimUliP6CCoordP7CCoordsi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__8C3DCPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__8C3DCPrim);
void C3DTPrim::_init() {
    m_PRIM = 0;
    m_TEST = (UINT)-1;
    m_ALPHA = (UINT)-1;
    m_ZBUF = (UINT)-1;
    m_CLAMP = (ULONGLONG)-1;
    m_SCISSOR = (ULONGLONG)-1;
    m_pCrd = NULL;
    m_pObj = NULL;
    m_V = NULL;
    m_fGeta = 0.0f;
    m_iRep = 0;
    m_iMax = 0;
    m_iZ = -1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _create__8C3DTPrimUliP6CCoordP7CCoordsi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__8C3DTPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__8C3DTPrim);
QWdata* C3DTPrim::makePkt_None(QWdata* pQw) {
    return pQw;
}
QWdata* C3DTPrim::makePkt_Line0(QWdata* pQw) {
    if (m_pCrd == NULL) {
        loadMatrix_VF04_07(CWorldEnv::getWorldScreenMatrix());
    } else {
        m_pCrd->getRotTransPersMatrix_VF04_07();
    }
    return (QWdata*)_makePkt_TexLine((ULONGLONG*)pQw, m_V, m_V + m_iMax);
}
QWdata* C3DTPrim::makePkt_Line1(QWdata* pQw) {
    return (QWdata*)_makePkt_TexLine((ULONGLONG*)pQw, m_V, m_V + m_iMax, &m_pObj->Crd(0));
}
QWdata* C3DTPrim::makePkt_Triangle0(QWdata* pQw) {
    if (m_pCrd == NULL) {
        loadMatrix_VF04_07(CWorldEnv::getWorldScreenMatrix());
    } else {
        m_pCrd->getRotTransPersMatrix_VF04_07();
    }
    return (QWdata*)_makePkt_TexTriangle((ULONGLONG*)pQw, m_V, m_V + m_iMax, isBackClip());
}
QWdata* C3DTPrim::makePkt_Triangle1(QWdata* pQw) {
    return (QWdata*)_makePkt_TexTriangle((ULONGLONG*)pQw, m_V, m_V + m_iMax, &m_pObj->Crd(0), isBackClip());
}
QWdata* C3DCPrim::makePkt_None(QWdata* pQw) {
    return pQw;
}
QWdata* C3DCPrim::makePkt_Line0(QWdata* pQw) {
    if (m_pCrd == NULL) {
        loadMatrix_VF04_07(CWorldEnv::getWorldScreenMatrix());
    } else {
        m_pCrd->getRotTransPersMatrix_VF04_07();
    }
    return (QWdata*)_makePkt_Line((ULONGLONG*)pQw, m_V, m_V + m_iMax);
}
QWdata* C3DCPrim::makePkt_Line1(QWdata* pQw) {
    return (QWdata*)_makePkt_Line((ULONGLONG*)pQw, m_V, m_V + m_iMax, &m_pObj->Crd(0));
}
QWdata* C3DCPrim::makePkt_Triangle0(QWdata* pQw) {
    if (m_pCrd == NULL) {
        loadMatrix_VF04_07(CWorldEnv::getWorldScreenMatrix());
    } else {
        m_pCrd->getRotTransPersMatrix_VF04_07();
    }
    return (QWdata*)_makePkt_Triangle((ULONGLONG*)pQw, m_V, m_V + m_iMax, isBackClip());
}
QWdata* C3DCPrim::makePkt_Triangle1(QWdata* pQw) {
    return (QWdata*)_makePkt_Triangle((ULONGLONG*)pQw, m_V, m_V + m_iMax, &m_pObj->Crd(0), isBackClip());
}
QWdata* C3DTPrim::makePkt_LineStrip0(QWdata* pQw) {
    G_STRIP* it = m_V;
    G_STRIP* end = m_V + m_iMax;
    if (m_pCrd == NULL) {
        loadMatrix_VF04_07(CWorldEnv::getWorldScreenMatrix());
    } else {
        m_pCrd->getRotTransPersMatrix_VF04_07();
    }
    return _makePkt_TexLineStrip(pQw, it, end);
}
QWdata* C3DTPrim::makePkt_LineStrip1(QWdata* pQw) {
    G_STRIP* it = m_V;
    G_STRIP* end = m_V + m_iMax;
    return _makePkt_TexLineStrip(pQw, it, end, &m_pObj->Crd(0));
}
QWdata* C3DTPrim::makePkt_TriangleStrip0(QWdata* pQw) {
    G_STRIP* it = m_V;
    G_STRIP* end = m_V + m_iMax;
    if (m_pCrd == NULL) {
        loadMatrix_VF04_07(CWorldEnv::getWorldScreenMatrix());
    } else {
        m_pCrd->getRotTransPersMatrix_VF04_07();
    }
    return _makePkt_TexTriangleStrip(pQw, it, end, isBackClip());
}
QWdata* C3DTPrim::makePkt_TriangleStrip1(QWdata* pQw) {
    G_STRIP* it = m_V;
    G_STRIP* end = m_V + m_iMax;
    return _makePkt_TexTriangleStrip(pQw, it, end, &m_pObj->Crd(0), isBackClip());
}
QWdata* C3DTPrim::makePkt_TriangleFan0(QWdata* pQw) {
    G_STRIP* it = m_V;
    G_STRIP* end = m_V + m_iMax;
    if (m_pCrd == NULL) {
        loadMatrix_VF04_07(CWorldEnv::getWorldScreenMatrix());
    } else {
        m_pCrd->getRotTransPersMatrix_VF04_07();
    }
    return _makePkt_TexTriangleFan(pQw, it, end, isBackClip());
}
QWdata* C3DTPrim::makePkt_TriangleFan1(QWdata* pQw) {
    G_STRIP* it = m_V;
    G_STRIP* end = m_V + m_iMax;
    return _makePkt_TexTriangleFan(pQw, it, end, &m_pObj->Crd(0), isBackClip());
}
QWdata* C3DCPrim::makePkt_LineStrip0(QWdata* pQw) {
    G_STRIP* it = m_V;
    G_STRIP* end = m_V + m_iMax;
    if (m_pCrd == NULL) {
        loadMatrix_VF04_07(CWorldEnv::getWorldScreenMatrix());
    } else {
        m_pCrd->getRotTransPersMatrix_VF04_07();
    }
    return _makePkt_LineStrip(pQw, it, end);
}
QWdata* C3DCPrim::makePkt_LineStrip1(QWdata* pQw) {
    G_STRIP* it = m_V;
    G_STRIP* end = m_V + m_iMax;
    return _makePkt_LineStrip(pQw, it, end, &m_pObj->Crd(0));
}
QWdata* C3DCPrim::makePkt_TriangleStrip0(QWdata* pQw) {
    G_STRIP* it = m_V;
    G_STRIP* end = m_V + m_iMax;
    if (m_pCrd == NULL) {
        loadMatrix_VF04_07(CWorldEnv::getWorldScreenMatrix());
    } else {
        m_pCrd->getRotTransPersMatrix_VF04_07();
    }
    return _makePkt_TriangleStrip(pQw, it, end, isBackClip());
}
QWdata* C3DCPrim::makePkt_TriangleStrip1(QWdata* pQw) {
    G_STRIP* it = m_V;
    G_STRIP* end = m_V + m_iMax;
    return _makePkt_TriangleStrip(pQw, it, end, &m_pObj->Crd(0), isBackClip());
}
QWdata* C3DCPrim::makePkt_TriangleFan0(QWdata* pQw) {
    G_STRIP* it = m_V;
    G_STRIP* end = m_V + m_iMax;
    if (m_pCrd == NULL) {
        loadMatrix_VF04_07(CWorldEnv::getWorldScreenMatrix());
    } else {
        m_pCrd->getRotTransPersMatrix_VF04_07();
    }
    return _makePkt_TriangleFan(pQw, it, end, isBackClip());
}
QWdata* C3DCPrim::makePkt_TriangleFan1(QWdata* pQw) {
    G_STRIP* it = m_V;
    G_STRIP* end = m_V + m_iMax;
    return _makePkt_TriangleFan(pQw, it, end, &m_pObj->Crd(0), isBackClip());
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", initAttribute__6CGPrimUiUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __6CGPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_6CGPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setSemiTrans__6CGPrimG5ALPHA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setShading__6CGPrimUc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setBackClip__6CGPrimUc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setTexNearest__6CGPrimUc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", isSemiTrans__6CGPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", isShading__6CGPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", isBackClip__6CGPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", isTexNearest__6CGPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__6CGPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_9CGTexPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __9CGTexPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setTPage__9CGTexPrimPC7TexInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getTPage__C9CGTexPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setTexture__9CGTexPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", clearTexCache__9CGTexPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", updateTexCache__9CGTexPrimPC7TexInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setTmpTexAreaForAllPrims__9CGTexPrimi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_11CGTexPrim2D);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __11CGTexPrim2D);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setTPage__11CGTexPrim2DPC7TexInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getTPage__C11CGTexPrim2D);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setTexture__11CGTexPrim2D);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", clearTexCache__11CGTexPrim2D);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", updateTexCache__11CGTexPrim2DPC7TexInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_11CScissoring);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C11CScissoring);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_6CZFill);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __6CZFill);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C6CZFill);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_5CTile);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C5CTile);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__5CTile);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_5CSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __5CSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setUVWH__5CSprtUsUsUsUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C5CSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__5CSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_8CPolyGT4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __8CPolyGT4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C8CPolyGT4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__8CPolyGT4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_10CPolyGT4ST);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C10CPolyGT4ST);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_7CLineF2);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C7CLineF2);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__7CLineF2);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_7CPolyG4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C7CPolyG4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__7CPolyG4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_7CCircle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C7CCircle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__7CCircle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_18CScreenColoredMask);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_8CRotSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__8CRotSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C8CRotSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __6CCPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_6CCPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", create__6CCPrimUli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", remove__6CCPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setTEST__6CCPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setALPHA__6CCPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setCLAMP__6CCPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setSCISSOR__6CCPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C6CCPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", init__6CCPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__6CCPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __6CTPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_6CTPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", create__6CTPrimUli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", remove__6CTPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setTEST__6CTPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setALPHA__6CTPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setCLAMP__6CTPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setSCISSOR__6CTPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C6CTPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", init__6CTPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__6CTPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_7C3DTile);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __7C3DTile);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C7C3DTile);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_7C3DSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __7C3DSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setUVWH__7C3DSprtUsUsUsUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C7C3DSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_10C3DRotSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __10C3DRotSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C10C3DRotSprt);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_10C3DPolyFT4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __10C3DPolyFT4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C10C3DPolyFT4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_10C3DPolyGT4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __10C3DPolyGT4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C10C3DPolyGT4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_10C3DPolyFT3);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __10C3DPolyFT3);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C10C3DPolyFT3);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_10C3DPolyGT3);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __10C3DPolyGT3);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C10C3DPolyGT3);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_7C3DLine);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C7C3DLine);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_13C3DLineCoord2);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C13C3DLineCoord2);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_8C3DLines);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C8C3DLines);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_9C3DPolyG4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C9C3DPolyG4);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_9C3DPolyG3);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C9C3DPolyG3);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_9C3DCircle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __9C3DCircle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C9C3DCircle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_13C3DPointLight);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C13C3DPointLight);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_14C3DPointLight2);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C14C3DPointLight2);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_12C3DStripBase);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __12C3DStripBase);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C12C3DStripBase);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_15C3DTexStripBase);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __15C3DTexStripBase);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C15C3DTexStripBase);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_12C3DLineStrip);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __12C3DLineStrip);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setCoord__12C3DLineStripP6CCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getCoord__12C3DLineStrip);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_17C3DLineStripCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __17C3DLineStripCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setObject__17C3DLineStripCoordP7CCoords);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getObject__17C3DLineStripCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_13C3DLineTStrip);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __13C3DLineTStrip);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setCoord__13C3DLineTStripP6CCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getCoord__13C3DLineTStrip);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_18C3DLineTStripCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __18C3DLineTStripCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setObject__18C3DLineTStripCoordP7CCoords);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getObject__18C3DLineTStripCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_12C3DPolyStrip);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __12C3DPolyStrip);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setCoord__12C3DPolyStripP6CCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getCoord__12C3DPolyStrip);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_17C3DPolyStripCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __17C3DPolyStripCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setObject__17C3DPolyStripCoordP7CCoords);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getObject__17C3DPolyStripCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_13C3DPolyTStrip);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __13C3DPolyTStrip);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setCoord__13C3DPolyTStripP6CCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getCoord__13C3DPolyTStrip);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_18C3DPolyTStripCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __18C3DPolyTStripCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setObject__18C3DPolyTStripCoordP7CCoords);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getObject__18C3DPolyTStripCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_10C3DPolyFan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __10C3DPolyFan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setCoord__10C3DPolyFanP6CCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getCoord__10C3DPolyFan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_15C3DPolyFanCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __15C3DPolyFanCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setObject__15C3DPolyFanCoordP7CCoords);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getObject__15C3DPolyFanCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_11C3DPolyTFan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __11C3DPolyTFan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setCoord__11C3DPolyTFanP6CCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getCoord__11C3DPolyTFan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_16C3DPolyTFanCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __16C3DPolyTFanCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setObject__16C3DPolyTFanCoordP7CCoords);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getObject__16C3DPolyTFanCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __8C3DCPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_8C3DCPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", create__8C3DCPrimUliP6CCoordi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", create__8C3DCPrimUliP7CCoordsi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", create__8C3DCPrimUli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", remove__8C3DCPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setTEST__8C3DCPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setALPHA__8C3DCPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setCLAMP__8C3DCPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setSCISSOR__8C3DCPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setZBUF__8C3DCPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getCoordPtr__C8C3DCPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getObjPtr__C8C3DCPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setCalcZ__8C3DCPrimi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C8C3DCPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", __8C3DTPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_8C3DTPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", create__8C3DTPrimUliP6CCoordi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", create__8C3DTPrimUliP7CCoordsi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", create__8C3DTPrimUli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", remove__8C3DTPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setTEST__8C3DTPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setALPHA__8C3DTPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setCLAMP__8C3DTPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setSCISSOR__8C3DTPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setZBUF__8C3DTPrimUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getCoordPtr__C8C3DTPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getObjPtr__C8C3DTPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", setCalcZ__8C3DTPrimi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPacketSize__C8C3DTPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", func_0015A310);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _$_t7CVector1Z6G_LINE);
