
#define RSRCDATA_H_FULL
#define RSRCDATA_H_PATH "c:/KHCOM/MainApp/System/RsrcData.h"
#define CTDMAN_H_PATH "c:/KHCOM/MainApp/System/CTD/CtdMan.h"
#define MODELDATA_H_TEXINFO /* MATCH: TexInfoStub (the original has its assert strings) */
#define MODELDATA_H_TEXINFO_CTOR /* MATCH: RSRC_NEW(TexInfo) in _setupTexData runs the constructor (memset) */
#include <string.h>
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Ctd/CtdMan.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/Effect/effect.h"
#include "System/RsrcDB.h"
#include "System/Graphic/TexUtil.h"
#include "System/VramMan.h"
#include "System/Graphic/ModelMan.h"
#include "System/Graphic/MotData.h"
#include "System/Graphic/FrecFont.h"
#include "System/Graphic/TexAnm.h"
#include "System/Sound/SoundMan.h"
#include "System/Debug.h"
#include "System/MainApp.h"
#include <string.h>


namespace D2 {
class C2DMan {
public:
    static HANDLE init(void* inFile, const CHAR* pBaseName);
    static void destroyBase(HANDLE handleBase);
};
}  // namespace D2

extern "C" {
TIM2_PICTUREHEADER* Tim2GetPictureHeader(void* pTim2, int imgno);
int snDllLoaded(void* pDll, void* pParam);
int snDllUnload(void* pDll);
void* snDllGetFreeMemory(void* pDll);
}

inline void operator delete[](void* p) { com_free(p); }
inline void* operator new[](size_t size, const char* pszFile) throw() {
    UINT nRA = RSRCDATA_RET_ADDR();
    return com_malloc_d(size, (char*)pszFile, nRA);
}

#define RSRC_NEW(line, T) new (__FILE__ "(" #line ")") T
#define RSRC_ALLOC(line, n) operator new(n, __FILE__ "(" #line ")")
#define _RSRC_STR(e) #e
#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, _RSRC_STR(e), 0)

static CRsrcData* createRsrcTex(const RsrcInfo& info, CBuf& data);
static CRsrcData* createRsrcFont(const RsrcInfo& info, CBuf& data);

CRsrcData* CRsrcDataFactory::createRsrcData(const RsrcInfo& info, CBuf& data) {
    CRsrcData* pRsrc = NULL;
    INT nRsrcType = info._nRsrcType & 0x7f;
    switch (nRsrcType) {
    case 1:
    case 10:
    case 20:
    case 21:
    case 26:
    case 30:
    case 32:
    case 34:
    case 35:
    case 36:
    case 38:
    case 50:
    case 58:
    case 62:
#line 79
        pRsrc = RSRC_NEW(79, CRsrcData)(info, data);
        break;
    case 13:
#line 82
        pRsrc = RSRC_NEW(82, CRsrcData)(info, data);
        break;
    case 25:
        pRsrc = createRsrcFont(info, data);
        break;
    case 2:
    case 16:
        pRsrc = createRsrcTex(info, data);
        break;
    case 3:
#line 89
        pRsrc = RSRC_NEW(89, CRsrcAAF)(info, data);
        break;
    case 4:
#line 91
        pRsrc = RSRC_NEW(91, CRsrcMCD)(info, data);
        break;
    case 6:
#line 93
        pRsrc = RSRC_NEW(93, CRsrcAMF)(info, data);
        break;
    case 8:
#line 95
        pRsrc = RSRC_NEW(95, CRsrcANM)(info, data);
        break;
    case 29:
#line 97
        pRsrc = RSRC_NEW(97, CRsrcANN)(info, data);
        break;
    case 9:
#line 99
        pRsrc = RSRC_NEW(99, CRsrcAZF)(info, data);
        break;
    case 11:
    case 42:
#line 102
        pRsrc = RSRC_NEW(102, CRsrcMDL)(info, data);
        break;
    case 28:
#line 106
        pRsrc = RSRC_NEW(106, CRsrcCAM)(info, data);
        break;
    case 31:
#line 108
        pRsrc = RSRC_NEW(108, CRsrcDLL)(info, data);
        break;
    case 39:
#line 110
        pRsrc = RSRC_NEW(110, CRsrcData)(info, data);
        break;
    case 40:
#line 112
        pRsrc = RSRC_NEW(112, CRsrcMEP)(info, data);
        break;
    case 45:
#line 118
        pRsrc = RSRC_NEW(118, CRsrcSND)(info, data);
        break;
    case 47:
#line 119
        pRsrc = RSRC_NEW(119, CRsrcRSD)(info, data);
        break;
    case 48:
#line 121
        pRsrc = RSRC_NEW(121, CRsrcSES)(info, data);
        break;
    case 51:
    case 52:
#line 124
        pRsrc = RSRC_NEW(124, CRsrcMMP)(info, data);
        break;
    case 54:
#line 126
        pRsrc = RSRC_NEW(126, CRsrcCTD)(info, data);
        break;
    case 53:
    case 61:
#line 129
        pRsrc = RSRC_NEW(129, CRsrcVSV)(info, data);
        break;
    case 56:
#line 131
        pRsrc = RSRC_NEW(131, CRsrcSPD)(info, data);
        break;
    case 59:
#line 132
        pRsrc = RSRC_NEW(132, CRsrcSQD)(info, data);
        break;
    case 60:
#line 133
        pRsrc = RSRC_NEW(133, CRsrcLAD)(info, data);
        break;
    case 5:
#line 135
        pRsrc = RSRC_NEW(135, CRsrcDPX)(info, data);
        break;
    case 24:
#line 136
        pRsrc = RSRC_NEW(136, CRsrcTXA)(info, data);
        break;
    case 22:
    case 41:
#line 140
        pRsrc = RSRC_NEW(140, CRsrcALF)(info, data);
        break;
    }
    return pRsrc;
}

