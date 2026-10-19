#define MODELDATA_H_TEXINFO
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h"
#define MODELDATA_H_PATH "c:/KHCOM/MainApp/System/Graphic/modeldata.h"
#define MODEL_H_PATH "c:/KHCOM/MainApp/System/Graphic/Model.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include <string.h>
extern "C" void qsort(void* base, size_t num, size_t size, int (*cmp)(const void*, const void*));
#include "System/Graphic/ModelMan.h"
#include "System/VramMan.h"
#define TASK_H_NO_SLC_OVERFLOW_MSG
#include "System/Coord.h"
#include "System/Graphic/ShadowModel.h"
#include "include_asm.h"

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

class CShadowModel;
class CPackMan { /* System/Graphic/PackMan.h */
public:
    static void pkCnt(UINT n);
    static void pkAddCode(UINT nCode);
    static void pkOpenDirectCode(INT n);
    static void pkCloseDirectCode();
    static void changeLayer(INT nLayer);
    static void* pkGetCurrentPointer();
    static BOOL isOverFlow(UINT nSize);
    static void setDefault_TEST_2(ULONGLONG n);
    static void setDefault_ALPHA_2(ULONGLONG n);
    static void setDefault_ZBUF_2(ULONGLONG n);
    static u_long128* pkOpenGifTagManual(u_long128 tag);
    static void pkCloseGifTagManual(UINT* p);
};

/* clears memory in quadwords (through the uncached segment if asked), then bytes (original name unknown) */
static inline void _memclr(void* pDst, UINT nSize, BOOL bUncached) {
    UINT nQw = nSize >> 4;
    nSize -= nQw << 4;
    UINT i;
    if (nQw) {
        u_long128* p = (u_long128*)pDst;
        if (bUncached) p = (u_long128*)((UINT)pDst | 0x20000000);
        for (i = 0; i < nQw; i++) *p++ = 0;
        pDst = p;
    }
    BYTE* pb = (BYTE*)pDst;
    for (i = 0; i < nSize; i++) *pb++ = 0;
}

/* GS ALPHA register for a primitive's alpha mode (original name unknown) */
static inline ULONGLONG _getAlphaReg(const ALPHA& rMode, const ALPHA& alpha) {
    ULONGLONG nC = 0;
    ULONGLONG nFix = 0;
    if (rMode._value & 0x80) {
        nC = 1;
    } else if (rMode._value & 0x40) {
        nC = 2;
        nFix = alpha.alphaValue;
    }
    switch (alpha.alphaMode & 0xf) {
    case 2:
        return 0x48 | (nC << 4) | (nFix << 32);
    case 3:
        return 0x42 | (nC << 4) | (nFix << 32);
    case 4:
        return 0x89;
    default:
        return 0x44 | (nC << 4) | (nFix << 32);
    }
}

static inline u_long128 _pack(u_long lo, u_long hi) {
    u_long128 r;
    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(r) : "r"(lo), "r"(hi));
    return r;
}


/* The display-list classes, complete (System/Graphic/DispList.h only has what System/Graphic/ModelMan needs). */
struct OTtag {
    CDrawable* _pHeadPrim;
    CDrawable* _pTailPrim;
    BOOL isEmpty() const { return _pHeadPrim == NULL; }
    void addTail(CDrawable* pPrim) {
        CDrawable* pTail = _pTailPrim;
        _pTailPrim = pPrim;
        if (pTail) pTail->m_pNext = pPrim;
        pPrim->m_pNext = NULL;
        if (!_pHeadPrim) _pHeadPrim = pPrim;
    }
};

struct SEMI_PACKET_LIST;

