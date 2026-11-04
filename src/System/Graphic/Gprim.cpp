
#pragma implementation "GprimFull.h" /* MATCH: the Gprim weak tail from C (user-approved pragma mechanism) */
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */

#include "khtypes.h"
#include "System/Graphic/Gprim.h"
#include "System/Coord.h"
#include "System/Tsin.h"
#include "System/VramMan.h"
#include "include_asm.h"

extern INT SCREEN_HEIGHT;
/* PARTIAL: System/RsrcData.h */
#include "System/RsrcDB.h"



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
#define _prefetch(p, ofs) __asm__ volatile("pref 0, %1(%0)" : : "r"(p), "i"(ofs))
static inline u_long128 _colToVec(const G_RGBA& col) {
    UINT iCol = col.col;
    u_long128 ivCol;
    __asm__ volatile("pextlb %0, $0, %1\n"
            "pextlh %0, $0, %0\n"
            : "=r"(ivCol)
            : "r"(iCol));
    return ivCol;
}
static inline ULONGLONG _vecToXYZ(u_long128 inIVec) {
    ULONGLONG outVec64;
    __asm__ volatile("pexcw %0, %1\n"
                     "pexch $8, %1\n"
                     "dsra32 %0, %0, 4\n"
                     "pextlw %0, %0, $8\n"
                     : "=&r"(outVec64)
                     : "r"(inIVec)
                     : "$8");
    return outVec64;
}
static inline ULONGLONG _vecToXYZ(const IVECTOR& v) { return _vecToXYZ(v._q); }
static inline u_long128 _screenClamp(const FVECTOR& inFVec) {
    u_long128 outVec128;
    u_long128 tmp;
    __asm__ volatile("lqc2 $vf4, 0x0(%2)\n"
                     "lui %0, 0x4500\n"
                     "lui %1, 0x457f\n"
                     "ori %1, %1, 0xf000\n"
                     "pextlw %0, %1, %0\n"
                     "pextlw %0, %0, %0\n"
                     "qmtc2.ni %0, $vf5\n"
                     "vadd.xy $vf4, $vf4, $vf5\n"
                     "vmaxz.xy $vf4, $vf4, $vf0z\n"
                     "vminiz.xy $vf4, $vf4, $vf5z\n"
                     "vftoi4.xyz $vf4, $vf4\n"
                     "qmfc2.ni %0, $vf4\n"
                     : "=r"(outVec128), "=&r"(tmp)
                     : "r"(&inFVec));
    return outVec128;
}
static inline u_long128 _gifTag(u_long tag, u_long regs) {
    u_long128 giftag;
    __asm__ volatile("pcpyld %0, %1, %2" : "=r"(giftag) : "r"(regs), "r"(tag));
    return giftag;
}
static inline u_long _gsXYZ(FIX4 x, FIX4 y, FIX4 z) {
    return (u_long)(x + 0x8000) | ((u_long)(y + 0x8000) << 16) | ((u_long)(z >> 4) << 32);
}
static inline u_long _gsUV(USHORT tu, USHORT tv) {
    INT u = tu << 4;
    INT v = tv << 4;
    return (u_long)(u | 8) | ((u_long)(v | 8) << 16);
}
static inline u_long _gsUV(const G_UV& t) {
    INT u = t.u << 4;
    INT v = t.v << 4;
    return (u_long)(u | 8) | ((u_long)(v | 8) << 16);
}
static inline u_long _gsXYZ(const G_XYZ& p) { return _gsXYZ(p.x, p.y, p.z); }
static inline void _setSTQ(FVECTOR& outSTQ, ULONGLONG inSTQ, FLOAT fW) {
    __asm__ volatile("mfc1 $8, %1\n"
                     "pcpyld $8, $8, %2\n"
                     "qmtc2.ni $8, $vf8\n"
                     "vmulz.xy $vf8, $vf8, $vf8z\n"
                     "sqc2 $vf8, 0x0(%0)\n"
                     :
                     : "r"(&outSTQ), "f"(fW), "r"(inSTQ)
                     : "$8", "memory");
}
QWdata* _makePkt_TexLineStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end, CCoord* pCrd);
static inline void _setGeta(FLOAT fGetaZ) {
    u_long128 inGeta;
    u_long g = *(UINT*)&fGetaZ;
    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(inGeta) : "r"((u_long)0), "r"(g));
    __asm__ volatile("qmtc2.ni %0, $vf3" : : "r"(inGeta));
}
static inline u_long128 _calcSTQ(ULONGLONG inSTQ, UINT iW) {
    u_long128 iRet;
    __asm__ volatile("pcpyld %0, %1, %2\n"
                     "qmtc2.ni %0, $vf8\n"
                     "vmulz.xy $vf8, $vf8, $vf8z\n"
                     "qmfc2.ni %0, $vf8\n"
                     : "=r"(iRet)
                     : "r"(iW), "r"(inSTQ));
    return iRet;
}
static inline UINT _floatBits(FLOAT f) {
    UINT i;
    __asm__ volatile("mfc1 %0, %1" : "=r"(i) : "f"(f));
    return i;
}
static inline u_long128 _calcSTQW(ULONGLONG inSTQ, FLOAT fW) {
    u_long128 iRet;
    __asm__ volatile("mfc1 %0, %2\n"
                     "pcpyld %0, %0, %1\n"
                     "qmtc2.ni %0, $vf8\n"
                     "vmulz.xy $vf8, $vf8, $vf8z\n"
                     "qmfc2.ni %0, $vf8\n"
                     : "=&r"(iRet)
                     : "r"(inSTQ), "f"(fW));
    return iRet;
}
static inline u_long128 _getHi64(u_long128 x) {
    u_long128 out;
    __asm__ volatile("pcpyud %0, %1, $0" : "=r"(out) : "r"(x));
    return out;
}
static inline ULONGLONG _toU64(u_long128 x) {
    ULONGLONG out64;
    __asm__ volatile("por %0, $0, %1" : "=r"(out64) : "r"(x));
    return out64;
}
static inline FLOAT _hiToFloat(ULONGLONG x) {
    FLOAT outf;
    ULONGLONG tmp;
    __asm__ volatile("dsra32 %1, %2, 0\n"
                     "mtc1 %1, %0\n"
                     : "=f"(outf), "=&r"(tmp)
                     : "r"(x));
    return outf;
}
static inline INT _toInt(u_long128 x) {
    INT tmp;
    __asm__ volatile("sll %0, %1, 0" : "=r"(tmp) : "r"(x));
    return tmp;
}
static inline FLOAT _getW(u_long128 x) { return _hiToFloat(_toU64(_getHi64(x))); }
static inline INT _getZ(u_long128 x) { return _toInt(_getHi64(x)); }
static inline u_long128 _calcSTQ(const G_ST& st, UINT iW) {
    ULONGLONG inSTQ = *(ULONGLONG*)&st;
    u_long128 iRet;
    __asm__ volatile("pcpyld %0, %2, %1\n"
                     "qmtc2.ni %0, $vf8\n"
                     "vmulz.xy $vf8, $vf8, $vf8z\n"
                     "qmfc2.ni %0, $vf8\n"
                     : "=r"(iRet)
                     : "r"(inSTQ), "r"(iW));
    return iRet;
}
INT BackClip(const FVECTOR* in_fV, INT iDirL);
FLOAT getPrimGetaZ(INT iz, const FVECTOR& Vert, const FMATRIX* pWMat, FLOAT fGeta);
static inline void _getTmpTexArea(INT nID, UINT& nTexPos, UINT& nClutPos) { /* PARTIAL: VramDef.h */
    if (nID == 0) {
        nTexPos = VRAM::_TEX_TMP1_START;
        nClutPos = VRAM::_CLUT_TMP1_START;
    } else {
        nTexPos = VRAM::_TEX_TMP2_START;
        nClutPos = VRAM::_CLUT_TMP2_START;
    }
}
static inline u_long128 _AD(u_long addr, u_long data) {
    u_long128 out128;
    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(out128) : "r"(data), "r"(addr));
    return out128;
}
TexInfo* CGTexPrim::m_pTexCache = NULL;
INT CGTexPrim::m_nTmpTexArea = 1;
unsigned char C3DCPrim::m_PackReglist[8] = {1, 1, 0, 1, 0, 0, 1, 1};
unsigned char C3DTPrim::m_PackReglist[8] = {1, 1, 0, 1, 0, 0, 1, 1};

template <class T> static inline T& _ref(T& v) { return v; }
#include "System/Graphic/WorldEnv.h"
#line 207
void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
static inline FVECTOR ApplyMatrix(const FMATRIX& m0, const FVECTOR& v1) { /* PARTIAL: System/Coord.h */
    FVECTOR b;
    ApplyMatrix(b, m0, v1);
    return b;
}
#include "System/Graphic/PackMan.h"



















#include "System/Graphic/ModelMan.h"

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
ULONGLONG CGTexPrim::setTexture(const TexInfo* pInInfo, BOOL bOnly2D) {
    UINT nTexPos;
    UINT nClutPos;
    if (!pInInfo) return 0;
    if (pInInfo->isFix()) return pInInfo->getTPage();

    _getTmpTexArea(m_nTmpTexArea, nTexPos, nClutPos);

    if (updateTexCache(pInInfo)) {
        CPackMan::pkCloseDirectCode();
        if (pInInfo->isFixTex()) {
            nTexPos = pInInfo->_TexInfo._nDistPos;
        } else {
            CPackMan::pkRefLoadImage(nTexPos, pInInfo->_TexInfo);
            CModelMan::incTexSendCount(1);
        }
        if (pInInfo->isFixClut()) {
            nClutPos = pInInfo->_ClutInfo._nDistPos;
        } else {
            CPackMan::pkRefLoadImage(nClutPos, pInInfo->_ClutInfo);
        }
        CPackMan::pkOpenDirectCode(0);
    } else {
        if (pInInfo->isFixTex()) nTexPos = pInInfo->_TexInfo._nDistPos;
        if (pInInfo->isFixClut()) nClutPos = pInInfo->_ClutInfo._nDistPos;
    }
    return pInInfo->getTPage(nTexPos, nClutPos);
}
ULONGLONG CGTexPrim2D::setTexture(const TexInfo* pInInfo, BOOL bOnly2D) {
    UINT nTexPos;
    UINT nClutPos;
    INT nID;
    if (!pInInfo) return 0;
    if (pInInfo->isFix()) return pInInfo->getTPage();

    _getTmpTexArea(0, nTexPos, nClutPos);

    nID = updateTexCache(pInInfo);
    if (nID < 0) {
        CPackMan::pkCloseDirectCode();
        nTexPos += m_nCacheIdxOld * 64;
        nClutPos += m_nCacheIdxOld * 4;
        CPackMan::pkRefLoadImage(nTexPos, pInInfo->_TexInfo);
        CPackMan::pkRefLoadImage(nClutPos, pInInfo->_ClutInfo);
        CModelMan::incTexSendCount(1);
        CPackMan::pkOpenDirectCode(0);
    } else {
        nTexPos += nID * 64;
        nClutPos += nID * 4;
    }
    return pInInfo->getTPage(nTexPos, nClutPos);
}
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
/* Parked: 8 lines - two scheduling ties: the original loads 0x206 before the shared zero for the PRIM/PRMODE A+D
 * pair, and strength-reduces i * 64 + 63 with its increment at the loop end while i++ sits inside the SCISSOR
 * expression (ours increments both there); this draft fixed the hoisted-constant order (XYZ helper with y first
 * and z by reference, as the original's PackMan.h inline) and i++ (line 277); tried before: _gifTag vs _AD, _ref
 * reloads, memory clobber, permuter 15 min x2; now also: _AD operand listing (breaks its other users), typed
 * zeros, swapped statements, ++i/(i+1) forms, separate x biv, for-increment variants, permuter 15 min (17083
 * candidates, no gain). CZFill's key function: the weak tail needs it */
#if 0
static inline u_long _gsXYZz(INT x, INT y, const FIX4& z) {
    return ((u_long)(y + 0x8000) << 16) | (u_long)(x + 0x8000) | ((u_long)(z >> 4) << 32);
}
void CZFill::makePkt() {
    QWdata* qp;
    INT i;

    qp = (QWdata*)CPackMan::pkOpenGifTag(_gifTag(0x1000000000008000UL, 0xE));

    qp[0].ul128 = _AD(0x48, 0x32001);
    qp[1].ul128 = _AD(0x4F, (_ref(VRAM::_ZBUF_START) / 32) | 0x31000000);
    qp[2].ul128 = _AD(0, 0x206);
    qp[3].ul128 = _AD(1, 0);
    qp += 4;

    for (i = 0; i < 8; qp += 3) {
        qp[0].ul128 = _AD(0x41, (u_long)(i * 64) | ((u_long)(i++ * 64 + 63) << 16) | ((u_long)(_ref(SCREEN_HEIGHT) - 1) << 48));
        qp[1].ul128 = _AD(5, _gsXYZz(-0x1000, -((_ref(SCREEN_HEIGHT) / 2) << 4), m_nZ));
        qp[2].ul128 = _AD(5, _gsXYZz(0x1000, (_ref(SCREEN_HEIGHT) / 2) << 4, m_nZ));
    }
    qp[0].ul128 = _AD(0x48, CPackMan::getDefault_TEST_2());
    qp[1].ul128 = _AD(0x4F, CPackMan::getDefault_ZBUF_2());
    qp[2].ul128 = _AD(0x41, CPackMan::getDefault_SCISSOR_2());

    CPackMan::pkCloseGifTag(&qp[3]);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__6CZFill);
