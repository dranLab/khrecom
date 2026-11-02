// PARTIAL: added for System/Movie/MSStream
#ifndef SYSTEM_RSRCDATA_H
#define SYSTEM_RSRCDATA_H

#include "khtypes.h"
#include "System/HeapMan.h"

#ifndef RSRCDATA_H_PATH // MATCH: path override for System/RcfCD
#define RSRCDATA_H_PATH "System/RsrcData.h"
#endif

#include "System/Buf.h"
#include "System/SJoint.h"
#include "System/Task.h"
#include <string.h>

/* PARTIAL: placement new for CRsrcData's inline allocation, declared only: the includers define it themselves */
inline void* operator new(size_t, void* p) throw();

extern "C" const char _ctype_[];
#define RSRCDATA_TOLOWER(c) (((_ctype_ + 1)[c] & 1) ? (c) + 0x20 : (c))

/* Iteration over a CSJointListT (the original's macro; see Game/Collision/CollMapCamera) */
#define RSRC_SJ_FOREACH(T, pEntry, list) \
    for (T *pEntry = (list).getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry; \
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL)

struct TexInfo;
struct MODELDATA;
struct ANIMDATA;
struct EFFECT_DATA;
struct MEP_DATA;
class CMotData;

#include "System/RsrcDB.h" /* NAME_CASE moved there, as in the ref */





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
    CRsrcGroupTag(ULONG nGrpID) { m_nID = nGrpID; }
    virtual ~CRsrcGroupTag() {}
    INT getID() const;
    INT getOptFlag() const;
    BOOL isOpt(INT nOptFlag) const;
};

class CRsrcGroupTagList : public CSJointListT<CRsrcGroupTag> {
public:
    CRsrcGroupTagList() {}
    virtual ~CRsrcGroupTagList() {
        RSRC_SJ_FOREACH(CRsrcGroupTag, __p, *this) delete __p;
        resetParam();
    }
    BOOL isEmpty() const { return getListSize() == 0; }
    BOOL hasTag(INT nGrpID) const;
    BOOL hasTagGID(INT nGrpID) const;
    BOOL hasTagOpt(INT nFlag) const;
    void addTag(INT nGrpID);
    BOOL delTag(INT nGrpID);
    BOOL delTagOpt(INT nFlag);
    void dumpTag() const;
};

#define RSRCDATA_RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

