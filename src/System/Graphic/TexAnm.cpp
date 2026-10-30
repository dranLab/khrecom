#define MODELDATA_H_PATH "c:/KHCOM/MainApp/System/Graphic/modeldata.h"
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/TexAnm.h"
#include "System/HeapMan.h"
#include "System/Debug.h"

extern "C" int strcasecmp(const char* s1, const char* s2);
extern "C" int rand();

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
/* non-volatile: the size load may be scheduled before it (CModelTexAnm::setup) */
#define RET_ADDR_NV() ({ UINT nRA; __asm__("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

struct VTransInfo {
    UINT _nDistPos;
    USHORT _nFormat;
    USHORT _nDistBufW;
    ULONGLONG* _pImage;
    UINT _qwImage;
    USHORT _nDistX;
    USHORT _nDistY;
    USHORT _nDistW;
    USHORT _nDistH;
    USHORT _nTw;
    USHORT _nTh;
    UINT _nTransSize;
};

struct TexInfo {
    ULONGLONG _nTPage;
    VTransInfo _TexInfo;
    VTransInfo _ClutInfo;
    TexInfo* _pExtendInfo0;
    TexInfo* _pExtendInfo1;
    TexInfo* _pExtendInfo2;
    UBYTE _nInfoType;
    UBYTE _nType;
    UBYTE _nParam;
    UBYTE _nFlag;
    float _fOfs[2];
    CHAR* _pName;
    CTexAnm* _pTexAnm;
    INT _nTexSize;
    INT _nClutSize;

    CHAR* getName() { return _pName; }
};

struct TexInfoStub {
    union {
        struct {
            TexInfo* _pTexInfo;
            SHORT _nTexPos;
            SHORT _nClutPos;
        };
        ULONGLONG _nValue;
    };
};



#include "System/RsrcDB.h"





#include "System/Graphic/PackMan.h"





CTexAnm::CTexAnm() {
    m_pData = NULL;
    m_pPlayData = NULL;
    m_pClut = NULL;
    m_pClutSrc = NULL;
    m_nClutQSize = 0;
    m_nPlayNum = 0;
    m_nSeqIdx = 0;
    m_fTimer = 0;
    m_fTimeMax = 0;
}

CTexAnm::~CTexAnm() {
    if (m_pClut) com_free(m_pClut);
    m_pClut = NULL;
}

void CTexAnm::setup(INT nGroup, INT nAnmGroupNum) {
    TEXANM_DATA* p = CRsrcDB::getRsrcTXA(nGroup);
    if (p) setup(*p, nAnmGroupNum);
}

#line 75
void CTexAnm::setup(const TEXANM_DATA& inData, INT nAnmGroupNum) {
    if (m_pClut) com_free(m_pClut);
    m_pClut = NULL;
    INT nNum = inData.getDataNum();
    if (nAnmGroupNum < 0 || nAnmGroupNum >= nNum) return;
    m_pData = (_TEXANM0*)&inData.sData[nAnmGroupNum];
    TexInfo* pTexInfo = CRsrcDB::getRsrcTex(m_pData->szDstTexName, NAME_CASE_SENSITIVE);
    if (!pTexInfo) {
        TRACE("TexAnm::\x93]\x91\x97\x90\xe6\x83" "e\x83N\x83X\x83`\x83\x83\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1 %s\n",
              m_pData->szDstTexName);
        m_pData = NULL;
        return;
    }
    if (m_pData->isClutAnm()) {
        m_pClutSrc = (u_long128*)pTexInfo->_ClutInfo._pImage;
        m_nClutQSize = pTexInfo->_ClutInfo._qwImage;
        if (!m_pClutSrc || !m_nClutQSize) {
            TRACE("TexAnm::\x93]\x91\x97\x90\xe6\x83N\x83\x89\x83" "b\x83g\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1 %s\n",
                  m_pData->szDstTexName);
            m_pData = NULL;
        } else {
            m_pClut = (u_long128*)com_malloc_d(m_nClutQSize * 0x100, __FILE__ "(96)", RET_ADDR());
            if (!m_pClut) AssertApp(__FILE__, __LINE__, "m_pClut", 0);
        }
    }
    setSeq(getDefaultName());
}