static CRsrcData* createRsrcTex(const RsrcInfo& info, CBuf& data) {
    RsrcInfo infoTmp(info);
    if (infoTmp._cOption & 0x30) infoTmp._nRsrcType |= 0x800;
#line 154
    CRsrcTex* pTex = RSRC_NEW(154, CRsrcTex)(infoTmp, data);
    CRsrcData* pRsrc = pTex;
    if (!pTex->getTexInfo()) {
        delete pTex;
        CBuf pTmp;
        pTmp.allocBuf(data.getBufSize());
        memcpy(pTmp.getBufPtr(), data.getBufPtr(), data.getBufSize());
        infoTmp._cOption = 2;
        infoTmp._cParam = 3;
#line 167
        pRsrc = RSRC_NEW(167, CRsrcTex)(infoTmp, pTmp);
    }
    return pRsrc;
}

static CRsrcData* createRsrcFont(const RsrcInfo& info, CBuf& data) {
    CFrecFont::setupFontTex(info, data);
    return NULL;
}

struct OldPackedRsrcHeader {
    char _szName[16];
    void* _pBody;
    BYTE _cType;
    BYTE _cPram;
    BYTE _pad[6];
    INT _cSize;
};

void PackedRsrcHeader::coodinateVersion(void* pHeader) {
    OldPackedRsrcHeader* pOld = (OldPackedRsrcHeader*)pHeader;
    if (pOld->_cSize < 0) {
        pOld->_cSize &= 0x7fffffff;
        return;
    }
    PackedRsrcHeader tmpHead;
    tmpHead._pBody = pOld->_pBody;
    strncpy(tmpHead._szName, pOld->_szName, 21);
    tmpHead._szName[21] = '\0';
    tmpHead._cType = pOld->_cType;
    tmpHead._cPram = pOld->_cPram;
    tmpHead._cSize = pOld->_cSize;
    memcpy(pHeader, &tmpHead, sizeof(PackedRsrcHeader));
}

INT CRsrcGroupTag::getID() const { return m_nID & 0x0fffffff; }

INT CRsrcGroupTag::getOptFlag() const { return m_nID & 0xf0000000; }

BOOL CRsrcGroupTag::isOpt(INT nOptFlag) const { return (m_nID & 0xf0000000) == nOptFlag; }

BOOL CRsrcGroupTagList::hasTag(INT nGrpID) const {
    nGrpID &= 0x0fffffff;
    RSRC_SJ_FOREACH(CRsrcGroupTag, pEntry, *this) {
        if (pEntry->getID() == nGrpID) return TRUE;
    }
    return FALSE;
}

BOOL CRsrcGroupTagList::hasTagGID(INT nGrpID) const {
    RSRC_SJ_FOREACH(CRsrcGroupTag, pEntry, *this) {
        TRACE("GID = %x\n", pEntry->getID());
        if (pEntry && (pEntry->getID() & nGrpID)) return TRUE;
    }
    return FALSE;
}

BOOL CRsrcGroupTagList::hasTagOpt(INT nFlag) const {
    RSRC_SJ_FOREACH(CRsrcGroupTag, pEntry, *this) {
        if (pEntry->isOpt(nFlag)) return TRUE;
    }
    return FALSE;
}

void CRsrcGroupTagList::addTag(INT nGrpID) {
#line 280
    CRsrcGroupTag* p = RSRC_NEW(280, CRsrcGroupTag)(nGrpID);
    if (p) pushBack(*p);
}

BOOL CRsrcGroupTagList::delTag(INT nGrpID) {
    BOOL bRes = FALSE;
    nGrpID &= 0x0fffffff;
    RSRC_SJ_FOREACH(CRsrcGroupTag, pEntry, *this) {
        if (pEntry->getID() == nGrpID) {
            remove(*pEntry);
            delete pEntry;
            bRes = TRUE;
        }
    }
    return bRes;
}

