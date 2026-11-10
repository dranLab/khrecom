#define MODELDATA_H_TEXINFO /* MATCH: TexInfoStub (the original has its assert strings) */
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define MODELDATA_H_PATH "c:/KHCOM/MainApp/System/Graphic/modeldata.h" /* MATCH: this unit names modeldata.h so (header default "System/Graphic/modeldata.h") */
#define MODEL_H_PATH "c:/KHCOM/MainApp/System/Graphic/Model.h" /* MATCH: this unit names Model.h so (header default "System/Graphic/Model.h") */
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

#include "System/Graphic/DispList.h"
class CDispListSemiSingleSort : public CDispListSemiBase { /* PARTIAL: stand-in for System/Graphic/ModelMan's class (defined in ModelMan.cpp, not in a header yet): only draw, defined here */
public:
    UINT draw(SEMI_PACKET_LIST* pList);
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
    u_long128* qp = CPackMan::pkOpenGifTagManual(_makeQW(0x1000800000008003, 0xe));
    qp[0] = _makeQW(nTest, 0x48);
    qp[1] = _makeQW(nAlpha, 0x43);
    qp[2] = _makeQW(nZbuf, 0x4f);
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
    vu0LoadMatrixVF28(&CWorldEnv::getWorldScreenMatrix());
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
    u_long128* p = CPackMan::pkOpenGifTagManual(_makeQW(0x1000800000008002, 0xe));
    p[0] = _makeQW(nTest, 0x48);
    p[1] = _makeQW(nZbuf, 0x4f);
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
    u_long128* qp = CPackMan::pkOpenGifTagManual(_makeQW(0x1000800000008001, 0xe));
    qp[0] = _makeQW(nAlpha, 0x43);
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
/* Parked: 59 of 289 lines (2026-11-10): two independent differences. (1) The draw loop over pBuf[j] (line 940): the
 * original keeps `li 12; mult` in the loop, ours strength-reduces it to a pointer. (2) Allocation: ours spills i and
 * keeps hHnd in s8, the original keeps i in s6 and spills hHnd (s6/s7/s8 renamed throughout). (2) does not follow from
 * (1): with the mult kept (draw loop padded to 148 insns with empty asms) i stays spilled. Global-alloc priorities
 * (-dl): hHnd 8 refs / live 106 = 2264, i 15 refs / live 262 = 1717; one more i use (an `asm("" : : "r"(i))` after
 * sortCShadowModel) plus 40 memory-clobber asms in the draw loop gave the original's allocation (53 lines, the rest
 * the giv), either alone did not. For (1) (-dL): the `li 12` movable is "not desirable" in the draw loop from about
 * 64 real insns (ours 28), but then loop pass 1 of the outer loop combines it with the outer `li 12` of
 * `&pSdwBuf[i]` (savings 2, life 2: hoisted while the outer loop has fewer than about 252 insns, ours 163) and pass 2
 * reduces the giv anyway; only above that (120 pads) the mult stays. So the original's loops held far more RTL at
 * loop and flow time (tens of insns in the draw loop, ~90 in the outer one) that combine/reload deleted, or the
 * draw loop's 12 was not a movable at all (pseudo set twice); the source of either is unknown (lines 942-943,
 * 945-946, 948-950, 952, 954 have no note; 947 (break) and 953 have code-less notes, 953 at the j++: nDispNum++ after
 * _draw fits, codegen-neutral). Tried 2026-11-10 without change: nDispNum++ after _draw, braces on the break,
 * pSdwBuf[i] for nNum/hHnd, pSdwBuf + i, a dead FVECTOR set (survives: worse), a dead counter, j = nNum before break
 * (becomes a biv increment), goto loop (mult stays, layout and allocation differ: 75). Shape from the .mdebug block tree and the line markers (this draft
 * follows them): i/j/bFirst function-level, the outer loop's body block at depth 1 (while), the then-block of
 * `if (pBuf[j].pModel)` holds a local .mdebug omits (a `CShadowModel&` reproduces the block, a pointer local would be
 * listed), the map loop as nested ifs with one _getNextDrawable at the end (966/967/968 separate lines). Codegen-neutral
 * differences left: getHandle's inline block sits directly in the if (pSMPModel) body (a `HANDLE x = getHandle();`
 * local reproduces it), CDrawable::getNextDrawable is an inline at Drawable.h:398, CShadowMapModel's setHandle/getHandle
 * are ShadowModel.h:276/277, HeapMan's delete[] has one more block level. Tried: while/goto/two j++ (layout changes),
 * inline index helper, pointer local, j = 0 / nDispNum = 0 placement, earlier hHnd placement, pSdwBuf[i++]; the draft
 * declares CShadowMapModel and Math3D::VectorMaxMinXYZ itself. */
#if 0
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
    CDispListBase* pListSmp;
    CShadowMapModel* pSMModelFirst;
    CDrawable* pPrim;
    INT nEntryNum;
    SHADOWMODELBUF* pSdwBuf;
    INT i;
    INT j;
    BOOL bFirst;
    pListSmp = CModelMan::getDispList(8);
    if (!pListSmp) return;
    if (!pListSmp->getEntryCount()) return;
    pSMModelFirst = (CShadowMapModel*)pListSmp->getHeadPrim(0);
    if (!pSMModelFirst) return;
    pPrim = inOT._pHeadPrim;
    nEntryNum = getEntryCount();
    pSdwBuf = new KH_AT("(895)") SHADOWMODELBUF[nEntryNum];
#line 896
    ASSERT(pSdwBuf);
    bFirst = TRUE;
    i = 0;
    while (pPrim) {
        pSdwBuf[i].pModel = (CShadowModel*)pPrim;
        i++;
        pPrim = _getNextDrawable(*pPrim);
    }
#line 907
    ASSERT(! (i != nEntryNum));
    sortCShadowModel(pSdwBuf, nEntryNum);
    i = 0;
    while (i < nEntryNum) {
        SHADOWMODELBUF* pBuf = &pSdwBuf[i];
        INT nNum = pBuf->nNum;
        HANDLE hHnd = pBuf->hHnd;
        INT nDispNum = 0;
        FVECTOR v[2];
        FVECTOR vMax;
        FVECTOR vMin;
        vMax.set(-1.7014118e+38f, -1.7014118e+38f, -1.7014118e+38f);
        vMin.set(1.7014118e+38f, 1.7014118e+38f, 1.7014118e+38f);
        for (j = 0; j < nNum; j++) {
            if (pBuf[j].pModel) {
                nDispNum++;
                pBuf[j].pModel->getParentModel()->getWorldBBox(v[0], v[1]);
                Math3D::VectorMaxMinXYZ(vMax, vMin, v[0], v[1]);
            }
        }
        i += nNum;
        if (nDispNum > 0) {
            CShadowModelMan::updateShadowMatrix(vMax, vMin, pBuf->pModel ? pBuf->pModel->getSize() : 0);
            nDispNum = 0;
            CShadowModelMan::drawBefore();
            for (j = 0; j < nNum; j++) {
                if (pBuf[j].pModel) {
                    CShadowModel& rSdw = *pBuf[j].pModel;
                    if (CPackMan::isOverFlow(rSdw.getPacketSize())) break;
                    nDispNum++;
                    _draw(rSdw);
                }
            }
            CShadowModelMan::drawAfter();
            if (nDispNum > 0) {
                FVECTOR vSMMax;
                FVECTOR vSMMin;
                CShadowMapModel* pSMModel = pSMModelFirst;
                while (pSMModel) {
                    CModel* pSMPModel = pSMModel->getParentModel();
                    if (pSMPModel) {
                        if (bFirst) {
                            if (!pSMPModel->isMap()) {
                                pSMModel->setHandle(getCShadowModelCoordsHandle(pSMPModel));
                            }
                        }
                        if (pSMModel->getHandle() != hHnd || hHnd == 0) {
                            pSMPModel->getWorldBBox(vSMMax, vSMMin);
                            if (chackBBOX(vMax, vMin, vSMMax, vSMMin)) {
                                if (CPackMan::isOverFlow(pSMModel->getPacketSize())) break;
                                pSMModel = (CShadowMapModel*)_draw(*pSMModel);
                                continue;
                            }
                        }
                    }
                    pSMModel = (CShadowMapModel*)_getNextDrawable(*pSMModel);
                }
                bFirst = FALSE;
            }
        }
    }
    if (pSdwBuf) delete[] pSdwBuf;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", _drawOT__19CDispListShadowDrawR5OTtag);
__asm__(".rdata\n" ".align 3\n" "D_0045FA20:\n" ".word 0x4B2F3A63\n" ".word 0x4D4F4348\n" ".word 0x69614D2F\n" ".word 0x7070416E\n" ".word 0x7379532F\n" ".word 0x2F6D6574\n" ".word 0x70617247\n" ".word 0x2F636968\n" ".word 0x70736944\n" ".word 0x7473694C\n" ".word 0x7070632E\n" ".word 0x35393828\n" ".word 0x00000029\n" ".word 0x00000000\n" "D_0045FA58:\n" ".word 0x77645370\n" ".word 0x00667542\n" "D_0045FA60:\n" ".word 0x69282021\n" ".word 0x203D2120\n" ".word 0x746E456E\n" ".word 0x754E7972\n" ".word 0x0000296D\n" "D_0045FA74:\n" ".word 0xFF000000\n" "D_0045FA78:\n" ".word 0x7F000000\n" ".word 0x00000000\n" ".text\n");
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
        FVECTOR& vCamPos = CShadowModelMan::getUpdateCameraPos();
        FLOAT fClip = CShadowModelMan::getClipDistanceSq();
        pModel->getWorldBBox(v[0], v[1]);
        vu0AddVectorXYZ(v[1], v[0], v[1]);
        vu0ScaleVectorXYZ(v[1], v[1], 0.5f);
        vu0SubVectorXYZ(v[0], v[0], v[1]);



        FLOAT fLen2 = vu0LengthSq(v[0]) + fClip;
        vu0SubVectorXYZ(v[0], vCamPos, v[1]);



        if (vu0LengthSq(v[0]) < fLen2) {
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
