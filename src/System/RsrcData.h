// PARTIAL: added for System/Movie/MSStream
#ifndef SYSTEM_RSRCDATA_H
#define SYSTEM_RSRCDATA_H

#include "khtypes.h"
#include "System/HeapMan.h"

#ifndef RSRCDATA_H_PATH // PARTIAL: path override added for System/RcfCD
#define RSRCDATA_H_PATH "System/RsrcData.h"
#endif

#ifndef RSRCDATA_H_FULL
/* Placeholder for the original header's inline allocation: its string ("System/RsrcData.h(156)") is in .rodata
 * of every includer. */
#line 1 RSRCDATA_H_PATH
inline void* _rsrcDataAlloc(size_t n) {
    return com_malloc_d(n, __FILE__ "(156)", (UINT)-1);
}
#else
/* PARTIAL: the classes, added for System/RsrcDB (define RSRCDATA_H_FULL before including) */
#include "System/Buf.h"
#include "System/SJoint.h"

#ifndef KH_PLACEMENT_NEW_DEFINED
#define KH_PLACEMENT_NEW_DEFINED
inline void* operator new(size_t, void* p) throw() { return p; }
#endif

extern "C" char _ctype_[];
#define RSRCDATA_TOLOWER(c) (((_ctype_ + 1)[c] & 1) ? (c) + 0x20 : (c))

/* Iteration over a CSJointListT (the original's macro; see Game/Collision/CollMapCamera) */
#define RSRC_SJ_FOREACH(T, pEntry, list) \
    for (T *pEntry = (list).getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry; \
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL)

struct TexInfo;
struct TexMemScroll;
struct MODELDATA;
struct ANIMDATA;
struct EFFECT_DATA;
class CMotData;

enum NAME_CASE {
    NAME_CASE_SENSITIVE = 0,
    NAME_CASE_UPPER = 1,
    NAME_CASE_LOWER = 2
};

struct RsrcInfo {
    CHAR* _pszName;
    INT _nGroupID;
    INT _nRefGroupID;
    INT _nRsrcType;
    BOOL _bSceneManaged;
    BYTE _cOption;
    BYTE _cParam;

    RsrcInfo() {
        _nRefGroupID = 0;
        _cOption = 0;
        _cParam = 1;
    }
    RsrcInfo(const RsrcInfo& src) {
        _pszName = src._pszName;
        _nGroupID = src._nGroupID;
        _nRefGroupID = src._nRefGroupID;
        _nRsrcType = src._nRsrcType;
        _bSceneManaged = src._bSceneManaged;
        _cOption = src._cOption;
        _cParam = src._cParam;
    }
};

class CRsrcGroupTag : public CSJointT<CRsrcGroupTag> {
private:
    INT m_nID; /* 0x8 */

public:
    virtual ~CRsrcGroupTag();
    INT getID() { return m_nID; }
};

class CRsrcGroupTagList : public CSJointListT<CRsrcGroupTag> {
public:
    CRsrcGroupTagList() {}
    virtual ~CRsrcGroupTagList() {
        RSRC_SJ_FOREACH(CRsrcGroupTag, __p, *this) delete __p;
        resetParam();
    }
    BOOL isEmpty() { return getListSize() == 0; }
    BOOL hasTag(INT nGrpID) const;
    BOOL hasTagGID(INT nGrpID) const;
    BOOL hasTagOpt(INT nFlag) const;
    void addTag(INT nGrpID);
    BOOL delTag(INT nGrpID);
    BOOL delTagOpt(INT nFlag);
    void dumpTag();
};

#define RSRCDATA_RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