BOOL CRsrcGroupTagList::delTagOpt(INT nFlag) {
    BOOL bRes = FALSE;
    RSRC_SJ_FOREACH(CRsrcGroupTag, pEntry, *this) {
        if (pEntry->isOpt(nFlag)) {
            remove(*pEntry);
            delete pEntry;
            bRes = TRUE;
        }
    }
    return bRes;
}

void CRsrcGroupTagList::dumpTag() const {
    RSRC_SJ_FOREACH(CRsrcGroupTag, pEntry, *this) TRACE("[%d]", pEntry->getID());
}

void CRsrcData::init(const RsrcInfo& info) {
    if (info._pszName) {
        strncpy(m_szName, info._pszName, 23);
        m_szName[23] = '\0';
    } else {
        m_szName[0] = '\0';
    }
    m_nRsrcType = info._nRsrcType;
    m_hThis = -1;
    m_hParent = 0;
    m_phChildren = NULL;
    m_nChildCount = 0;
    m_nChildRest = 0;
    m_nRsrcGroup = info._nGroupID & 0x0fffffff;
    INT nRefGroup = info._nRefGroupID;
    if (!nRefGroup) nRefGroup = info._nGroupID;
    addGroupTag(nRefGroup);
}

CRsrcData::~CRsrcData() {
    if (m_pDataSrc) delete m_pDataSrc;
    if (m_phChildren) {
        setHandleToChildren(0);
        delete m_phChildren;
    }
    if (m_hParent) {
        CRsrcData* pParentRsrc = CRsrcDB::getRsrc(m_hParent);
        if (pParentRsrc) pParentRsrc->unregisterChild(m_hThis);
    }
}

HANDLE* CRsrcData::createChildHandleTbl(USHORT n) {
#line 393
    ASSERT((NULL == m_phChildren) && n);
    m_phChildren = (HANDLE*)com_malloc_d(n * sizeof(HANDLE), __FILE__ "(394)", (UINT)-1);
#line 395
    ASSERT(m_phChildren);
    m_nChildCount = n;
    m_nChildRest = 0;
    return m_phChildren;
}

void CRsrcData::addGroupTag(INT nID) {
    m_refTagList.addTag(nID);
    if (m_nChildCount) {
        HANDLE* p = m_phChildren;
#line 412
        ASSERT(p);
        for (UINT i = 0; i < m_nChildCount; i++, p++) {
            CRsrcData* pChild = CRsrcDB::getRsrc(*p & 0x7fffffff);
            if (pChild) {
                if (0) TRACE("> Add group-tag only ... Name[ %s ], RsrcID[ %d ], GroupID[ %d ]\n", pChild->getName(),
                             pChild->getRsrcType(), nID);
                pChild->addGroupTag(nID);
            }
        }
    }
}

void CRsrcData::setHandleToChildren(HANDLE hParent) {
    UINT nMax = m_nChildCount;
    if (!m_phChildren || !nMax) return;
    for (UINT i = 0; i < nMax; i++) {
        HANDLE h = m_phChildren[i];
        if (!(h & 0x80000000)) {
            CRsrcData* pData = CRsrcDB::getRsrc(h);
            if (pData) pData->setParentHandle(hParent);
        }
    }
}

void CRsrcData::unregisterChild(HANDLE hChild) {
    UINT nMax = m_nChildCount;
    if (m_phChildren) {
        if (!nMax || !m_nChildRest) return;
        for (UINT i = 0; i < nMax; i++) {
            if (hChild == m_phChildren[i]) {
                m_phChildren[i] = 0;
                m_nChildRest--;
            }
        }
        if (!m_nChildRest && isGroupTagEmpty()) {
            if (0) TRACE("--- Delete Rsrc [%s] --- ... from child.\n", getName());
            CRsrcDB::removeRsrc(*this);
        }
    }
}

CRsrcDataDD::CRsrcDataDD(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) { m_nRsrcType |= 0x2000; }

CRsrcDataDD::CRsrcDataDD(const RsrcInfo& info) : CRsrcData(info) { m_nRsrcType |= 0x2000; }

CRsrcTex::~CRsrcTex() {
    TexInfo* pInfo = m_pTexInfo;
    if (pInfo) {
        INT nID = getRsrcType();
        if (nID & 0x100) CVramMan::releaseTex(pInfo->_TexInfo._nDistPos, getAllocType());
        if (nID & 0x200) CVramMan::releaseClut(m_pTexInfo->_ClutInfo._nDistPos, getAllocType());
        com_free(m_pTexInfo->_pExtendInfo0);
        com_free(m_pTexInfo->_pExtendInfo1);
        com_free(m_pTexInfo->_pExtendInfo2);
        com_free(m_pTexInfo);
        if (m_pScr) com_free(m_pScr);
        m_pScr = NULL;
    }
}