class CRsrcData {
    friend class CRsrcDB;

protected:
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

private:
    void setHandle(HANDLE h) { m_hThis = h; }
    void setParentHandle(HANDLE h) { m_hParent = h; }
    void setHandleToChildren(HANDLE hParent);
    HANDLE* createChildHandleTbl(USHORT n);
    void unregisterChild(HANDLE hChild);

public:
#line 155 RSRCDATA_H_PATH
    CRsrcData(const RsrcInfo& info, CBuf& data) {
        init(info);
        m_pDataSrc = new (com_malloc_d(sizeof(CBuf), __FILE__ "(156)", RSRCDATA_RET_ADDR())) CBuf;
        data.copyTo(*m_pDataSrc);
    }
    CRsrcData(const RsrcInfo& info) : m_pDataSrc(NULL) { init(info); }
    virtual ~CRsrcData();
    virtual void reset(INT nArg) {}
    CBuf* getDataSrc() const { return m_pDataSrc; }
    BYTE* getRsrcData() const {
        BYTE* p = NULL;
        if (m_pDataSrc) p = m_pDataSrc->getBufPtr();
        return p;
    }
    INT getDataSize() const { return m_pDataSrc ? m_pDataSrc->getBufSize() : -1; }
    INT getRsrcType() const { return m_nRsrcType; }
    INT getRsrcGroupID() const { return m_nRsrcGroup; }
    CHAR* getName() const { return (CHAR*)m_szName; }
    BOOL isMatchName(const CHAR* pszName, NAME_CASE nc) const {
        if (!pszName) return FALSE;
        if (nc == NAME_CASE_SENSITIVE) return !strcmp(pszName, m_szName);
        return !strcasecmp(pszName, m_szName);
    }
    BOOL isMatchNameWithoutExt(const CHAR* pszName, NAME_CASE nc) const {
        if (!pszName) return FALSE;
        int c1;
        int c0;
        const CHAR* p = m_szName;
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
    BOOL isMatchNameHash(INT nNameHash) const { return nNameHash == *(INT*)m_szName; }
    void addGroupTag(INT nID);
    BOOL delGroupTag(INT nID) { return m_refTagList.delTag(nID); }
    BOOL delGroupTagOpt(INT nFlag) { return m_refTagList.delTagOpt(nFlag); }
    BOOL hasGroupTag(INT nID) const { return m_refTagList.hasTag(nID); }
    BOOL hasGroupTagOpt(INT nFlag) const { return m_refTagList.hasTagOpt(nFlag); }
    BOOL isGroupTagEmpty() const { return m_refTagList.isEmpty(); }
    const CRsrcGroupTagList& getGroupTagList() const { return m_refTagList; }
    HANDLE getHandle() const { return m_hThis; }
    HANDLE getParentHandle() const { return m_hParent; }
    BOOL hasChild() const { return m_nChildRest != 0; }

protected:
    CBuf* getDataSrc() { return m_pDataSrc; }
    void setDataSrc(CBuf& data) {
        if (m_pDataSrc) delete m_pDataSrc;
        m_pDataSrc = &data;
    }
    virtual void delDataSrc() { getDataSrc()->releaseBuf(FALSE); }

private:
    void init(const RsrcInfo& info);
};

class CRsrcDataDD : public CRsrcData {
public:
    CRsrcDataDD(const RsrcInfo& info, CBuf& data);
    CRsrcDataDD(const RsrcInfo& info);

protected:
    virtual void delDataSrc() { getDataSrc()->releaseBuf(TRUE); }
};

struct TexMemScroll {
    INT m_UvTime;
    INT m_UvStatus;
    INT m_UvFps;
    INT m_NowLine;
    INT m_MaxLine;
};

class CRsrcTex : public CRsrcData {
    friend class CRsrcDB;

private:
    TexInfo* m_pTexInfo;     /* 0x48 */
    TexMemScroll* m_pScr;    /* 0x4c */

public:
    CRsrcTex(const RsrcInfo& info, CBuf& data) : CRsrcData(info) {
        m_pTexInfo = NULL;
        m_pScr = NULL;
        _setupTexData(data, info._cOption, info._cParam);
    }
    virtual ~CRsrcTex();
    INT getAllocType() const;
    TexInfo* getTexInfo() const { return m_pTexInfo; }
    TexMemScroll* getTexMemScroll() const { return m_pScr; }

protected:
    void _setupTexData(CBuf& data, INT nType, INT nOpt);
    void _setupExTexData(TexInfo* exTexInfo, ULONGLONG* pSrc, ULONGLONG* pClut, INT nClutSize, INT nOpt);
};

class CRsrcMDL : public CRsrcDataDD {
    friend class CRsrcDB;

private:
    SHORT m_nAMFCount; /* 0x48 */
    USHORT m_nFlag;    /* 0x4a */

    void _setupMDL();

public:
    CRsrcMDL(const RsrcInfo& info, CBuf& data) : CRsrcDataDD(info, data) {
        m_nAMFCount = 0;
        m_nFlag = 0;
        _setupMDL();
    }
    BOOL isTexIncomplete() const { return m_nFlag & 1; }
    MODELDATA** getMDLData() {
        if (isTexIncomplete()) reSetupTex();
        return (MODELDATA**)getRsrcData();
    }
    INT getMDLCount() const { return m_nAMFCount; }
    void reSetupTex();
};

class CRsrcAMF : public CRsrcData {
private:
    void _setupAMF();

public:
    CRsrcAMF(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) { _setupAMF(); }
    MODELDATA* getAMFData() const { return (MODELDATA*)getRsrcData(); }
    static BOOL setupAMF(MODELDATA* pData, BOOL bExitOnTexErr);
    static BOOL setupAMFTex(MODELDATA* pData, BOOL bExitOnErr);
};

class CRsrcANM : public CRsrcData {
private:
    INT m_nAAFCount; /* 0x48 */

