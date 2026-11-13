#ifndef SYSTEM_GRAPHIC_DISPLIST_H
#define SYSTEM_GRAPHIC_DISPLIST_H

#include "khtypes.h"
#include "System/Graphic/Gprim.h"

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
    friend class CModelMan; // MATCH: CModelMan::drawDispListFG reads the OT table inline; getOTtagTbl is protected in the ref (friends are not in the debug info)
private:
    OTtag* m_pOTtagTbl;   /* 0x0 */
    UINT m_nOTtagTblSize; /* 0x4 */
    KH_DEBUG(UINT m_iDebugFlags;   /* 0x8 */)

public:
    CDispListBase() {
        m_pOTtagTbl = NULL;
        m_nOTtagTblSize = 0;
        KH_DEBUG(m_iDebugFlags = 0;)
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
    CDrawable* _draw(CDrawable& inPrim) KH_DEBUG(;) KH_RELEASE({ inPrim.makePkt(); return inPrim.m_pNext; })
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

#endif /* SYSTEM_GRAPHIC_DISPLIST_H */