void CTexAnm::_updateClut() {
    INT nNum = m_nClutQSize;
    INT* pSrc = (INT*)m_pClutSrc;
    INT* pDst = (INT*)m_pData->pDataClt;
    INT* pOut = (INT*)m_pClut;
    _TEXANM2& d = m_pPlayData->pData2[m_nSeqIdx];
    FLOAT fFromPar = d.nFromPar * 0.01f;
    FLOAT fToPar = d.nToPar * 0.01f;
    FLOAT fPar = m_fTimeMax;
    if (fPar < 1.0f) fPar = 1.0f;
    fPar = m_fTimer / fPar * (fToPar - fFromPar) + fFromPar;
    if (fPar <= 0.0f) {
        u_long128* pOut128 = (u_long128*)pOut;
        u_long128* pSrc128 = (u_long128*)pSrc;
        for (INT i = nNum; i > 0; i--) {
            *pOut128++ = *pSrc128++;
        }
    } else if (1.0f <= fPar) {
        u_long128* pOut128 = (u_long128*)pOut;
        u_long128* pDst128 = (u_long128*)pDst;
        for (INT i = nNum; i > 0; i--) {
            *pOut128++ = *pDst128++;
        }
    } else {
        __asm__ volatile("mfc1 $8, %0\n"
                         "qmtc2.ni $8, $vf6\n"
                         :
                         : "f"(fPar)
                         : "$8");
        for (INT i = nNum * 4; i > 0; i--) {
            __asm__ volatile(".set noreorder\n"
                             "lw $8, 0(%1)\n"
                             "lw $9, 0(%2)\n"
                             "beq $8, $9, 1f\n"
                             "pextlb $8, $0, $8\n"
                             "pextlh $8, $0, $8\n"
                             "pextlb $9, $0, $9\n"
                             "pextlh $9, $0, $9\n"
                             "qmtc2.ni $8, $vf4\n"
                             "qmtc2.ni $9, $vf5\n"
                             "vitof0.xyzw $vf4, $vf4\n"
                             "vitof0.xyzw $vf5, $vf5\n"
                             "vsub.xyzw $vf5, $vf5, $vf4\n"
                             "vmulax.xyzw $ACC, $vf5, $vf6x\n"
                             "vmaddw.xyzw $vf5, $vf4, $vf0w\n"
                             "vftoi0.xyzw $vf5, $vf5\n"
                             "qmfc2.ni $9, $vf5\n"
                             "ppach $9, $0, $9\n"
                             "ppacb $9, $0, $9\n"
                             "1:\n"
                             "sw $9, 0(%0)\n"
                             ".set reorder\n"
                             :
                             : "r"(pOut), "r"(pSrc), "r"(pDst)
                             : "$8", "$9", "memory");
            pOut++;
            pSrc++;
            pDst++;
        }
    }
}

INT CTexAnm::setSeq(const CHAR* pszAnmName) {
    if (m_pData) {
        m_pPlayData = NULL;
        if (pszAnmName || (pszAnmName = getDefaultName())) {
            INT nNum = m_pData->nNum;
            _TEXANM1* pData1 = m_pData->pData1;
            for (INT i = 0; i < nNum; i++) {
                if (!strcasecmp(pData1[i].szName, pszAnmName)) return setSeq(i);
            }
        }
    }
    return -1;
}

INT CTexAnm::setSeq(INT nAnmNum) {
    INT nNum;
    if (m_pData) {
        m_pPlayData = NULL;
        if (nAnmNum < m_pData->nNum) {
            if (nAnmNum < 0) return setSeq(getDefaultName());
            m_pPlayData = m_pData->pData1 + nAnmNum;
            m_nSeqIdx = 0;
            m_nPlayNum = nAnmNum;
            _setupTimer();
            return m_nPlayNum;
        }
    }
    return -1;
}