class CDispListBase {
private:
    OTtag* m_pOTtagTbl;   /* 0x0 */
    UINT m_nOTtagTblSize; /* 0x4 */
    UINT m_iDebugFlags;   /* 0x8 */

public:
    CDispListBase() {
        m_pOTtagTbl = NULL;
        m_nOTtagTblSize = 0;
        m_iDebugFlags = 0;
    }
    virtual ~CDispListBase();
    virtual void insert(CDrawable& inPrim, FIX4 inZ) = 0;
    virtual void clear(BOOL bUncached);
    void draw(INT nStart, INT nSize);
    virtual UINT getEntryCount() const { return 0; }
    BOOL hasAnyPrim(UINT nTblIdx) {
        if (m_pOTtagTbl && nTblIdx < m_nOTtagTblSize) {
            OTtag& tag = m_pOTtagTbl[nTblIdx];
            return !tag.isEmpty();
        }
        return FALSE;
    }
    CDrawable* getHeadPrim(INT nTblIdx);

protected:
    virtual void _beginDraw() = 0;
    virtual void _drawOT(OTtag& inOT) = 0;
    virtual void _endDraw() = 0;
    void setupOTtagTbl(UINT nInSize);
    OTtag* getOTtagTbl() const { return m_pOTtagTbl; }
    INT getOTtagTblSize() const { return m_nOTtagTblSize; }
    void _startDirectMode();
    void _endDirectMode();
    G_LAYER checkCache(CDrawable& inPrim, G_LAYER nLayer);
    CDrawable* _draw(CDrawable& inPrim);
    CDrawable* _getNextDrawable(CDrawable& inPrim) { return inPrim.m_pNext; }
};

class CDispList : public CDispListBase {
protected:
    UINT m_nEntryCount; /* 0x10 */

public:
    CDispList() {
        setupOTtagTbl(26);
        clear(FALSE);
    }
    virtual void clear(BOOL bUncached) {
        CDispListBase::clear(bUncached);
        m_nEntryCount = 0;
    }
    virtual void insert(CDrawable& inPrim, FIX4 inZ);
    virtual UINT getEntryCount() const { return m_nEntryCount; }

protected:
    virtual void _beginDraw();
    virtual void _drawOT(OTtag& inOT);
    virtual void _endDraw();
};

class CDispListSemiBase : public CDispListBase {
private:
    ULONGLONG m_nTestFlag; /* 0x10 */
    ALPHA m_nAlphaMode;    /* 0x18 */

public:

protected:
    virtual void _beginDraw();
    virtual void _drawOT(OTtag& inOT);
    virtual void _endDraw();
    BOOL _checkTestMode(const CDrawable& inPrim) {
        if (inPrim.m_LayerInfo._nID & 0x10) return FALSE;
        BOOL bRes = FALSE;
        if (inPrim.m_nAlphaMode != m_nAlphaMode || inPrim.m_nTestFlag != m_nTestFlag) bRes = TRUE;
        return bRes;
    }
    void _updateTestMode(const CDrawable* pInPrim);
    void _resetTestMode() { _updateTestMode(NULL); }
};

class CDispListSemi : public CDispListSemiBase {
public:
    CDispListSemi() {
        setupOTtagTbl(0x800);
        clear(TRUE);
    }
    virtual void insert(CDrawable& inPrim, FIX4 inZ);
};

class CDispListSemiSingleSort : public CDispListSemiBase {
public:
    UINT draw(SEMI_PACKET_LIST* pList);
};

class CDispListWithoutZ : public CDispList {
private:
    ALPHA m_nAlphaMode; /* 0x14 */

public:
    virtual void insert(CDrawable& inPrim, FIX4 inZ);

protected:
    virtual void _beginDraw();
    virtual void _drawOT(OTtag& inOT);
    virtual void _endDraw();
    void _enableZTest(BOOL bEnable);
    void _updateAlphaMode(ALPHA inAlpha);
};

class CDispListBG : public CDispListWithoutZ {
public:

protected:
    virtual void _beginDraw();
    virtual void _endDraw();
};

class CDispListFG : public CDispListWithoutZ {
public:

protected:
    virtual void _beginDraw();
    virtual void _endDraw();
};

class CDispListShadowDraw : public CDispListBase {
protected:
    UINT m_nEntryCount; /* 0x10 */

public:

protected:
    virtual void _beginDraw();
    virtual void _drawOT(OTtag& inOT);
    virtual void _endDraw();

public:
    CDispListShadowDraw() {
        setupOTtagTbl(1);
        clear(FALSE);
    }
    virtual void insert(CDrawable& inPrim, FIX4 inZ) {
        getOTtagTbl()[0].addTail(&inPrim);
        inPrim.m_nZ = inZ;
        m_nEntryCount++;
    }
    virtual void clear(BOOL bUncached) {
        CDispListBase::clear(bUncached);
        m_nEntryCount = 0;
    }
    virtual UINT getEntryCount() const { return m_nEntryCount; }
};

