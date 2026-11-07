#define KH_PLACEMENT_NEW_DEFINED /* MATCH: placement new is not in the ref headers (the units define it themselves); defined once per unit */
inline void* operator new(unsigned int, void* p) throw() { return p; }
#define RSRCDATA_H_PATH "c:/KHCOM/MainApp/System/RsrcData.h" /* MATCH: this unit names RsrcData.h so (header default "System/RsrcData.h") */
#define LINKEDBUF_H_PATH "c:/KHCOM/MainApp/System/LinkedBuf.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Task.h"
#include "System/RsrcData.h"
#include "System/RsrcDB.h"
#include "System/LinkedBuf.h"
#include "System/FifoFpl.h"
#include "System/RcfCD.h"
#include "System/Debug.h"
#include "System/MainApp.h"
#include "Game/SceneInfo.h"
#include <string.h>
#include "System/HeapMan.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);



#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

#define RCID_MAX 63
#define GROUP_ID_BASE 10000
#define GROUP_OPT_SCENEID_MAX 7
#define GROUP_OPT_SCENE(n) ((((n) << 28) & 0x70000000) | 0x80000000)

struct IMZ_HEADER {
    unsigned char id[4];
    USHORT version;
    CHAR cache;
    CHAR padding;
    INT index_ofs;
    INT image_max;
};

struct IMZ_INDEX {
    UINT ofs;
    UINT size;
};

class CExtID : public CSJointT<CExtID> {
private:
    char m_szExt[16]; /* 0x08 */
    INT m_nID;        /* 0x18 */

public:
    CExtID(const CHAR* lpszInExt, INT nInID) { init(lpszInExt, nInID); }
    virtual ~CExtID() {}
    /* after the constructor: not inlined into it */
    void init(const CHAR* pszExt, INT nInID) {
        m_nID = nInID;
        if (pszExt) {
            strncpy(m_szExt, pszExt, 15);
            m_szExt[15] = '\0';
        } else {
            m_szExt[0] = '\0';
        }
    }
    CHAR* getExt() { return m_szExt; }
    INT getID() { return m_nID; }
};

class CExtIDList : public CSJointListT<CExtID> {
public:
    virtual ~CExtIDList() {
        RSRC_SJ_FOREACH(CExtID, __p, *this) delete __p;
        resetParam();
    }
};

class _CRsrcSLCManager : public ISLCManagerT<CRsrcData>, private CFifoFpl {
public:
    _CRsrcSLCManager() { initialize(sizeof(SLContainerT<CRsrcData>), 0x400, NULL); }
    /* defined below, after ~ISLCManagerT (weak function order) */
    virtual SLContainerT<CRsrcData>* createContainer();
    virtual void releaseContainer(SLContainerT<CRsrcData>& inContainer);
    virtual SLContainerT<CRsrcData>* getContainer(HANDLE hID);
    CRsrcData* getRsrc(HANDLE hID) {
        SLContainerT<CRsrcData>* pContainer = getContainer(hID);
        return pContainer ? pContainer->getValue() : NULL;
    }
};