void CTexAnm::_setupTimer() {
    _TEXANM2& pData2 = m_pPlayData->pData2[m_nSeqIdx];
    if (pData2.Frame1 > 0) {
        int nRange = pData2.Frame1 - pData2.Frame0;
        if (nRange <= 0) nRange = 1;
        m_fTimeMax = rand() % nRange + pData2.Frame0;
    } else {
        m_fTimeMax = pData2.Frame0;
    }
    m_fTimer = 0;
}

void CTexAnm::update(FLOAT fR) {
    if (!m_pData || !m_pPlayData) return;
    m_fTimer += fR;
    if (m_fTimeMax <= m_fTimer) {
        m_nSeqIdx++;
        if (m_nSeqIdx >= m_pPlayData->nNum) m_nSeqIdx += m_pPlayData->nJump;
        _setupTimer();
    }
    if (m_pClut) _updateClut();
}

CHAR* CTexAnm::getDstTexName() {
    if (m_pData) return m_pData->szDstTexName;
    return NULL;
}

CHAR* CTexAnm::getDefaultName() {
    return m_pData->szName;
}

#line 315
void CTexAnm::sendTexture(USHORT nTBP, const VTransInfo& Tinfo, USHORT nCBP, const VTransInfo& Cinfo) {
    _TEXANM0* pData = m_pData;
    if (!pData || !m_pPlayData) return;
    if (m_pClut) {
        CPackMan::pkRefLoadImage(nCBP, Cinfo._nFormat, Cinfo._nDistBufW, (unsigned long long*)m_pClut, m_nClutQSize,
                                 Cinfo._nDistX, Cinfo._nDistY, Cinfo._nDistW, Cinfo._nDistH);
        return;
    }
    _TEXANM2& pData2 = m_pPlayData->pData2[m_nSeqIdx];
    if (!pData2.pDataTex) return;
    static const unsigned char psmDhift[5] = {3, 3, 2, 1, 0};
    INT nLineSize = (pData->DstW >> 1) << psmDhift[Tinfo._nFormat & 7];
    INT nSize = nLineSize * pData->DstH;
#line 334
    if (!(!(nSize & 0x0f))) AssertApp(__FILE__, __LINE__, "!(nSize&0x0f)", 0);
    u_long128* pImage = (u_long128*)pData2.pDataTex;
#line 339
    if (!(!((u_int)pImage & 0x0f))) AssertApp(__FILE__, __LINE__, "!((u_int)pImage&0x0f)", 0);
    CPackMan::pkRefLoadImage(nTBP, Tinfo._nFormat, Tinfo._nDistBufW, (unsigned long long*)pImage, nSize >> 4, pData->DstX,
                             pData->DstY, pData->DstW, pData->DstH);
}

CModelTexAnm::CModelTexAnm() {
    m_nTexAnmNum = 0;
    m_pTexAnm = NULL;
    m_pID = NULL;
}

CModelTexAnm::~CModelTexAnm() {
    _dispose();
}

void CModelTexAnm::_dispose() {
    if (m_pTexAnm) com_free(m_pTexAnm);
    m_pTexAnm = NULL;
    if (m_pID) com_free(m_pID);
    m_pID = NULL;
    m_nTexAnmNum = 0;
}

#line 395
void CModelTexAnm::setup(CTexAnm* pATbl, INT nANum, TexInfoStub* pTTbl, INT nTNum) {
    INT nCount;
    _dispose();
    m_nTexAnmNum = getTexAnmCount(pATbl, nANum, pTTbl, nTNum);
    if (m_nTexAnmNum <= 0) return;
    m_pTexAnm = (CTexAnm**)com_malloc_d(m_nTexAnmNum * sizeof(CTexAnm*), __FILE__ "(400)", RET_ADDR());
    if (!m_pTexAnm) AssertApp(__FILE__, __LINE__, "m_pTexAnm", 0);
    m_pID = (UBYTE*)com_malloc_d(m_nTexAnmNum, __FILE__ "(402)", RET_ADDR_NV());
    if (!m_pID) AssertApp(__FILE__, __LINE__, "m_pID", 0);
    nCount = 0;
    for (INT i = 0; i < nTNum; i++) {
        TexInfoStub& info = pTTbl[i];
        for (INT j = 0; j < nANum; j++) {
            CHAR* pDstTex = pATbl[j].getDstTexName();
            if (pDstTex && !strcasecmp(info._pTexInfo->getName(), pDstTex)) {
#line 413
                if (!(!(nCount >= m_nTexAnmNum))) AssertApp(__FILE__, __LINE__, "!(nCount >= m_nTexAnmNum)", 0);
                m_pTexAnm[nCount] = &pATbl[j];
                m_pID[nCount] = i;
                nCount++;
            }
        }
    }
#line 421
    if (!(!(nCount != m_nTexAnmNum))) AssertApp(__FILE__, __LINE__, "!(nCount != m_nTexAnmNum)", 0);
}

