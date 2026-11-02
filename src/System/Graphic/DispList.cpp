#define MODELDATA_H_TEXINFO /* MATCH: TexInfoStub (the original has its assert strings) */
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

#include "System/Coord.h"
#include "System/Graphic/ShadowModel.h"
#include "System/Graphic/WorldEnv.h"
#include "include_asm.h"

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

#include "System/Graphic/PackMan.h"


















static inline void _loadMatrixVf28(const FMATRIX& m) {
    asm volatile("lqc2 $vf28, 0x0(%0)\n"
                 "lqc2 $vf29, 0x10(%0)\n"
                 "lqc2 $vf30, 0x20(%0)\n"
                 "lqc2 $vf31, 0x30(%0)\n"
                 :
                 : "r"(&m));
}

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

/* libgraph.h */
#define SCE_GS_SET_ALPHA(a, b, c, d, fix) \
    ((u_long)(a) | ((u_long)(b) << 2) | ((u_long)(c) << 4) | ((u_long)(d) << 6) | ((u_long)(fix) << 32))

/* semi-transparent primitives never write Z: ZTST forced to GEQUAL unless already set (original name unknown) */
static inline void _fixTestReg(ULONGLONG& nTest) {
    if (nTest & 0x10000) return;
    nTest = (nTest & ~0x70000) | 0x30000;
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
CDrawable* CDispListBase::getHeadPrim(INT nTblIdx) {
    OTtag* pOT = getOTtagTbl();
    if (!pOT) return NULL;
    if ((UINT)nTblIdx >= getOTtagTblSize()) return NULL;
    pOT = &pOT[nTblIdx];
    return pOT->_pHeadPrim;
}
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
void CDispListSemiBase::_updateTestMode(const CDrawable* pInPrim) {
    ALPHA nAlphaMode;
    ULONGLONG nAlpha;
    ULONGLONG nTest;
    BOOL bWriteZ;
    INT c;
    INT nFixValue;
    ULONGLONG nZbuf;
    if (pInPrim) {
        nAlphaMode = pInPrim->m_nAlphaMode;
        m_nTestFlag = pInPrim->m_nTestFlag;
    } else {
        nAlphaMode._value = 0;
        m_nTestFlag = 0x5000d;
    }
    m_nAlphaMode = nAlphaMode;
    nTest = m_nTestFlag;
    _fixTestReg(nTest);
    bWriteZ = FALSE;
    c = 0;
    nFixValue = 0;
    if ((INT)m_nAlphaMode & 0x80) {
        c = 1;
    } else if ((INT)m_nAlphaMode & 0x40) {
        c = 2;
        nFixValue = nAlphaMode.getAlphaValue();
    }
    switch (nAlphaMode.alphaMode & 0xf) {
    case 1:
        nAlpha = SCE_GS_SET_ALPHA(0, 1, c, 1, nFixValue);
        break;
    case 2:
        nAlpha = SCE_GS_SET_ALPHA(0, 2, c, 1, nFixValue);
        break;
    case 3:
        nAlpha = SCE_GS_SET_ALPHA(2, 0, c, 1, nFixValue);
        break;
    case 4:
        nAlpha = SCE_GS_SET_ALPHA(1, 2, 0, 2, 0);
        break;
    default:
        nAlpha = SCE_GS_SET_ALPHA(0, 1, c, 1, nFixValue);
        bWriteZ = TRUE;
        break;
    }
    nZbuf = (ULONGLONG)(VRAM::_ZBUF_START / 32) | 0x31000000;
    if (!bWriteZ) nZbuf |= (ULONGLONG)1 << 32;
    CPackMan::setDefault_TEST_2(nTest);
    CPackMan::setDefault_ALPHA_2(nAlpha);
    CPackMan::setDefault_ZBUF_2(nZbuf);
    u_long128* qp = CPackMan::pkOpenGifTagManual(_pack(0x1000800000008003, 0xe));
    qp[0] = _pack(nTest, 0x48);
    qp[1] = _pack(nAlpha, 0x43);
    qp[2] = _pack(nZbuf, 0x4f);
    CPackMan::pkCloseGifTagManual((UINT*)(qp + 3));
}
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
UINT CDispListSemiSingleSort::draw(SEMI_PACKET_LIST* pList) {
    INT nIdx = 0;
    CModel::setSendCtrl(0xe01);
    _loadMatrixVf28(CWorldEnv::getWorldScreenMatrix());
    OTtag* pOT = getOTtagTbl();
#line 421
    ASSERT(pOT);
    UINT nMax = getOTtagTblSize();
    for (INT i = 0; i < nMax; i++, pOT++) {
        if (pOT->isEmpty()) continue;
        CDrawable* pPrim = pOT->_pHeadPrim;
        while (pPrim) {
            if (CPackMan::isOverFlow(pPrim->getPacketSize())) {
                pPrim = pPrim->m_pNext;
                continue;
            }
            pList[nIdx].pPkt = (void*)((UINT)CPackMan::pkGetCurrentPointer() & 0xfffffff);
            pList[nIdx].sLayerInfo = pPrim->m_LayerInfo;
            pList[nIdx].nZ = pPrim->m_nZ;
            CPackMan::pkCnt(0);
            CPackMan::pkAddCode(0x13000000);
            CPackMan::pkOpenDirectCode(0);
            if (pPrim->m_LayerInfo._nID & 0x10) {
                pList[nIdx].pMProg = ((CSemiModel*)pPrim)->getParentModel()->getMicroProg();
            } else {
                pList[nIdx].pMProg = NULL;
                CGTexPrim::clearTexCache();
                CGTexPrim2D::clearTexCache();
                _updateTestMode(pPrim);
            }
            nIdx++;
            pPrim = _draw(*pPrim);
            CPackMan::pkCloseDirectCode();
            CPackMan::pkRet(0);
            CPackMan::pkAddCode(0x13000000);
            CPackMan::pkTerminate();
        }
    }
    CModel::resetWorldScreenMatrix();
    CModelMan::clearTexCache();
    CModel::resetMicroProgram();
    CModel::resetSendCtrl(-1);
    return nIdx;
}
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
void CDispListWithoutZ::_updateAlphaMode(ALPHA inAlpha) {
    ULONGLONG nAlpha;
    INT c;
    INT nFixValue;
    m_nAlphaMode = inAlpha;
    c = 0;
    nFixValue = 0;
    if ((INT)m_nAlphaMode & 0x80) {
        c = 1;
    } else if ((INT)m_nAlphaMode & 0x40) {
        c = 2;
        nFixValue = inAlpha.getAlphaValue();
    }
    switch (inAlpha.alphaMode & 0xf) {
    case 2:
        nAlpha = SCE_GS_SET_ALPHA(0, 2, c, 1, nFixValue);
        break;
    case 3:
        nAlpha = SCE_GS_SET_ALPHA(2, 0, c, 1, nFixValue);
        break;
    case 4:
        nAlpha = SCE_GS_SET_ALPHA(1, 2, 0, 2, 0);
        break;
    default:
        nAlpha = SCE_GS_SET_ALPHA(0, 1, c, 1, nFixValue);
        break;
    }
    CPackMan::setDefault_ALPHA_2(nAlpha);
    u_long128* qp = CPackMan::pkOpenGifTagManual(_pack(0x1000800000008001, 0xe));
    qp[0] = _pack(nAlpha, 0x43);
    CPackMan::pkCloseGifTagManual((UINT*)(qp + 1));
}
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
        HANDLE hHnd = getCShadowModelCoordsHandle(pSdw->getParentModel());
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
/* Parked: 62 of 291 lines, 4 bytes long - logic matches (CShadowModel::getSize inline). Left: the original does
 * not strength-reduce the draw loop over pBuf[j] (li 12 + mult every iteration), which frees an s-register: it keeps
 * i in s6 and spills hHnd, ours spills i. Tried: hHnd as a function-level local or read via pBuf->hHnd, pSdwBuf[i++],
 * j++ in both paths (for/while, 74), if (pSdw) {...} j++ (62); goto/while forms earlier (2026-10-19). The draft
 * declares CShadowMapModel (ref: lost+found.h, owner unknown) and Math3D::VectorMaxMinXYZ itself. */
#if 0
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
class CShadowMapModel : public CFadeModel {
    HANDLE m_hHnd;
public:
    virtual void changeLayerExt(BOOL bOn, INT nDepth);
    virtual FIX4 calcZ();
    void setHandle(HANDLE hHnd) { m_hHnd = hHnd; }
    HANDLE getHandle() { return m_hHnd; }
};
namespace Math3D { void VectorMaxMinXYZ(FVECTOR& inoutMax, FVECTOR& inoutMin, const FVECTOR& inMax, const FVECTOR& inMin); }
void CDispListShadowDraw::_drawOT(OTtag& inOT) {
    CDispListBase* pListSmp = CModelMan::getDispList(8);
    if (!pListSmp) return;
    if (!pListSmp->getEntryCount()) return;
    CShadowMapModel* pSMModelFirst = (CShadowMapModel*)pListSmp->getHeadPrim(0);
    if (!pSMModelFirst) return;
    CDrawable* pPrim = inOT._pHeadPrim;
    INT nEntryNum = getEntryCount();
    SHADOWMODELBUF* pSdwBuf = (SHADOWMODELBUF*)com_malloc_d(nEntryNum * sizeof(SHADOWMODELBUF), __FILE__ "(895)", RET_ADDR());
#line 896
    ASSERT(pSdwBuf);
    BOOL bFirst = TRUE;
    INT i = 0;
    while (pPrim) {
        pSdwBuf[i].pModel = (CShadowModel*)pPrim;
        i++;
        pPrim = pPrim->m_pNext;
    }
#line 907
    ASSERT(! (i != nEntryNum));
    sortCShadowModel(pSdwBuf, nEntryNum);
    for (i = 0; i < nEntryNum;) {
        INT j;
        SHADOWMODELBUF* pBuf = &pSdwBuf[i];
        HANDLE hHnd = pBuf->hHnd;
        INT nNum = pBuf->nNum;
        INT nDispNum = 0;
        FVECTOR v[2];
        FVECTOR vSMMax;
        FVECTOR vSMMin;
        vSMMax.set(-1.7014118e+38f, -1.7014118e+38f, -1.7014118e+38f);
        vSMMin.set(1.7014118e+38f, 1.7014118e+38f, 1.7014118e+38f);
        for (j = 0; j < nNum; j++) {
            if (pBuf[j].pModel) {
                nDispNum++;
                pBuf[j].pModel->getParentModel()->getWorldBBox(v[0], v[1]);
                Math3D::VectorMaxMinXYZ(vSMMax, vSMMin, v[0], v[1]);
            }
        }
        i += nNum;
        if (nDispNum <= 0) continue;
        CShadowModelMan::updateShadowMatrix(vSMMax, vSMMin, pBuf->pModel ? pBuf->pModel->getSize() : 0);
        nDispNum = 0;
        CShadowModelMan::drawBefore();
        for (j = 0; j < nNum; j++) {
            CShadowModel* pSdw = pBuf[j].pModel;
            if (!pSdw) continue;
            if (CPackMan::isOverFlow(pSdw->getPacketSize())) break;
            nDispNum++;
            _draw(*pSdw);
        }
        CShadowModelMan::drawAfter();
        if (nDispNum <= 0) continue;
        CShadowMapModel* pSMModel = pSMModelFirst;
        while (pSMModel) {
            CModel* pSMPModel = pSMModel->getParentModel();
            if (!pSMPModel) {
                pSMModel = (CShadowMapModel*)pSMModel->m_pNext;
                continue;
            }
            if (bFirst && !pSMPModel->isMap()) {
                pSMModel->setHandle(getCShadowModelCoordsHandle(pSMPModel));
            }
            if (pSMModel->getHandle() == hHnd && hHnd != 0) {
                pSMModel = (CShadowMapModel*)pSMModel->m_pNext;
                continue;
            }
            FVECTOR vMax;
            FVECTOR vMin;
            pSMPModel->getWorldBBox(vMax, vMin);
            if (!chackBBOX(vSMMax, vSMMin, vMax, vMin)) {
                pSMModel = (CShadowMapModel*)pSMModel->m_pNext;
                continue;
            }
            if (CPackMan::isOverFlow(pSMModel->getPacketSize())) break;
            pSMModel = (CShadowMapModel*)_draw(*pSMModel);
        }
        bFirst = FALSE;
    }
    if (pSdwBuf) com_free(pSdwBuf);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", _drawOT__19CDispListShadowDrawR5OTtag);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x70736944\n.word 0x7473694C\n.word 0x7070632E\n.word 0x35393828\n.word 0x00000029\n.word 0x00000000\n.word 0x77645370\n.word 0x00667542\n.word 0x69282021\n.word 0x203D2120\n.word 0x746E456E\n.word 0x754E7972\n.word 0x0000296D\n.word 0xFF000000\n.word 0x7F000000\n.word 0x00000000\n.text\n");
#endif
void CDispListShadowDraw::_endDraw() {
    CModelMan::clearTexCache();
    CGTexPrim::clearTexCache();
    CGTexPrim2D::clearTexCache();
    _endDirectMode();
}
void CDispListShadowMap::insert(CDrawable& inPrim, FIX4 inZ) {
    CModel* pModel = ((CShadowModel&)inPrim).getParentModel();
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