class CDispListShadowMap : public CDispListBase {
protected:
    UINT m_nEntryCount; /* 0x10 */

public:

protected:
    virtual void _beginDraw();
    virtual void _drawOT(OTtag& inOT);
    virtual void _endDraw();

public:
    CDispListShadowMap() {
        setupOTtagTbl(1);
        clear(FALSE);
    }
    virtual void insert(CDrawable& inPrim, FIX4 inZ);
    virtual void clear(BOOL bUncached) {
        CDispListBase::clear(bUncached);
        m_nEntryCount = 0;
    }
    virtual UINT getEntryCount() const { return m_nEntryCount; }
};

CDispListBase::~CDispListBase() {
    if (m_pOTtagTbl) com_free(m_pOTtagTbl);
}
void CDispListBase::setupOTtagTbl(UINT nInSize) {
    if (m_pOTtagTbl) com_free(m_pOTtagTbl);
    m_pOTtagTbl = NULL;
    if (nInSize) {
#line 40
        m_pOTtagTbl = (OTtag*)com_malloc_d(nInSize * sizeof(OTtag), __FILE__ "(40)", (UINT)-1);
    }
    m_nOTtagTblSize = nInSize;
}
void CDispListBase::clear(BOOL bUncached) {
#line 49
    ASSERT(m_pOTtagTbl);
    _memclr(m_pOTtagTbl, m_nOTtagTblSize * sizeof(OTtag), bUncached);
}
void CDispListBase::draw(INT nStart, INT nSize) {
#line 74
    ASSERT(m_pOTtagTbl);
    _beginDraw();
    if (nStart < 0) nStart = 0;
    INT nEnd = (nSize < 0) ? m_nOTtagTblSize : nStart + nSize;
    OTtag* pTag = &m_pOTtagTbl[nStart];
    for (INT i = nStart; i < nEnd; i++, pTag++) {
        if (pTag->_pHeadPrim) _drawOT(*pTag);
    }
    _endDraw();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", getHeadPrim__13CDispListBasei);
void CDispListBase::_startDirectMode() {
    CPackMan::pkCnt(0);
    CPackMan::pkAddCode(0x13000000);
    CPackMan::pkOpenDirectCode(0);
}
void CDispListBase::_endDirectMode() { CPackMan::pkCloseDirectCode(); }
G_LAYER CDispListBase::checkCache(CDrawable& inPrim, G_LAYER nLayer) {
    G_LAYER nPrimLayer = inPrim.m_LayerInfo;
    if ((nPrimLayer._nInfo ^ nLayer._nInfo) & 0x10) {
        if (nPrimLayer._nInfo & 0x10) {
            CModelMan::clearTexCache();
        } else {
            CGTexPrim2D::clearTexCache();
        }
        CModelMan::setLayerInfo(nPrimLayer);
        nLayer = nPrimLayer;
    }
    return nLayer;
}
CDrawable* CDispListBase::_draw(CDrawable& inPrim) {
    UINT nStart = (UINT)CPackMan::pkGetCurrentPointer();
    inPrim.makePkt();
    if (dbgButtonDown(USR_SYSTEM, 0x10, 1)) m_iDebugFlags &= ~1;
    if (!(m_iDebugFlags & 1)) {
        UINT nUsed = ((UINT)CPackMan::pkGetCurrentPointer() - nStart) >> 4;
        INT nSize = inPrim.getPacketSize();
        if (nSize < (INT)nUsed) {
            TRACE("$$$$$$$$$$$$$ makePkt()\x8a\xd6\x90\x94\x82\xcc\x95\xd4\x82\xb7\x83T\x83" "C\x83Y\x82\xaa\x8f\xad\x82\xc8\x82\xa2\x82\xc5\x82\xb7!!!->(%d) \x8e\xc0\x8d\xdb\x82\xcc\x8eg\x97p\x83T\x83" "C\x83Y(%d) $$$$$$$$$$$$$\n",
                  nSize, nUsed);
            inPrim.getPacketSize();
            m_iDebugFlags |= 1;
        }
    }
    return _getNextDrawable(inPrim);
}
void CDispList::insert(CDrawable& inPrim, FIX4 inZ) {
    INT nIdx = inPrim.m_LayerInfo._nDepth;
    if (nIdx >= getOTtagTblSize()) {
        nIdx = getOTtagTblSize() - 1;
    } else if (nIdx < 0) {
        nIdx = 0;
    }
    OTtag& tag = getOTtagTbl()[nIdx];
    tag.addTail(&inPrim);
    inPrim.m_nZ = inZ;
    m_nEntryCount++;
}
void CDispList::_beginDraw() {}
void CDispList::_drawOT(OTtag& inOT) {
    CDrawable* pPrim = inOT._pHeadPrim;
    G_LAYER nLayer = CModelMan::getLayerInfo();
    while (pPrim) {
        if (CPackMan::isOverFlow(pPrim->getPacketSize())) {
            pPrim = pPrim->m_pNext;
        } else {
            nLayer = checkCache(*pPrim, nLayer);
            pPrim = _draw(*pPrim);
        }
    }
}
void CDispList::_endDraw() {}
void CDispListSemiBase::_beginDraw() {
    _startDirectMode();
    _updateTestMode(NULL);
}
void CDispListSemiBase::_drawOT(OTtag& inOT) {
    CDrawable* pPrim = inOT._pHeadPrim;
    G_LAYER nLayer = CModelMan::getLayerInfo();
    while (pPrim) {
        if (CPackMan::isOverFlow(pPrim->getPacketSize())) {
            pPrim = pPrim->m_pNext;
        } else {
            if (_checkTestMode(*pPrim)) _updateTestMode(pPrim);
            nLayer = checkCache(*pPrim, nLayer);
            pPrim = _draw(*pPrim);
        }
    }
}
void CDispListSemiBase::_endDraw() { _endDirectMode(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", _updateTestMode__17CDispListSemiBasePC9CDrawable);
void CDispListSemi::insert(CDrawable& inPrim, FIX4 inZ) {
    if (inZ < 0) {
        inZ = 0x7fffff0;
    } else {
        inZ &= 0x7fffff0;
    }
    inZ >>= 17;
    OTtag& tag = getOTtagTbl()[inZ];
    tag.addTail(&inPrim);
    inPrim.m_nZ = inZ;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", draw__23CDispListSemiSingleSortP16SEMI_PACKET_LIST);
__asm__(".rdata\n.align 3\n.word 0x00544F70\n.word 0x00000000\n.text\n");
void CDispListWithoutZ::insert(CDrawable& inPrim, FIX4 inZ) { CDispList::insert(inPrim, inZ); }
void CDispListWithoutZ::_beginDraw() {
    _startDirectMode();
    _enableZTest(FALSE);
    _updateAlphaMode((ALPHA)0);
}
void CDispListWithoutZ::_drawOT(OTtag& inOT) {
    CDrawable* pPrim = inOT._pHeadPrim;
    G_LAYER nLayer = CModelMan::getLayerInfo();
    while (pPrim) {
        if (CPackMan::isOverFlow(pPrim->getPacketSize())) {
            TRACE("CDispListWithoutZ : \x83p\x83P\x83" "b\x83g\x83I\x81[\x83o\x81[\x82\xcc\x82\xbd\x82\xdf\x95`\x89\xe6\x83X\x83L\x83" "b\x83v [%d]\n",
                  pPrim->getPacketSize());
            pPrim = pPrim->m_pNext;
        } else {
            ALPHA a = pPrim->m_nAlphaMode;
            if (!((BYTE)pPrim->m_LayerInfo._nID & 0x10) && a != m_nAlphaMode) {
                _updateAlphaMode(pPrim->m_nAlphaMode);
            }
            nLayer = checkCache(*pPrim, nLayer);
            pPrim = _draw(*pPrim);
        }
    }
}
void CDispListWithoutZ::_endDraw() {
    _enableZTest(TRUE);
    _endDirectMode();
}
void CDispListWithoutZ::_enableZTest(BOOL bEnable) {
    ULONGLONG nTest = bEnable ? 0x5000d : 0x3000d;
    ULONGLONG nZbuf = (ULONGLONG)(VRAM::_ZBUF_START / 32) | 0x31000000;
    if (!bEnable) nZbuf |= (ULONGLONG)1 << 32;
    CPackMan::setDefault_TEST_2(nTest);
    CPackMan::setDefault_ZBUF_2(nZbuf);
    u_long128* p = CPackMan::pkOpenGifTagManual(_pack(0x1000800000008002, 0xe));
    p[0] = _pack(nTest, 0x48);
    p[1] = _pack(nZbuf, 0x4f);
    CPackMan::pkCloseGifTagManual((UINT*)(p + 2));
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", _updateAlphaMode__17CDispListWithoutZG5ALPHA);
void CDispListBG::_beginDraw() {
    CPackMan::changeLayer(1);
    _startDirectMode();
    CModelMan::clearTexCache();
    CGTexPrim::clearTexCache();
    CGTexPrim2D::clearTexCache();
    CModel::resetWorldScreenMatrix();
    CModel::resetMicroProgram();
    CPackMan::pkCloseDirectCode();
    CModelMan::setupDrawMode(0);
    CPackMan::pkOpenDirectCode(0);
    _updateAlphaMode((ALPHA)0);
}
void CDispListBG::_endDraw() {
    _endDirectMode();
    CPackMan::changeLayer(0);
}
void CDispListFG::_beginDraw() {
    CPackMan::changeLayer(2);
    _startDirectMode();
    _enableZTest(FALSE);
    _updateAlphaMode((ALPHA)0);
}
void CDispListFG::_endDraw() {
    _enableZTest(TRUE);
    _endDirectMode();
    CPackMan::changeLayer(0);
}
void CDispListShadowDraw::_beginDraw() { _startDirectMode(); }
/* PARTIAL: only what this file needs (System/Graphic/SemiModel.h, FadeModel.h, ShadowModel.h) */
class CShadowModel : public CDrawable {
protected:
    CModel* m_pModel; /* 0x18 CSemiModel::m_pModel */
public:
    CModel* getModel() { return m_pModel; }
};
/* VU0 helpers (original names unknown) */
static inline void _vecAdd(FVECTOR& vOut, const FVECTOR& v0, const FVECTOR& v1) {
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vadd.xyz $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&vOut), "r"(&v0), "r"(&v1)
                 : "memory");
}
static inline void _vecSub(FVECTOR& vOut, const FVECTOR& v0, const FVECTOR& v1) {
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vsub.xyz $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&vOut), "r"(&v0), "r"(&v1)
                 : "memory");
}
static inline void _vecScale(FVECTOR& vOut, const FVECTOR& v0, FLOAT f) {
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "mfc1 $3, %2\n"
                 "qmtc2.ni $3, $vf5\n"
                 "vmulx.xyz $vf4, $vf4, $vf5x\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&vOut), "r"(&v0), "f"(f)
                 : "$3", "memory");
}
/* the original keeps the integer copy in s5/s6 and the result in f0/f1: a macro over explicit register locals */
#define _LENGTH_SQ(ret, iTmp, v0)                                    \
    asm volatile("lqc2 $vf4, 0x0(%1)\n"                              \
                 "vaddy.x $vf6, $vf0, $vf4y\n"                       \
                 "vaddz.x $vf7, $vf0, $vf4z\n"                       \
                 "vmulax.x $ACC, $vf4, $vf4x\n"                      \
                 "vmadday.x $ACC, $vf6, $vf4y\n"                     \
                 "vmaddz.x $vf5, $vf7, $vf4z\n"                      \
                 "qmfc2.ni %0, $vf5\n"                               \
                 : "=r"(iTmp)                                        \
                 : "r"(&(v0)));                                      \
    asm volatile("mtc1 %1, %0\n"                                     \
                 "nop\n"                                             \
                 : "=f"(ret)                                         \
                 : "r"(iTmp))