#endif
void CTile::makePkt() {
    QWdata giftag;
    giftag.ul64[0] = 0x4400000000008000UL;
    giftag.ul64[1] = 0x4410;
    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x206 | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(m_C0.col);
    CPackMan::pkAddGsData(_gsXYZ(m_P0));
    CPackMan::pkAddGsData(_gsXYZ(m_P1.x, m_P1.y, m_P0.z));
    CPackMan::pkCloseGifTag();
}
void CSprt::makePkt() {
    QWdata giftag;
    ULONGLONG tex;
    if (m_pTexInfo == NULL) {
        return;
    }
    tex = setTexture();
    giftag.ul64[0] = 0x7400000000008000UL;
    giftag.ul64[1] = 0x4343170;
    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x316 | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(tex);
    CPackMan::pkAddGsData(m_C0.col);
    CPackMan::pkAddGsData(_gsUV(m_T0));
    CPackMan::pkAddGsData(_gsXYZ(m_P0));
    CPackMan::pkAddGsData(_gsUV(m_T1));
    CPackMan::pkAddGsData(_gsXYZ(m_P1.x, m_P1.y, m_P0.z));
    CPackMan::pkCloseGifTag();
}
void CPolyGT4::makePkt() {
    QWdata giftag;
    ULONGLONG tex;
    if (m_pTexInfo == NULL) {
        return;
    }
    tex = setTexture();
    giftag.ul64[0] = 0xE400000000008000UL;
    giftag.ul64[1] = 0x43143143143170UL;
    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x31C | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(tex);
    CPackMan::pkAddGsData(m_C0.col);
    CPackMan::pkAddGsData(_gsUV(m_T0));
    CPackMan::pkAddGsData(_gsXYZ(m_P0));
    CPackMan::pkAddGsData(m_C1.col);
    CPackMan::pkAddGsData(_gsUV(m_T1));
    CPackMan::pkAddGsData(_gsXYZ(m_P1));
    CPackMan::pkAddGsData(m_C2.col);
    CPackMan::pkAddGsData(_gsUV(m_T2));
    CPackMan::pkAddGsData(_gsXYZ(m_P2));
    CPackMan::pkAddGsData(m_C3.col);
    CPackMan::pkAddGsData(_gsUV(m_T3));
    CPackMan::pkAddGsData(_gsXYZ(m_P3));
    CPackMan::pkCloseGifTag();
}
void CPolyGT4ST::makePkt() {
    ULONGLONG* p64;
    QWdata* qp;
    ULONGLONG tex;
    if (m_pTexInfo == NULL) {
        return;
    }
    tex = setTexture();
    if (isTexNearest()) {
        qp = (QWdata*)CPackMan::pkOpenGifTag(_gifTag(0x1000000000008000UL, 0xE));
        qp[0].ul128 = _AD(0x15, 1);
        CPackMan::pkCloseGifTag(&qp[1]);
    }

    p64 = (ULONGLONG*)CPackMan::pkOpenGifTagManual(_gifTag(0xE400000000008001UL, 0x41241241241270UL));
    p64[0] = (u_long)0x21C | ((u_long)isSemiTrans() << 6);
    p64[1] = tex;

    p64[2] = (u_long)*(UINT*)&m_T0.s | ((u_long)*(UINT*)&m_T0.t << 32);
    p64[3] = (u_long)m_C0.col | 0x3F80000000000000UL;
    p64[4] = (u_long)(m_P0.x + 0x8000) | ((u_long)(m_P0.y + 0x8000) << 16);

    p64[5] = (u_long)*(UINT*)&m_T1.s | ((u_long)*(UINT*)&m_T1.t << 32);
    p64[6] = (u_long)m_C1.col | 0x3F80000000000000UL;
    p64[7] = (u_long)(m_P1.x + 0x8000) | ((u_long)(m_P1.y + 0x8000) << 16);

    p64[8] = (u_long)*(UINT*)&m_T2.s | ((u_long)*(UINT*)&m_T2.t << 32);
    p64[9] = (u_long)m_C2.col | 0x3F80000000000000UL;
    p64[10] = (u_long)(m_P2.x + 0x8000) | ((u_long)(m_P2.y + 0x8000) << 16);

    p64[11] = (u_long)*(UINT*)&m_T3.s | ((u_long)*(UINT*)&m_T3.t << 32);
    p64[12] = (u_long)m_C3.col | 0x3F80000000000000UL;
    p64[13] = (u_long)(m_P3.x + 0x8000) | ((u_long)(m_P3.y + 0x8000) << 16);

    CPackMan::pkCloseGifTagManual((UINT*)&p64[14]);

    if (isTexNearest()) {
        qp = (QWdata*)CPackMan::pkOpenGifTag(_gifTag(0x1000000000008000UL, 0xE));
        qp[0].ul128 = _AD(0x15, 0x61);
        CPackMan::pkCloseGifTag(&qp[1]);
    }
}
void CLineF2::makePkt() {
    QWdata giftag;
    giftag.ul64[0] = 0x4400000000008000UL;
    giftag.ul64[1] = 0x4410;
    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x281 | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(m_C0.col);
    CPackMan::pkAddGsData(_gsXYZ(m_P0));
    CPackMan::pkAddGsData(_gsXYZ(m_P1.x, m_P1.y, m_P1.z));
    CPackMan::pkCloseGifTag();
}
void CPolyG4::makePkt() {
    QWdata giftag;
    giftag.ul64[0] = 0x9400000000008000UL;
    giftag.ul64[1] = 0x414141410UL;
    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x30C | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(m_C0.col);
    CPackMan::pkAddGsData(_gsXYZ(m_P0));
    CPackMan::pkAddGsData(m_C1.col);
    CPackMan::pkAddGsData(_gsXYZ(m_P1));
    CPackMan::pkAddGsData(m_C2.col);
    CPackMan::pkAddGsData(_gsXYZ(m_P2));
    CPackMan::pkAddGsData(m_C3.col);
    CPackMan::pkAddGsData(_gsXYZ(m_P3));
    CPackMan::pkCloseGifTag();
}
void CCircle::makePkt() {
    QWdata giftag;
    FLOAT fRx;
    FLOAT fRy;
    G_XYZ edge;
    INT deg;

    giftag.ul64[0] = 0x4400000000008000UL;
    giftag.ul64[1] = 0x1410;

    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x20D | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(m_C0.col);
    CPackMan::pkAddGsData(_gsXYZ(m_P0));
    CPackMan::pkAddGsData(m_C1.col);
    CPackMan::pkCloseGifTag();

    fRx = m_RX;
    fRy = m_RY;

    edge.z = m_P0.z;
    giftag.ul64[0] = 0x1400000000008000UL;
    giftag.ul64[1] = 4;
    CPackMan::pkOpenGifTag(giftag.ul128);
    for (deg = 0; deg <= 3600; deg += 200) {
        edge.x = fRx * tcos(deg);
        edge.y = fRy * tsin(deg);
        CPackMan::pkAddGsData(_gsXYZ(edge));
    }
    CPackMan::pkCloseGifTag();
}
CScreenColoredMask::CScreenColoredMask() {
    setLayerFG(1);
    m_P0.set(-0x1000, -(_ref(SCREEN_HEIGHT) * 8));
    m_P1.set(0x1000, _ref(SCREEN_HEIGHT) * 8);
}
/* Parked: the corner setup before the two calcVert calls (load/store order of b, c and ab: the original keeps a
 * separate copy of m_P0.y for ab.set's _y parameter) and the register numbering that follows; a reference to m_P1
 * (not in the ref's locals) reproduces the original's s0 = &m_P1 read of m_P1.y in the second set (41 -> 39
 * lines); the ref also has no tex local although setTexture()'s result lives in s3 until its pkAddGsData; tried:
 * direct member stores, reference locals for m_P0/m_P1, cd.set before the first calcVert, mid-function
 * declarations, permuter 15 min (49 -> 48). CRotSprt's key function: the weak tail needs it */