static inline TexInfo* _newTexInfo(TexInfo* p) {
    if (p) memset(p, 0, sizeof(TexInfo));
    return p;
}

void CRsrcTex::_setupTexData(CBuf& data, INT nType, INT nOpt) {
    BYTE* pSrc = data.getBufPtr();
    BOOL bImd = (getRsrcType() & 0x7f) == 0x10;
    INT nAllocType = getAllocType();
#line 535
    m_pTexInfo = RSRC_NEW(535, TexInfo);
#line 536
    ASSERT(m_pTexInfo);
    BOOL bResult;
    if (!bImd) bResult = CTexUtil::setupTexInfo(*m_pTexInfo, pSrc, nAllocType);
    else bResult = CTexUtil::setupTexInfoImd(*m_pTexInfo, pSrc, nAllocType);
    if (bResult) {
        CBuf* pNewData = NULL;
        switch (nAllocType) {
        case 1:
            m_pTexInfo->_nFlag |= 3;
            m_pTexInfo->_nType = 2;
            m_pTexInfo->_pName = m_szName;
            break;
        case 0:
            CVramMan::reqTransTex(*m_pTexInfo, data);
            m_pTexInfo->_TexInfo._pImage = NULL;
            m_pTexInfo->_ClutInfo._pImage = NULL;
            m_pTexInfo->_nType = 2;
            m_pTexInfo->_pName = m_szName;
            break;
        case 3: {
    #line 576
            pNewData = RSRC_NEW(576, CBuf);
            data.copyTo(*pNewData);
            m_pTexInfo->_nInfoType = 0;
            m_pTexInfo->_nType = nType;
            m_pTexInfo->_nParam = nOpt;
            m_pTexInfo->_pName = m_szName;
            TIM2_PICTUREHEADER* pHd = NULL;
            if (!bImd) pHd = Tim2GetPictureHeader(pSrc, 0);
            if (pHd) {
                m_pTexInfo->_fOfs[0] = pHd->fSOfs;
                m_pTexInfo->_fOfs[1] = pHd->fTOfs;
            } else {
                m_pTexInfo->_fOfs[0] = 0.0f;
                m_pTexInfo->_fOfs[1] = 0.0f;
            }
            if (nType & 0x30) {
    #line 598
                m_pScr = (TexMemScroll*)RSRC_ALLOC(598, sizeof(TexMemScroll));
    #line 599
                ASSERT(m_pScr);
                m_pScr->m_UvTime = 0;
                m_pScr->m_UvStatus = 0;
                if (nOpt) m_pScr->m_UvFps = nOpt;
                else m_pScr->m_UvFps = 1;
                m_pScr->m_NowLine = 0;
                m_pScr->m_MaxLine = m_pTexInfo->_TexInfo._nDistW;
            }
            if (nType & 0x40) {
                INT extend_size = CTexUtil::getClutSize(pSrc);
                ULONGLONG* pSrc = m_pTexInfo->_ClutInfo._pImage;
                ULONGLONG* p = pSrc;
    #line 616
                m_pTexInfo->_pExtendInfo0 = RSRC_NEW(616, TexInfo);
                _setupExTexData(m_pTexInfo->_pExtendInfo0, pSrc, p, extend_size, 1);
                p = m_pTexInfo->_pExtendInfo0->_ClutInfo._pImage;
    #line 620
                m_pTexInfo->_pExtendInfo1 = RSRC_NEW(620, TexInfo);
                _setupExTexData(m_pTexInfo->_pExtendInfo1, pSrc, p, extend_size, 2);
                p = m_pTexInfo->_pExtendInfo1->_ClutInfo._pImage;
    #line 624
                m_pTexInfo->_pExtendInfo2 = RSRC_NEW(624, TexInfo);
                _setupExTexData(m_pTexInfo->_pExtendInfo2, pSrc, p, extend_size, 3);
            }
            break;
        }
        }
    if (pNewData) setDataSrc(*pNewData);
    } else {
        TRACE("\x83" "e\x83N\x83X\x83`\x83\x83\x83" "f\x81[\x83^\x82\xcc\x83Z\x83" "b\x83g\x83" "A\x83" "b\x83v\x82\xc9\x8e\xb8\x94s %s\n",
              m_szName);
        if (m_pTexInfo) com_free(m_pTexInfo);
        m_pTexInfo = NULL;
    }
}

void CRsrcTex::_setupExTexData(TexInfo* exTexInfo, ULONGLONG* pSrc, ULONGLONG* pClut, INT nClutSize, INT nOpt) {
    ULONGLONG* pp = (ULONGLONG*)((BYTE*)pClut + nClutSize);
    INT inColNum = (m_pTexInfo->_ClutInfo._nTransSize == 1) ? 0x10 : 0x100;
    CTexUtil::makeCustomClut(pSrc, pp, nOpt, inColNum);
    *exTexInfo = *m_pTexInfo;
    exTexInfo->_ClutInfo._pImage = pp;
    exTexInfo->_pExtendInfo0 = m_pTexInfo;
    exTexInfo->_pExtendInfo1 = m_pTexInfo;
    exTexInfo->_pExtendInfo2 = m_pTexInfo;
    exTexInfo->_nInfoType = nOpt;
}