BOOL CModelTexAnm::check(INT nID) {
    INT nNum = m_nTexAnmNum;
    if (nNum > 0) {
        UBYTE* pID = m_pID;
        for (INT i = 0; i < nNum; i++, pID++) {
            if (*pID == nID) return TRUE;
        }
    }
    return FALSE;
}

void CModelTexAnm::sendTexture(INT nID, USHORT nTBP, const VTransInfo& Tinfo, USHORT nCBP, const VTransInfo& Cinfo) {
    INT nNum = m_nTexAnmNum;
    if (nNum > 0) {
        UBYTE* pID = m_pID;
        for (INT i = 0; i < nNum; i++, pID++) {
            if (*pID == nID) m_pTexAnm[i]->sendTexture(nTBP, Tinfo, nCBP, Cinfo);
        }
    }
}

INT CModelTexAnm::getTexAnmCount(CTexAnm* pATbl, INT nANum, TexInfoStub* pTTbl, INT nTNum) {
    INT nNum = 0;
    for (INT i = 0; i < nTNum; i++) {
        TexInfoStub& info = pTTbl[i];
        for (INT j = 0; j < nANum; j++) {
            CHAR* pDstTex = pATbl[j].getDstTexName();
            if (pDstTex && !strcasecmp(info._pTexInfo->getName(), pDstTex)) nNum++;
        }
    }
    return nNum;
}

void TEXANM_DATA::mapping() {
    INT nNum0;
    INT nNum1;
    INT nNum2;
    _TEXANM0* pData0;
    _TEXANM1* pData1;
    INT i;
    INT j;
    INT k;
    check();
    nNum0 = sHead.nNum;
    pData0 = sData;
    for (i = 0; i < nNum0; i++) {
        pData0[i].pData1 = (_TEXANM1*)(pData0[i].nOffset + (INT)this);
        if (!pData0[i].DstWH) pData0[i].pDataClt = (void*)(pData0[i].nOffsetClt + (INT)this);
        nNum1 = pData0[i].nNum;
        pData1 = pData0[i].pData1;
        for (j = 0; j < nNum1; j++) {
            pData1[j].pData2 = (_TEXANM2*)(pData1[j].nOffset + (INT)this);
            if (!pData0[i].isClutAnm()) {
                nNum2 = pData1[j].nNum;
                _TEXANM2* pData2 = pData1[j].pData2;
                for (k = 0; k < nNum2; k++) {
                    if (pData2[k].nOffset) pData2[k].pDataTex = (void*)(pData2[k].nOffset + (INT)this);
                }
            }
        }
    }
}

#line 529
void TEXANM_DATA::check() const {
    if (!this) AssertApp(__FILE__, __LINE__, "this", 0);
    if (!(!(this->sHead.id != (0x00415854)))) AssertApp(__FILE__, __LINE__, "! (this->sHead.id != (0x00415854))", 0);
    if (!(!(this->sHead.nNum <= 0))) AssertApp(__FILE__, __LINE__, "! (this->sHead.nNum <= 0)", 0);
}

int TEXANM_DATA::getDataNum() const {
    check();
    return sHead.nNum;
}

int TEXANM_DATA::getSeqNum(int nDataNum) const {
    int nRet;
    check();
    nRet = 0;
    if (nDataNum < sHead.nNum) nRet = sData[nDataNum].nNum;
    return nRet;
}

// SOURCE DATA: .rodata