class CRsrcData {
    friend class CRsrcDB;

private:
    CBuf* m_pDataSrc;      /* 0x00 */
    char m_szName[24];     /* 0x04 */
    INT m_nRsrcType;       /* 0x1c */
    INT m_nRsrcGroup;      /* 0x20 */
    HANDLE m_hThis;        /* 0x24 */
    HANDLE m_hParent;      /* 0x28 */
    HANDLE* m_phChildren;  /* 0x2c */
    USHORT m_nChildCount;  /* 0x30 */
    USHORT m_nChildRest;   /* 0x32 */

protected:
    CRsrcGroupTagList m_refTagList; /* 0x34 */

protected:
#line 155 RSRCDATA_H_PATH
    void setDataSrc(CBuf& data) {
        m_pDataSrc = new (com_malloc_d(sizeof(CBuf), __FILE__ "(156)", RSRCDATA_RET_ADDR())) CBuf;
        data.copyTo(*m_pDataSrc);
    }

private:
    void setHandle(HANDLE h) { m_hThis = h; }
    void setHandleToChildren(HANDLE hParent);
    HANDLE* createChildHandleTbl(USHORT n);

public:
    CRsrcData(const RsrcInfo& info) : m_pDataSrc(NULL) { init(info); }
    CRsrcData(const RsrcInfo& info, CBuf& data) {
        init(info);
        setDataSrc(data);
    }
    virtual ~CRsrcData();
    virtual void reset(INT nArg);
    BYTE* getRsrcData() {
        BYTE* p = NULL;
        if (m_pDataSrc) p = m_pDataSrc->getBufPtr();
        return p;
    }
    INT getRsrcType() { return m_nRsrcType; }
    INT getRsrcGroupID() { return m_nRsrcGroup; }
    CHAR* getName() { return m_szName; }
    BOOL isMatchNameWithoutExt(CHAR* pszName, NAME_CASE nc) {
        int c1;
        int c0;
        CHAR* p = m_szName;
        while ((c0 = *pszName) != '\0' && c0 != '.') {
            c1 = *p;
            pszName++;
            p++;
            if (nc != NAME_CASE_SENSITIVE) {
                c0 = RSRCDATA_TOLOWER(c0);
                c1 = RSRCDATA_TOLOWER(c1);
            }
            if (c0 != c1) return FALSE;
        }
        c1 = *p;
        return c1 == '\0' || c1 == '.';
    }
    BOOL isMatchNameHash(INT nNameHash) { return nNameHash == *(INT*)m_szName; }
    void addGroupTag(INT nID);
    BOOL delGroupTag(INT nID) { return m_refTagList.delTag(nID); }
    BOOL delGroupTagOpt(INT nFlag) { return m_refTagList.delTagOpt(nFlag); }
    BOOL hasGroupTag(INT nID) { return m_refTagList.hasTag(nID); }
    BOOL hasGroupTagOpt(INT nFlag) { return m_refTagList.hasTagOpt(nFlag); }
    BOOL isGroupTagEmpty() { return m_refTagList.isEmpty(); }
    CRsrcGroupTagList& getGroupTagList() { return m_refTagList; }
    HANDLE getHandle() { return m_hThis; }
    BOOL hasChild() { return m_nChildRest; }

protected:
    CBuf* getDataSrc() { return m_pDataSrc; }
    virtual void delDataSrc();

private:
    void init(const RsrcInfo& info);
};

class CRsrcDataDD : public CRsrcData {
public:
    CRsrcDataDD(const RsrcInfo& info, CBuf& data);

protected:
    virtual void delDataSrc() { getDataSrc()->releaseBuf(TRUE); }
};

class CRsrcTex : public CRsrcData {
    friend class CRsrcDB;

public:
    CRsrcTex(const RsrcInfo& info, CBuf& data);

private:
    TexInfo* m_pTexInfo;     /* 0x48 */
    TexMemScroll* m_pScr;    /* 0x4c */

public:
    TexInfo* getTexInfo() { return m_pTexInfo; }
};

class CRsrcMDL : public CRsrcDataDD {
    friend class CRsrcDB;

private:
    SHORT m_nAMFCount; /* 0x48 */
    USHORT m_nFlag;    /* 0x4a */

public:
    CRsrcMDL(const RsrcInfo& info, CBuf& data);
    BOOL isTexIncomplete() { return m_nFlag & 1; }
    MODELDATA** getMDLData() {
        if (isTexIncomplete()) reSetupTex();
        return (MODELDATA**)getRsrcData();
    }
    INT getMDLCount() { return m_nAMFCount; }
    void reSetupTex();
};

class CRsrcMMP : public CRsrcData {
private:
    BYTE* m_pMMP;     /* 0x48 */
    INT m_nMMPCount;  /* 0x4c */

public:
    CRsrcMMP(const RsrcInfo& info, CBuf& data);
    CHAR* getMMPName(INT nInPtdNo);
    BYTE* getMMPData(const CHAR* lpszInName);
    BYTE* getMMPData(INT nInPtdNo);
};

class CRsrcCTD : public CRsrcData {
private:
    BYTE* m_pCTD;     /* 0x48 */
    INT m_nCTDCount;  /* 0x4c */

public:
    CRsrcCTD(const RsrcInfo& info, CBuf& data);
    CHAR* getCTDName(INT nInPtdNo);
    BYTE* getCTDData(const CHAR* lpszInName);
    BYTE* getCTDData(INT nInPtdNo);
};

class CRsrcANN : public CRsrcData {
    friend class CRsrcDB;

private:
    INT m_nCount; /* 0x48 */

public:
    CRsrcANN(const RsrcInfo& info, CBuf& data);
    INT getMotCount() { return m_nCount; }
    CMotData* getMotData(INT nInMotNo) {
        if (nInMotNo < 0 || nInMotNo >= m_nCount) return NULL;
        CMotData** ppMot = (CMotData**)getRsrcData();
        if (!ppMot) return NULL;
        return ppMot[nInMotNo];
    }
};

class CRsrcDPX : public CRsrcData {
    friend class CRsrcDB;

private:
    EFFECT_DATA* m_pEffData; /* 0x48 */

public:
    CRsrcDPX(const RsrcInfo& info, CBuf& data);
    EFFECT_DATA* getDPXData() { return m_pEffData; }
};

class CRsrcDataFactory {
public:
    static CRsrcData* createRsrcData(const RsrcInfo& info, CBuf& data);
};
#endif /* RSRCDATA_H_FULL */

#endif /* SYSTEM_RSRCDATA_H */