INT CRsrcTex::getAllocType() const {
    INT nType = 3;
    if (getRsrcType() & 0x400) nType = 1;
    else if (getRsrcType() & 0x300) nType = 0;
    return nType;
}

void CRsrcMDL::_setupMDL() {
    MODELDATA** ppTop = (MODELDATA**)getRsrcData();
    if (!ppTop) return;
    INT nTexErr = 0;
    m_nAMFCount = 0;
    for (MODELDATA** ppAmd = ppTop; *ppAmd; ppAmd++) {
        *ppAmd = (MODELDATA*)((INT)*ppAmd + (INT)ppTop);
        if (*ppAmd && !CRsrcAMF::setupAMF(*ppAmd, TRUE)) {
            *ppAmd = (MODELDATA*)((UINT)*ppAmd - 0x80000000U);
            nTexErr++;
        }
        m_nAMFCount++;
    }
    if (nTexErr) m_nFlag |= 1;
}

void CRsrcMDL::reSetupTex() {
    MODELDATA** ppTop = (MODELDATA**)getRsrcData();
    if (ppTop) {
        MODELDATA** ppAmd = ppTop;
        for (; *ppAmd; ppAmd++) {
            if ((UINT)*ppAmd & 0x80000000) {
                *ppAmd = (MODELDATA*)((UINT)*ppAmd & 0x7fffffff);
                CRsrcAMF::setupAMFTex(*ppAmd, FALSE);
            }
        }
        m_nFlag &= ~1;
    }
}

void CRsrcAMF::_setupAMF() { setupAMF((MODELDATA*)getRsrcData(), FALSE); }

BOOL CRsrcAMF::setupAMF(MODELDATA* pData, BOOL bExitOnTexErr) {
    if (!pData) return FALSE;
    if (pData->m_pBoneLabel) pData->m_pBoneLabel = (void*)((INT)pData->m_pBoneLabel + (INT)pData);
    if (pData->m_pBones) pData->m_pBones = (void*)((INT)pData->m_pBones + (INT)pData);
    pData->m_pTexParam = (void*)((INT)pData->m_pTexParam + (INT)pData);
    pData->m_pData = (void*)((INT)pData->m_pData + (INT)pData);
    if (pData->m_pSemiData) pData->m_pSemiData = (void*)((INT)pData->m_pSemiData + (INT)pData);
    pData->m_pBBox = (BBOX*)((INT)pData->m_pBBox + (INT)pData);
    return setupAMFTex(pData, bExitOnTexErr);
}

#include "Game/Map.h"













struct TEXPARAM {
    char m_szName[16];
    TexInfoStub m_Info;
    unsigned char dummy[8];
};

BOOL CRsrcAMF::setupAMFTex(MODELDATA* pData, BOOL bExitOnErr) {
    if (!pData) return FALSE;
    SHORT nTmpTex = 0;
    SHORT nTmpClut = 0;
    USHORT nTransTexCount = 0;
    for (INT i = 0; i < pData->m_nTexNum; i++) {
        TEXPARAM* pParam = &((TEXPARAM*)pData->m_pTexParam)[i];
        TexInfo* pInfo = CRsrcDB::getRsrcTex(pParam->m_szName, NAME_CASE_SENSITIVE);
        if (!pInfo) {
            if (bExitOnErr) return FALSE;
            pInfo = CRsrcDB::getRsrcTex("error", NAME_CASE_SENSITIVE);
#line 813
            ASSERT(pInfo);
        }
        pParam->m_Info.init(pInfo, nTmpTex, nTmpClut);
        if (!pInfo->isFix()) {
            CTexUtil::sumupTmpTexSize(*pInfo, nTmpTex, nTmpClut);
            nTransTexCount++;
        }
    }
    pData->m_nPacketSize = CModelMan::getPacketSize(pData->m_nBoneNum, pData->m_nTexNum, nTransTexCount, pData);
    return TRUE;
}

void CRsrcANM::_setupANM() {
    INT* pHead = (INT*)getRsrcData();
    if (!pHead) return;
    m_nAAFCount = 0;
    for (INT* p = pHead; *p; p++) {
        if (*p > 0) {
            *p += (INT)pHead;
            CRsrcAAF::setupAAF((BYTE*)*p);
        }
        m_nAAFCount++;
    }
}

