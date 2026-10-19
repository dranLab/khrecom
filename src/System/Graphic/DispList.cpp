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
#include "System/Graphic/ModelMan.h"
#include "System/VramMan.h"
#include "include_asm.h"

class CPackMan { /* System/Graphic/PackMan.h */
public:
    static void pkCnt(UINT n);
    static void pkAddCode(UINT nCode);
    static void pkOpenDirectCode(INT n);
    static void pkCloseDirectCode();
    static void changeLayer(INT nLayer);
    static void setDefault_TEST_2(ULONGLONG n);
    static void setDefault_ALPHA_2(ULONGLONG n);
    static void setDefault_ZBUF_2(ULONGLONG n);
    static u_long128* pkOpenGifTagManual(u_long128 tag);
    static void pkCloseGifTagManual(UINT* p);
};

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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", clear__13CDispListBasei);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x70736944\n.word 0x7473694C\n.word 0x7070632E\n.word 0x00000000\n.word 0x4F705F6D\n.word 0x67617454\n.word 0x006C6254\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", draw__13CDispListBaseii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", getHeadPrim__13CDispListBasei);
void CDispListBase::_startDirectMode() {
    CPackMan::pkCnt(0);
    CPackMan::pkAddCode(0x13000000);
    CPackMan::pkOpenDirectCode(0);
}
void CDispListBase::_endDirectMode() { CPackMan::pkCloseDirectCode(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", checkCache__13CDispListBaseR9CDrawableG7G_LAYER);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", _draw__13CDispListBaseR9CDrawable);
__asm__(".rdata\n.align 3\n.word 0x24242424\n.word 0x24242424\n.word 0x24242424\n.word 0x616D2024\n.word 0x6B50656B\n.word 0x8A292874\n.word 0x829490D6\n.word 0x82D495CC\n.word 0x835483B7\n.word 0x82598343\n.word 0x82AD8FAA\n.word 0x82A282C8\n.word 0x21B782C5\n.word 0x3E2D2121\n.word 0x29642528\n.word 0x8DC08E20\n.word 0x8ECC82DB\n.word 0x83709767\n.word 0x83438354\n.word 0x64252859\n.word 0x24242029\n.word 0x24242424\n.word 0x24242424\n.word 0x0A242424\n.word 0x00000000\n.word 0x00000000\n.text\n");
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", _drawOT__9CDispListR5OTtag);
void CDispList::_endDraw() {}
void CDispListSemiBase::_beginDraw() {
    _startDirectMode();
    _updateTestMode(NULL);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", _drawOT__17CDispListSemiBaseR5OTtag);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", _drawOT__17CDispListWithoutZR5OTtag);
__asm__(".rdata\n.align 3\n.word 0x73694443\n.word 0x73694C70\n.word 0x74695774\n.word 0x74756F68\n.word 0x203A205A\n.word 0x50837083\n.word 0x67836283\n.word 0x5B814983\n.word 0x5B816F83\n.word 0xBD82CC82\n.word 0x6095DF82\n.word 0x5883E689\n.word 0x62834C83\n.word 0x5B207683\n.word 0x0A5D6425\n.word 0x00000000\n.text\n");
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", ShadowModel_QSortComp__FPCvT0);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", getCShadowModelCoordsHandle__FP6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", sortCShadowModel__FP14SHADOWMODELBUFi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", chackBBOX__FRC7FVECTORN30);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", _drawOT__19CDispListShadowDrawR5OTtag);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x70736944\n.word 0x7473694C\n.word 0x7070632E\n.word 0x35393828\n.word 0x00000029\n.word 0x00000000\n.word 0x77645370\n.word 0x00667542\n.word 0x69282021\n.word 0x203D2120\n.word 0x746E456E\n.word 0x754E7972\n.word 0x0000296D\n.word 0xFF000000\n.word 0x7F000000\n.word 0x00000000\n.text\n");
void CDispListShadowDraw::_endDraw() {
    CModelMan::clearTexCache();
    CGTexPrim::clearTexCache();
    CGTexPrim2D::clearTexCache();
    _endDirectMode();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", insert__18CDispListShadowMapR9CDrawablei);
void CDispListShadowMap::_beginDraw() { _startDirectMode(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/DispList", _drawOT__18CDispListShadowMapR5OTtag);
void CDispListShadowMap::_endDraw() {
    CModelMan::clearTexCache();
    CGTexPrim::clearTexCache();
    CGTexPrim2D::clearTexCache();
    _endDirectMode();
}
// SOURCE DATA: .rodata