struct SHADOWMODELBUF {
    CShadowModel* pModel;
    HANDLE hHnd;
    INT nNum;
};
static int ShadowModel_QSortComp(const void* ax, const void* ay) {
    return ((SHADOWMODELBUF*)ax)->hHnd - ((SHADOWMODELBUF*)ay)->hHnd;
}
static HANDLE getCShadowModelCoordsHandle(CModel* pModel) {
    HANDLE hHnd = -1;
    if (pModel) {
        CCoords* pCrds = pModel->getCoords();
        if (pCrds) {
            CCoord* pCrd = pCrds->Crd();
            if (pCrd) {
                if (pCrd->getParent()) {
                    hHnd = pCrd->getParent()->m_hHandle;
                } else {
                    hHnd = pCrds->getSysHandle();
                }
            }
        }
    }
    return hHnd;
}
static void sortCShadowModel(SHADOWMODELBUF* pBuf, INT nEntryNum) {
    INT i;
    INT j;
    for (i = 0; i < nEntryNum; i++) {
        CShadowModel* pSdw = pBuf[i].pModel;
        HANDLE hHnd = getCShadowModelCoordsHandle(pSdw->getModel());
        if (hHnd != -1) {
            pBuf[i].hHnd = hHnd;
        } else {
            pBuf[i].pModel = NULL;
            pBuf[i].hHnd = 0;
        }
    }
    if (nEntryNum > 1) qsort(pBuf, nEntryNum, sizeof(SHADOWMODELBUF), ShadowModel_QSortComp);
    for (i = 0; i < nEntryNum; i++) {
        HANDLE hHnd = pBuf[i].hHnd;
        if (hHnd == 0) {
            pBuf[i].nNum = 1;
        } else {
            INT nNum = 1;
            for (j = i + 1; j < nEntryNum; j++) {
                if (hHnd != pBuf[j].hHnd) break;
                nNum++;
            }
            pBuf[i].nNum = nNum;
        }
    }
}
static BOOL chackBBOX(const FVECTOR& vMax, const FVECTOR& vMin, const FVECTOR& vSMMax, const FVECTOR& vSMMin) {
    if (vMax.y < vSMMin.y) return FALSE;
    if (vMin.y > vSMMax.y) return FALSE;
    if (vMax.x < vSMMin.x) return FALSE;
    if (vMin.x > vSMMax.x) return FALSE;
    if (vMax.z < vSMMin.z) return FALSE;
    if (vMin.z > vSMMax.z) return FALSE;
    return TRUE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", _drawOT__19CDispListShadowDrawR5OTtag);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x70736944\n.word 0x7473694C\n.word 0x7070632E\n.word 0x35393828\n.word 0x00000029\n.word 0x00000000\n.word 0x77645370\n.word 0x00667542\n.word 0x69282021\n.word 0x203D2120\n.word 0x746E456E\n.word 0x754E7972\n.word 0x0000296D\n.word 0xFF000000\n.word 0x7F000000\n.word 0x00000000\n.text\n");