#if 0
void CRotSprt::makePkt() {
    G_XYZ b;
    G_XYZ c;
    IVECTOR ab;
    IVECTOR cd;
    ULONGLONG tex;
    if (getTPage() == NULL) {
        return;
    }
    tex = setTexture();

    const G_XYZ& p1 = m_P1;
    b.set(m_P1.x, m_P0.y);
    c.set(m_P0.x, m_P1.y);

    ab.set(m_P0.x, m_P0.y, b.x, b.y);
    calcVert(ab);
    cd.set(c.x, c.y, m_P1.x, p1.y);
    calcVert(cd);

    G_UV tb;
    G_UV tc;
    tb.set(m_T1.u, m_T0.v);
    tc.set(m_T0.u, m_T1.v);

    QWdata giftag;
    giftag.ul64[0] = 0xB400000000008000UL;
    giftag.ul64[1] = 0x43434343170UL;

    FIX4 _x = m_Center.x + 0x8000;
    FIX4 _y = m_Center.y + 0x8000;
    FIX4 _z = m_Center.z >> 4;
    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x31C | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(tex);
    CPackMan::pkAddGsData(m_C0.col);
    CPackMan::pkAddGsData(_gsUV(m_T0));
    CPackMan::pkAddGsData((u_long)(ab.x + _x) | ((u_long)(ab.y + _y) << 16) | ((u_long)_z << 32));
    CPackMan::pkAddGsData(_gsUV(tb));
    CPackMan::pkAddGsData((u_long)(ab.z + _x) | ((u_long)(ab.w + _y) << 16) | ((u_long)_z << 32));
    CPackMan::pkAddGsData(_gsUV(tc));
    CPackMan::pkAddGsData((u_long)(cd.x + _x) | ((u_long)(cd.y + _y) << 16) | ((u_long)_z << 32));
    CPackMan::pkAddGsData(_gsUV(m_T1));
    CPackMan::pkAddGsData((u_long)(cd.z + _x) | ((u_long)(cd.w + _y) << 16) | ((u_long)_z << 32));
    CPackMan::pkCloseGifTag();
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", makePkt__8CRotSprt);
#endif
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
void CCPrim::makePkt() {
    ULONGLONG* p64;
    QWdata* pQw;
    UINT iEnvChange;
    FVERTEX* pV;
    if (m_iMax <= 0) {
        return;
    }

    iEnvChange = 0;
    if ((INT)m_TEST >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 1;
    }
    if ((long)m_CLAMP >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 2;
    }
    if ((long)m_SCISSOR >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 4;
    }
    if ((INT)m_ALPHA >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 8;
    }
    if (iEnvChange) {
        pQw = (QWdata*)CPackMan::pkOpenGifTagManual(_gifTag((u_long)(iEnvChange >> 12) | 0x1000000000000000UL, 0xE));
        if (iEnvChange & 1) (pQw++)->ul128 = _AD(0x48, m_TEST);
        if (iEnvChange & 2) (pQw++)->ul128 = _AD(0x9, m_CLAMP);
        if (iEnvChange & 4) (pQw++)->ul128 = _AD(0x41, m_SCISSOR);
        if (iEnvChange & 8) (pQw++)->ul128 = _AD(0x43, ((u_long)m_ALPHA << 16) | m_ALPHA);
        CPackMan::pkCloseGifTagManual((UINT*)pQw);
    }

    p64 = (ULONGLONG*)CPackMan::pkOpenGifTag(_gifTag(0x1400000000000000UL, 0));
    {
        ULONGLONG abe = (u_long)isSemiTrans() << 6;
        ULONGLONG prim = m_PRIM & 0xCF;
        prim |= 0x200;
        p64[0] = abe | prim;
    }
    CPackMan::pkCloseGifTag(&p64[1]);

    p64 = (ULONGLONG*)CPackMan::pkOpenGifTag(_gifTag(0x2400000000008000UL, 0x41));
    INT i;
    for (i = m_iMax, pV = m_V; i > 0; i--, pV++) {
        _prefetch(pV, 0x10);
        if (pV->fP.z >= 0.0f) {
            p64[0] = pV->C.col;
            p64[1] = _vecToXYZ(_screenClamp(pV->fP));
            p64 += 2;
        }
    }
    CPackMan::pkCloseGifTag(p64);

    if (iEnvChange) {
        pQw = (QWdata*)CPackMan::pkOpenGifTagManual(
            _gifTag((u_long)((iEnvChange >> 12) | 0x8000) | 0x1000000000000000UL, 0xE));
        if (iEnvChange & 1) (pQw++)->ul128 = _AD(0x48, CPackMan::getDefault_TEST_2());
        if (iEnvChange & 2) (pQw++)->ul128 = _AD(0x9, CPackMan::getDefault_CLAMP_2());
        if (iEnvChange & 4) (pQw++)->ul128 = _AD(0x41, CPackMan::getDefault_SCISSOR_2());
        if (iEnvChange & 8) (pQw++)->ul128 = _AD(0x43, CPackMan::getDefault_ALPHA_2());
        CPackMan::pkCloseGifTagManual((UINT*)pQw);
    }
}
BOOL CCPrim::isRect(INT i, const FVECTOR* vRect) {
    return __isRect(*(FVECTOR*)&m_V[i], vRect);
}
void CTPrim::makePkt() {
    ULONGLONG* p64;
    QWdata* pQw;
    UINT iEnvChange;
    FTVERTEX* pV;
    ULONGLONG tex;
    if (m_iMax <= 0) {
        return;
    }
    tex = setTexture();

    iEnvChange = 0;
    if ((INT)m_TEST >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 1;
    }
    if ((long)m_CLAMP >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 2;
    }
    if ((long)m_SCISSOR >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 4;
    }
    if ((INT)m_ALPHA >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 8;
    }
    if (iEnvChange) {
        pQw = (QWdata*)CPackMan::pkOpenGifTagManual(_gifTag((u_long)(iEnvChange >> 12) | 0x1000000000000000UL, 0xE));
        if (iEnvChange & 1) (pQw++)->ul128 = _AD(0x48, m_TEST);
        if (iEnvChange & 2) (pQw++)->ul128 = _AD(0x9, m_CLAMP);
        if (iEnvChange & 4) (pQw++)->ul128 = _AD(0x41, m_SCISSOR);
        if (iEnvChange & 8) (pQw++)->ul128 = _AD(0x43, ((u_long)m_ALPHA << 16) | m_ALPHA);
        CPackMan::pkCloseGifTagManual((UINT*)pQw);
    }

    p64 = (ULONGLONG*)CPackMan::pkOpenGifTag(_gifTag(0x2400000000000000UL, 7));
    p64[0] = tex;
    {
        ULONGLONG abe = (u_long)isSemiTrans() << 6;
        ULONGLONG prim = m_PRIM & 0xCF;
        prim |= 0x210;
        p64[1] = abe | prim;
    }
    CPackMan::pkCloseGifTag(&p64[2]);

    p64 = (ULONGLONG*)CPackMan::pkOpenGifTag(_gifTag(0x3400000000008000UL, 0x412));
    INT i;
    for (i = m_iMax, pV = m_V; i > 0; i--, pV++) {
        _prefetch(pV, 0x20);
        if (pV->fP.z >= 0.0f) {
            p64[0] = *(ULONGLONG*)pV->fT.v;
            p64[1] = (u_long)pV->C.col | ((u_long)*(UINT*)&pV->fT.v[2] << 32);
            p64[2] = _vecToXYZ(_screenClamp(pV->fP));
            p64 += 3;
        }
    }
    CPackMan::pkCloseGifTag(p64);

    if (iEnvChange) {
        pQw = (QWdata*)CPackMan::pkOpenGifTagManual(
            _gifTag((u_long)((iEnvChange >> 12) | 0x8000) | 0x1000000000000000UL, 0xE));
        if (iEnvChange & 1) (pQw++)->ul128 = _AD(0x48, CPackMan::getDefault_TEST_2());
        if (iEnvChange & 2) (pQw++)->ul128 = _AD(0x9, CPackMan::getDefault_CLAMP_2());
        if (iEnvChange & 4) (pQw++)->ul128 = _AD(0x41, CPackMan::getDefault_SCISSOR_2());
        if (iEnvChange & 8) (pQw++)->ul128 = _AD(0x43, CPackMan::getDefault_ALPHA_2());
        CPackMan::pkCloseGifTagManual((UINT*)pQw);
    }
}
BOOL CTPrim::isRect(INT i, const FVECTOR* vRect) {
    return __isRect(m_V[i].fP, vRect);
}
FLOAT RotTransPers(FVECTOR& out_v, const FMATRIX& in_m, const FVECTOR& in_v) {
    INT iQ;
    FLOAT fQ;
    __asm__ volatile("lqc2 $vf8, 0x0(%0)" : : "r"(&in_v));
    __asm__ volatile("lqc2 $vf4, 0x0(%0)" : : "r"(&in_m));
    __asm__ volatile("lqc2 $vf5, 0x10(%0)" : : "r"(&in_m));
    __asm__ volatile("lqc2 $vf6, 0x20(%0)" : : "r"(&in_m));
    __asm__ volatile("lqc2 $vf7, 0x30(%0)" : : "r"(&in_m));
    __asm__ volatile("vmulax.xyzw $ACC, $vf4, $vf8x");
    __asm__ volatile("vmadday.xyzw $ACC, $vf5, $vf8y");
    __asm__ volatile("vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                     "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
                     "vdiv $Q, $vf0w, $vf8w");
    __asm__ volatile("vwaitq");
    __asm__ volatile("vmulq.xyz $vf8, $vf8, $Q");
    __asm__ volatile("cfc2.ni %0, $vi22" : "=r"(iQ));
    __asm__ volatile("mtc1 %1, %0" : "=f"(fQ) : "r"(iQ));
    __asm__ volatile("sqc2 $vf8, 0x0(%0)" : : "r"(&out_v));
    return fQ;
}
FLOAT RotTransPers(IVECTOR& out_v, const FMATRIX& in_m, const FVECTOR& in_v) {
    INT iQ;
    FLOAT fQ;
    __asm__ volatile("lqc2 $vf8, 0x0(%0)" : : "r"(&in_v));
    __asm__ volatile("lqc2 $vf4, 0x0(%0)" : : "r"(&in_m));
    __asm__ volatile("lqc2 $vf5, 0x10(%0)" : : "r"(&in_m));
    __asm__ volatile("lqc2 $vf6, 0x20(%0)" : : "r"(&in_m));
    __asm__ volatile("lqc2 $vf7, 0x30(%0)" : : "r"(&in_m));
    __asm__ volatile("vmulax.xyzw $ACC, $vf4, $vf8x");
    __asm__ volatile("vmadday.xyzw $ACC, $vf5, $vf8y");
    __asm__ volatile("vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                     "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
                     "vdiv $Q, $vf0w, $vf8w");
    __asm__ volatile("vwaitq");
    __asm__ volatile("vmulq.xyz $vf8, $vf8, $Q");
    __asm__ volatile("cfc2.ni %0, $vi22" : "=r"(iQ));
    __asm__ volatile("mtc1 %1, %0" : "=f"(fQ) : "r"(iQ));
    __asm__ volatile("vftoi4.xyzw $vf8, $vf8");
    __asm__ volatile("sqc2 $vf8, 0x0(%0)" : : "r"(&out_v));
    return fQ;
}
u_long128 RotTransPersW(const FMATRIX& in_m, const FVECTOR& in_v) {
    u_long128 out;
    __asm__ volatile("lqc2 $vf8, 0x0(%0)" : : "r"(&in_v));
    __asm__ volatile("lqc2 $vf4, 0x0(%0)" : : "r"(&in_m));
    __asm__ volatile("lqc2 $vf5, 0x10(%0)" : : "r"(&in_m));
    __asm__ volatile("lqc2 $vf6, 0x20(%0)" : : "r"(&in_m));
    __asm__ volatile("lqc2 $vf7, 0x30(%0)" : : "r"(&in_m));
    __asm__ volatile("vmulax.xyzw $ACC, $vf4, $vf8x");
    __asm__ volatile("vmadday.xyzw $ACC, $vf5, $vf8y");
    __asm__ volatile("vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                     "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
                     "vdiv $Q, $vf0w, $vf8w");
    __asm__ volatile("vmax.w $vf8, $vf0, $vf0");
    __asm__ volatile("vwaitq");
    __asm__ volatile("vmulq.xyzw $vf8, $vf8, $Q");
    __asm__ volatile("vftoi4.xyz $vf8, $vf8");
    __asm__ volatile("qmfc2.ni %0, $vf8" : "=r"(out));
    return out;
}
FLOAT RotTransPersClipW(IVECTOR& out_v, const FVECTOR& in_v) {
    INT iQ;
    FLOAT fQ;
    INT iClip;
    u_long128 v4096;
    UINT k4096;
    __asm__ volatile("lqc2 $vf8, 0x0(%0)" : : "r"(&in_v));
    __asm__ volatile("vmulax.xyzw $ACC, $vf4, $vf8x");
    __asm__ volatile("vmadday.xyzw $ACC, $vf5, $vf8y");
    __asm__ volatile("vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                     "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
                     "vdiv $Q, $vf0w, $vf8w");
    __asm__ volatile("vmulaw.xyz $ACC, $vf3, $vf8w");
    __asm__ volatile("vmaddw.xyz $vf8, $vf8, $vf0w");
    __asm__ volatile("vsub.xyzw $vf10, $vf0, $vf0");
    k4096 = 0x45800000;
    __asm__ volatile("vwaitq");
    __asm__ volatile("vmulq.xyz $vf8, $vf8, $Q");
    __asm__ volatile("pextlw %0, %1, %1" : "=r"(v4096) : "r"(k4096));
    __asm__ volatile("qmtc2.ni %0, $vf11" : : "r"(v4096));
    __asm__ volatile("ctc2.ni $0, $vi16");
    __asm__ volatile("vsub.xyzw $vf0, $vf8, $vf10");
    __asm__ volatile("vsub.xy $vf0, $vf11, $vf8");
    __asm__ volatile("vftoi4.xyzw $vf9, $vf8");
    __asm__ volatile("vnop");
    __asm__ volatile("cfc2.ni %0, $vi22" : "=r"(iQ));
    __asm__ volatile("mtc1 %1, %0" : "=f"(fQ) : "r"(iQ));
    __asm__ volatile("sqc2 $vf9, 0x0(%0)" : : "r"(&out_v));
    /* clip flags -> out_v.w (0x8000 if clipped); stays asm: C gets a movn here (if-conversion) or, with that
     * blocked, an unfilled branch instead of the original's branch-likely */
    __asm__ volatile("cfc2.ni %0, $vi16\n"
                     "andi %0, %0, 0xC0\n"
                     ".set noreorder\n"
                     "bnel %0, $0, 0f\n"
                     "ori %0, $0, 0x8000\n"
                     ".set reorder\n"
                     "0:\n"
                     "sw %0, 0xC(%1)"
                     : "=&r"(iClip)
                     : "r"(&out_v));
    return fQ;
}
FLOAT RotTransPersClipW(IVECTOR& out_v, FVECTOR& out_fv, const FVECTOR& in_v) {
    INT iQ;
    FLOAT fQ;
    INT iClip;
    u_long128 v4096;
    UINT k4096;
    __asm__ volatile("lqc2 $vf8, 0x0(%0)" : : "r"(&in_v));
    __asm__ volatile("vmulax.xyzw $ACC, $vf4, $vf8x");
    __asm__ volatile("vmadday.xyzw $ACC, $vf5, $vf8y");
    __asm__ volatile("vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                     "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
                     "vdiv $Q, $vf0w, $vf8w");
    __asm__ volatile("vmulaw.xyz $ACC, $vf3, $vf8w");
    __asm__ volatile("vmaddw.xyz $vf8, $vf8, $vf0w");
    __asm__ volatile("vsub.xyzw $vf10, $vf0, $vf0");
    k4096 = 0x45800000;
    __asm__ volatile("vwaitq");
    __asm__ volatile("vmulq.xyz $vf8, $vf8, $Q");
    __asm__ volatile("pextlw %0, %1, %1" : "=r"(v4096) : "r"(k4096));
    __asm__ volatile("qmtc2.ni %0, $vf11" : : "r"(v4096));
    __asm__ volatile("ctc2.ni $0, $vi16");
    __asm__ volatile("vsub.xyzw $vf0, $vf8, $vf10");
    __asm__ volatile("vsub.xy $vf0, $vf11, $vf8");
    __asm__ volatile("vftoi4.xyzw $vf9, $vf8");
    __asm__ volatile("sqc2 $vf8, 0x0(%0)" : : "r"(&out_fv));
    __asm__ volatile("cfc2.ni %0, $vi22" : "=r"(iQ));
    __asm__ volatile("mtc1 %1, %0" : "=f"(fQ) : "r"(iQ));
    __asm__ volatile("sqc2 $vf9, 0x0(%0)" : : "r"(&out_v));
    /* clip flags -> out_v.w (0x8000 if clipped); stays asm: C gets a movn here (if-conversion) or, with that
     * blocked, an unfilled branch instead of the original's branch-likely */
    __asm__ volatile("cfc2.ni %0, $vi16\n"
                     "andi %0, %0, 0xC0\n"
                     ".set noreorder\n"
                     "bnel %0, $0, 0f\n"
                     "ori %0, $0, 0x8000\n"
                     ".set reorder\n"
                     "0:\n"
                     "sw %0, 0xC(%1)"
                     : "=&r"(iClip)
                     : "r"(&out_v));
    return fQ;
}
INT BackClip(const FVECTOR* in_fV, INT iDirL) {
    INT iRet;
    register u_long128 vN asm("$3");
    register ULONGLONG lZ asm("$3");
    ULONGLONG lDir;
    __asm__ volatile("lqc2 $vf8, 0x0(%0)" : : "r"(in_fV));
    __asm__ volatile("lqc2 $vf9, 0x10(%0)" : : "r"(in_fV));
    __asm__ volatile("lqc2 $vf10, 0x20(%0)" : : "r"(in_fV));
    __asm__ volatile("vsub.xyzw $vf9, $vf9, $vf8");
    __asm__ volatile("vsub.xyzw $vf10, $vf10, $vf8");
    lDir = (ULONGLONG)iDirL << 31;
    __asm__ volatile("vopmula.xyz $ACC, $vf9, $vf10");
    __asm__ volatile("vopmsub.xyz $vf10, $vf10, $vf9");
    __asm__ volatile("qmfc2.ni %0, $vf10" : "=r"(vN));
    __asm__ volatile("pcpyud %0, %1, $0" : "=r"(lZ) : "r"(vN));
    __asm__ volatile("dsrl %0, %1, 16" : "=r"(iRet) : "r"(lZ ^ lDir));
    iRet &= 0x8000;
    return iRet;
}
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
static inline void _setRotTransPersMatrix_VF04_07(CCoord* pCrd) {
    if (pCrd == NULL) {
        loadMatrix_VF04_07(CWorldEnv::getWorldScreenMatrix());
    } else {
        pCrd->getRotTransPersMatrix_VF04_07();
    }
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
void C3DPolyFT4::makePkt() {
    ULONGLONG* p64;
    ULONGLONG tex = setTexture();
    p64 = (ULONGLONG*)CPackMan::pkOpenGifTagManual(_gifTag(0xE400000000008001UL, 0x42142142142170UL));
    p64[0] = (u_long)0x214 | ((u_long)isSemiTrans() << 6);
    p64[1] = tex;

    p64[2] = (u_long)m_C0.col | ((u_long)p0.w << 32);
    p64[3] = (ULONGLONG)_calcSTQW(*(ULONGLONG*)&m_S0, *(FLOAT*)&p0.w);
    p64[4] = _vecToXYZ(p0);

    p64[5] = (u_long)m_C0.col | ((u_long)p1.w << 32);
    p64[6] = (ULONGLONG)_calcSTQW(*(ULONGLONG*)&m_S1, *(FLOAT*)&p1.w);
    p64[7] = _vecToXYZ(p1);

    p64[8] = (u_long)m_C0.col | ((u_long)p2.w << 32);
    p64[9] = (ULONGLONG)_calcSTQW(*(ULONGLONG*)&m_S2, *(FLOAT*)&p2.w);
    p64[10] = _vecToXYZ(p2);

    p64[11] = (u_long)m_C0.col | ((u_long)p3.w << 32);
    p64[12] = (ULONGLONG)_calcSTQW(*(ULONGLONG*)&m_S3, *(FLOAT*)&p3.w);
    p64[13] = _vecToXYZ(p3);

    CPackMan::pkCloseGifTagManual((UINT*)&p64[14]);
}
FIX4 C3DPolyGT4::calcZ() {
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
void C3DPolyGT4::makePkt() {
    ULONGLONG* p64;
    ULONGLONG tex = setTexture();
    p64 = (ULONGLONG*)CPackMan::pkOpenGifTagManual(_gifTag(0xE400000000008001UL, 0x42142142142170UL));
    p64[0] = (u_long)0x21C | ((u_long)isSemiTrans() << 6);
    p64[1] = tex;

    p64[2] = (u_long)m_C0.col | ((u_long)p0.w << 32);
    p64[3] = (ULONGLONG)_calcSTQW(*(ULONGLONG*)&m_S0, *(FLOAT*)&p0.w);
    p64[4] = _vecToXYZ(p0);

    p64[5] = (u_long)m_C1.col | ((u_long)p1.w << 32);
    p64[6] = (ULONGLONG)_calcSTQW(*(ULONGLONG*)&m_S1, *(FLOAT*)&p1.w);
    p64[7] = _vecToXYZ(p1);

    p64[8] = (u_long)m_C2.col | ((u_long)p2.w << 32);
    p64[9] = (ULONGLONG)_calcSTQW(*(ULONGLONG*)&m_S2, *(FLOAT*)&p2.w);
    p64[10] = _vecToXYZ(p2);

    p64[11] = (u_long)m_C3.col | ((u_long)p3.w << 32);
    p64[12] = (ULONGLONG)_calcSTQW(*(ULONGLONG*)&m_S3, *(FLOAT*)&p3.w);
    p64[13] = _vecToXYZ(p3);

    CPackMan::pkCloseGifTagManual((UINT*)&p64[14]);
}
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
void C3DPolyFT3::makePkt() {
    ULONGLONG* p64;
    ULONGLONG tex = setTexture();
    p64 = (ULONGLONG*)CPackMan::pkOpenGifTagManual(_gifTag(0xB400000000008001UL, 0x42142142170UL));
    p64[0] = (u_long)0x213 | ((u_long)isSemiTrans() << 6);
    p64[1] = tex;

    p64[2] = (u_long)m_C0.col | ((u_long)p0.w << 32);
    p64[3] = (ULONGLONG)_calcSTQW(*(ULONGLONG*)&m_S0, *(FLOAT*)&p0.w);
    p64[4] = _vecToXYZ(p0);

    p64[5] = (u_long)m_C0.col | ((u_long)p1.w << 32);
    p64[6] = (ULONGLONG)_calcSTQW(*(ULONGLONG*)&m_S1, *(FLOAT*)&p1.w);
    p64[7] = _vecToXYZ(p1);

    p64[8] = (u_long)m_C0.col | ((u_long)p2.w << 32);
    p64[9] = (ULONGLONG)_calcSTQW(*(ULONGLONG*)&m_S2, *(FLOAT*)&p2.w);
    p64[10] = _vecToXYZ(p2);

    CPackMan::pkCloseGifTagManual((UINT*)&p64[11]);
}
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
void C3DPolyGT3::makePkt() {
    ULONGLONG* p64;
    ULONGLONG tex = setTexture();
    p64 = (ULONGLONG*)CPackMan::pkOpenGifTagManual(_gifTag(0xB400000000008001UL, 0x42142142170UL));
    p64[0] = (u_long)0x21B | ((u_long)isSemiTrans() << 6);
    p64[1] = tex;

    p64[2] = (u_long)m_C0.col | ((u_long)p0.w << 32);
    p64[3] = (ULONGLONG)_calcSTQW(*(ULONGLONG*)&m_S0, *(FLOAT*)&p0.w);
    p64[4] = _vecToXYZ(p0);

    p64[5] = (u_long)m_C1.col | ((u_long)p1.w << 32);
    p64[6] = (ULONGLONG)_calcSTQW(*(ULONGLONG*)&m_S1, *(FLOAT*)&p1.w);
    p64[7] = _vecToXYZ(p1);

    p64[8] = (u_long)m_C2.col | ((u_long)p2.w << 32);
    p64[9] = (ULONGLONG)_calcSTQW(*(ULONGLONG*)&m_S2, *(FLOAT*)&p2.w);
    p64[10] = _vecToXYZ(p2);

    CPackMan::pkCloseGifTagManual((UINT*)&p64[11]);
}
FIX4 C3DTile::calcZ() {
    FVECTOR c;
    FMATRIX& vs = CWorldEnv::getViewScreenMatrix();
    c = ApplyMatrix(m_pCrd->getWorldMatrix(), m_Center);
    c = ApplyMatrix(CWorldEnv::getWorldViewMatrix(), c);

    c.x += m_P0.x;
    c.y += m_P0.y;

    RotTransPers(p0, vs, c);

    c.x += m_P1.x - m_P0.x;
    c.y += m_P1.y - m_P0.y;
    RotTransPers(p1, vs, c);

    p1.z = p0.z;

    return p0.z << 4;
}
void C3DTile::makePkt() {
    QWdata giftag;
    if (p0.z <= 0 || p0.z > 0xFFFF28 || p0.x < 0 || p0.x > 0xFFFF || p0.y < 0 || p0.y > 0xFFFF ||
        p1.z > 0xFFFF28 || p1.z <= 0 || p1.x < 0 || p1.x > 0xFFFF || p1.y < 0 || p1.y > 0xFFFF) {
        return;
    }
    giftag.ul64[0] = 0x4400000000008000UL;
    giftag.ul64[1] = 0x4410;
    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x206 | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(m_C0.col);
    CPackMan::pkAddGsData((u_long)p0.x | ((u_long)p0.y << 16) | ((u_long)p0.z << 32));
    CPackMan::pkAddGsData((u_long)p1.x | ((u_long)p1.y << 16) | ((u_long)p1.z << 32));
    CPackMan::pkCloseGifTag();
}
FIX4 C3DSprt::calcZ() {
    FVECTOR c;
    FMATRIX& vs = CWorldEnv::getViewScreenMatrix();
    c = ApplyMatrix(m_pCrd->getWorldMatrix(), m_Center);
    c = ApplyMatrix(CWorldEnv::getWorldViewMatrix(), c);

    c.w = 1.0f;
    c.x += m_P0.x;
    c.y += m_P0.y;

    RotTransPers(p0, vs, c);

    c.x += m_P1.x - m_P0.x;
    c.y += m_P1.y - m_P0.y;
    RotTransPers(p1, vs, c);

    return p0.z << 4;
}
void C3DSprt::makePkt() {
    QWdata giftag;
    ULONGLONG tex;
    if (p0.z <= 0 || p0.z > 0xFFFF28 || p0.x < 0 || p0.x > 0xFFFF || p0.y < 0 || p0.y > 0xFFFF ||
        p1.z > 0xFFFF28 || p1.z <= 0 || p1.x < 0 || p1.x > 0xFFFF || p1.y < 0 || p1.y > 0xFFFF) {
        return;
    }
    tex = setTexture();
    giftag.ul64[0] = 0x7400000000008000UL;
    giftag.ul64[1] = 0x4343170;
    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x316 | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(tex);
    CPackMan::pkAddGsData((u_long)m_C0.r | ((u_long)m_C0.g << 8) | ((u_long)m_C0.b << 16) | ((u_long)m_C0.a << 24));
    CPackMan::pkAddGsData(_gsUV(m_T0));
    CPackMan::pkAddGsData((u_long)p0.x | ((u_long)p0.y << 16) | ((u_long)p0.z << 32));
    CPackMan::pkAddGsData(_gsUV(m_T1));
    CPackMan::pkAddGsData((u_long)p1.x | ((u_long)p1.y << 16) | ((u_long)p1.z << 32));
    CPackMan::pkCloseGifTag();
}
FIX4 C3DRotSprt::calcZ() {
    FVECTOR c0;
    FMATRIX& vs = CWorldEnv::getViewScreenMatrix();
    c0 = ApplyMatrix(m_pCrd->getWorldMatrix(), m_Center);
    c0 = ApplyMatrix(CWorldEnv::getWorldViewMatrix(), c0);

    c0.w = 1.0f;

    FVECTOR c;
    FVECTOR p0x = (FVECTOR){{m_P0.x * tcos(m_Rot), m_P0.x * tsin(m_Rot), 0.0f, 0.0f}};
    FVECTOR p0y = (FVECTOR){{-m_P0.y * tsin(m_Rot), m_P0.y * tcos(m_Rot), 0.0f, 0.0f}};
    FVECTOR p1x = (FVECTOR){{m_P1.x * tcos(m_Rot), m_P1.x * tsin(m_Rot), 0.0f, 0.0f}};
    FVECTOR p1y = (FVECTOR){{-m_P1.y * tsin(m_Rot), m_P1.y * tcos(m_Rot), 0.0f, 0.0f}};

    c = c0 + p0x + p0y;
    RotTransPers(p0, vs, c);

    c = c0 + p1x + p1y;
    RotTransPers(p1, vs, c);

    c = c0 + p0x + p1y;
    RotTransPers(p01, vs, c);

    c = c0 + p1x + p0y;
    RotTransPers(p10, vs, c);

    return p0.z << 4;
}
void C3DRotSprt::makePkt() {
    QWdata giftag;
    ULONGLONG tex;
    if (p0.z <= 0 || p0.z > 0xFFFF28 || p0.x < 0 || p0.x > 0xFFFF || p0.y < 0 || p0.y > 0xFFFF ||
        p1.z > 0xFFFF28 || p1.z <= 0 || p1.x < 0 || p1.x > 0xFFFF || p1.y < 0 || p1.y > 0xFFFF ||
        p01.z > 0xFFFF28 || p10.z <= 0 || p01.x < 0 || p01.x > 0xFFFF || p01.y < 0 || p01.y > 0xFFFF ||
        p10.z > 0xFFFF28 || p01.z <= 0 || p10.x < 0 || p10.x > 0xFFFF || p10.y < 0 || p10.y > 0xFFFF) {
        return;
    }
    tex = setTexture();

    giftag.ul64[0] = 0xB400000000008000UL;
    giftag.ul64[1] = 0x43434343170UL;

    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x314 | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(tex);
    CPackMan::pkAddGsData((u_long)m_C0.r | ((u_long)m_C0.g << 8) | ((u_long)m_C0.b << 16) | ((u_long)m_C0.a << 24));

    CPackMan::pkAddGsData(_gsUV(m_T0));
    CPackMan::pkAddGsData((u_long)p0.x | ((u_long)p0.y << 16) | ((u_long)p0.z << 32));

    CPackMan::pkAddGsData(_gsUV(m_T0.u, m_T1.v));
    CPackMan::pkAddGsData((u_long)p01.x | ((u_long)p01.y << 16) | ((u_long)p01.z << 32));

    CPackMan::pkAddGsData(_gsUV(m_T1.u, m_T0.v));
    CPackMan::pkAddGsData((u_long)p10.x | ((u_long)p10.y << 16) | ((u_long)p10.z << 32));

    CPackMan::pkAddGsData(_gsUV(m_T1));
    CPackMan::pkAddGsData((u_long)p1.x | ((u_long)p1.y << 16) | ((u_long)p1.z << 32));

    CPackMan::pkCloseGifTag();
}
FIX4 C3DCircle::calcZ() {
    FVECTOR c;
    FMATRIX& vs = CWorldEnv::getViewScreenMatrix();
    c = ApplyMatrix(m_pCrd->getWorldMatrix(), m_Center);
    c = ApplyMatrix(CWorldEnv::getWorldViewMatrix(), c);

    c.x -= m_R0;
    c.y -= m_R0;

    RotTransPers(p0, vs, c);

    c.x += m_R0 * 2.0f;
    c.y += m_R0 * 2.0f;
    RotTransPers(p1, vs, c);

    if (m_Zoffset != 0.0f) {
        IVECTOR p;

        c.x -= m_R0;
        c.y -= m_R0;

        c.z += m_Zoffset;

        RotTransPers(p, vs, c);

        p0.z = p.z;
    }

    p1.z = p0.z;

    return p0.z;
}
void C3DCircle::makePkt() {
    INT cx;
    INT cy;
    INT z;
    FLOAT rx;
    FLOAT ry;
    INT dx;
    INT dy;
    QWdata giftag;
    if (p0.z <= 0 || p0.z > 0xFFFF28 || p0.x < 0 || p0.x > 0xFFFF || p0.y < 0 || p0.y > 0xFFFF ||
        p1.z > 0xFFFF28 || p1.z <= 0 || p1.x < 0 || p1.x > 0xFFFF || p1.y < 0 || p1.y > 0xFFFF) {
        return;
    }

    cx = (p0.x + p1.x) / 2;
    rx = (p1.x - p0.x) / 2;

    cy = (p0.y + p1.y) / 2;
    ry = (p1.y - p0.y) / 2;

    z = p0.z >> 4;

    giftag.ul64[0] = 0x4400000000008000UL;
    giftag.ul64[1] = 0x1410;

    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x20D | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(m_C0.col);
    CPackMan::pkAddGsData((u_long)cx | ((u_long)cy << 16) | ((u_long)z << 32));
    CPackMan::pkAddGsData(m_C1.col);
    CPackMan::pkCloseGifTag();

    giftag.ul64[0] = 0x1400000000008000UL;
    giftag.ul64[1] = 4;

    CPackMan::pkOpenGifTag(giftag.ul128);
    for (int deg = 0; deg <= 3600; deg += 200) {
        dx = rx * tcos(deg);
        dy = ry * tsin(deg);
        CPackMan::pkAddGsData((u_long)(cx + dx) | ((u_long)(cy + dy) << 16) | ((u_long)z << 32));
    }
    CPackMan::pkCloseGifTag();
}
void C3DPointLight::makePkt() {
    INT cx;
    INT cy;
    INT z;
    FLOAT rx;
    FLOAT ry;
    INT dx;
    INT dy;
    FVECTOR c;
    FLOAT rx0;
    FLOAT ry0;
    if (p0.z <= 0 || p0.z > 0xFFFF28 || p0.x < 0 || p0.x > 0xFFFF || p0.y < 0 || p0.y > 0xFFFF ||
        p1.z > 0xFFFF28 || p1.z <= 0 || p1.x < 0 || p1.x > 0xFFFF || p1.y < 0 || p1.y > 0xFFFF) {
        return;
    }

    z = p0.z >> 4;

    m_C0.a = m_C1.a = 0x80;

    cx = (p0.x + p1.x) / 2;
    rx = (p1.x - p0.x) / 2;

    cy = (p0.y + p1.y) / 2;
    ry = (p1.y - p0.y) / 2;

    FMATRIX& vs = CWorldEnv::getViewScreenMatrix();
    c = ApplyMatrix(m_pCrd->getWorldMatrix(), m_Center);
    c = ApplyMatrix(CWorldEnv::getWorldViewMatrix(), c);
    IVECTOR p00;
    IVECTOR p01;

    c.x -= m_R00;
    c.y -= m_R00;

    RotTransPers(p00, vs, c);

    c.x += m_R00 * 2.0f;
    c.y += m_R00 * 2.0f;
    RotTransPers(p01, vs, c);
    rx0 = (p01.x - p00.x) / 2;
    ry0 = (p01.y - p00.y) / 2;

    QWdata giftag;
    giftag.ul64[0] = 0x4400000000008000UL;
    giftag.ul64[1] = 0x1410;

    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x20A | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(m_C0.col);
    CPackMan::pkAddGsData((u_long)cx | ((u_long)cy << 16) | ((u_long)z << 32));
    CPackMan::pkAddGsData(m_C1.col);
    CPackMan::pkCloseGifTag();

    giftag.ul64[0] = 0x1400000000008000UL;
    giftag.ul64[1] = 4;

    CPackMan::pkOpenGifTag(giftag.ul128);

    int deg;
    for (deg = 0; deg <= 3600; deg += 200) {
        dy = -(INT)(ry * tcos(deg));
        dx = rx * tsin(deg);
        CPackMan::pkAddGsData((u_long)(cx + dx) | ((u_long)(cy + dy) << 16) | ((u_long)z << 32));
    }
    for (deg = 0; deg <= 3600; deg += 200) {
        dy = -(INT)(ry0 * tcos(deg));
        dx = rx0 * tsin(deg);
        CPackMan::pkAddGsData((u_long)(cx + dx) | ((u_long)(cy + dy) << 16) | ((u_long)z << 32));
    }
    CPackMan::pkCloseGifTag();

    if (m_Active) {
        int deg;
        m_C0.a = m_C1.a = 0x40;

        giftag.ul64[0] = 0x4400000000008000UL;
        giftag.ul64[1] = 0x1410;

        CPackMan::pkOpenGifTag(giftag.ul128);
        CPackMan::pkAddGsData((u_long)0x20D | ((u_long)isSemiTrans() << 6));
        CPackMan::pkAddGsData(m_C0.col);
        CPackMan::pkAddGsData((u_long)cx | ((u_long)cy << 16) | ((u_long)z << 32));
        CPackMan::pkAddGsData(m_C1.col);
        CPackMan::pkCloseGifTag();

        giftag.ul64[0] = 0x1400000000008000UL;
        giftag.ul64[1] = 4;

        CPackMan::pkOpenGifTag(giftag.ul128);
        for (deg = 0; deg <= 3600; deg += 200) {
            dy = -(INT)(ry0 * tcos(deg));
            dx = rx0 * tsin(deg);
            CPackMan::pkAddGsData((u_long)(cx + dx) | ((u_long)(cy + dy) << 16) | ((u_long)z << 32));
        }
        CPackMan::pkCloseGifTag();
    }
}
FIX4 C3DPointLight2::calcZ() {
    FIX4 nRet = ((CGPrim&)m_Cl[0]).calcZ();
    if (m_nType == 1) {
        ((CGPrim&)m_Cl[1]).calcZ();
    }
    return nRet;
}
#define PI 3.1415927f
extern "C" {
float atan2f(float, float);
float cosf(float);
float sinf(float);
}
namespace Math3D {
FLOAT LimitPI(FLOAT pai);
}
void C3DPointLight2::makePkt() {
    BOOL bRet;
    FVECTOR c[2];
    FVECTOR r[2];
    FVECTOR r1[2];
    INT z0;
    INT z1;
    QWdata giftag;
    FLOAT ftheta;
    IVECTOR ip;
    INT j;

    bRet = _makePktSub(0);
    if (m_nType != 1) return;
    if (!_makePktSub(1) || !bRet) return;

    for (j = 0; j < 2; j++) {
        c[j].x = (m_Cl[j].p0.x + m_Cl[j].p1.x) / 2;
        r[j].x = (m_Cl[j].p1.x - m_Cl[j].p0.x) / 2;
        c[j].y = (m_Cl[j].p0.y + m_Cl[j].p1.y) / 2;
        r[j].y = (m_Cl[j].p1.y - m_Cl[j].p0.y) / 2;

        r1[j].x = (p01[j].x - p00[j].x) / 2;
        r1[j].y = (p01[j].y - p00[j].y) / 2;

        m_Cl[j].m_C0.a = m_Cl[j].m_C1.a = 0x80;
    }
    z0 = m_Cl[0].p0.z >> 4;
    z1 = m_Cl[1].p0.z >> 4;

    giftag.ul64[0] = 0x1400000000008000UL;
    giftag.ul64[1] = 0;
    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x209 | ((u_long)isSemiTrans() << 6));
    CPackMan::pkCloseGifTag();

    giftag.ul64[0] = 0x2400000000008000UL;
    giftag.ul64[1] = 0x41;
    CPackMan::pkOpenGifTag(giftag.ul128);

    CPackMan::pkAddGsData(m_Cl[0].m_C0.col);
    CPackMan::pkAddGsData((u_long)(INT)c[0].x | ((u_long)(INT)c[0].y << 16) | ((u_long)z0 << 32));
    CPackMan::pkAddGsData(m_Cl[1].m_C1.col);
    CPackMan::pkAddGsData((u_long)(INT)c[1].x | ((u_long)(INT)c[1].y << 16) | ((u_long)z1 << 32));

    ftheta = atan2f(c[1].y - c[0].y, c[1].x - c[0].x);

    ip.x = c[0].x + r[0].x * cosf(Math3D::LimitPI(ftheta + PI / 2));
    ip.y = c[0].y + r[0].y * sinf(Math3D::LimitPI(ftheta + PI / 2));
    CPackMan::pkAddGsData(m_Cl[0].m_C0.col);
    CPackMan::pkAddGsData((u_long)ip.x | ((u_long)ip.y << 16) | ((u_long)z0 << 32));
    ip.x = c[1].x + r[1].x * cosf(Math3D::LimitPI(ftheta + PI / 2));
    ip.y = c[1].y + r[1].y * sinf(Math3D::LimitPI(ftheta + PI / 2));
    CPackMan::pkAddGsData(m_Cl[1].m_C1.col);
    CPackMan::pkAddGsData((u_long)ip.x | ((u_long)ip.y << 16) | ((u_long)z1 << 32));
    ip.x = c[0].x + r[0].x * cosf(Math3D::LimitPI(ftheta - PI / 2));
    ip.y = c[0].y + r[0].y * sinf(Math3D::LimitPI(ftheta - PI / 2));
    CPackMan::pkAddGsData(m_Cl[0].m_C0.col);
    CPackMan::pkAddGsData((u_long)ip.x | ((u_long)ip.y << 16) | ((u_long)z0 << 32));
    ip.x = c[1].x + r[1].x * cosf(Math3D::LimitPI(ftheta - PI / 2));
    ip.y = c[1].y + r[1].y * sinf(Math3D::LimitPI(ftheta - PI / 2));
    CPackMan::pkAddGsData(m_Cl[1].m_C1.col);
    CPackMan::pkAddGsData((u_long)ip.x | ((u_long)ip.y << 16) | ((u_long)z1 << 32));

    ip.x = c[0].x + r1[0].x * cosf(Math3D::LimitPI(ftheta + PI / 2));
    ip.y = c[0].y + r1[0].y * sinf(Math3D::LimitPI(ftheta + PI / 2));
    CPackMan::pkAddGsData(m_Cl[0].m_C0.col);
    CPackMan::pkAddGsData((u_long)ip.x | ((u_long)ip.y << 16) | ((u_long)z0 << 32));
    ip.x = c[1].x + r1[1].x * cosf(Math3D::LimitPI(ftheta + PI / 2));
    ip.y = c[1].y + r1[1].y * sinf(Math3D::LimitPI(ftheta + PI / 2));
    CPackMan::pkAddGsData(m_Cl[1].m_C1.col);
    CPackMan::pkAddGsData((u_long)ip.x | ((u_long)ip.y << 16) | ((u_long)z1 << 32));
    ip.x = c[0].x + r1[0].x * cosf(Math3D::LimitPI(ftheta - PI / 2));
    ip.y = c[0].y + r1[0].y * sinf(Math3D::LimitPI(ftheta - PI / 2));
    CPackMan::pkAddGsData(m_Cl[0].m_C0.col);
    CPackMan::pkAddGsData((u_long)ip.x | ((u_long)ip.y << 16) | ((u_long)z0 << 32));
    ip.x = c[1].x + r1[1].x * cosf(Math3D::LimitPI(ftheta - PI / 2));
    ip.y = c[1].y + r1[1].y * sinf(Math3D::LimitPI(ftheta - PI / 2));
    CPackMan::pkAddGsData(m_Cl[1].m_C1.col);
    CPackMan::pkAddGsData((u_long)ip.x | ((u_long)ip.y << 16) | ((u_long)z1 << 32));

    CPackMan::pkCloseGifTag();
}
BOOL C3DPointLight2::_makePktSub(INT nIdx) {
    C3DCircle& cl = m_Cl[nIdx];
    INT cx;
    INT cy;
    INT z;
    FLOAT rx;
    FLOAT ry;
    INT dx;
    INT dy;
    FVECTOR c;
    FLOAT rx0;
    FLOAT ry0;
    if (cl.p0.z <= 0 || cl.p0.z > 0xFFFF28 || cl.p0.x < 0 || cl.p0.x > 0xFFFF || cl.p0.y < 0 || cl.p0.y > 0xFFFF ||
        cl.p1.z > 0xFFFF28 || cl.p1.z <= 0 || cl.p1.x < 0 || cl.p1.x > 0xFFFF || cl.p1.y < 0 || cl.p1.y > 0xFFFF) {
        return FALSE;
    }

    z = cl.p0.z >> 4;

    cl.m_C0.a = cl.m_C1.a = 0x80;

    cx = (cl.p0.x + cl.p1.x) / 2;
    rx = (cl.p1.x - cl.p0.x) / 2;

    cy = (cl.p0.y + cl.p1.y) / 2;
    ry = (cl.p1.y - cl.p0.y) / 2;

    FMATRIX& vs = CWorldEnv::getViewScreenMatrix();
    c = ApplyMatrix(cl.m_pCrd->getWorldMatrix(), cl.m_Center);
    c = ApplyMatrix(CWorldEnv::getWorldViewMatrix(), c);

    c.x -= m_R00;
    c.y -= m_R00;

    RotTransPers(p00[nIdx], vs, c);

    c.x += m_R00 * 2.0f;
    c.y += m_R00 * 2.0f;
    RotTransPers(p01[nIdx], vs, c);

    rx0 = (p01[nIdx].x - p00[nIdx].x) / 2;
    ry0 = (p01[nIdx].y - p00[nIdx].y) / 2;

    QWdata giftag;
    giftag.ul64[0] = 0x4400000000008000UL;
    giftag.ul64[1] = 0x1410;

    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x20A | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(cl.m_C0.col);
    CPackMan::pkAddGsData((u_long)cx | ((u_long)cy << 16) | ((u_long)z << 32));
    CPackMan::pkAddGsData(cl.m_C1.col);
    CPackMan::pkCloseGifTag();

    giftag.ul64[0] = 0x1400000000008000UL;
    giftag.ul64[1] = 4;

    CPackMan::pkOpenGifTag(giftag.ul128);

    int deg;
    for (deg = 0; deg <= 3600; deg += 200) {
        dy = -(INT)(ry * tcos(deg));
        dx = rx * tsin(deg);
        CPackMan::pkAddGsData((u_long)(cx + dx) | ((u_long)(cy + dy) << 16) | ((u_long)z << 32));
    }
    for (deg = 0; deg <= 3600; deg += 200) {
        dy = -(INT)(ry0 * tcos(deg));
        dx = rx0 * tsin(deg);
        CPackMan::pkAddGsData((u_long)(cx + dx) | ((u_long)(cy + dy) << 16) | ((u_long)z << 32));
    }
    CPackMan::pkCloseGifTag();

    if (m_Active) {
        int deg;
        cl.m_C0.a = cl.m_C1.a = 0x40;

        giftag.ul64[0] = 0x4400000000008000UL;
        giftag.ul64[1] = 0x1410;

        CPackMan::pkOpenGifTag(giftag.ul128);
        CPackMan::pkAddGsData((u_long)0x20D | ((u_long)isSemiTrans() << 6));
        CPackMan::pkAddGsData(cl.m_C0.col);
        CPackMan::pkAddGsData((u_long)cx | ((u_long)cy << 16) | ((u_long)z << 32));
        CPackMan::pkAddGsData(cl.m_C1.col);
        CPackMan::pkCloseGifTag();

        giftag.ul64[0] = 0x1400000000008000UL;
        giftag.ul64[1] = 4;

        CPackMan::pkOpenGifTag(giftag.ul128);
        for (deg = 0; deg <= 3600; deg += 200) {
            dy = -(INT)(ry0 * tcos(deg));
            dx = rx0 * tsin(deg);
            CPackMan::pkAddGsData((u_long)(cx + dx) | ((u_long)(cy + dy) << 16) | ((u_long)z << 32));
        }
        CPackMan::pkCloseGifTag();
    }
    return TRUE;
}
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
void C3DPolyG4::makePkt() {
    ULONGLONG* p64;
    p64 = (ULONGLONG*)CPackMan::pkOpenGifTagManual(_gifTag(0x9400000000008001UL, 0x414141410UL));
    p64[0] = (u_long)0x20C | ((u_long)isSemiTrans() << 6);
    p64[1] = m_C0.col;
    p64[2] = _vecToXYZ(p0);
    p64[3] = m_C1.col;
    p64[4] = _vecToXYZ(p1);
    p64[5] = m_C2.col;
    p64[6] = _vecToXYZ(p2);
    p64[7] = m_C3.col;
    p64[8] = _vecToXYZ(p3);
    CPackMan::pkCloseGifTagManual((UINT*)&p64[9]);
}
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
void C3DPolyG3::makePkt() {
    ULONGLONG* p64;
    p64 = (ULONGLONG*)CPackMan::pkOpenGifTagManual(_gifTag(0x7400000000008001UL, 0x4141410));
    p64[0] = (u_long)0x20C | ((u_long)isSemiTrans() << 6);
    p64[1] = m_C0.col;
    p64[2] = _vecToXYZ(p0);
    p64[3] = m_C1.col;
    p64[4] = _vecToXYZ(p1);
    p64[5] = m_C2.col;
    p64[6] = _vecToXYZ(p2);
    CPackMan::pkCloseGifTagManual((UINT*)&p64[7]);
}
FIX4 C3DLine::calcZ() {
    FMATRIX mat0;
    m_pCrd->getRotTransPersMatrix(mat0);
    setRotTransPersMatrix(mat0);
    RotTransPersClipW(p0, m_P0);
    RotTransPersClipW(p1, m_P1);
    return p0.z;
}
void C3DLine::makePkt() {
    QWdata giftag;
    if (p0.w || p1.w) {
        return;
    }
    giftag.ul64[0] = 0x5400000000008000UL;
    giftag.ul64[1] = 0x41410;
    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x209 | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(m_C0.col);
    CPackMan::pkAddGsData(_vecToXYZ(p0));
    CPackMan::pkAddGsData(m_C1.col);
    CPackMan::pkAddGsData(_vecToXYZ(p1));
    CPackMan::pkCloseGifTag();
}
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
void C3DLineCoord2::makePkt() {
    QWdata giftag;
    if (p0.w || p1.w) {
        return;
    }
    giftag.ul64[0] = 0x5400000000008000UL;
    giftag.ul64[1] = 0x41410;
    CPackMan::pkOpenGifTag(giftag.ul128);
    CPackMan::pkAddGsData((u_long)0x209 | ((u_long)isSemiTrans() << 6));
    CPackMan::pkAddGsData(m_C0.col);
    CPackMan::pkAddGsData(_vecToXYZ(p0));
    CPackMan::pkAddGsData(m_C1.col);
    CPackMan::pkAddGsData(_vecToXYZ(p1));
    CPackMan::pkCloseGifTag();
}
FIX4 C3DLines::calcZ() {
    FMATRIX mat0;
    G_LINE* it;
    if (m_vLINE.size() == 0) {
        return 0;
    }

    if (m_pCrd) {
        m_pCrd->getRotTransPersMatrix(mat0);
    } else {
        mat0 = CWorldEnv::getWorldScreenMatrix();
    }
    setRotTransPersMatrix(mat0);

    for (it = m_vLINE.begin(); it != m_vLINE.end();) {
        RotTransPersClipW(it->p0, it->P0);
        RotTransPersClipW(it->p1, it->P1);
        it++;
    }

    return m_vLINE.begin()->p0.z;
}
void C3DLines::makePkt() {
    QWdata giftag;
    G_LINE* it;
    if (m_vLINE.size() == 0) {
        return;
    }
    giftag.ul64[0] = 0x5400000000008000UL;
    giftag.ul64[1] = 0x41410;
    CPackMan::pkOpenGifTag(giftag.ul128);
    for (it = m_vLINE.begin(); it != m_vLINE.end(); it++) {
        if (it->p0.w) {
            continue;
        }
        CPackMan::pkAddGsData((u_long)0x209 | ((u_long)isSemiTrans() << 6));
        CPackMan::pkAddGsData(it->C0.col);
        CPackMan::pkAddGsData(_vecToXYZ(it->p0));
        CPackMan::pkAddGsData(it->C1.col);
        CPackMan::pkAddGsData(_vecToXYZ(it->p1));
    }
    CPackMan::pkCloseGifTag();
}
/* Parked: registers only - the original loads it->S into v1 before the mfc1 of fW0 (filling the beql slot) and keeps
 * iW in a0, ours puts iW in v1 and loads it->S after; tried: STQ inline taking G_ST&/ULONGLONG/FLOAT, iW before/after
 * the call, an inSTQ block local, permuter 15 min (15280 candidates, no gain) */
#if 0
ULONGLONG* _makePkt_TexLine(ULONGLONG* p64, G_STRIP* it, G_STRIP* end) {
    IVECTOR p[3];
    FLOAT fW0;
    FLOAT fW1;
    UINT iW;
    while (it < end) {
        _prefetch(it, 0x20);
        fW0 = RotTransPersClipW(p[0], it[0].P);
        if (p[0].w) {
            it += 2;
            continue;
        }
        _prefetch(it, 0x40);
        fW1 = RotTransPersClipW(p[1], it[1].P);
        if (p[1].w) {
            it += 2;
            continue;
        }
        iW = *(UINT*)&fW0;
        p64[0] = (ULONGLONG)_calcSTQ(*(ULONGLONG*)&it[0].S, iW);
        p64[1] = it[0].C.col | ((ULONGLONG)iW << 32);
        p64[2] = _vecToXYZ(p[0]);
        iW = *(UINT*)&fW1;
        p64[3] = (ULONGLONG)_calcSTQ(*(ULONGLONG*)&it[1].S, iW);
        p64[4] = it[1].C.col | ((ULONGLONG)iW << 32);
        p64[5] = _vecToXYZ(p[1]);
        it += 2;
        p64 += 6;
    }
    return p64;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexLine__FPUlP7G_STRIPT1);
#endif
/* Parked: registers only - as in the CCoord-less version the original keeps iW in a0 and the copy of the mfc1 in v1
 * (ld of it->S after it), ours swaps them; tried: STQ inline taking G_ST&+UINT, G_ST&+FLOAT, operand order in the
 * pcpyld asm, iW assigned inside the call, permuter 15 min (12213 candidates, no gain) */
#if 0
ULONGLONG* _makePkt_TexLine(ULONGLONG* p64, G_STRIP* it, G_STRIP* end, CCoord* pCrd) {
    IVECTOR p[3];
    FLOAT fW0;
    FLOAT fW1;
    UINT iW;
    INT iBone = -1;
    while (it < end) {
        _prefetch(it, 0x20);
        if (iBone != it[0].Bone) {
            iBone = it[0].Bone;
            pCrd[iBone].getRotTransPersMatrix_VF04_07();
        }
        fW0 = RotTransPersClipW(p[0], it[0].P);
        if (p[0].w) {
            it += 2;
            continue;
        }
        _prefetch(it, 0x40);
        if (iBone != it[1].Bone) {
            iBone = it[1].Bone;
            pCrd[iBone].getRotTransPersMatrix_VF04_07();
        }
        fW1 = RotTransPersClipW(p[1], it[1].P);
        if (p[1].w) {
            it += 2;
            continue;
        }
        iW = *(UINT*)&fW0;
        p64[0] = (ULONGLONG)_calcSTQ(it[0].S, iW);
        p64[1] = it[0].C.col | ((ULONGLONG)iW << 32);
        p64[2] = _vecToXYZ(p[0]);
        iW = *(UINT*)&fW1;
        p64[3] = (ULONGLONG)_calcSTQ(it[1].S, iW);
        p64[4] = it[1].C.col | ((ULONGLONG)iW << 32);
        p64[5] = _vecToXYZ(p[1]);
        it += 2;
        p64 += 6;
    }
    return p64;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexLine__FPUlP7G_STRIPT1P6CCoord);
#endif
/* Parked: registers only - per vertex the original loads it->S into v1 (the first one in the beql slot) and copies
 * the mfc1 result into a0 for iW, ours puts iW in v1 and loads it->S into v0; loop shape, per-loop fW and mfc1
 * order (volatile _floatBits) match; tried: S read into a local first/last, STQ inline with swapped operands, iW
 * assigned inside the call, register asm a0 for iW, a C bit-cast inline, permuter 2x15 min (9270 candidates, no
 * gain) */
#if 0
ULONGLONG* _makePkt_TexTriangle(ULONGLONG* p64, G_STRIP* it, G_STRIP* end, BOOL Back) {
    IVECTOR p[3];
    UINT iW;
    if (!Back) {
        FLOAT fW0;
        FLOAT fW1;
        FLOAT fW2;
        while (it < end) {
            _prefetch(it, 0x20);
            fW0 = RotTransPersClipW(p[0], it[0].P);
            if (p[0].w) {
                goto next0;
            }
            _prefetch(it, 0x40);
            fW1 = RotTransPersClipW(p[1], it[1].P);
            if (p[1].w) {
                goto next0;
            }
            _prefetch(it, 0x60);
            fW2 = RotTransPersClipW(p[2], it[2].P);
            if (p[2].w) {
            next0:
                it += 3;
            } else {
                iW = _floatBits(fW0);
                p64[0] = _toU64(_calcSTQ(*(ULONGLONG*)&it[0].S, iW));
                p64[1] = it[0].C.col | ((ULONGLONG)iW << 32);
                p64[2] = _vecToXYZ(p[0]);
                iW = _floatBits(fW1);
                p64[3] = _toU64(_calcSTQ(*(ULONGLONG*)&it[1].S, iW));
                p64[4] = it[1].C.col | ((ULONGLONG)iW << 32);
                p64[5] = _vecToXYZ(p[1]);
                iW = _floatBits(fW2);
                p64[6] = _toU64(_calcSTQ(*(ULONGLONG*)&it[2].S, iW));
                p64[7] = it[2].C.col | ((ULONGLONG)iW << 32);
                p64[8] = _vecToXYZ(p[2]);
                it += 3;
                p64 += 9;
            }
        }
    } else {
        FVECTOR vDir[3];
        FLOAT fW0;
        FLOAT fW1;
        FLOAT fW2;
        while (it < end) {
            _prefetch(it, 0x20);
            fW0 = RotTransPersClipW(p[0], vDir[0], it[0].P);
            if (p[0].w) {
                goto next1;
            }
            _prefetch(it, 0x40);
            fW1 = RotTransPersClipW(p[1], vDir[1], it[1].P);
            if (p[1].w) {
                goto next1;
            }
            _prefetch(it, 0x60);
            fW2 = RotTransPersClipW(p[2], vDir[2], it[2].P);
            if (p[2].w) {
                goto next1;
            }
            if (BackClip(vDir, 0)) {
            next1:
                it += 3;
            } else {
                iW = _floatBits(fW0);
                p64[0] = _toU64(_calcSTQ(*(ULONGLONG*)&it[0].S, iW));
                p64[1] = it[0].C.col | ((ULONGLONG)iW << 32);
                p64[2] = _vecToXYZ(p[0]);
                iW = _floatBits(fW1);
                p64[3] = _toU64(_calcSTQ(*(ULONGLONG*)&it[1].S, iW));
                p64[4] = it[1].C.col | ((ULONGLONG)iW << 32);
                p64[5] = _vecToXYZ(p[1]);
                iW = _floatBits(fW2);
                p64[6] = _toU64(_calcSTQ(*(ULONGLONG*)&it[2].S, iW));
                p64[7] = it[2].C.col | ((ULONGLONG)iW << 32);
                p64[8] = _vecToXYZ(p[2]);
                it += 3;
                p64 += 9;
            }
        }
    }
    return p64;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexTriangle__FPUlP7G_STRIPT1i);
#endif
/* Parked: registers (as the CCoord-less version: it->S into v1 and the mfc1 copy in a0 for iW, ours swapped) and
 * one loop-alignment nop before the second loop; the draft is the CCoord-less one plus the per-vertex bone checks
 * of the matched _makePkt_Triangle (CCoord); tried: the CCoord-less version's levers (see there) */
#if 0
ULONGLONG* _makePkt_TexTriangle(ULONGLONG* p64, G_STRIP* it, G_STRIP* end, CCoord* pCrd, BOOL Back) {
    IVECTOR p[3];
    INT iBone;
    UINT iW;
    iBone = -1;
    if (!Back) {
        FLOAT fW0;
        FLOAT fW1;
        FLOAT fW2;
        while (it < end) {
            _prefetch(it, 0x20);
            if (iBone != it[0].Bone) {
                iBone = it[0].Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            fW0 = RotTransPersClipW(p[0], it[0].P);
            if (p[0].w) {
                goto next0;
            }
            _prefetch(it, 0x40);
            if (iBone != it[1].Bone) {
                iBone = it[1].Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            fW1 = RotTransPersClipW(p[1], it[1].P);
            if (p[1].w) {
                goto next0;
            }
            _prefetch(it, 0x60);
            if (iBone != it[2].Bone) {
                iBone = it[2].Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            fW2 = RotTransPersClipW(p[2], it[2].P);
            if (p[2].w) {
            next0:
                it += 3;
            } else {
                iW = _floatBits(fW0);
                p64[0] = _toU64(_calcSTQ(*(ULONGLONG*)&it[0].S, iW));
                p64[1] = it[0].C.col | ((ULONGLONG)iW << 32);
                p64[2] = _vecToXYZ(p[0]);
                iW = _floatBits(fW1);
                p64[3] = _toU64(_calcSTQ(*(ULONGLONG*)&it[1].S, iW));
                p64[4] = it[1].C.col | ((ULONGLONG)iW << 32);
                p64[5] = _vecToXYZ(p[1]);
                iW = _floatBits(fW2);
                p64[6] = _toU64(_calcSTQ(*(ULONGLONG*)&it[2].S, iW));
                p64[7] = it[2].C.col | ((ULONGLONG)iW << 32);
                p64[8] = _vecToXYZ(p[2]);
                it += 3;
                p64 += 9;
            }
        }
    } else {
        FVECTOR vDir[3];
        FLOAT fW0;
        FLOAT fW1;
        FLOAT fW2;
        while (it < end) {
            _prefetch(it, 0x20);
            if (iBone != it[0].Bone) {
                iBone = it[0].Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            fW0 = RotTransPersClipW(p[0], vDir[0], it[0].P);
            if (p[0].w) {
                goto next1;
            }
            _prefetch(it, 0x40);
            if (iBone != it[1].Bone) {
                iBone = it[1].Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            fW1 = RotTransPersClipW(p[1], vDir[1], it[1].P);
            if (p[1].w) {
                goto next1;
            }
            _prefetch(it, 0x60);
            if (iBone != it[2].Bone) {
                iBone = it[2].Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            fW2 = RotTransPersClipW(p[2], vDir[2], it[2].P);
            if (p[2].w) {
                goto next1;
            }
            if (BackClip(vDir, 0)) {
            next1:
                it += 3;
            } else {
                iW = _floatBits(fW0);
                p64[0] = _toU64(_calcSTQ(*(ULONGLONG*)&it[0].S, iW));
                p64[1] = it[0].C.col | ((ULONGLONG)iW << 32);
                p64[2] = _vecToXYZ(p[0]);
                iW = _floatBits(fW1);
                p64[3] = _toU64(_calcSTQ(*(ULONGLONG*)&it[1].S, iW));
                p64[4] = it[1].C.col | ((ULONGLONG)iW << 32);
                p64[5] = _vecToXYZ(p[1]);
                iW = _floatBits(fW2);
                p64[6] = _toU64(_calcSTQ(*(ULONGLONG*)&it[2].S, iW));
                p64[7] = it[2].C.col | ((ULONGLONG)iW << 32);
                p64[8] = _vecToXYZ(p[2]);
                it += 3;
                p64 += 9;
            }
        }
    }
    return p64;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexTriangle__FPUlP7G_STRIPT1P6CCoordi);
#endif
ULONGLONG* _makePkt_Line(ULONGLONG* p64, G_STRIP* it, G_STRIP* end) {
    IVECTOR p[3];
    while (it < end) {
        _prefetch(it, 0x20);
        RotTransPersClipW(p[0], it[0].P);
        if (p[0].w) {
            it += 2;
            continue;
        }
        _prefetch(it, 0x40);
        RotTransPersClipW(p[1], it[1].P);
        if (p[1].w) {
            it += 2;
            continue;
        }
        p64[0] = it[0].C.col;
        p64[1] = _vecToXYZ(p[0]);
        p64[2] = it[1].C.col;
        p64[3] = _vecToXYZ(p[1]);
        it += 2;
        p64 += 4;
    }
    return p64;
}
ULONGLONG* _makePkt_Line(ULONGLONG* p64, G_STRIP* it, G_STRIP* end, CCoord* pCrd) {
    IVECTOR p[3];
    INT iBone = -1;
    while (it < end) {
        _prefetch(it, 0x20);
        if (iBone != it[0].Bone) {
            iBone = it[0].Bone;
            pCrd[iBone].getRotTransPersMatrix_VF04_07();
        }
        RotTransPersClipW(p[0], it[0].P);
        if (p[0].w) {
            it += 2;
            continue;
        }
        _prefetch(it, 0x40);
        if (iBone != it[1].Bone) {
            iBone = it[1].Bone;
            pCrd[iBone].getRotTransPersMatrix_VF04_07();
        }
        RotTransPersClipW(p[1], it[1].P);
        if (p[1].w) {
            it += 2;
            continue;
        }
        p64[0] = it[0].C.col;
        p64[1] = _vecToXYZ(p[0]);
        p64[2] = it[1].C.col;
        p64[3] = _vecToXYZ(p[1]);
        it += 2;
        p64 += 4;
    }
    return p64;
}
ULONGLONG* _makePkt_Triangle(ULONGLONG* p64, G_STRIP* it, G_STRIP* end, BOOL Back) {
    IVECTOR p[3];
    if (!Back) {
        while (it < end) {
            _prefetch(it, 0x20);
            RotTransPersClipW(p[0], it[0].P);
            if (p[0].w) {
                goto next0;
            }
            _prefetch(it, 0x40);
            RotTransPersClipW(p[1], it[1].P);
            if (p[1].w) {
                goto next0;
            }
            _prefetch(it, 0x60);
            RotTransPersClipW(p[2], it[2].P);
            if (p[2].w) {
            next0:
                it += 3;
            } else {
                p64[0] = it[0].C.col;
                p64[1] = _vecToXYZ(p[0]);
                p64[2] = it[1].C.col;
                p64[3] = _vecToXYZ(p[1]);
                p64[4] = it[2].C.col;
                p64[5] = _vecToXYZ(p[2]);
                it += 3;
                p64 += 6;
            }
        }
    } else {
        FVECTOR vDir[3];
        while (it < end) {
            _prefetch(it, 0x20);
            RotTransPersClipW(p[0], vDir[0], it[0].P);
            if (p[0].w) {
                goto next1;
            }
            _prefetch(it, 0x40);
            RotTransPersClipW(p[1], vDir[1], it[1].P);
            if (p[1].w) {
                goto next1;
            }
            _prefetch(it, 0x60);
            RotTransPersClipW(p[2], vDir[2], it[2].P);
            if (p[2].w) {
                goto next1;
            }
            if (BackClip(vDir, 0)) {
            next1:
                it += 3;
            } else {
                p64[0] = it[0].C.col;
                p64[1] = _vecToXYZ(p[0]);
                p64[2] = it[1].C.col;
                p64[3] = _vecToXYZ(p[1]);
                p64[4] = it[2].C.col;
                p64[5] = _vecToXYZ(p[2]);
                it += 3;
                p64 += 6;
            }
        }
    }
    return p64;
}
ULONGLONG* _makePkt_Triangle(ULONGLONG* p64, G_STRIP* it, G_STRIP* end, CCoord* pCrd, BOOL Back) {
    IVECTOR p[3];
    INT iBone;
    iBone = -1;
    if (!Back) {
        while (it < end) {
            _prefetch(it, 0x20);
            if (iBone != it[0].Bone) {
                iBone = it[0].Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            RotTransPersClipW(p[0], it[0].P);
            if (p[0].w) {
                goto next0;
            }
            _prefetch(it, 0x40);
            if (iBone != it[1].Bone) {
                iBone = it[1].Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            RotTransPersClipW(p[1], it[1].P);
            if (p[1].w) {
                goto next0;
            }
            _prefetch(it, 0x60);
            if (iBone != it[2].Bone) {
                iBone = it[2].Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            RotTransPersClipW(p[2], it[2].P);
            if (p[2].w) {
            next0:
                it += 3;
            } else {
                p64[0] = it[0].C.col;
                p64[1] = _vecToXYZ(p[0]);
                p64[2] = it[1].C.col;
                p64[3] = _vecToXYZ(p[1]);
                p64[4] = it[2].C.col;
                p64[5] = _vecToXYZ(p[2]);
                it += 3;
                p64 += 6;
            }
        }
    } else {
        FVECTOR vDir[3];
        while (it < end) {
            _prefetch(it, 0x20);
            if (iBone != it[0].Bone) {
                iBone = it[0].Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            RotTransPersClipW(p[0], vDir[0], it[0].P);
            if (p[0].w) {
                goto next1;
            }
            _prefetch(it, 0x40);
            if (iBone != it[1].Bone) {
                iBone = it[1].Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            RotTransPersClipW(p[1], vDir[1], it[1].P);
            if (p[1].w) {
                goto next1;
            }
            _prefetch(it, 0x60);
            if (iBone != it[2].Bone) {
                iBone = it[2].Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            RotTransPersClipW(p[2], vDir[2], it[2].P);
            if (p[2].w) {
                goto next1;
            }
            if (BackClip(vDir, 0)) {
            next1:
                it += 3;
            } else {
                p64[0] = it[0].C.col;
                p64[1] = _vecToXYZ(p[0]);
                p64[2] = it[1].C.col;
                p64[3] = _vecToXYZ(p[1]);
                p64[4] = it[2].C.col;
                p64[5] = _vecToXYZ(p[2]);
                it += 3;
                p64 += 6;
            }
        }
    }
    return p64;
}
QWdata* _makePkt_TexLineStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end) {
    IVECTOR p;
    UINT fg;
    FLOAT fW;
    fg = 1;
    for (; it < end; it++, pQw += 3) {
        _prefetch(it, 0x20);
        fW = RotTransPersClipW(p, it->P);
        if (p.w) {
            fg |= 2;
        } else if (fg) {
            p.w = 0x8000;
        }
        if (it->Term) {
            fg = 1;
        } else {
            fg >>= 1;
        }
        _setSTQ(*(FVECTOR*)&pQw[0], *(ULONGLONG*)&it->S, fW);
        pQw[1].ul128 = _colToVec(it->C);
        pQw[2].ul128 = p._q;
    }
    return pQw;
}
QWdata* _makePkt_TexLineStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end, CCoord* pCrd) {
    IVECTOR p;
    UINT fg;
    FLOAT fW;
    INT iBone;
    fg = 1;
    iBone = -1;
    for (; it < end; it++, pQw += 3) {
        if (iBone != it->Bone) {
            iBone = it->Bone;
            pCrd[iBone].getRotTransPersMatrix_VF04_07();
        }
        _prefetch(it, 0x20);
        fW = RotTransPersClipW(p, it->P);
        if (p.w) {
            fg |= 2;
        } else if (fg) {
            p.w = 0x8000;
        }
        if (it->Term) {
            fg = 1;
        } else {
            fg >>= 1;
        }
        _setSTQ(*(FVECTOR*)&pQw[0], *(ULONGLONG*)&it->S, fW);
        pQw[1].ul128 = _colToVec(it->C);
        pQw[2].ul128 = p._q;
    }
    return pQw;
}
QWdata* _makePkt_TexTriangleStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end, BOOL Back) {
    IVECTOR p;
    UINT fg;
    FLOAT fW;
    fg = 2;

    if (!Back) {
        for (; it < end; it++, pQw += 3) {
            _prefetch(it, 0x20);
            fW = RotTransPersClipW(p, it->P);
            if (p.w) {
                fg |= 4;
            } else if (fg) {
                p.w = 0x8000;
            }
            if (it->Term) {
                fg = 2;
            } else {
                fg >>= 1;
            }
            _setSTQ(*(FVECTOR*)&pQw[0], *(ULONGLONG*)&it->S, fW);
            pQw[1].ul128 = _colToVec(it->C);
            pQw[2].ul128 = p._q;
        }
        return pQw;
    } else {
        FVECTOR vDir[3];
        INT iDirL = 0;
        for (; it < end; it++, pQw += 3) {
            _prefetch(it, 0x20);
            fW = RotTransPersClipW(p, vDir[2], it->P);
            if (p.w) {
                fg |= 4;
            } else if (fg) {
                p.w = 0x8000;
            } else {
                p.w |= BackClip(vDir, iDirL);
            }
            if (it->Term) {
                fg = 2;
                iDirL = 0;
            } else {
                fg >>= 1;
                vDir[0] = vDir[1];
                vDir[1] = vDir[2];
                iDirL ^= 1;
            }
            _setSTQ(*(FVECTOR*)&pQw[0], *(ULONGLONG*)&it->S, fW);
            pQw[1].ul128 = _colToVec(it->C);
            pQw[2].ul128 = p._q;
        }
    }
    return pQw;
}
QWdata* _makePkt_TexTriangleStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end, CCoord* pCrd, BOOL Back) {
    IVECTOR p;
    UINT fg;
    INT iBone;
    FLOAT fW;
    fg = 2;
    iBone = -1;

    if (!Back) {
        for (; it < end; it++, pQw += 3) {
            if (iBone != it->Bone) {
                iBone = it->Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            _prefetch(it, 0x20);
            fW = RotTransPersClipW(p, it->P);
            if (p.w) {
                fg |= 4;
            } else if (fg) {
                p.w = 0x8000;
            }
            if (it->Term) {
                fg = 2;
            } else {
                fg >>= 1;
            }
            _setSTQ(*(FVECTOR*)&pQw[0], *(ULONGLONG*)&it->S, fW);
            pQw[1].ul128 = _colToVec(it->C);
            pQw[2].ul128 = p._q;
        }
        return pQw;
    } else {
        FVECTOR vDir[3];
        INT iDirL = 0;
        for (; it < end; it++, pQw += 3) {
            if (iBone != it->Bone) {
                iBone = it->Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            _prefetch(it, 0x20);
            fW = RotTransPersClipW(p, vDir[2], it->P);
            if (p.w) {
                fg |= 4;
            } else if (fg) {
                p.w = 0x8000;
            } else {
                p.w |= BackClip(vDir, iDirL);
            }
            if (it->Term) {
                fg = 2;
                iDirL = 0;
            } else {
                fg >>= 1;
                vDir[0] = vDir[1];
                vDir[1] = vDir[2];
                iDirL ^= 1;
            }
            _setSTQ(*(FVECTOR*)&pQw[0], *(ULONGLONG*)&it->S, fW);
            pQw[1].ul128 = _colToVec(it->C);
            pQw[2].ul128 = p._q;
        }
    }
    return pQw;
}
/* Parked: the strip-skip loop entry only - the original jumps into the rolled loop at its Term test (b X with the
 * lhu in the slot), ours copies the test to the loop entry (jump.c duplicate_loop_exit_test) or keeps a redundant
 * it < end test; fg/end and iDir/0x8000 registers fixed by the plain while (it < end) form and iDir = 0 first;
 * tried: while/do-while/for(;;) skip loops, goto into the loop, bLoop variable with a bottom label, an inline Term
 * test, continue in the body, permuter 2x15 min (30 -> 23, no gain after) */
#if 0
QWdata* _makePkt_TexTriangleFan(QWdata* pQw, G_STRIP* it, G_STRIP* end, BOOL Back) {
    IVECTOR p;
    UINT fg = 2;
    INT bTop = 1;
    FLOAT fW;

    if (!Back) {
        while (it < end) {
            _prefetch(it, 0x20);
            fW = RotTransPersClipW(p, it->P);
            if (bTop) {
                if (p.w) {
                    if (!(it < end)) return pQw;
                    do {
                        if (!(it < end)) return pQw;
                    } while (!(it++)->Term);
                    goto next0;
                }
                p.w = 0x8000;
                bTop = 0;
            } else if (p.w) {
                fg |= 2;
            } else if (fg) {
                p.w = 0x8000;
            }
            if (it->Term) {
                fg = 2;
                bTop = 2;
            } else {
                fg >>= 1;
            }
            _setSTQ(*(FVECTOR*)&pQw[0], *(ULONGLONG*)&it->S, fW);
            pQw[1].ul128 = _colToVec(it->C);
            pQw[2].ul128 = p._q;
            it++;
            pQw += 3;
        next0:;
        }
        return pQw;
    } else {
        FVECTOR vDir[3];
        INT iDir = 0;
        while (it < end) {
            _prefetch(it, 0x20);
            fW = RotTransPersClipW(p, vDir[iDir], it->P);
            if (bTop) {
                if (p.w) {
                    if (!(it < end)) return pQw;
                    do {
                        if (!(it < end)) return pQw;
                    } while (!(it++)->Term);
                    goto next1;
                }
                p.w = 0x8000;
                bTop = 0;
            } else if (p.w) {
                fg |= 2;
            } else if (fg) {
                p.w = 0x8000;
            } else {
                p.w |= BackClip(vDir, 0);
            }
            if (it->Term) {
                iDir = 0;
                fg = 2;
                bTop = 2;
            } else {
                fg >>= 1;
                vDir[1] = vDir[2];
                iDir = 2;
            }
            _setSTQ(*(FVECTOR*)&pQw[0], *(ULONGLONG*)&it->S, fW);
            pQw[1].ul128 = _colToVec(it->C);
            pQw[2].ul128 = p._q;
            it++;
            pQw += 3;
        next1:;
        }
    }
    return pQw;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexTriangleFan__FP6QWdataP7G_STRIPT1i);
#endif
/* Parked: the strip-skip loop entry only, as in the CCoord-less version (the original enters the rolled skip loop
 * at its Term test, ours copies the test to the entry or keeps a redundant it < end test); bone check at the loop
 * top, iDir = 0 last in this one; tried: the CCoord-less version's levers (see there) */
#if 0
QWdata* _makePkt_TexTriangleFan(QWdata* pQw, G_STRIP* it, G_STRIP* end, CCoord* pCrd, BOOL Back) {
    IVECTOR p;
    UINT fg = 2;
    INT bTop = 1;
    INT iBone = -1;
    FLOAT fW;

    if (!Back) {
        while (it < end) {
            if (iBone != it->Bone) {
                iBone = it->Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            _prefetch(it, 0x20);
            fW = RotTransPersClipW(p, it->P);
            if (bTop) {
                if (p.w) {
                    if (!(it < end)) return pQw;
                    do {
                        if (!(it < end)) return pQw;
                    } while (!(it++)->Term);
                    goto next0;
                }
                p.w = 0x8000;
                bTop = 0;
            } else if (p.w) {
                fg |= 2;
            } else if (fg) {
                p.w = 0x8000;
            }
            if (it->Term) {
                fg = 2;
                bTop = 2;
            } else {
                fg >>= 1;
            }
            _setSTQ(*(FVECTOR*)&pQw[0], *(ULONGLONG*)&it->S, fW);
            pQw[1].ul128 = _colToVec(it->C);
            pQw[2].ul128 = p._q;
            it++;
            pQw += 3;
        next0:;
        }
        return pQw;
    } else {
        FVECTOR vDir[3];
        INT iDir = 0;
        while (it < end) {
            if (iBone != it->Bone) {
                iBone = it->Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            _prefetch(it, 0x20);
            fW = RotTransPersClipW(p, vDir[iDir], it->P);
            if (bTop) {
                if (p.w) {
                    if (!(it < end)) return pQw;
                    do {
                        if (!(it < end)) return pQw;
                    } while (!(it++)->Term);
                    goto next1;
                }
                p.w = 0x8000;
                bTop = 0;
            } else if (p.w) {
                fg |= 2;
            } else if (fg) {
                p.w = 0x8000;
            } else {
                p.w |= BackClip(vDir, 0);
            }
            if (it->Term) {
                fg = 2;
                bTop = 2;
                iDir = 0;
            } else {
                fg >>= 1;
                vDir[1] = vDir[2];
                iDir = 2;
            }
            _setSTQ(*(FVECTOR*)&pQw[0], *(ULONGLONG*)&it->S, fW);
            pQw[1].ul128 = _colToVec(it->C);
            pQw[2].ul128 = p._q;
            it++;
            pQw += 3;
        next1:;
        }
    }
    return pQw;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TexTriangleFan__FP6QWdataP7G_STRIPT1P6CCoordi);
#endif
QWdata* _makePkt_LineStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end) {
    IVECTOR p;
    UINT fg;
    fg = 1;
    for (; it < end; it++, pQw += 2) {
        _prefetch(it, 0x20);
        RotTransPersClipW(p, it->P);
        if (p.w) {
            fg |= 2;
        } else if (fg) {
            p.w = 0x8000;
        }
        if (it->Term) {
            fg = 1;
        } else {
            fg >>= 1;
        }
        pQw[0].ul128 = _colToVec(it->C);
        pQw[1].ul128 = p._q;
    }
    return pQw;
}
QWdata* _makePkt_LineStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end, CCoord* pCrd) {
    IVECTOR p;
    UINT fg;
    INT iBone;
    fg = 1;
    iBone = -1;
    for (; it < end; it++, pQw += 2) {
        if (iBone != it->Bone) {
            iBone = it->Bone;
            pCrd[iBone].getRotTransPersMatrix_VF04_07();
        }
        _prefetch(it, 0x20);
        RotTransPersClipW(p, it->P);
        if (p.w) {
            fg |= 2;
        } else if (fg) {
            p.w = 0x8000;
        }
        if (it->Term) {
            fg = 1;
        } else {
            fg >>= 1;
        }
        pQw[0].ul128 = _colToVec(it->C);
        pQw[1].ul128 = p._q;
    }
    return pQw;
}
QWdata* _makePkt_TriangleStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end, BOOL Back) {
    IVECTOR p;
    UINT fg;
    fg = 2;

    if (!Back) {
        for (; it < end; it++, pQw += 2) {
            _prefetch(it, 0x20);
            RotTransPersClipW(p, it->P);
            if (p.w) {
                fg |= 4;
            } else if (fg) {
                p.w = 0x8000;
            }
            if (it->Term) {
                fg = 2;
            } else {
                fg >>= 1;
            }
            pQw[0].ul128 = _colToVec(it->C);
            pQw[1].ul128 = p._q;
        }
        return pQw;
    } else {
        FVECTOR vDir[3];
        INT iDirL = 0;
        for (; it < end; it++, pQw += 2) {
            _prefetch(it, 0x20);
            RotTransPersClipW(p, vDir[2], it->P);
            if (p.w) {
                fg |= 4;
            } else if (fg) {
                p.w = 0x8000;
            } else {
                p.w |= BackClip(vDir, iDirL);
            }
            if (it->Term) {
                fg = 2;
                iDirL = 0;
            } else {
                fg >>= 1;
                vDir[0] = vDir[1];
                vDir[1] = vDir[2];
                iDirL ^= 1;
            }
            pQw[0].ul128 = _colToVec(it->C);
            pQw[1].ul128 = p._q;
        }
    }
    return pQw;
}
QWdata* _makePkt_TriangleStrip(QWdata* pQw, G_STRIP* it, G_STRIP* end, CCoord* pCrd, BOOL Back) {
    IVECTOR p;
    UINT fg;
    INT iBone;
    fg = 2;
    iBone = -1;

    if (!Back) {
        for (; it < end; it++, pQw += 2) {
            if (iBone != it->Bone) {
                iBone = it->Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            _prefetch(it, 0x20);
            RotTransPersClipW(p, it->P);
            if (p.w) {
                fg |= 4;
            } else if (fg) {
                p.w = 0x8000;
            }
            if (it->Term) {
                fg = 2;
            } else {
                fg >>= 1;
            }
            pQw[0].ul128 = _colToVec(it->C);
            pQw[1].ul128 = p._q;
        }
        return pQw;
    } else {
        FVECTOR vDir[3];
        INT iDirL = 0;
        for (; it < end; it++, pQw += 2) {
            if (iBone != it->Bone) {
                iBone = it->Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            _prefetch(it, 0x20);
            RotTransPersClipW(p, vDir[2], it->P);
            if (p.w) {
                fg |= 4;
            } else if (fg) {
                p.w = 0x8000;
            } else {
                p.w |= BackClip(vDir, iDirL);
            }
            if (it->Term) {
                fg = 2;
                iDirL = 0;
            } else {
                fg >>= 1;
                vDir[0] = vDir[1];
                vDir[1] = vDir[2];
                iDirL ^= 1;
            }
            pQw[0].ul128 = _colToVec(it->C);
            pQw[1].ul128 = p._q;
        }
    }
    return pQw;
}
/* Parked: the strip-skip loop entry only (as _makePkt_TexTriangleFan) - the original jumps into the rolled skip
 * loop at its Term test, ours copies that test to the loop entry or keeps a redundant it < end test; this draft
 * (11 lines) fixes the fg/end registers and the bLoop scheduling of the earlier one (32) with a plain while (it <
 * end) loop, a goto to the loop end instead of continue and iDir = 0 first; tried before: while/do-while/goto
 * shapes, an explicit BOOL bLoop, declaration orders, permuter 15 min; now also: for(;;)/goto-into-loop skip
 * loops, an inline Term test, continue in the body, permuter 2x15 min on the textured twin */
#if 0
QWdata* _makePkt_TriangleFan(QWdata* pQw, G_STRIP* it, G_STRIP* end, BOOL Back) {
    IVECTOR p;
    UINT fg = 2;
    INT bTop = 1;

    if (!Back) {
        while (it < end) {
            _prefetch(it, 0x20);
            RotTransPersClipW(p, it->P);
            if (bTop) {
                if (p.w) {
                    if (!(it < end)) return pQw;
                    do {
                        if (!(it < end)) return pQw;
                    } while (!(it++)->Term);
                    goto next0;
                }
                p.w = 0x8000;
                bTop = 0;
            } else if (p.w) {
                fg |= 2;
            } else if (fg) {
                p.w = 0x8000;
            }
            if (it->Term) {
                fg = 2;
                bTop = 2;
            } else {
                fg >>= 1;
            }
            pQw[0].ul128 = _colToVec(it->C);
            pQw[1].ul128 = p._q;
            it++;
            pQw += 2;
        next0:;
        }
        return pQw;
    } else {
        FVECTOR vDir[3];
        INT iDir = 0;
        while (it < end) {
            _prefetch(it, 0x20);
            RotTransPersClipW(p, vDir[iDir], it->P);
            if (bTop) {
                if (p.w) {
                    if (!(it < end)) return pQw;
                    do {
                        if (!(it < end)) return pQw;
                    } while (!(it++)->Term);
                    goto next1;
                }
                p.w = 0x8000;
                bTop = 0;
            } else if (p.w) {
                fg |= 2;
            } else if (fg) {
                p.w = 0x8000;
            } else {
                p.w |= BackClip(vDir, 0);
            }
            if (it->Term) {
                iDir = 0;
                fg = 2;
                bTop = 2;
            } else {
                fg >>= 1;
                vDir[1] = vDir[2];
                iDir = 2;
            }
            pQw[0].ul128 = _colToVec(it->C);
            pQw[1].ul128 = p._q;
            it++;
            pQw += 2;
        next1:;
        }
    }
    return pQw;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TriangleFan__FP6QWdataP7G_STRIPT1i);
#endif
/* Parked: the strip-skip loop entry only, as in _makePkt_TexTriangleFan; bone check at the loop top, iDir = 0
 * last; tried: the levers of the CCoord-less fans (see there) */
#if 0
QWdata* _makePkt_TriangleFan(QWdata* pQw, G_STRIP* it, G_STRIP* end, CCoord* pCrd, BOOL Back) {
    IVECTOR p;
    UINT fg = 2;
    INT bTop = 1;
    INT iBone = -1;

    if (!Back) {
        while (it < end) {
            if (iBone != it->Bone) {
                iBone = it->Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            _prefetch(it, 0x20);
            RotTransPersClipW(p, it->P);
            if (bTop) {
                if (p.w) {
                    if (!(it < end)) return pQw;
                    do {
                        if (!(it < end)) return pQw;
                    } while (!(it++)->Term);
                    goto next0;
                }
                p.w = 0x8000;
                bTop = 0;
            } else if (p.w) {
                fg |= 2;
            } else if (fg) {
                p.w = 0x8000;
            }
            if (it->Term) {
                fg = 2;
                bTop = 2;
            } else {
                fg >>= 1;
            }
            pQw[0].ul128 = _colToVec(it->C);
            pQw[1].ul128 = p._q;
            it++;
            pQw += 2;
        next0:;
        }
        return pQw;
    } else {
        FVECTOR vDir[3];
        INT iDir = 0;
        while (it < end) {
            if (iBone != it->Bone) {
                iBone = it->Bone;
                pCrd[iBone].getRotTransPersMatrix_VF04_07();
            }
            _prefetch(it, 0x20);
            RotTransPersClipW(p, vDir[iDir], it->P);
            if (bTop) {
                if (p.w) {
                    if (!(it < end)) return pQw;
                    do {
                        if (!(it < end)) return pQw;
                    } while (!(it++)->Term);
                    goto next1;
                }
                p.w = 0x8000;
                bTop = 0;
            } else if (p.w) {
                fg |= 2;
            } else if (fg) {
                p.w = 0x8000;
            } else {
                p.w |= BackClip(vDir, 0);
            }
            if (it->Term) {
                fg = 2;
                bTop = 2;
                iDir = 0;
            } else {
                fg >>= 1;
                vDir[1] = vDir[2];
                iDir = 2;
            }
            pQw[0].ul128 = _colToVec(it->C);
            pQw[1].ul128 = p._q;
            it++;
            pQw += 2;
        next1:;
        }
    }
    return pQw;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", _makePkt_TriangleFan__FP6QWdataP7G_STRIPT1P6CCoordi);
#endif
/* Parked: argument order of the RotTransPersW call (the original sets a1 before a0; sched1 issues a1 first only
 * when its source REG_DEADs, ours keeps vW's address live) and with it the TImode result register (a1 vs a0);
 * tried: matrix ref local, temp for the result, 0.0f > w, wrapper with swapped parameters, const attribute,
 * element helpers as asm (pcpyud/por/dsra32+mtc1/sll: 53 -> 10 lines), permuter 15 min (9926 candidates, no gain) */
#if 0
FLOAT getPrimGetaZ(INT iz, const FVECTOR& Vert, const FMATRIX* pWMat, FLOAT fGeta) {
    CCamera* pCam = CWorldEnv::getActiveCamera();
    if (pCam) {
        FVECTOR vW;
        FVECTOR vN;
        u_long128 vGeta;
        INT iGetaZ;
        vN.set(0.0f, 0.0f, -1.0f, 1.0f);
        if (pWMat) ApplyMatrix(vW, *pWMat, Vert); else vW = Vert;
        ApplyMatrix33(vN, ((const CCamera*)pCam)->Crd(0).m_World, vN);
        ScaleVectorXYZ(vN, vN, fGeta);
        AddVectorXYZ(vW, vW, vN);
        vGeta = RotTransPersW(CWorldEnv::getWorldScreenMatrix(), vW);

        if (_getW(vGeta) < 0.0f) {
            iGetaZ = 0xFFFF280;
        } else {
            iGetaZ = _getZ(vGeta);
            if (iGetaZ < 0) iGetaZ = 0;
        }

        iGetaZ = (iGetaZ - iz) >> 4;
        if (fGeta > 0.0f) {
            if (iGetaZ <= 0) iGetaZ = 1;
        } else {
            if (iGetaZ >= 0) iGetaZ = -1;
        }

        return iGetaZ;
    }
    return 0.0f;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", getPrimGetaZ__FiRC7FVECTORPC7FMATRIXf);
#endif
/* Parked: registers only - the original loads a1 (it->P) before a0 (mMat) for the RotTransPersW call and keeps the
 * TImode result in a1 (ours a0); sched1 puts a1 first only when its source dies at the call (toy test), here it
 * stays live for getPrimGetaZ; tried: copies/recomputed it, swapped-parameter wrapper, const attribute,
 * declaration-only callee, permuter 15 min on C3DCPrim::calcZ (10285 candidates) and on getPrimGetaZ (same cause),
 * no gain */
#if 0
FIX4 C3DStripBase::calcZ() {
    G_STRIP* it;
    FMATRIX mMat;
    u_long128 v;
    INT iz;
    if (m_vVERT.size() == 0) {
        return 0;
    }
    it = m_vVERT.begin();

    if (m_pObj) {
        m_pObj->Crd(it->Bone).getRotTransPersMatrix(mMat);
    } else if (m_pCrd) {
        m_pCrd->getRotTransPersMatrix(mMat);
    } else {
        mMat = CWorldEnv::getWorldScreenMatrix();
    }

    v = RotTransPersW(mMat, it->P);

    if (_getW(v) < 0.0f) {
        iz = 0xFFFF280;
    } else {
        iz = _getZ(v);
        if (iz < 0) iz = 0;
    }

    if (m_fGeta != 0.0f) {
        FMATRIX* pWMat;
        if (m_pObj) {
            pWMat = &m_pObj->Crd(it->Bone).m_World;
        } else {
            pWMat = m_pCrd ? &m_pCrd->m_World : NULL;
        }
        m_fGetaZ = getPrimGetaZ(iz, it->P, pWMat, m_fGeta);
    } else {
        m_fGetaZ = 0.0f;
    }

    return iz;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__12C3DStripBase);
#endif
void C3DStripBase::makePkt() {
    QWdata* pQw;
    QWdata* pQwTop;
    QWdata* pQwPkt;
    if (m_vVERT.size() == 0) {
        return;
    }
    pQwTop = (QWdata*)CPackMan::pkGetCurrentPointer();
    pQw = (QWdata*)CPackMan::pkOpenGifTag(_gifTag(
        (((u_long)m_nType | ((u_long)isShading() << 3) | ((u_long)isSemiTrans() << 6) | (u_long)0x200) << 47) |
            0x2000400000008000UL,
        0x41));
    _setGeta(m_fGetaZ);
    pQwPkt = pQw;
    pQw = makePktSub(pQwPkt);
    if (pQwPkt != pQw) {
        CPackMan::pkCloseGifTag(pQw);
    } else {
        CPackMan::pkSetCurrentPointer(pQwTop);
    }
}
/* Parked: registers only - the original loads a1 (it->P) before a0 (mMat) for the RotTransPersW call and keeps the
 * TImode result in a1 (ours a0); sched1 puts a1 first only when its source dies at the call (toy test), here it
 * stays live for getPrimGetaZ; tried: copies/recomputed it, swapped-parameter wrapper, const attribute,
 * declaration-only callee, permuter 15 min on C3DCPrim::calcZ (10285 candidates) and on getPrimGetaZ (same cause),
 * no gain */
#if 0
FIX4 C3DTexStripBase::calcZ() {
    G_STRIP* it;
    FMATRIX mMat;
    u_long128 v;
    INT iz;
    if (m_vVERT.size() == 0) {
        return 0;
    }
    it = m_vVERT.begin();

    if (m_pObj) {
        m_pObj->Crd(it->Bone).getRotTransPersMatrix(mMat);
    } else if (m_pCrd) {
        m_pCrd->getRotTransPersMatrix(mMat);
    } else {
        mMat = CWorldEnv::getWorldScreenMatrix();
    }

    v = RotTransPersW(mMat, it->P);

    if (_getW(v) < 0.0f) {
        iz = 0xFFFF280;
    } else {
        iz = _getZ(v);
        if (iz < 0) iz = 0;
    }

    if (m_fGeta != 0.0f) {
        FMATRIX* pWMat;
        if (m_pObj) {
            pWMat = &m_pObj->Crd(it->Bone).m_World;
        } else {
            pWMat = m_pCrd ? &m_pCrd->m_World : NULL;
        }
        m_fGetaZ = getPrimGetaZ(iz, it->P, pWMat, m_fGeta);
    } else {
        m_fGetaZ = 0.0f;
    }

    return iz;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__15C3DTexStripBase);
#endif
void C3DTexStripBase::makePkt() {
    QWdata* pQw;
    QWdata* pQwTop;
    QWdata* pQwPkt;
    ULONGLONG* p64;
    ULONGLONG tex;
    if (m_vVERT.size() == 0) {
        return;
    }
    tex = setTexture();
    pQwTop = (QWdata*)CPackMan::pkGetCurrentPointer();
    p64 = (ULONGLONG*)CPackMan::pkOpenGifTagManual(_gifTag(0x1400000000000001UL, 0x7));
    p64[0] = tex;
    CPackMan::pkCloseGifTagManual((UINT*)&p64[1]);
    pQw = (QWdata*)CPackMan::pkOpenGifTag(_gifTag(
        (((u_long)m_nType | ((u_long)isShading() << 3) | ((u_long)1 << 4) | ((u_long)isSemiTrans() << 6) |
          (u_long)0x200)
         << 47) |
            0x3000400000008000UL,
        0x412));
    _setGeta(m_fGetaZ);
    pQwPkt = pQw;
    pQw = makePktSub(pQwPkt);
    if (pQwPkt != pQw) {
        CPackMan::pkCloseGifTag(pQw);
    } else {
        CPackMan::pkSetCurrentPointer(pQwTop);
    }
}
QWdata* C3DLineTStrip::makePktSub(QWdata* pQw) {
    G_STRIP* it = m_vVERT.begin();
    G_STRIP* end = m_vVERT.end();
    _setRotTransPersMatrix_VF04_07(m_pCrd);
    return _makePkt_TexLineStrip(pQw, it, end);
}
QWdata* C3DLineTStripCoord::makePktSub(QWdata* pQw) {
    G_STRIP* it = m_vVERT.begin();
    G_STRIP* end = m_vVERT.end();
    return _makePkt_TexLineStrip(pQw, it, end, &m_pObj->Crd(0));
}
QWdata* C3DPolyTStrip::makePktSub(QWdata* pQw) {
    G_STRIP* it = m_vVERT.begin();
    G_STRIP* end = m_vVERT.end();
    _setRotTransPersMatrix_VF04_07(m_pCrd);
    return _makePkt_TexTriangleStrip(pQw, it, end, isBackClip());
}
QWdata* C3DPolyTStripCoord::makePktSub(QWdata* pQw) {
    G_STRIP* it = m_vVERT.begin();
    G_STRIP* end = m_vVERT.end();
    return _makePkt_TexTriangleStrip(pQw, it, end, &m_pObj->Crd(0), isBackClip());
}
QWdata* C3DPolyTFan::makePktSub(QWdata* pQw) {
    G_STRIP* it = m_vVERT.begin();
    G_STRIP* end = m_vVERT.end();
    _setRotTransPersMatrix_VF04_07(m_pCrd);
    return _makePkt_TexTriangleFan(pQw, it, end, isBackClip());
}
QWdata* C3DPolyTFanCoord::makePktSub(QWdata* pQw) {
    G_STRIP* it = m_vVERT.begin();
    G_STRIP* end = m_vVERT.end();
    return _makePkt_TexTriangleFan(pQw, it, end, &m_pObj->Crd(0), isBackClip());
}
QWdata* C3DLineStrip::makePktSub(QWdata* pQw) {
    G_STRIP* it = m_vVERT.begin();
    G_STRIP* end = m_vVERT.end();
    _setRotTransPersMatrix_VF04_07(m_pCrd);
    return _makePkt_LineStrip(pQw, it, end);
}
QWdata* C3DLineStripCoord::makePktSub(QWdata* pQw) {
    G_STRIP* it = m_vVERT.begin();
    G_STRIP* end = m_vVERT.end();
    return _makePkt_LineStrip(pQw, it, end, &m_pObj->Crd(0));
}
QWdata* C3DPolyStrip::makePktSub(QWdata* pQw) {
    G_STRIP* it = m_vVERT.begin();
    G_STRIP* end = m_vVERT.end();
    _setRotTransPersMatrix_VF04_07(m_pCrd);
    return _makePkt_TriangleStrip(pQw, it, end, isBackClip());
}
QWdata* C3DPolyStripCoord::makePktSub(QWdata* pQw) {
    G_STRIP* it = m_vVERT.begin();
    G_STRIP* end = m_vVERT.end();
    return _makePkt_TriangleStrip(pQw, it, end, &m_pObj->Crd(0), isBackClip());
}
QWdata* C3DPolyFan::makePktSub(QWdata* pQw) {
    G_STRIP* it = m_vVERT.begin();
    G_STRIP* end = m_vVERT.end();
    _setRotTransPersMatrix_VF04_07(m_pCrd);
    return _makePkt_TriangleFan(pQw, it, end, isBackClip());
}
QWdata* C3DPolyFanCoord::makePktSub(QWdata* pQw) {
    G_STRIP* it = m_vVERT.begin();
    G_STRIP* end = m_vVERT.end();
    return _makePkt_TriangleFan(pQw, it, end, &m_pObj->Crd(0), isBackClip());
}
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
void C3DCPrim::_create(ULONGLONG iPrim, INT iVertexNum, CCoord* pCParent, CCoords* pOParent, INT iRep) {
    remove();
    m_PRIM = iPrim;
    m_pCrd = pCParent;
    m_pObj = pOParent;
    m_iRep = iRep;
    m_iMax = iVertexNum;
#line 4121
    m_V = (G_STRIP*)com_malloc_d(m_iMax * sizeof(G_STRIP), __FILE__ "(4121)", (UINT)-1);
    if (!m_V) AssertApp(__FILE__, __LINE__, "m_V", 0);
}
/* Parked: registers only - the original loads a1 (it->P) before a0 (mMat) for the RotTransPersW call and keeps the
 * TImode result in a1 (ours a0); sched1 puts a1 first only when its source dies at the call (toy test), here it
 * stays live for getPrimGetaZ; tried: copies/recomputed it, swapped-parameter wrapper, const attribute,
 * declaration-only callee, permuter 15 min on C3DCPrim::calcZ (10285 candidates) and on getPrimGetaZ (same cause),
 * no gain */
#if 0
FIX4 C3DCPrim::calcZ() {
    G_STRIP* it;
    FMATRIX mMat;
    u_long128 v;
    INT iz;
    if (m_iMax == 0) {
        return 0;
    }
    it = &m_V[m_iRep];

    if (m_pObj) {
        m_pObj->Crd(it->Bone).getRotTransPersMatrix(mMat);
    } else if (m_pCrd) {
        m_pCrd->getRotTransPersMatrix(mMat);
    } else {
        mMat = CWorldEnv::getWorldScreenMatrix();
    }

    v = RotTransPersW(mMat, it->P);

    if (_getW(v) < 0.0f) {
        iz = 0xFFFF280;
    } else {
        iz = _getZ(v);
        if (iz < 0) iz = 0;
    }

    if (m_fGeta != 0.0f) {
        FMATRIX* pWMat;
        if (m_pObj) {
            pWMat = &m_pObj->Crd(it->Bone).m_World;
        } else {
            pWMat = m_pCrd ? &m_pCrd->m_World : NULL;
        }
        m_fGetaZ = getPrimGetaZ(iz, it->P, pWMat, m_fGeta);
    } else {
        m_fGetaZ = 0.0f;
    }

    return m_iZ < 0 ? iz : m_iZ << 4;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__8C3DCPrim);
#endif
void C3DCPrim::makePkt() {
    QWdata* pQw;
    QWdata* pQwTop;
    QWdata* pQwPkt;
    ULONGLONG* p64;
    UINT iEnvChange;
    ULONGLONG lPrim;
    INT iIdx;
    static QWdata* (C3DCPrim::*draw_Tbl[2][8])(QWdata*) = {
        {&C3DCPrim::makePkt_None, &C3DCPrim::makePkt_Line0, &C3DCPrim::makePkt_LineStrip0,
         &C3DCPrim::makePkt_Triangle0, &C3DCPrim::makePkt_TriangleStrip0, &C3DCPrim::makePkt_TriangleFan0,
         &C3DCPrim::makePkt_None, &C3DCPrim::makePkt_None},
        {&C3DCPrim::makePkt_None, &C3DCPrim::makePkt_Line1, &C3DCPrim::makePkt_LineStrip1,
         &C3DCPrim::makePkt_Triangle1, &C3DCPrim::makePkt_TriangleStrip1, &C3DCPrim::makePkt_TriangleFan1,
         &C3DCPrim::makePkt_None, &C3DCPrim::makePkt_None},
    };
    if (m_iMax == 0) {
        return;
    }
    pQwTop = (QWdata*)CPackMan::pkGetCurrentPointer();

    iEnvChange = 0;
    if ((INT)m_TEST >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 1;
    }
    if ((long)m_CLAMP >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 2;
    }
    if ((long)m_SCISSOR >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 4;
    }
    if ((INT)m_ALPHA >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 8;
    }
    if ((INT)m_ZBUF >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 0x10;
    }
    if (iEnvChange) {
        pQw = (QWdata*)CPackMan::pkOpenGifTagManual(_gifTag((u_long)(iEnvChange >> 12) | 0x1000000000000000UL, 0xE));
        if (iEnvChange & 1) (pQw++)->ul128 = _AD(0x48, m_TEST);
        if (iEnvChange & 2) (pQw++)->ul128 = _AD(0x9, m_CLAMP);
        if (iEnvChange & 4) (pQw++)->ul128 = _AD(0x41, m_SCISSOR);
        if (iEnvChange & 8) (pQw++)->ul128 = _AD(0x43, ((u_long)m_ALPHA << 16) | m_ALPHA);
        if (iEnvChange & 0x10) {
            ULONGLONG ZMSK = (ULONGLONG)(m_ZBUF & 0xF0000000) << 2;
            ULONGLONG ZBUF = m_ZBUF & 0x0FFFFFFF;
            ULONGLONG x;
            if (!ZBUF) {
                ZBUF = CPackMan::getDefault_ZBUF_2() & 0xFFFFFFFF;
                x = ZBUF | ZMSK;
            } else {
                x = ZBUF | ZMSK;
            }
            (pQw++)->ul128 = _AD(0x4F, x);
        }
        CPackMan::pkCloseGifTagManual((UINT*)pQw);
    }

    {
        ULONGLONG x = m_PRIM & 0xCF;
        x |= 0x200;
        lPrim = ((u_long)isSemiTrans() << 6) | x;
    }
    if (m_PackReglist[m_PRIM & 7]) {
        p64 = (ULONGLONG*)CPackMan::pkOpenGifTagManual(_gifTag(0x1400000000000001UL, 0));
        p64[0] = lPrim;
        CPackMan::pkCloseGifTagManual((UINT*)&p64[1]);
        pQw = (QWdata*)CPackMan::pkOpenGifTag(_gifTag(0x2400000000008000UL, 0x41));
    } else {
        pQw = (QWdata*)CPackMan::pkOpenGifTag(_gifTag((lPrim << 47) | 0x2000400000008000UL, 0x41));
    }

    _setGeta(m_fGetaZ);

    iIdx = m_pObj != NULL;

    pQwPkt = pQw;
    pQw = (this->*draw_Tbl[iIdx][m_PRIM & 7])(pQwPkt);

    if (pQwPkt != pQw) {
        CPackMan::pkCloseGifTag(pQw);
        if (iEnvChange) {
            pQw = (QWdata*)CPackMan::pkOpenGifTagManual(
                _gifTag((u_long)((iEnvChange >> 12) | 0x8000) | 0x1000000000000000UL, 0xE));
            if (iEnvChange & 1) (pQw++)->ul128 = _AD(0x48, CPackMan::getDefault_TEST_2());
            if (iEnvChange & 2) (pQw++)->ul128 = _AD(0x9, CPackMan::getDefault_CLAMP_2());
            if (iEnvChange & 4) (pQw++)->ul128 = _AD(0x41, CPackMan::getDefault_SCISSOR_2());
            if (iEnvChange & 8) (pQw++)->ul128 = _AD(0x43, CPackMan::getDefault_ALPHA_2());
            if (iEnvChange & 0x10) (pQw++)->ul128 = _AD(0x4F, CPackMan::getDefault_ZBUF_2());
            CPackMan::pkCloseGifTagManual((UINT*)pQw);
        }
    } else {
        CPackMan::pkSetCurrentPointer(pQwTop);
    }
}
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
void C3DTPrim::_create(ULONGLONG iPrim, INT iVertexNum, CCoord* pCParent, CCoords* pOParent, INT iRep) {
    remove();
    m_PRIM = iPrim;
    m_pCrd = pCParent;
    m_pObj = pOParent;
    m_iRep = iRep;
    m_iMax = iVertexNum;
#line 4316
    m_V = (G_STRIP*)com_malloc_d(m_iMax * sizeof(G_STRIP), __FILE__ "(4316)", (UINT)-1);
    if (!m_V) AssertApp(__FILE__, __LINE__, "m_V", 0);
}
/* Parked: registers only - the original loads a1 (it->P) before a0 (mMat) for the RotTransPersW call and keeps the
 * TImode result in a1 (ours a0); sched1 puts a1 first only when its source dies at the call (toy test), here it
 * stays live for getPrimGetaZ; tried: copies/recomputed it, swapped-parameter wrapper, const attribute,
 * declaration-only callee, permuter 15 min on C3DCPrim::calcZ (10285 candidates) and on getPrimGetaZ (same cause),
 * no gain */
#if 0
FIX4 C3DTPrim::calcZ() {
    G_STRIP* it;
    FMATRIX mMat;
    u_long128 v;
    INT iz;
    if (m_iMax == 0) {
        return 0;
    }
    it = &m_V[m_iRep];

    if (m_pObj) {
        m_pObj->Crd(it->Bone).getRotTransPersMatrix(mMat);
    } else if (m_pCrd) {
        m_pCrd->getRotTransPersMatrix(mMat);
    } else {
        mMat = CWorldEnv::getWorldScreenMatrix();
    }

    v = RotTransPersW(mMat, it->P);

    if (_getW(v) < 0.0f) {
        iz = 0xFFFF280;
    } else {
        iz = _getZ(v);
        if (iz < 0) iz = 0;
    }

    if (m_fGeta != 0.0f) {
        FMATRIX* pWMat;
        if (m_pObj) {
            pWMat = &m_pObj->Crd(it->Bone).m_World;
        } else {
            pWMat = m_pCrd ? &m_pCrd->m_World : NULL;
        }
        m_fGetaZ = getPrimGetaZ(iz, it->P, pWMat, m_fGeta);
    } else {
        m_fGetaZ = 0.0f;
    }

    return m_iZ < 0 ? iz : m_iZ << 4;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Gprim", calcZ__8C3DTPrim);
#endif
void C3DTPrim::makePkt() {
    ULONGLONG lTex0;
    QWdata* pQw;
    QWdata* pQwTop;
    QWdata* pQwPkt;
    ULONGLONG* p64;
    UINT iEnvChange;
    ULONGLONG lPrim;
    INT iIdx;
    static QWdata* (C3DTPrim::*draw_Tbl[2][8])(QWdata*) = {
        {&C3DTPrim::makePkt_None, &C3DTPrim::makePkt_Line0, &C3DTPrim::makePkt_LineStrip0,
         &C3DTPrim::makePkt_Triangle0, &C3DTPrim::makePkt_TriangleStrip0, &C3DTPrim::makePkt_TriangleFan0,
         &C3DTPrim::makePkt_None, &C3DTPrim::makePkt_None},
        {&C3DTPrim::makePkt_None, &C3DTPrim::makePkt_Line1, &C3DTPrim::makePkt_LineStrip1,
         &C3DTPrim::makePkt_Triangle1, &C3DTPrim::makePkt_TriangleStrip1, &C3DTPrim::makePkt_TriangleFan1,
         &C3DTPrim::makePkt_None, &C3DTPrim::makePkt_None},
    };
    if (m_iMax == 0) {
        return;
    }
    lTex0 = setTexture();
    pQwTop = (QWdata*)CPackMan::pkGetCurrentPointer();

    iEnvChange = 0;
    if ((INT)m_TEST >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 1;
    }
    if ((long)m_CLAMP >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 2;
    }
    if ((long)m_SCISSOR >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 4;
    }
    if ((INT)m_ALPHA >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 8;
    }
    if ((INT)m_ZBUF >= 0) {
        iEnvChange += 0x1000;
        iEnvChange |= 0x10;
    }
    if (iEnvChange) {
        pQw = (QWdata*)CPackMan::pkOpenGifTagManual(_gifTag((u_long)(iEnvChange >> 12) | 0x1000000000000000UL, 0xE));
        if (iEnvChange & 1) (pQw++)->ul128 = _AD(0x48, m_TEST);
        if (iEnvChange & 2) (pQw++)->ul128 = _AD(0x9, m_CLAMP);
        if (iEnvChange & 4) (pQw++)->ul128 = _AD(0x41, m_SCISSOR);
        if (iEnvChange & 8) (pQw++)->ul128 = _AD(0x43, ((u_long)m_ALPHA << 16) | m_ALPHA);
        if (iEnvChange & 0x10) {
            ULONGLONG ZMSK = (ULONGLONG)(m_ZBUF & 0xF0000000) << 2;
            ULONGLONG ZBUF = m_ZBUF & 0x0FFFFFFF;
            ULONGLONG x;
            if (!ZBUF) {
                ZBUF = CPackMan::getDefault_ZBUF_2() & 0xFFFFFFFF;
                x = ZBUF | ZMSK;
            } else {
                x = ZBUF | ZMSK;
            }
            (pQw++)->ul128 = _AD(0x4F, x);
        }
        CPackMan::pkCloseGifTagManual((UINT*)pQw);
    }

    {
        ULONGLONG x = m_PRIM & 0xCF;
        x |= 0x210;
        lPrim = ((u_long)isSemiTrans() << 6) | x;
    }
    if (m_PackReglist[m_PRIM & 7]) {
        p64 = (ULONGLONG*)CPackMan::pkOpenGifTagManual(_gifTag(0x2400000000000001UL, 0x7));
        p64[0] = lTex0;
        p64[1] = lPrim;
        CPackMan::pkCloseGifTagManual((UINT*)&p64[2]);
        pQw = (QWdata*)CPackMan::pkOpenGifTag(_gifTag(0x3400000000008000UL, 0x412));
    } else {
        p64 = (ULONGLONG*)CPackMan::pkOpenGifTagManual(_gifTag(0x1400000000000001UL, 0x7));
        p64[0] = lTex0;
        CPackMan::pkCloseGifTagManual((UINT*)&p64[1]);
        pQw = (QWdata*)CPackMan::pkOpenGifTag(_gifTag((lPrim << 47) | 0x3000400000008000UL, 0x412));
    }

    _setGeta(m_fGetaZ);

    iIdx = m_pObj != NULL;

    pQwPkt = pQw;
    pQw = (this->*draw_Tbl[iIdx][m_PRIM & 7])(pQwPkt);

    if (pQwPkt != pQw) {
        CPackMan::pkCloseGifTag(pQw);
        if (iEnvChange) {
            pQw = (QWdata*)CPackMan::pkOpenGifTagManual(
                _gifTag((u_long)((iEnvChange >> 12) | 0x8000) | 0x1000000000000000UL, 0xE));
            if (iEnvChange & 1) (pQw++)->ul128 = _AD(0x48, CPackMan::getDefault_TEST_2());
            if (iEnvChange & 2) (pQw++)->ul128 = _AD(0x9, CPackMan::getDefault_CLAMP_2());
            if (iEnvChange & 4) (pQw++)->ul128 = _AD(0x41, CPackMan::getDefault_SCISSOR_2());
            if (iEnvChange & 8) (pQw++)->ul128 = _AD(0x43, CPackMan::getDefault_ALPHA_2());
            if (iEnvChange & 0x10) (pQw++)->ul128 = _AD(0x4F, CPackMan::getDefault_ZBUF_2());
            CPackMan::pkCloseGifTagManual((UINT*)pQw);
        }
    } else {
        CPackMan::pkSetCurrentPointer(pQwTop);
    }
}
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
// SOURCE DATA: .data .rodata