void CRsrcAAF::setupAAF(BYTE* pInSrc) {
    if (!pInSrc) return;
    INT* pHead = (INT*)pInSrc;
    pHead[1] += (INT)pInSrc;
    pHead[2] += (INT)pInSrc;
    for (INT i = 0; i < pHead[0]; i++) {
        ((INT*)pHead[1])[i] += (INT)pInSrc;
        ((INT*)pHead[2])[i] += (INT)pInSrc;
    }
}

void CRsrcMMP::_setupMMP() {
    m_nMMPCount = 0;
    m_pMMP = getRsrcData();
    PackedRsrcHeader::coodinateVersion(m_pMMP);
    for (PackedRsrcHeader* pData = (PackedRsrcHeader*)m_pMMP; pData->_pBody; pData++) {
        pData->_pBody = m_pMMP + (INT)pData->_pBody;
        m_nMMPCount++;
    }
}

CHAR* CRsrcMMP::getMMPName(INT nInPtdNo) {
    if (m_nMMPCount < nInPtdNo) return NULL;
    PackedRsrcHeader* pData = (PackedRsrcHeader*)m_pMMP;
    for (INT i = 0; i < nInPtdNo; i++) pData++;
    return pData->_szName;
}

BYTE* CRsrcMMP::getMMPData(INT nInPtdNo) {
    if (m_nMMPCount < nInPtdNo) return NULL;
    PackedRsrcHeader* pData = (PackedRsrcHeader*)m_pMMP;
    for (INT i = 0; i < nInPtdNo; i++) pData++;
    return (BYTE*)pData->_pBody;
}

BYTE* CRsrcMMP::getMMPData(const CHAR* lpszInName) {
    PackedRsrcHeader* pData = (PackedRsrcHeader*)m_pMMP;
    for (INT i = 0; i < m_nMMPCount; i++) {
        if (!strcmp(pData->_szName, lpszInName)) break;
        pData++;
    }
    return (BYTE*)pData->_pBody;
}

void CRsrcCTD::_setupCTD() {
    m_nCTDCount = 0;
    m_pCTD = getRsrcData();
    PackedRsrcHeader::coodinateVersion(m_pCTD);
    for (PackedRsrcHeader* pData = (PackedRsrcHeader*)m_pCTD; pData->_pBody; pData++) {
        pData->_pBody = m_pCTD + (INT)pData->_pBody;
        CCtdMan::instance().initializeData(pData->_pBody);
        m_nCTDCount++;
    }
}

void CRsrcCTD::_disposeCTD() {
    m_nCTDCount = 0;
    for (PackedRsrcHeader* pData = (PackedRsrcHeader*)m_pCTD; pData->_pBody; pData++) {
        CCtdMan::instance().disposeData(pData->_pBody);
        m_nCTDCount++;
    }
}

CHAR* CRsrcCTD::getCTDName(INT nInPtdNo) {
    if (m_nCTDCount < nInPtdNo) return NULL;
    PackedRsrcHeader* pData = (PackedRsrcHeader*)m_pCTD;
    for (INT i = 0; i < nInPtdNo; i++) pData++;
    return pData->_szName;
}

BYTE* CRsrcCTD::getCTDData(INT nInPtdNo) {
    if (m_nCTDCount < nInPtdNo) return NULL;
    PackedRsrcHeader* pData = (PackedRsrcHeader*)m_pCTD;
    for (INT i = 0; i < nInPtdNo; i++) pData++;
    return (BYTE*)pData->_pBody;
}

BYTE* CRsrcCTD::getCTDData(const CHAR* lpszInName) {
    PackedRsrcHeader* pData = (PackedRsrcHeader*)m_pCTD;
    for (INT i = 0; i < m_nCTDCount; i++) {
        if (!strcmp(pData->_szName, lpszInName)) break;
        pData++;
    }
    return (BYTE*)pData->_pBody;
}

void CRsrcMCD::_setupMCD() {}

CRsrcANN::~CRsrcANN() {
    CMotData** ppMot = (CMotData**)getRsrcData();
    if (ppMot) {
        for (INT i = 0; i < m_nCount; i++) {
            if (ppMot[i]) {
                delete[] ppMot[i];
                ppMot[i] = NULL;
                break;
            }
        }
    }
}

struct MOTBIN {
    LONG sz;
    INT pad[4];
};