inline SLContainerT<CRsrcData>* _CRsrcSLCManager::createContainer() {
    SLContainerT<CRsrcData>* pContainer = (SLContainerT<CRsrcData>*)allocBlock();
    if (!pContainer) TRACE("\x83\x8a\x83\\\x81[\x83X\x97p\x83R\x83\x93\x83" "e\x83i\x82\xaa MAX \x82\xc9\x93\x9e\x92" "B\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
    return pContainer;
}
inline void _CRsrcSLCManager::releaseContainer(SLContainerT<CRsrcData>& inContainer) { releaseBlock(&inContainer); }
inline SLContainerT<CRsrcData>* _CRsrcSLCManager::getContainer(HANDLE hID) {
    return (SLContainerT<CRsrcData>*)getValidActiveBlock(hID);
}

template <class T>
class CRsrcPackT : public T {
private:
    INT m_nPackCount;

    void _countData() {
        INT n = 0;
        PackedRsrcHeader* pData = (PackedRsrcHeader*)this->getRsrcData();
        while (pData->_pBody) {
            pData++;
            n++;
        }
        m_nPackCount = n;
    }

public:
    CRsrcPackT(const RsrcInfo& info, CBuf& data) : T(info, data) {
        m_nPackCount = 0;
        _countData();
    }
};

namespace {
_CRsrcSLCManager* s_pRsrcSLCManager;
CRsrcList* s_pRsrcListTbl[RCID_MAX];
SLContainerT<CRsrcData>* s_pRsrcEnumerater;
CRsrcList* s_pTexExList;
CExtIDList* s_pExtIDList;
INT s_nSeekRsrcType;
INT s_nSeekGrpID;
UINT s_nSceneFlag;
}  // namespace

static CRsrcList& _getRsrcList(UINT nRsrcType);

void CRsrcDB::initialize() {
#line 183
    s_pRsrcSLCManager = new (__FILE__ "(183)") _CRsrcSLCManager;
    for (INT i = 0; i < RCID_MAX; i++) {
#line 185
        s_pRsrcListTbl[i] = new (__FILE__ "(185)") CRsrcList(*s_pRsrcSLCManager);
    }
#line 186
    s_pTexExList = new (__FILE__ "(186)") CRsrcList(*s_pRsrcSLCManager);
#line 187
    s_pExtIDList = new (__FILE__ "(187)") CExtIDList;
    s_pRsrcEnumerater = NULL;
    s_nSeekRsrcType = 0;
    s_nSeekGrpID = 0;
    s_nSceneFlag = 0;
    RsrcInfo info;
    info._nRsrcType = 0;
    info._nGroupID = 0;
    info._pszName = "dummy";
    info._bSceneManaged = FALSE;
#line 204
    CRsrcData* pRsrc = new (__FILE__ "(204)") CRsrcData(info);
    s_pRsrcListTbl[0]->pushBack(*pRsrc);
    pRsrc->setHandle(0);
}

void CRsrcDB::finalize() {
    for (INT i = 0; i < RCID_MAX; i++) {
        delete s_pRsrcListTbl[i];
        s_pRsrcListTbl[i] = NULL;
    }
    delete s_pTexExList;
    s_pTexExList = NULL;
    delete s_pExtIDList;
    s_pExtIDList = NULL;
    delete s_pRsrcSLCManager;
    s_pRsrcSLCManager = NULL;
}

INT CRsrcDB::addRsrc(const RsrcInfo& info, CBuf& data, CRsrcData** ppOutRsrc) {
    if ((info._nRsrcType & 0x7f) == 0x18) return addRsrc_NoCheck(info, data, ppOutRsrc);
    CRsrcData* pRsrc = getRsrc(info._nRsrcType, info._pszName, NAME_CASE_SENSITIVE);
    if (!pRsrc) return addRsrc_NoCheck(info, data, ppOutRsrc);
    INT nGroupID = info._nRefGroupID;
    if (!nGroupID) nGroupID = info._nGroupID;
    addRsrcTag(*pRsrc, nGroupID, info._bSceneManaged);
    if (ppOutRsrc) *ppOutRsrc = pRsrc;
    return 1;
}

static INT _setupGroupFlag(INT nGroupID, BOOL bSceneManaged) {
    if (!bSceneManaged) nGroupID &= 0x0fffffff;
    else nGroupID |= s_nSceneFlag;
    return nGroupID;
}

INT CRsrcDB::addRsrc_NoCheck(const RsrcInfo& info, CBuf& data, CRsrcData** ppOutRsrc) {
    INT nResult = -1;
    ((RsrcInfo&)info)._nGroupID = _setupGroupFlag(info._nGroupID, info._bSceneManaged);
    if (info._nRefGroupID) ((RsrcInfo&)info)._nRefGroupID = _setupGroupFlag(info._nRefGroupID, info._bSceneManaged);
    CRsrcData* pRsrc = NULL;
    BOOL bDeletable = FALSE;
    INT nPkResult = -1;
    RsrcInfo infoTmp;
    INT nRsrcType = info._nRsrcType;
    switch (nRsrcType & 0x7f) {
    case 15: {
        nRsrcType |= 0x300;
        infoTmp = info;
        INT nGroupID = info._nGroupID;
        INT nMin;
        if (nGroupID == 1 || nGroupID == 5 || nGroupID == 6 ||
            (nMin = 0x22344, nGroupID >= nMin && nGroupID <= 0x22351) ||
            (nMin = 0x2272c, nGroupID >= nMin && nGroupID <= 0x22739)) goto raw;
        goto pack;
    raw:
        infoTmp._nRsrcType = nRsrcType & 0x7f;
        {
            CBuf tmp;
#line 312
            pRsrc = new (__FILE__ "(312)") CRsrcData(infoTmp, tmp);
        }
        bDeletable = TRUE;
        infoTmp._nRsrcType = nRsrcType;
        goto done;
    pack:
        nRsrcType |= 0x400;
        infoTmp._nRsrcType = nRsrcType;
#line 334
        pRsrc = new (__FILE__ "(334)") CRsrcPackT<CRsrcDataDD>(infoTmp, data);
    done:
        nPkResult = addPackedRsrc(infoTmp, data, pRsrc, FALSE);
        break;
    }
    case 14:
        infoTmp = info;
        nRsrcType |= 0x1000;
        infoTmp._nRsrcType = nRsrcType;
#line 348
        pRsrc = new (__FILE__ "(348)") CRsrcPackT<CRsrcData>(infoTmp, data);
        nPkResult = addPackedRsrc(infoTmp, data, pRsrc, FALSE);
        break;
    case 7: {
        INT* pnPrefix = (INT*)data.getBufPtr();
#line 357
        ASSERT(pnPrefix);
        if (*pnPrefix == 0x504e4540 || data.getBufSize() == 10) {
            ((RsrcInfo&)info)._nRsrcType = (info._nRsrcType & ~0x7f) | 0x15;
            break;
        }
    }
    case 18:
    case 23:
    case 27:
    case 33:
    case 37:
    case 46:
    case 49:
    case 55:
    case 57:
#line 377
        pRsrc = new (__FILE__ "(377)") CRsrcPackT<CRsrcData>(info, data);
        nPkResult = addPackedRsrc(info, data, pRsrc, FALSE);
        break;
    case 12:
    case 19:
#line 387
        pRsrc = new (__FILE__ "(387)") CRsrcPackT<CRsrcDataDD>(info, data);
        nPkResult = addPackedRsrc(info, data, pRsrc, FALSE);
        break;
    case 44: {
        {
            CBuf tmp;
#line 400
            pRsrc = new (__FILE__ "(400)") CRsrcData(info, tmp);
        }
        bDeletable = TRUE;
        nPkResult = addPackedRsrc(info, data, pRsrc, FALSE);
        break;
    }
    case 17:
#line 409
        pRsrc = new (__FILE__ "(409)") CRsrcIMZ(info, data);
        nPkResult = addPackedRsrcIMZ(info, data, pRsrc);
        break;
    }
    if (0) TRACE("+++ Add Rsrc [%s] +++\n", info._pszName);
    if (0) TRACE("--- Del Rsrc-Contents [%s] ---\n", info._pszName);
    if (!pRsrc) {
        pRsrc = CRsrcDataFactory::createRsrcData(info, data);
        if (!pRsrc) goto end;
    }
    {
        SLContainerT<CRsrcData>* pContainer = _getRsrcList(pRsrc->getRsrcType()).pushBack(*pRsrc);
        if (pContainer) {
            HANDLE hHandle = pContainer->getHandle();
            pRsrc->setHandle(hHandle);
            pRsrc->setHandleToChildren(hHandle);
        }
    }
    if (nPkResult >= 0 && (nPkResult & 4)) {
        bDeletable = TRUE;
        pRsrc->delDataSrc();
    }
    nResult = 0;
    if (nRsrcType & 0x300) nResult |= 2;
end:
    if (bDeletable) nResult |= 4;
    if (ppOutRsrc) *ppOutRsrc = pRsrc;
    return nResult;
}

INT CRsrcDB::addPackedRsrc(const RsrcInfo& info, CBuf& data, CRsrcData* pParent, BOOL bFragment) {
    BYTE* pDataTop = data.getBufPtr();
    if (!pDataTop) return -1;
    char szName[22];
    CBuf buf;
    PackedRsrcHeader* pHead = (PackedRsrcHeader*)pDataTop;
    USHORT nMax = 0;
    PackedRsrcHeader::coodinateVersion(pHead);
    while (pHead->_pBody) {
        pHead++;
        nMax++;
        PackedRsrcHeader::coodinateVersion(pHead);
    }
    HANDLE* pChildTbl = NULL;
    if (pParent) pChildTbl = pParent->createChildHandleTbl(nMax);
    INT nAddResult = -1;
    CRsrcData* pRsrc;
    pHead = (PackedRsrcHeader*)pDataTop;
    for (UINT i = 0; i < nMax; i++, pHead++) {
        pHead->_pBody = pDataTop + (INT)pHead->_pBody;
        if (pHead->_cSize == 0) {
            TRACE("[%s] ...", pHead->_szName);
#line 516
            AssertApp(__FILE__, __LINE__, "RsrcFileSize = 0", 0);
        }
        buf.attachBuf((BYTE*)pHead->_pBody, pHead->_cSize);
        INT nRsrcType = 0;
        strncpy(szName, pHead->_szName, 21);
        szName[21] = '\0';
        RsrcInfo infoNew(info);
        infoNew._pszName = szName;
        infoNew._cOption = pHead->_cType;
        infoNew._cParam = pHead->_cPram;
        if (info._nRsrcType & 0x300) {
            CHAR* pszExt = strchr(szName, '.');
            if (pszExt) nRsrcType = getRsrcType(pszExt + 1);
            switch (nRsrcType) {
            case 2:
            case 16:
            case 17:
            case 25:
                infoNew._nRsrcType = (infoNew._nRsrcType & 0x700) | nRsrcType;
                nAddResult = addRsrc(infoNew, buf, &pRsrc);
                break;
            }
        } else {
            nRsrcType = info._nRsrcType & 0x7f;
            switch (nRsrcType) {
            case 23:
                nRsrcType = 1;
                break;
            case 18:
                nRsrcType = 0x20;
                break;
            case 33:
                nRsrcType = 0x22;
                break;
            case 37:
                nRsrcType = 0x26;
                break;
            case 44:
                nRsrcType = 0x2d;
                break;
            case 46:
                nRsrcType = 0x2f;
                break;
            case 49:
                nRsrcType = 0x32;
                break;
            case 51:
                nRsrcType = 0x34;
                break;
            case 57:
                nRsrcType = 0x3a;
                break;
            case 19:
                nRsrcType = 0x18;
                break;
            case 7:
                nRsrcType = 0x15;
                break;
            case 12:
            case 14:
            case 27:
            case 55: {
                CHAR* pszExt = strchr(szName, '.');
                if (pszExt) nRsrcType = getRsrcType(pszExt + 1);
                break;
            }
            }
            infoNew._nRsrcType = nRsrcType;
            nAddResult = addRsrc(infoNew, buf, &pRsrc);
        }
        if (pChildTbl) {
            HANDLE hChild = 0;
            if (nAddResult >= 0) {
                hChild = pRsrc->getHandle();
                if (nAddResult & 1) hChild |= 0x80000000;
                else pParent->m_nChildRest++;
            }
            pChildTbl[i] = hChild;
        }
    }
    if (pChildTbl && !pParent->m_nChildRest) return 4;
    return 0;
}

INT CRsrcDB::addPackedRsrcIMZ(const RsrcInfo& info, CBuf& data, CRsrcData* pParent) {
    BYTE* pDataTop = data.getBufPtr();
    if (!pDataTop) return -1;
    IMZ_INDEX* pImzIndex = (IMZ_INDEX*)(pDataTop + sizeof(IMZ_HEADER));
    HANDLE* pChildTbl = NULL;
    if (pParent) pChildTbl = pParent->createChildHandleTbl(((IMZ_HEADER*)pDataTop)->image_max);
    RsrcInfo infoTmp(info);
    char szName[22];
    strcpy(szName, info._pszName);
    CHAR* pExt = strstr(szName, ".imz");
    *pExt = ':';
    infoTmp._pszName = szName;
    infoTmp._nRsrcType = (infoTmp._nRsrcType & ~0x7f) | 0x10;
    CBuf bufTmp;
    pExt++;
    for (INT i = 0; i < ((IMZ_HEADER*)pDataTop)->image_max; i++) {
        sprintf(pExt, "%02d", i);
        bufTmp.attachBuf(pDataTop + pImzIndex[i].ofs, -1);
        CRsrcData* pRsrc;
        INT nAddResult = addRsrc(infoTmp, bufTmp, &pRsrc);
        if (pChildTbl) {
            HANDLE hChild = 0;
            if (nAddResult >= 0) {
                hChild = pRsrc->getHandle();
                if (nAddResult & 1) hChild |= 0x80000000;
                else pParent->m_nChildRest++;
            }
            pChildTbl[i] = hChild;
        }
    }
    if (pChildTbl && !pParent->m_nChildRest) return 4;
    return 0;
}

void CRsrcDB::addRsrcTag(CRsrcData& data, INT nGroupID, BOOL bSceneManaged) {
    INT nID = _setupGroupFlag(nGroupID, bSceneManaged);
    if (!data.hasGroupTag(nID)) {
        if (0) TRACE("Add group-tag only ... Name[ %s ], RsrcID[ %d ], GroupID[ %d ]\n", data.getName(),
                     data.getRsrcType(), nID);
        data.addGroupTag(nID);
    } else {
        if (0) TRACE("Detect group-tag... Skip... Name[ %s ], RsrcID[ %d ], GroupID[ %d ]\n", data.getName(),
                     data.getRsrcType(), nID);
    }
}

static void _delGroup(CRsrcList& inList, INT nGrpID, BOOL bForce) {
    INT nSceneGrpOpt = 0;
    if (!bForce) {
        AREA_SCENE as(CSceneInfo::getCurrentAreaScene());
        nSceneGrpOpt = CSceneInfo::getSceneCacheID(as);
        nSceneGrpOpt = (nSceneGrpOpt >= 0) ? GROUP_OPT_SCENE(nSceneGrpOpt) : 0;
    }
    for (SLContainerT<CRsrcData>* pContainer = inList.getHead(); pContainer; pContainer = pContainer->getNext()) {
        CRsrcData* pRsrc = pContainer->getValue();
        BOOL bDel = FALSE;
        if (pRsrc->delGroupTag(nGrpID)) {
            if (0) TRACE("--- Delete group-tag [%s] ---\n", pRsrc->getName());
            bDel = pRsrc->isGroupTagEmpty();
        }
        if (!bDel) {
            if ((pRsrc->getRsrcGroupID() & 0x0fffffff) == nGrpID) {
                if (bForce) {
                    bDel = TRUE;
                } else if (nSceneGrpOpt) {
                    pRsrc->delGroupTagOpt(nSceneGrpOpt);
                    bDel = pRsrc->isGroupTagEmpty();
                }
            }
        }
        if (bDel) {
            if (pRsrc->hasChild()) {
                if (0) TRACE("try Delete Rsrc [%s] ... but child survived...\n", pRsrc->getName());
            } else {
                if (0) TRACE("--- Delete Rsrc [%s] ---\n", pRsrc->getName());
                pContainer->setValue(NULL);
                if (pRsrc->getRsrcType() & 0x2000) DelayedDelete(pRsrc, FALSE);
                else delete pRsrc;
            }
        }
    }
    inList.sweep();
}

static void _delGroupOpt(CRsrcList& inList, INT nFlag) {
    for (SLContainerT<CRsrcData>* pContainer = inList.getHead(); pContainer; pContainer = pContainer->getNext()) {
        CRsrcData* pRsrc = pContainer->getValue();
        BOOL bDel = FALSE;
        if (pRsrc->delGroupTagOpt(nFlag)) bDel = pRsrc->isGroupTagEmpty();
        if (bDel || (pRsrc->getRsrcGroupID() & 0xf0000000) == nFlag) {
            if (!pRsrc->hasChild()) {
                pContainer->setValue(NULL);
                if (pRsrc->getRsrcType() & 0x2000) DelayedDelete(pRsrc, FALSE);
                else delete pRsrc;
            }
        }
    }
    inList.sweep();
}

void _removeRsrc(INT nID, BOOL bForce) {
    if (nID <= 0) return;
#line 835
    ASSERT(s_pRsrcListTbl);
    for (INT i = 0; i < RCID_MAX; i++) _delGroup(*s_pRsrcListTbl[i], nID, bForce);
#line 842
    ASSERT(s_pTexExList);
    _delGroup(*s_pTexExList, nID, bForce);
}

void CRsrcDB::removeRsrc(INT nID) { _removeRsrc(nID, FALSE); }

void CRsrcDB::removeRsrcForce(INT nID) { _removeRsrc(nID, TRUE); }

void CRsrcDB::removeRsrcWithType(INT nID, INT nRsrc) {
    if (nID <= 0) return;
#line 871
    ASSERT(s_pRsrcListTbl[ nRsrc ]);
    _delGroup(*s_pRsrcListTbl[nRsrc], nID, FALSE);
}

void CRsrcDB::removeRsrcWithTypeForce(INT nID, INT nRsrc) {
    if (nID <= 0) return;
#line 885
    ASSERT(s_pRsrcListTbl[ nRsrc ]);
    _delGroup(*s_pRsrcListTbl[nRsrc], nID, TRUE);
}

void CRsrcDB::removeGroup(INT nGroupID, INT nMemberID) { _removeRsrc(nGroupID * GROUP_ID_BASE + nMemberID, FALSE); }

void CRsrcDB::removeGroupForce(INT nGroupID, INT nMemberID) {
    _removeRsrc(nGroupID * GROUP_ID_BASE + nMemberID, TRUE);
}

void CRsrcDB::removeRsrc(CRsrcData& rsrc) {
    INT nRsrcType = rsrc.getRsrcType() & 0x7f;
    HANDLE hHandle = rsrc.getHandle();
#line 913
    ASSERT(s_pRsrcListTbl[ nRsrcType ]);
    CRsrcList& lst = *s_pRsrcListTbl[nRsrcType];
    for (SLContainerT<CRsrcData>* pContainer = lst.getHead(); pContainer; pContainer = pContainer->getNext()) {
        if (pContainer->getValue() && pContainer->getValue()->getHandle() == hHandle) {
            pContainer->setValue(NULL);
            if (rsrc.getRsrcType() & 0x2000) DelayedDelete(&rsrc, FALSE);
            else delete &rsrc;
        }
    }
    lst.sweep();
}

void CRsrcDB::removeSceneAttachedData(UINT nNum) {
    INT nFlag = GROUP_OPT_SCENE(nNum);
    for (INT i = 0; i < RCID_MAX; i++) _delGroupOpt(*s_pRsrcListTbl[i], nFlag);
#line 946
    ASSERT(s_pTexExList);
    _delGroupOpt(*s_pTexExList, nFlag);
}

void CRsrcDB::addStreamInfo(const CHAR* pszFile, INT nGroupID, UINT nStartSect, UINT nSectSize, INT nRsrcType) {
    RsrcInfo info;
    info._pszName = (CHAR*)pszFile;
    info._nGroupID = nGroupID;
    info._nRsrcType = nRsrcType;
    info._bSceneManaged = FALSE;
    if (getRsrc(nRsrcType, pszFile, NAME_CASE_SENSITIVE)) return;
#line 964
    CRsrcVSV* pRsrc = new (__FILE__ "(964)") CRsrcVSV(info, nStartSect, nSectSize);
    if (pRsrc) _getRsrcList(nRsrcType).pushBack(*pRsrc);
}

void CRsrcDB::addRsrcType(const CHAR* pszExt, INT nID) {
    if (!pszExt || !*pszExt || nID <= 0) return;
    if (getRsrcType(pszExt) >= 0) return;
#line 981
    CExtID* pExt = new (__FILE__ "(981)") CExtID(pszExt, nID);
#line 982
    ASSERT(s_pExtIDList);
    s_pExtIDList->pushBack(*pExt);
}

INT CRsrcDB::getRsrcType(const CHAR* pszExt) {
    if (pszExt && *pszExt) {
        RSRC_SJ_FOREACH(CExtID, pExt, *s_pExtIDList) {
            if (!strcmp(pExt->getExt(), pszExt)) return pExt->getID();
        }
    }
    return -1;
}

UINT CRsrcDB::getRsrcAlignmentSize(INT nRsrcType) {
    switch (nRsrcType) {
    case 0x1f:
        return 0x80;
    case 0x39:
        return 0x100;
    }
    return 0x40;
}

BOOL CRsrcDB::isSceneAttachMode() { return s_nSceneFlag >> 31; }

UINT CRsrcDB::getSceneAttachNum() { return (s_nSceneFlag >> 28) & 7; }

void CRsrcDB::setSceneAttachMode(UINT nNum) {
#line 1046
    ASSERT(nNum <= GROUP_OPT_SCENEID_MAX);
    s_nSceneFlag = GROUP_OPT_SCENE(nNum);
}

void CRsrcDB::clearSceneAttachMode() { s_nSceneFlag = 0; }

static void _resetRsrc(CRsrcList& lst, INT nArg) {
    for (SLContainerT<CRsrcData>* pContainer = lst.getHead(); pContainer; pContainer = pContainer->getNext()) {
        pContainer->getValue()->reset(nArg);
    }
}

void CRsrcDB::resetRsrc(INT nRsrcType, INT nArg) {
    if (nRsrcType < 0) {
        for (INT i = 0; i < RCID_MAX; i++) _resetRsrc(_getRsrcList(i), nArg);
    } else {
        _resetRsrc(_getRsrcList(nRsrcType), nArg);
    }
}

static CRsrcList& _getRsrcList(UINT nRsrcType) {
    if (nRsrcType & 0x800) {
#line 1100
        ASSERT(s_pTexExList);
        return *s_pTexExList;
    }
    INT nID = nRsrcType & 0x7f;
#line 1104
    ASSERT((RCID_MAX > nID) && s_pRsrcListTbl[nID]);
    return *s_pRsrcListTbl[nID];
}

CRsrcList& CRsrcDB::getRsrcList(UINT nRsrcType) { return _getRsrcList(nRsrcType); }

CRsrcData* CRsrcDB::getRsrc(const CHAR* pszName, NAME_CASE nc) {
    char szName[24];
    INT nRes;
    if (!s_pRsrcListTbl) return NULL;
    if (!pszName) return NULL;
    INT i;
    for (i = 0; *pszName && i < sizeof(szName) - 1; i++) szName[i] = *pszName++;
    szName[i] = '\0';
    for (INT i = 0; i < RCID_MAX; i++) {
        for (SLContainerT<CRsrcData>* pContainer = _getRsrcList(i).getHead(); pContainer;
             pContainer = pContainer->getNext()) {
            CRsrcData* pData = pContainer->getValue();
            if (nc == NAME_CASE_SENSITIVE) nRes = strcmp(pData->getName(), szName);
            else nRes = strcasecmp(pData->getName(), szName);
            if (!nRes) return pData;
        }
    }
    for (SLContainerT<CRsrcData>* pContainer = s_pTexExList->getHead(); pContainer;
         pContainer = pContainer->getNext()) {
        CRsrcData* pData = pContainer->getValue();
        if (nc == NAME_CASE_SENSITIVE) nRes = strcmp(pData->getName(), szName);
        else nRes = strcasecmp(pData->getName(), szName);
        if (!nRes) return pData;
    }
    return NULL;
}

static SLContainerT<CRsrcData>* _seekNext(SLContainerT<CRsrcData>*& pSeekContainer, INT nGrpID, BOOL bReal);

CRsrcData* CRsrcDB::getRsrc(INT nRsrcType, const CHAR* pszName, NAME_CASE nc) {
    char szName[24];
    INT i;
    for (i = 0; *pszName && i < sizeof(szName) - 1; i++) szName[i] = *pszName++;
    szName[i] = '\0';
    SLContainerT<CRsrcData>* pSeekContainer = _getRsrcList(nRsrcType).getHead();
    SLContainerT<CRsrcData>* pContainer;
    while ((pContainer = _seekNext(pSeekContainer, -1, FALSE))) {
        CRsrcData* pData = pContainer->getValue();
        if (pData->isMatchNameWithoutExt(szName, nc)) return pData;
    }
    return NULL;
}

CRsrcData* CRsrcDB::getRsrc(HANDLE hRsrc) {
    if (s_pRsrcSLCManager && hRsrc) return s_pRsrcSLCManager->getRsrc(hRsrc);
    return NULL;
}

CRsrcData* CRsrcDB::getRsrcWithHash(INT nRsrcType, INT nNameHash) {
    SLContainerT<CRsrcData>* pSeekContainer = _getRsrcList(nRsrcType).getHead();
    SLContainerT<CRsrcData>* pContainer;
    while ((pContainer = _seekNext(pSeekContainer, -1, FALSE))) {
        if (pContainer->getValue()->isMatchNameHash(nNameHash)) return pContainer->getValue();
    }
    return NULL;
}

CRsrcData* CRsrcDB::getRsrcWithIndex(INT nRsrcType, INT nIndex, BOOL bOnlyCurrentScene) {
    SLContainerT<CRsrcData>* pContainer = NULL;
    SLContainerT<CRsrcData>* pTopContainer = _getRsrcList(nRsrcType).getHead();
    SLContainerT<CRsrcData>* pSeekContainer = pTopContainer;
    CRsrcData* pData;
    while (pSeekContainer) {
        if (pSeekContainer->getValue() && pSeekContainer->getValue()->hasGroupTagOpt(s_nSceneFlag)) break;
        pSeekContainer = pSeekContainer->getNext();
    }
    if (pSeekContainer) {
        for (INT i = 0; i < nIndex; i++) pContainer = _seekNext(pSeekContainer, -1, FALSE);
    }
    pData = pContainer ? pContainer->getValue() : NULL;
    if (!pData && !bOnlyCurrentScene) pData = pTopContainer->getValue();
    return pData;
}

void* CRsrcDB::getRsrcData(const CRsrcData& data) { return data.m_pDataSrc ? (void*)data.m_pDataSrc->getBufPtr() : NULL; }

CRsrcData* CRsrcDB::getFirstRsrc(INT nRsrcType) {
    SLContainerT<CRsrcData>* pContainer = _getRsrcList(nRsrcType).getHead();
    return pContainer ? pContainer->getValue() : NULL;
}

void* CRsrcDB::getFirstData(INT nRsrcType) {
    SLContainerT<CRsrcData>* pContainer = _getRsrcList(nRsrcType).getHead();
    CRsrcData* pRsrc = pContainer ? pContainer->getValue() : NULL;
    return pRsrc ? pRsrc->getRsrcData() : NULL;
}

CRsrcData* CRsrcDB::getFirstRsrc(INT nRsrcType, INT nGroupID, BOOL bFollowLink) {
    SLContainerT<CRsrcData>* pFirstContainer = _getRsrcList(nRsrcType).getHead();
    SLContainerT<CRsrcData>* pSeekContainer = pFirstContainer;
    SLContainerT<CRsrcData>* pContainer = _seekNext(pSeekContainer, nGroupID, TRUE);
    if (!pContainer && bFollowLink) {
        pSeekContainer = pFirstContainer;
        pContainer = _seekNext(pSeekContainer, nGroupID, FALSE);
    }
    return pContainer ? pContainer->getValue() : NULL;
}

void* CRsrcDB::getFirstData(INT nRsrcType, INT nGroupID, BOOL bFollowLink) {
    CRsrcData* pRsrc = getFirstRsrc(nRsrcType, nGroupID, bFollowLink);
    return pRsrc ? pRsrc->getRsrcData() : NULL;
}

CRsrcData* CRsrcDB::getFirstRsrc_Scene(INT nRsrcType) {
    SLContainerT<CRsrcData>* pContainer = _getRsrcList(nRsrcType).getHead();
    CRsrcData* pTopRsrc = pContainer ? pContainer->getValue() : NULL;
    for (; pContainer; pContainer = pContainer->getNext()) {
        CRsrcData* pRsrc = pContainer->getValue();
        if (pRsrc && pRsrc->hasGroupTagOpt(s_nSceneFlag)) return pRsrc;
    }
    switch (nRsrcType) {
    case 4:
    case 9:
    case 13:
    case 32:
    case 34:
        return NULL;
    }
    return pTopRsrc;
}

void* CRsrcDB::getFirstData_Scene(INT nRsrcType) {
    CRsrcData* pRsrc = getFirstRsrc_Scene(nRsrcType);
    return pRsrc ? pRsrc->getRsrcData() : NULL;
}

CRsrcData* CRsrcDB::getFirstRsrc_Scene(INT nRsrcType, INT nGroupID) {
    CRsrcData* pRsrc = NULL;
    SLContainerT<CRsrcData>* pTopContainer = _getRsrcList(nRsrcType).getHead();
    SLContainerT<CRsrcData>* pSeekContainer = pTopContainer;
    SLContainerT<CRsrcData>* pContainer = _seekNext(pSeekContainer, nGroupID | s_nSceneFlag, FALSE);
    if (pContainer) pRsrc = pContainer->getValue();
    if (!pRsrc) {
        switch (nRsrcType) {
        case 4:
        case 9:
        case 13:
        case 32:
        case 34:
            break;
        default:
            pSeekContainer = pTopContainer;
            pContainer = _seekNext(pSeekContainer, nGroupID, FALSE);
            pRsrc = pContainer ? pContainer->getValue() : NULL;
            break;
        }
    }
    if (pRsrc) TRACE_COL(_TTY_PURPLE, "_getFirstEntryScene %s\n", pRsrc->getName());
    return pRsrc;
}

void* CRsrcDB::getFirstData_Scene(INT nRsrcType, INT nGroupID) {
    CRsrcData* pRsrc = getFirstRsrc_Scene(nRsrcType, nGroupID);
    return pRsrc ? pRsrc->getRsrcData() : NULL;
}

static SLContainerT<CRsrcData>* _seekNext(SLContainerT<CRsrcData>*& pSeekContainer, INT nGrpID, BOOL bReal) {
    SLContainerT<CRsrcData>* pContainer = NULL;
    while (pSeekContainer) {
        CRsrcData* pSeek = pSeekContainer->getValue();
        BOOL bBingo = nGrpID < 0;
        INT nID = nGrpID & 0x0fffffff;
        if (!bBingo) {
            if (!(nGrpID & 0xf0000000) || pSeek->hasGroupTagOpt(nGrpID & 0xf0000000)) {
                if (bReal) bBingo = pSeek->getRsrcGroupID() == nID;
                else bBingo = pSeek->getRsrcGroupID() == nID || pSeek->hasGroupTag(nGrpID);
            }
        }
        if (bBingo) pContainer = pSeekContainer;
        pSeekContainer = pSeekContainer->getNext();
        if (pContainer) break;
    }
    return pContainer;
}

void CRsrcDB::initSeekRsrc(INT nRsrcType, INT nGroupID) {
    s_pRsrcEnumerater = _getRsrcList(nRsrcType).getHead();
    s_nSeekRsrcType = nRsrcType;
    s_nSeekGrpID = nGroupID;
}

CRsrcData* CRsrcDB::seekNextRsrc() {
    SLContainerT<CRsrcData>* p = _seekNext(s_pRsrcEnumerater, s_nSeekGrpID, FALSE);
    return p ? p->getValue() : NULL;
}

static SLContainerT<CRsrcData>* _seekNext_Opt(SLContainerT<CRsrcData>*& pSeek, INT nOptFlag) {
    SLContainerT<CRsrcData>* pContainer = NULL;
    while (pSeek) {
        CRsrcData* pRsrc = pSeek->getValue();
        if (nOptFlag == -1 || pRsrc->hasGroupTagOpt(nOptFlag)) pContainer = pSeek;
        pSeek = pSeek->getNext();
        if (pContainer) break;
    }
    return pContainer;
}

CRsrcData* CRsrcDB::seekNextRsrc_Opt() {
    SLContainerT<CRsrcData>* p = _seekNext_Opt(s_pRsrcEnumerater, s_nSeekGrpID);
    return p ? p->getValue() : NULL;
}

CRsrcData* CRsrcDB::seekNextRsrc_Scene() {
    SLContainerT<CRsrcData>* p;
    if (s_nSeekGrpID < 0) p = _seekNext_Opt(s_pRsrcEnumerater, s_nSceneFlag);
    else p = _seekNext(s_pRsrcEnumerater, s_nSeekGrpID | s_nSceneFlag, FALSE);
    return p ? p->getValue() : NULL;
}

static SLContainerT<CRsrcData>* _seekNext_RcOpt(SLContainerT<CRsrcData>*& pSeek, INT nRsrcType, INT nGroupID) {
    SLContainerT<CRsrcData>* pContainer = NULL;
    INT nChkOpt = nRsrcType & 0xf000;
    while (pSeek) {
        CRsrcData* pRsrc = pSeek->getValue();
        INT nID = nGroupID & 0x0fffffff;
        if (nGroupID < 0 || pRsrc->getRsrcGroupID() == nID || pRsrc->hasGroupTag(nGroupID)) {
            if (pRsrc->getRsrcType() & nChkOpt) pContainer = pSeek;
        }
        pSeek = pSeek->getNext();
        if (pContainer) break;
    }
    return pContainer;
}

CRsrcData* CRsrcDB::seekNextRsrc_RcOpt() {
    SLContainerT<CRsrcData>* p = _seekNext_RcOpt(s_pRsrcEnumerater, s_nSeekRsrcType, -1);
    return p ? p->getValue() : NULL;
}

INT CRsrcDB::checkGroupID(INT nGroupOfs, INT nGroupID) {
    return (nGroupID < GROUP_ID_BASE) ? nGroupID + nGroupOfs : nGroupID;
}

UINT CRsrcDB::getRsrcGrpSize(INT nGroupID, INT nMemberID) {
    return CRcfCD::getRsrcFileSize(nGroupID * GROUP_ID_BASE + nMemberID);
}

UINT CRsrcDB::getRsrcGrpSizeOnMemory(INT nGroupID, INT nMemberID) {
    return CRcfCD::getRsrcFileSizeOnMemory(nGroupID * GROUP_ID_BASE + nMemberID);
}

static BOOL _hasGroup(CRsrcList& inList, INT nGrpID) {
    for (SLContainerT<CRsrcData>* pContainer = inList.getHead(); pContainer; pContainer = pContainer->getNext()) {
        if (pContainer->getValue()->hasGroupTag(nGrpID)) return TRUE;
    }
    return FALSE;
}

BOOL CRsrcDB::hasRsrcGroup(INT nGroupID, INT nMemberID) {
    INT nID = nGroupID * GROUP_ID_BASE + nMemberID;
    for (INT i = 0; i < RCID_MAX; i++) {
        if (_hasGroup(*s_pRsrcListTbl[i], nID)) return TRUE;
    }
    return s_pTexExList && _hasGroup(*s_pTexExList, nID);
}

CRsrcList& CRsrcDB::getScrollTexList() {
#line 1570
    ASSERT(s_pTexExList);
    return *s_pTexExList;
}

TexInfo* CRsrcDB::getRsrcTex(const CHAR* pszName, NAME_CASE nc) {
    CRsrcTex* pRsrc;
    if (!pszName) {
        TRACE_COL(_TTY_PURPLE, "!!!!!!!!!!! ERROR \x83" "e\x83N\x83X\x83`\x83\x83\x96\xbc\x82\xc9NULL\x82\xaa\x94\xad\x90\xb6\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81" "B !!!!!!!!!!!\n");
        pszName = "error";
        pRsrc = (CRsrcTex*)getRsrc(2, pszName, nc);
        if (!pRsrc) pRsrc = (CRsrcTex*)getRsrc(0x800, pszName, nc);
    } else {
        pRsrc = (CRsrcTex*)getRsrc(2, pszName, nc);
        if (!pRsrc) pRsrc = (CRsrcTex*)getRsrc(0x10, pszName, nc);
        if (!pRsrc) pRsrc = (CRsrcTex*)getRsrc(0x800, pszName, nc);
    }
    return pRsrc ? pRsrc->getTexInfo() : NULL;
}

MODELDATA** CRsrcDB::getRsrcMDL(const CHAR* pszName, NAME_CASE nc) {
    CRsrcMDL* pRsrc = (CRsrcMDL*)getRsrc(0xb, pszName, nc);
    if (!pRsrc) return NULL;
    return pRsrc->getMDLData();
}

MODELDATA** CRsrcDB::getRsrcMDL(INT nGroup) {
#line 1623
    ASSERT((INT)GROUP_ID_BASE < nGroup);
    CRsrcMDL* pRsrc = (CRsrcMDL*)getFirstRsrc(0xb, nGroup, TRUE);
    if (!pRsrc) return NULL;
    return pRsrc->getMDLData();
}

static inline void* _getRsrcDataByName(INT nRsrcType, const CHAR* pszName) {
    CRsrcData* pData = CRsrcDB::getRsrc(nRsrcType, pszName, NAME_CASE_SENSITIVE);
    return pData ? CRsrcDB::getRsrcData(*pData) : NULL;
}

MODELDATA** CRsrcDB::getRsrcSMD(const CHAR* pszName) {
    CRsrcData* pRsrc = getRsrc(0x2a, pszName, NAME_CASE_SENSITIVE);
    return pRsrc ? (MODELDATA**)pRsrc->getRsrcData() : NULL;
}

MODELDATA** CRsrcDB::getRsrcSMD(INT nGroup) {
#line 1647
    ASSERT((INT)GROUP_ID_BASE < nGroup);
    return (MODELDATA**)getFirstData(0x2a, nGroup, TRUE);
}

MODELDATA* CRsrcDB::getRsrcAMF(const CHAR* pszName, NAME_CASE nc) {
    CRsrcData* pRsrc = getRsrc(6, pszName, nc);
    return pRsrc ? (MODELDATA*)pRsrc->getRsrcData() : NULL;
}

ANIMDATA** CRsrcDB::getRsrcANM(const CHAR* pszName, NAME_CASE nc) {
    CRsrcData* pRsrc = getRsrc(8, pszName, nc);
    return pRsrc ? (ANIMDATA**)pRsrc->getRsrcData() : NULL;
}

BYTE* CRsrcDB::getRsrcSPD(const CHAR* pszName) { return (BYTE*)_getRsrcDataByName(0x38, pszName); }

BYTE* CRsrcDB::getRsrcSQD(const CHAR* pszName) { return (BYTE*)_getRsrcDataByName(0x3b, pszName); }

BYTE* CRsrcDB::getRsrcLAD(const CHAR* pszName) { return (BYTE*)_getRsrcDataByName(0x3c, pszName); }

CBuf* CRsrcDB::getRsrcBIN(const CHAR* pszName, NAME_CASE nc) {
    CRsrcData* pRsrc = getRsrc(1, pszName, nc);
    return pRsrc ? pRsrc->getDataSrc() : NULL;
}

CBuf* CRsrcDB::getRsrcABC(INT nAtelNo) {
    CRsrcData* pRsrc = getRsrcWithIndex(0x26, nAtelNo, TRUE);
    return pRsrc ? pRsrc->getDataSrc() : NULL;
}

CBuf* CRsrcDB::getRsrcMCD(INT nMapRsrcGrpID) {
    CRsrcData* pRsrc;
    if (nMapRsrcGrpID) pRsrc = getFirstRsrc(4, nMapRsrcGrpID, TRUE);
    else pRsrc = getFirstRsrc_Scene(4);
    return pRsrc ? pRsrc->getDataSrc() : NULL;
}

BYTE* CRsrcDB::getRsrcOCD(INT nGroup) {
#line 1728
    ASSERT((INT)GROUP_ID_BASE < nGroup);
    return (BYTE*)getFirstData(0x1e, nGroup, TRUE);
}

EFFECT_DATA* CRsrcDB::getRsrcDPX(const CHAR* pszName) {
    CRsrcDPX* pRsrc = (CRsrcDPX*)getRsrc(5, pszName, NAME_CASE_SENSITIVE);
    if (pRsrc) return pRsrc->getDPXData();
    char szDpxName[24];
    if (*pszName == '_') {
        strncpy(szDpxName, pszName + 1, 23);
    } else {
        szDpxName[0] = '_';
        strncpy(szDpxName + 1, pszName, 23);
    }
    szDpxName[23] = '\0';
    pRsrc = (CRsrcDPX*)getRsrc(5, szDpxName, NAME_CASE_SENSITIVE);
    return pRsrc ? pRsrc->getDPXData() : NULL;
}

SBYTE* CRsrcDB::getRsrcDPD(const CHAR* pszName) { return (SBYTE*)_getRsrcDataByName(0xa, pszName); }

TEXANM_DATA* CRsrcDB::getRsrcTXA(INT nGroup) {
#line 1774
    ASSERT((INT)GROUP_ID_BASE < nGroup);
    return (TEXANM_DATA*)getFirstData(0x18, nGroup, FALSE);
}

EFFECT_SE* CRsrcDB::getRsrcESE(const CHAR* pszName) {
    EFFECT_SE* pData = (EFFECT_SE*)_getRsrcDataByName(0x3e, pszName);
    if (!pData) {
        char szEseName[24];
        if (*pszName == '_') {
            strncpy(szEseName, pszName + 1, 23);
        } else {
            szEseName[0] = '_';
            strncpy(szEseName + 1, pszName, 23);
        }
        szEseName[23] = '\0';
        pData = (EFFECT_SE*)_getRsrcDataByName(0x3e, szEseName);
    }
    return pData;
}

BYTE* CRsrcDB::getRsrcEET(const CHAR* pszName) { return (BYTE*)_getRsrcDataByName(0x1a, pszName); }

void* CRsrcDB::getRsrcCAM(const CHAR* pszName) { return _getRsrcDataByName(0x1c, pszName); }

MEP_DATA* CRsrcDB::getRsrcMEP(INT nGroup) {
#line 1826
    ASSERT((INT)GROUP_ID_BASE < nGroup);
    return (MEP_DATA*)getFirstData(0x28, nGroup, TRUE);
}

BYTE* CRsrcDB::getRsrcPTD(INT nPtdNo) {
    CRsrcData* pRsrc = getRsrcWithIndex(0x32, nPtdNo, TRUE);
    return pRsrc ? pRsrc->getDataSrc()->getBufPtr() : NULL;
}

BYTE* CRsrcDB::getRsrcPTD(const CHAR* pszName) {
    CRsrcData* pRsrc = getRsrc(0x32, pszName, NAME_CASE_SENSITIVE);
    return pRsrc ? pRsrc->getDataSrc()->getBufPtr() : NULL;
}

CHAR* CRsrcDB::getRsrcPTDName(INT nPtdNo) {
    CRsrcData* pRsrc = getRsrcWithIndex(0x32, nPtdNo, TRUE);
    return pRsrc ? pRsrc->getName() : NULL;
}

BYTE* CRsrcDB::getRsrcSES(INT nGroup) {
#line 1864
    ASSERT((INT)GROUP_ID_BASE < nGroup);
    return (BYTE*)getFirstData(0x30, nGroup, TRUE);
}

BYTE* CRsrcDB::getRsrcMMP(INT nGroup, INT nPtdNo) {
    CRsrcMMP* pRsrc = (CRsrcMMP*)getFirstRsrc(0x33, nGroup, TRUE);
    return pRsrc ? pRsrc->getMMPData(nPtdNo) : NULL;
}

BYTE* CRsrcDB::getRsrcMMP(INT nGroup, const CHAR* pszName) {
    CRsrcMMP* pRsrc = (CRsrcMMP*)getFirstRsrc(0x33, nGroup, TRUE);
    return pRsrc ? pRsrc->getMMPData(pszName) : NULL;
}

CHAR* CRsrcDB::getRsrcMMPName(INT nGroup, INT nPtdNo) {
    CRsrcMMP* pRsrc = (CRsrcMMP*)getFirstRsrc(0x33, nGroup, TRUE);
    return pRsrc ? pRsrc->getMMPName(nPtdNo) : NULL;
}

BYTE* CRsrcDB::getRsrcCTD(INT nGroup, INT nPtdNo) {
    CRsrcCTD* pRsrc = (CRsrcCTD*)getFirstRsrc(0x36, nGroup, TRUE);
    return pRsrc ? pRsrc->getCTDData(nPtdNo) : NULL;
}

BYTE* CRsrcDB::getRsrcCTD(INT nGroup, const CHAR* pszName) {
    CRsrcCTD* pRsrc = (CRsrcCTD*)getFirstRsrc(0x36, nGroup, TRUE);
    return pRsrc ? pRsrc->getCTDData(pszName) : NULL;
}

CHAR* CRsrcDB::getRsrcCTDName(INT nGroup, INT nPtdNo) {
    CRsrcCTD* pRsrc = (CRsrcCTD*)getFirstRsrc(0x36, nGroup, TRUE);
    return pRsrc ? pRsrc->getCTDName(nPtdNo) : NULL;
}

INT CRsrcDB::getModelCount(const CHAR* pszName, NAME_CASE nc) {
    CRsrcMDL* pRsrc = (CRsrcMDL*)getRsrc(0xb, pszName, nc);
    return pRsrc ? pRsrc->getMDLCount() : 0;
}

INT CRsrcDB::getModelCount(INT nGroup) {
#line 2005
    ASSERT((INT)GROUP_ID_BASE < nGroup);
    CRsrcMDL* pRsrc = (CRsrcMDL*)getFirstRsrc(0xb, nGroup, TRUE);
    return pRsrc ? pRsrc->getMDLCount() : 0;
}

INT CRsrcDB::getSModelCount(const CHAR* pszName) {
    CRsrcMDL* pRsrc = (CRsrcMDL*)getRsrc(0x2a, pszName, NAME_CASE_SENSITIVE);
    return pRsrc ? pRsrc->getMDLCount() : 0;
}

INT CRsrcDB::getSModelCount(INT nGroup) {
#line 2026
    ASSERT((INT)GROUP_ID_BASE < nGroup);
    CRsrcMDL* pRsrc = (CRsrcMDL*)getFirstRsrc(0x2a, nGroup, TRUE);
    return pRsrc ? pRsrc->getMDLCount() : 0;
}

INT CRsrcDB::getAnimeCount(const CHAR* pszName, NAME_CASE nc) { return getMotionCount(pszName, nc); }

INT CRsrcDB::getAnimeCount(INT nGroup) {
    CRsrcANN* pRsrc = (CRsrcANN*)getFirstRsrc(0x1d, nGroup, TRUE);
    return pRsrc ? pRsrc->getMotCount() : 0;
}

INT CRsrcDB::getMotionCount(const CHAR* pszName, NAME_CASE nc) {
    CRsrcANN* pRsrc = (CRsrcANN*)getRsrc(0x1d, pszName, nc);
    return pRsrc ? pRsrc->getMotCount() : 0;
}

CMotData* CRsrcDB::getMotionData(const CHAR* pszName, INT nMotNo, NAME_CASE nc) {
    CRsrcANN* pRsrc = (CRsrcANN*)getRsrc(0x1d, pszName, nc);
    if (!pRsrc) return NULL;
    return pRsrc->getMotData(nMotNo);
}

CRsrcANN* CRsrcDB::getRsrcANN(INT nGroup) { return (CRsrcANN*)getFirstRsrc(0x1d, nGroup, TRUE); }

CRsrcANN* CRsrcDB::getRsrcANN(const CHAR* pszName, NAME_CASE nc) { return (CRsrcANN*)getRsrc(0x1d, pszName, nc); }

INT CRsrcDB::getMotionIdxFromMotName(const CHAR* pszName, const CHAR* pszMotName, NAME_CASE nc) {
    INT n = getMotionCount(pszName, nc);
    if (!n) return -1;
    for (INT i = 0; i < n; i++) {
        CMotData* md = getMotionData(pszName, i, nc);
        if (md) {
            INT r;
            if (nc == NAME_CASE_SENSITIVE) r = strcmp((CHAR*)md, pszMotName);
            else r = strcasecmp((CHAR*)md, pszMotName);
            if (!r) return i;
        }
    }
    return -1;
}
// SOURCE DATA: .rodata .bss