void CDispListShadowDraw::_endDraw() {
    CModelMan::clearTexCache();
    CGTexPrim::clearTexCache();
    CGTexPrim2D::clearTexCache();
    _endDirectMode();
}
void CDispListShadowMap::insert(CDrawable& inPrim, FIX4 inZ) {
    CModel* pModel = ((CShadowModel&)inPrim).getModel();
    if (pModel) {
        FVECTOR v[2];
        FVECTOR& vCamPos = CShadowModelMan::m_vCamPos;
        FLOAT fClip = CShadowModelMan::m_fClipDistSq;
        pModel->getWorldBBox(v[0], v[1]);
        _vecAdd(v[1], v[0], v[1]);
        _vecScale(v[1], v[1], 0.5f);
        _vecSub(v[0], v[0], v[1]);
        register INT iTmp asm("$21");
        register FLOAT ret asm("$f0");
        _LENGTH_SQ(ret, iTmp, v[0]);
        FLOAT fLen2 = ret + fClip;
        _vecSub(v[0], vCamPos, v[1]);
        register INT iTmp2 asm("$22");
        register FLOAT ret2 asm("$f1");
        _LENGTH_SQ(ret2, iTmp2, v[0]);
        if (ret2 < fLen2) {
            OTtag& tag = getOTtagTbl()[0];
            tag.addTail(&inPrim);
            inPrim.m_nZ = inZ;
            m_nEntryCount++;
        }
    }
}
void CDispListShadowMap::_beginDraw() { _startDirectMode(); }
void CDispListShadowMap::_drawOT(OTtag& inOT) {
    CDrawable* pPrim = inOT._pHeadPrim;
    G_LAYER nNowLayer = CModelMan::getLayerInfo();
    while (pPrim) {
        if (CPackMan::isOverFlow(pPrim->getPacketSize())) {
            pPrim = pPrim->m_pNext;
        } else {
            nNowLayer = checkCache(*pPrim, nNowLayer);
            pPrim = _draw(*pPrim);
        }
    }
}
void CDispListShadowMap::_endDraw() {
    CModelMan::clearTexCache();
    CGTexPrim::clearTexCache();
    CGTexPrim2D::clearTexCache();
    _endDirectMode();
}
// SOURCE DATA: .rodata
