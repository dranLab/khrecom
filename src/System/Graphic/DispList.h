// PARTIAL: added for System/Graphic/ModelMan
#ifndef SYSTEM_GRAPHIC_DISPLIST_H
#define SYSTEM_GRAPHIC_DISPLIST_H

#include "khtypes.h"
#include "System/Graphic/Gprim.h"

struct OTtag {
    CDrawable* m_pHead;
    CDrawable* m_pTail;
};

struct SEMI_PACKET_LIST;

class CDispListBase {
private:
    OTtag* m_pOTtagTbl;      /* 0x0 */
    UINT m_nOTtagTblSize;    /* 0x4 */
    UINT m_iDebugFlags;      /* 0x8 */

public:
    CDispListBase() {
        m_pOTtagTbl = NULL;
        m_nOTtagTblSize = 0;
        m_iDebugFlags = 0;
    }
    virtual ~CDispListBase();
    virtual void insert(CDrawable& inPrim, FIX4 inZ);
    virtual void clear(BOOL bUncached);
    void draw(INT nStart, INT nEnd);
    virtual UINT getEntryCount() const;

protected:
    virtual void _beginDraw();
    virtual void _drawOT(OTtag& inTag);
    virtual void _endDraw();
    void setupOTtagTbl(UINT nSize);
public:
    OTtag* getOTtagTbl() { return m_pOTtagTbl; }
    INT getOTtagTblSize() { return m_nOTtagTblSize; }
protected:
    /* adds a primitive at the end of an OT entry (original name unknown) */
    void _insertTail(OTtag& tag, CDrawable& inPrim) {
        CDrawable* pTailOld = tag.m_pTail;
        tag.m_pTail = &inPrim;
        if (pTailOld) pTailOld->m_pNext = &inPrim;
        inPrim.m_pNext = NULL;
        if (!tag.m_pHead) tag.m_pHead = &inPrim;
    }
    void _setZ(CDrawable& inPrim, FIX4 inZ) { inPrim.m_nZ = inZ; }
};

class CDispList : public CDispListBase {
private:
    UINT _m_0x10[2]; /* 0x10 */
public:
    CDispList() {
        setupOTtagTbl(26);
        clear(FALSE);
    }
    virtual ~CDispList();
    virtual void insert(CDrawable& inPrim, FIX4 inZ);
    virtual void clear(BOOL bUncached);
};

class CDispListBG : public CDispList {
public:
    CDispListBG() {}
    virtual ~CDispListBG();
};

class CDispListFG : public CDispList {
public:
    CDispListFG() {}
    virtual ~CDispListFG();
};

class CDispListShadowDraw : public CDispListBase {
private:
    UINT _m_0x10; /* 0x10 */
public:
    CDispListShadowDraw() {
        setupOTtagTbl(1);
        clear(FALSE);
    }
    virtual ~CDispListShadowDraw();
    virtual void insert(CDrawable& inPrim, FIX4 inZ);
    virtual void clear(BOOL bUncached);
};

class CDispListShadowMap : public CDispListBase {
private:
    UINT _m_0x10; /* 0x10 */
public:
    CDispListShadowMap() {
        setupOTtagTbl(1);
        clear(FALSE);
    }
    virtual ~CDispListShadowMap();
    virtual void insert(CDrawable& inPrim, FIX4 inZ);
    virtual void clear(BOOL bUncached);
};

class CDispListSemiBase : public CDispListBase {
private:
    ULONGLONG m_nTestFlag; /* 0x10 */
    ULONGLONG m_nAlphaMode; /* 0x18 ALPHA */

protected:
    virtual void _beginDraw();
    virtual void _drawOT(OTtag& inTag);
    virtual void _endDraw();
};

class CDispListSemi : public CDispListSemiBase {
public:
    CDispListSemi() {
        setupOTtagTbl(0x800);
        clear(TRUE);
    }
    virtual ~CDispListSemi();
    virtual void insert(CDrawable& inPrim, FIX4 inZ);
};

#endif /* SYSTEM_GRAPHIC_DISPLIST_H */