    void _setupANM();

public:
    CRsrcANM(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) {
        m_nAAFCount = 0;
        _setupANM();
    }
    ANIMDATA** getANMData() const { return (ANIMDATA**)getRsrcData(); }
    INT getAAFCount() const { return m_nAAFCount; }
};

class CRsrcAAF : public CRsrcData {
private:
    void _setupAAF() { setupAAF(getRsrcData()); }

public:
    CRsrcAAF(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) { _setupAAF(); }
    ANIMDATA* getAAFData() const { return (ANIMDATA*)getRsrcData(); }
    static void setupAAF(BYTE* pInSrc);
};

class CRsrcMMP : public CRsrcData {
private:
    BYTE* m_pMMP;     /* 0x48 */
    INT m_nMMPCount;  /* 0x4c */

    void _setupMMP();

public:
    CRsrcMMP(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) { _setupMMP(); }
    CHAR* getMMPName(INT nInPtdNo);
    BYTE* getMMPData(const CHAR* lpszInName);
    BYTE* getMMPData(INT nInPtdNo);
};

class CRsrcCTD : public CRsrcData {
private:
    BYTE* m_pCTD;     /* 0x48 */
    INT m_nCTDCount;  /* 0x4c */

    void _setupCTD();
    void _disposeCTD();

public:
    CRsrcCTD(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) { _setupCTD(); }
    virtual ~CRsrcCTD() { _disposeCTD(); }
    CHAR* getCTDName(INT nInPtdNo);
    BYTE* getCTDData(const CHAR* lpszInName);
    BYTE* getCTDData(INT nInPtdNo);
};

class CRsrcMCD : public CRsrcData {
private:
    void _setupMCD();

public:
    CRsrcMCD(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) { _setupMCD(); }
};

class CRsrcANN : public CRsrcData {
    friend class CRsrcDB;

private:
    INT m_nCount; /* 0x48 */

    void _setupANN();

public:
    CRsrcANN(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) {
        m_nCount = 0;
        _setupANN();
    }
    virtual ~CRsrcANN();
    INT getMotCount() const { return m_nCount; }
    CMotData* getMotData(INT nInMotNo) const {
        if (nInMotNo < 0 || nInMotNo >= m_nCount) return NULL;
        CMotData** ppMot = (CMotData**)getRsrcData();
        if (!ppMot) return NULL;
        return ppMot[nInMotNo];
    }
};

class CRsrcAZF : public CRsrcDataDD {
private:
    USHORT m_nAMFCount; /* 0x48 */
    USHORT m_nFlag;     /* 0x4a */

    void _setupAZF();

public:
    CRsrcAZF(const RsrcInfo& info, CBuf& data) : CRsrcDataDD(info, data) {
        m_nAMFCount = 0;
        m_nFlag = 0;
        _setupAZF();
    }
    BOOL isTexIncomplete() { return m_nFlag & 1; } // PARTIAL: added for Game/Map (body from its inlined test)
    void reSetupTex();
};

class CRsrcCAM : public CRsrcData {
private:
    INT m_nCAMCount; /* 0x48 */

    void _setupDataCAM();

public:
    CRsrcCAM(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) { _setupDataCAM(); }
    INT getCAMCount(/* parameters unknown */);
};

class CRsrcDLL : public CRsrcData {
private:
    void _setupDLL();

public:
    CRsrcDLL(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) { _setupDLL(); }
    virtual ~CRsrcDLL();
};

class CRsrcMEP : public CRsrcData {
private:
    MEP_DATA* m_pMEP; /* 0x48 */

    void _setupMEP();

public:
    CRsrcMEP(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) { _setupMEP(); }
};

class CRsrcSND : public CRsrcData {
private:
    INT m_nWaveHandle; /* 0x48 */