void CRsrcANN::_setupANN() {
    BYTE* pSrc = getRsrcData();
    if (!pSrc) return;
    m_nCount = *(INT*)pSrc;
    BYTE* pBinSrc = pSrc + 4;
    BYTE* pMotSrc = pBinSrc + m_nCount * sizeof(MOTBIN);
#line 1231
    CBuf* pTmp = RSRC_NEW(1231, CBuf);
#line 1232
    ASSERT(pTmp);
    pTmp->allocBuf(m_nCount * sizeof(CMotData*));
    CMotData** ppDst = (CMotData**)pTmp->getBufPtr();
    INT nMotCnt = 0;
    CMotData* pCMot = NULL;
    MOTBIN* pBin = (MOTBIN*)pBinSrc;
    for (INT i = 0; i < m_nCount; i++) {
        LONG sz = pBin->sz;
        if (sz) nMotCnt++;
        pBin++;
    }
    if (nMotCnt > 0) {
#line 1250
        pCMot = new (__FILE__ "(1250)") CMotData[nMotCnt];
#line 1251
        ASSERT(pCMot);
    }
    INT nIdx = 0;
    for (INT i = 0; i < m_nCount; i++) {
        LONG sz = ((MOTBIN*)pBinSrc)->sz;
        if (sz) {
            ppDst[i] = &pCMot[nIdx++];
#line 1264
            BYTE* tmpBin = (BYTE*)RSRC_ALLOC(1264, sz);
#line 1265
            ASSERT(tmpBin);
            memcpy(tmpBin, pMotSrc, sz);
            ppDst[i]->DecodeBin(tmpBin);
            if (tmpBin) delete tmpBin;
            pMotSrc += ((MOTBIN*)pBinSrc)->sz;
            pBinSrc += sizeof(MOTBIN);
        } else {
            ppDst[i] = NULL;
            pBinSrc += sizeof(MOTBIN);
        }
    }
    setDataSrc(*pTmp);
}

void CRsrcAZF::_setupAZF() {
    MODELDATA** ppAmdTop = (MODELDATA**)getRsrcData();
    if (!ppAmdTop) return;
    ZONEDATA* pZone = (ZONEDATA*)((INT)ppAmdTop[0] + (INT)ppAmdTop);
    ppAmdTop[0] = (MODELDATA*)pZone;
    pZone->parts = (ZONEPARTS*)((INT)pZone->parts + (INT)pZone);
    MODELDATA** ppAmd = ppAmdTop + 1;
    if (((ZONEDATA*)ppAmdTop[0])->group_suu > 0) {
        ((ZONEDATA*)ppAmdTop[0])->grouptable = (INT**)((INT)((ZONEDATA*)ppAmdTop[0])->grouptable + (INT)pZone);
        INT nTblAddr = (INT)((ZONEDATA*)ppAmdTop[0])->grouptable;
        for (INT i = 0; i < ((ZONEDATA*)ppAmdTop[0])->group_suu; i++) ((INT*)nTblAddr)[i] += nTblAddr;
    }
    INT nTexErr = 0;
    m_nAMFCount = 0;
    for (MODELDATA** pp = ppAmd; *pp; pp++) {
        *pp = (MODELDATA*)((INT)*pp + (INT)ppAmdTop);
        if (*pp && !CRsrcAMF::setupAMF(*pp, TRUE)) {
            *pp = (MODELDATA*)((UINT)*pp - 0x80000000U);
            nTexErr++;
        }
        m_nAMFCount++;
    }
    if (nTexErr) m_nFlag |= 1;
}

void CRsrcAZF::reSetupTex() {
    MODELDATA** ppTop = (MODELDATA**)getRsrcData();
    if (ppTop) {
        for (MODELDATA** ppAmd = ppTop + 1; *ppAmd; ppAmd++) {
            if ((UINT)*ppAmd & 0x80000000) {
                *ppAmd = (MODELDATA*)((UINT)*ppAmd & 0x7fffffff);
                CRsrcAMF::setupAMFTex(*ppAmd, FALSE);
            }
        }
        m_nFlag &= ~1;
    }
}

void CRsrcCAM::_setupDataCAM() {
    INT* p = (INT*)getRsrcData();
    if (!p) return;
    p[1] += (INT)p;
    p[2] += (INT)p;
    p[3] += (INT)p;
    p[4] += (INT)p;
    p[5] += (INT)p;
    p[6] += (INT)p;
}

CRsrcDLL::~CRsrcDLL() {
    if (snDllUnload(getRsrcData()))
        TRACE_COL(_TTY_PURPLE, "DLL\x83" "A\x83\x93\x83" "C\x83\x93\x83X\x83g\x81[\x83\x8b\x83G\x83\x89\x81[\n");
}

void CRsrcDLL::_setupDLL() {
    BYTE* pDll = getRsrcData();
    if (!pDll) return;
    if (snDllLoaded(pDll, NULL)) {
        TRACE_COL(_TTY_PURPLE, "DLL\x83" "C\x83\x93\x83X\x83g\x81[\x83\x8b\x83G\x83\x89\x81[\n");
        return;
    }
    BYTE* pFree = (BYTE*)snDllGetFreeMemory(pDll);
    TRACE_COL(_TTY_BLUE, "DLL \"%s\"(%07X:size=%d) is Loaded... (free-address:%07X)\n", m_szName, pDll, getDataSize(), pFree);
}

void CRsrcMEP::_setupMEP() { m_pMEP = (MEP_DATA*)getRsrcData(); }

void CRsrcSND::_setupSND() {
    m_nWaveHandle = -1;
    CBuf* pData = getDataSrc();
    if (pData) {
        m_nWaveHandle = CSound::setupRsrcSND(m_szName, *pData, 0);
        pData->releaseBuf(FALSE);
    }
}

CRsrcSND::~CRsrcSND() { CSound::disposeRsrcSND(m_szName, m_nWaveHandle); }

void CRsrcRSD::_setupRSD(INT nArg) {
    m_nWaveHandle = -1;
    if (getDataSrc()) m_nWaveHandle = CSound::setupRsrcSND(m_szName, *getDataSrc(), nArg);
}

CRsrcRSD::~CRsrcRSD() { CSound::disposeRsrcSND(m_szName, m_nWaveHandle); }

void CRsrcVSV::_setupVSV() { m_nBgmID = CSound::getBgmID_withName(m_szName); }

void CRsrcSPD::_setupSPD() { m_hSPD = D2::C2DMan::init(getRsrcData(), m_szName); }

void CRsrcSPD::_disposeSPD() { D2::C2DMan::destroyBase(m_hSPD); }

void CRsrcDPX::_setupDPX() {
    BYTE* pData = getRsrcData();
    if (!pData) return;
    EFFECT_DATA::Setup((u_long128*)pData, m_szName[0] == '_');
#line 1652
    EFFECT_DATA* p = (EFFECT_DATA*)RSRC_ALLOC(1652, sizeof(EFFECT_DATA));
    if (p) *(u_long128**)p = NULL;
    m_pEffData = p;
    if (m_pEffData) m_pEffData->set((u_long128*)pData);
}

void CRsrcDPX::_disposeDPX() {
    if (m_pEffData) delete m_pEffData;
    m_pEffData = NULL;
}

void CRsrcTXA::_setupTXA() {
    TEXANM_DATA* p = (TEXANM_DATA*)getRsrcData();
    if (p) p->mapping();
}

struct POINTLIGHT_OLD {
    FVECTOR pos;
    FVECTOR col;
    FLOAT fIn;
    FLOAT fOut;
    FLOAT pad[2];
};

struct POINTLIGHT_NEW {
    FVECTOR pos;
    FVECTOR col;
    FLOAT fIn;
    FLOAT fOut;
    FLOAT pad[6];
};











void CRsrcALF::_setupALF() {
    BYTE* pSrc = getRsrcData();
    if (!pSrc) return;
    INT nBytes = getDataSize();
    if (nBytes <= 0) return;
    ALF_DATA* pAlf = (ALF_DATA*)pSrc;
    if (pAlf->nVer >= 3) return;
    INT nNewBytes = pAlf->infinite_suu * 0x20 + 0x10;
    nNewBytes += pAlf->point_suu * 0x40;
    nNewBytes += pAlf->spot_suu * 0x20;
    nNewBytes += 0x10;
#line 1710
    CBuf* pTmp = RSRC_NEW(1710, CBuf);
#line 1711
    ASSERT(pTmp);
    pTmp->allocBuf(nNewBytes);
    BYTE* pDst = pTmp->getBufPtr();
    BYTE* pS = pSrc;
    INT nCopySize = 0x10;
    memcpy(pDst, pS, nCopySize);
    pS += nCopySize;
    pDst += nCopySize;
    nCopySize = pAlf->infinite_suu * 0x20;
    if (nCopySize > 0) {
        memcpy(pDst, pS, nCopySize);
        pDst += nCopySize;
        pS += nCopySize;
    }
    if (pAlf->point_suu)
    for (INT i = 0; i < pAlf->point_suu; i++) {
        memset(pDst, 0, sizeof(POINTLIGHT_NEW));
        ((POINTLIGHT_NEW*)pDst)->pos = ((POINTLIGHT_OLD*)pS)->pos;
        ((POINTLIGHT_NEW*)pDst)->col = ((POINTLIGHT_OLD*)pS)->col;
        ((POINTLIGHT_NEW*)pDst)->fIn = ((POINTLIGHT_OLD*)pS)->fIn;
        ((POINTLIGHT_NEW*)pDst)->fOut = ((POINTLIGHT_OLD*)pS)->fOut;
        pS += sizeof(POINTLIGHT_OLD);
        pDst += sizeof(POINTLIGHT_NEW);
    }
    nCopySize = pAlf->spot_suu * 0x20;
    if (nCopySize > 0) {
        memcpy(pDst, pS, nCopySize);
        pDst += nCopySize;
        pS += nCopySize;
    }
    nCopySize = 0x10;
    memcpy(pDst, pS, nCopySize);
    setDataSrc(*pTmp);
    TRACE("CRsrcALF::VerUp %dbyte -> %dbyte\n", nBytes, nNewBytes);
}

// SOURCE DATA: .rodata