    void _setupSND();

public:
    CRsrcSND(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) { _setupSND(); }
    virtual ~CRsrcSND();
};

class CRsrcRSD : public CRsrcData {
private:
    INT m_nWaveHandle; /* 0x48 */

    void _setupRSD(INT nArg);

public:
    CRsrcRSD(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) { _setupRSD(0); }
    virtual ~CRsrcRSD();
    virtual void reset(INT nArg) { _setupRSD(nArg); }
};

class CRsrcSES : public CRsrcData {
public:
    CRsrcSES(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) {}
};

class CRsrcList : public CSLCListT<CRsrcData> {
public:
    CRsrcList(ISLCManagerT<CRsrcData>& inManager) : CSLCListT<CRsrcData>(inManager) {}
    virtual ~CRsrcList() {
        for (SLContainerT<CRsrcData>* __p = getHead(); __p; __p = __p->getNext()) {
            if (__p->getValue()) {
                delete __p->getValue();
                __p->setValue(NULL);
            }
        }
        removeAll();
    }
};

class CRsrcVSV : public CRsrcData {
private:
    UINT m_nStartSect; /* 0x48 */
    UINT m_nSectSize;  /* 0x4c */
    UINT m_nBgmID;     /* 0x50 */

    void _setupVSV();

public:
    CRsrcVSV(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) {
        m_nStartSect = 0;
        m_nSectSize = 0;
        _setupVSV();
    }
    CRsrcVSV(const RsrcInfo& info, UINT nStartSect, UINT nSectSize) : CRsrcData(info) {
        m_nStartSect = nStartSect;
        m_nSectSize = nSectSize;
        _setupVSV();
    }
    CHAR* getFilePath();
    // PARTIAL: bodies added for System/Sound/SoundMan (from its inlined reads)
    UINT getStartSector() { return m_nStartSect; }
    UINT getSectorSize() { return m_nSectSize; }
    UINT getBgmID() { return m_nBgmID; }
};

class CRsrcSPD : public CRsrcData {
private:
    HANDLE m_hSPD; /* 0x48 */

    void _setupSPD();
    void _disposeSPD();

public:
    CRsrcSPD(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) { _setupSPD(); }
    virtual ~CRsrcSPD() { _disposeSPD(); }
};

class CRsrcSQD : public CRsrcSPD {
public:
    CRsrcSQD(const RsrcInfo& info, CBuf& data) : CRsrcSPD(info, data) {}
};

class CRsrcLAD : public CRsrcSPD {
public:
    CRsrcLAD(const RsrcInfo& info, CBuf& data) : CRsrcSPD(info, data) {}
};

class CRsrcDPX : public CRsrcData {
    friend class CRsrcDB;

private:
    EFFECT_DATA* m_pEffData; /* 0x48 */

    void _setupDPX();
    void _disposeDPX();

public:
    CRsrcDPX(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) {
        m_pEffData = NULL;
        _setupDPX();
    }
    virtual ~CRsrcDPX() { _disposeDPX(); }
    EFFECT_DATA* getDPXData() const { return m_pEffData; }
};

class CRsrcTXA : public CRsrcData {
private:
    void _setupTXA();

public:
    CRsrcTXA(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) { _setupTXA(); }
};

class CRsrcALF : public CRsrcData {
private:
    void _setupALF();

public:
    CRsrcALF(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) { _setupALF(); }
};

class CRsrcDataFactory {
public:
    static CRsrcData* createRsrcData(const RsrcInfo& info, CBuf& data);
};

/* PARTIAL: PackedRsrcHeader and CRsrcIMZ are in System/RsrcDB.h in the ref (they need CRsrcData, so they are here) */
struct PackedRsrcHeader {
    void* _pBody;
    char _szName[22];
    BYTE _cType;
    BYTE _cPram;
    INT _cSize;

    static void coodinateVersion(void* pHead);
};

class CRsrcIMZ : public CRsrcData {
public:
    CRsrcIMZ(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) {}
};


#endif /* SYSTEM_RSRCDATA_H */
