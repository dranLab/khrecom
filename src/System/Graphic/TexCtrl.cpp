#include "System/Graphic/TexCtrl.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Debug.h"
#include <string.h>

struct TIM2_PICTUREHEADER {
    TIM2_UINT32 TotalSize;
    TIM2_UINT32 ClutSize;
    TIM2_UINT32 ImageSize;
    TIM2_UINT16 HeaderSize;
    TIM2_UINT16 ClutColors;
    TIM2_UCHAR8 PictFormat;
    TIM2_UCHAR8 MipMapTextures;
    TIM2_UCHAR8 ClutType;
    TIM2_UCHAR8 ImageType;
    TIM2_UINT16 ImageWidth;
    TIM2_UINT16 ImageHeight;
};

extern "C" {
TIM2_PICTUREHEADER* Tim2GetPictureHeader(void* pTim2, int imgno);
void* Tim2GetImage(TIM2_PICTUREHEADER* ph, int mipmap);
}

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

class CTexAnm {
private:
    BYTE m_pad[0x24];
public:
    CHAR* getDstTexName();
    void sendTexture(USHORT nTex, const VTransInfo& rTex, USHORT nClut, const VTransInfo& rClut);
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

    BOOL isFixTex() { return _nFlag & 1; }
    BOOL isFixClut() { return _nFlag & 2; }
    BOOL isFix() { return _nFlag & 3; }
};

struct TexMemScroll {
    INT m_UvTime;
    INT m_UvStatus;
    INT m_UvFps;
    INT m_NowLine;
    INT m_MaxLine;
};

class CBuf {
private:
    BYTE* m_pBuf;
    INT m_nBufSize;
    BOOL m_bManaged;
public:
    virtual ~CBuf();
    BYTE* getBuf() { return m_pBuf; }
};

class CRsrcData {
private:
    CBuf* m_pDataSrc;
    char m_szName[24];
    INT m_nRsrcType;
    INT m_nRsrcGroup;
    HANDLE m_hThis;
    HANDLE m_hParent;
    HANDLE* m_phChildren;
    USHORT m_nChildCount;
    USHORT m_nChildRest;
    BYTE m_refTagList[0x10];
public:
    virtual ~CRsrcData();
    CBuf* getDataSrc() { return m_pDataSrc; }
    BYTE* getRsrcData() {
        BYTE* pData = NULL;
        if (getDataSrc()) {
            pData = getDataSrc()->getBuf();
        }
        return pData;
    }
    CHAR* getName() { return m_szName; }
};

class CRsrcTex : public CRsrcData {
private:
    TexInfo* m_pTexInfo;
    TexMemScroll* m_pScr;
public:
    virtual ~CRsrcTex();
    TexInfo* getTexInfo() { return m_pTexInfo; }
    TexMemScroll* getTexMemScroll() { return m_pScr; }
};

typedef CSLCListT<CRsrcData> CRsrcList;

class CRsrcDB {
public:
    static CRsrcList& getRsrcList(UINT nType);
    static CRsrcList& getScrollTexList();
};

class CPackMan {
public:
    static UINT getEnableDraw();
    static void pkRefLoadImage(const VTransInfo& rInfo);
};

#define _getTexInfo(p) ((p) ? ((CRsrcTex*)(p))->getTexInfo() : NULL)

CTexCtrl::CTexCtrl(CBfObj* pInParent) : CTask(pInParent, 0x4000) {
    setPriority(0x8000);
    declareSysTask(11);
}

CTexCtrl::~CTexCtrl() {}

void CTexCtrl::main() {
    _scrollTexUV();
}

void CTexCtrl::_scrollTexUV() {
    SLContainerT<CRsrcData>* pContainer;
    TexMemScroll* pScr;
    BYTE Type;
    INT Fps;

    for (pContainer = CRsrcDB::getScrollTexList().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CRsrcTex* pTex = (CRsrcTex*)pContainer->getValue();
        pScr = pTex->getTexMemScroll();
        if (!pScr) {
            continue;
        }
        if (pScr->m_UvStatus & 0x80) {
            continue;
        }
        TIMEQ lag = getFrameRate();
        Fps = 3000 / pScr->m_UvFps;
        Type = pTex->getTexInfo()->_nType;
        pScr->m_UvTime += lag;
        if (Fps < pScr->m_UvTime) {
            _doTexScroll(*(TIM2_FILEHEADER*)pTex->getRsrcData(), Type, pScr->m_UvTime / Fps);
            pScr->m_NowLine += pScr->m_UvTime / Fps;
            if (pScr->m_MaxLine < pScr->m_NowLine) {
                pScr->m_NowLine -= pScr->m_MaxLine;
            }
            pScr->m_UvTime = 0;
        }
    }
}

void CTexCtrl::_doTexScroll(TIM2_FILEHEADER& inTim2, INT nInType, INT inFpsCnt) {
    TIM2_PICTUREHEADER* pHead;
    BYTE* pTex;
    INT nWidth;
    INT iWidthLine;
    INT nImageSize;
    u_long128* pSrc;
    u_long128* pDst;
    INT i;

    if (inFpsCnt <= 0) {
        return;
    }
    pHead = Tim2GetPictureHeader(&inTim2, 0);
    if (!pHead) {
        return;
    }
    pTex = (BYTE*)Tim2GetImage(pHead, 0);
    nWidth = 0;
    switch (pHead->ImageType) {
    case 3:
        break;
    case 4:
        nWidth = pHead->ImageWidth >> 1;
        break;
    case 5:
        nWidth = pHead->ImageWidth;
        break;
    }
    if (nWidth & 0xF) {
        TRACE("\x94\xbc\x92\x5b\x82\xc8\x89\xa1\x83\x54\x83\x43\x83\x59\x82\xcc\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x82\xf0\x83\x58\x83\x4e\x83\x8d\x81\x5b\x83\x8b\x82\xb5\x82\xe6\x82\xa4\x82\xc6\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7 %d\n", nWidth);
    }
    nWidth >>= 4;
    if (nWidth <= 0 || nWidth >= 32) {
        return;
    }
    iWidthLine = inFpsCnt * nWidth;
    nImageSize = pHead->ImageSize >> 4;
    while (iWidthLine >= nImageSize) {
        iWidthLine -= nImageSize;
    }
    if (iWidthLine <= 0) {
        return;
    }
    {
        u_long128 pTmp[iWidthLine];

        if (nInType & 0x10) {
            pSrc = (u_long128*)pTex;
            pDst = pTmp;
            for (i = iWidthLine; i > 0; i--) {
                *pDst++ = *pSrc++;
            }
            pSrc = (u_long128*)pTex + iWidthLine;
            pDst = (u_long128*)pTex;
            for (i = nImageSize - iWidthLine; i > 0; i--) {
                *pDst++ = *pSrc++;
            }
            pSrc = pTmp;
            pDst = (u_long128*)pTex + (nImageSize - iWidthLine);
            for (i = iWidthLine; i > 0; i--) {
                *pDst++ = *pSrc++;
            }
        } else if (nInType & 0x20) {
            pSrc = (u_long128*)pTex + (nImageSize - iWidthLine);
            pDst = pTmp;
            for (i = iWidthLine; i > 0; i--) {
                *pDst++ = *pSrc++;
            }
            pDst = (u_long128*)pTex + (nImageSize - 1);
            pSrc = (u_long128*)pTex + (nImageSize - iWidthLine - 1);
            for (i = nImageSize - iWidthLine; i > 0; i--) {
                *pDst-- = *pSrc--;
            }
            pSrc = pTmp;
            pDst = (u_long128*)pTex;
            for (i = iWidthLine; i > 0; i--) {
                *pDst++ = *pSrc++;
            }
        }
    }
}

INT CTexCtrl::StopUvScroll(CHAR* nITexName) {
    SLContainerT<CRsrcData>* pContainer;
    TexMemScroll* pScr;

    for (pContainer = CRsrcDB::getScrollTexList().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CRsrcTex* pTex = (CRsrcTex*)pContainer->getValue();
        if (strcmp(pTex->getName(), nITexName) == 0) {
            pScr = pTex->getTexMemScroll();
            if (pScr) {
                pScr->m_UvStatus |= 0x80;
                return 0;
            }
            return -1;
        }
    }
    return -1;
}

INT CTexCtrl::StartUvScroll(CHAR* nITexName) {
    SLContainerT<CRsrcData>* pContainer;
    TexMemScroll* pScr;

    for (pContainer = CRsrcDB::getScrollTexList().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CRsrcTex* pTex = (CRsrcTex*)pContainer->getValue();
        if (strcmp(pTex->getName(), nITexName) == 0) {
            pScr = pTex->getTexMemScroll();
            if (pScr) {
                pScr->m_UvStatus &= 0x7F;
                return 0;
            }
            return -1;
        }
    }
    return -1;
}

INT CTexCtrl::InitUvScroll(CHAR* nITexName) {
    SLContainerT<CRsrcData>* pContainer;
    TexMemScroll* pScr;

    for (pContainer = CRsrcDB::getScrollTexList().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CRsrcTex* pTex = (CRsrcTex*)pContainer->getValue();
        if (strcmp(pTex->getName(), nITexName) == 0) {
            pScr = pTex->getTexMemScroll();
            if (!pScr) {
                return -1;
            }
            _doTexScroll(*(TIM2_FILEHEADER*)pTex->getRsrcData(), pTex->getTexInfo()->_nType,
                         pScr->m_MaxLine - pScr->m_NowLine);
            pScr->m_NowLine = 0;
            pScr->m_UvStatus |= 0x80;
            return 0;
        }
    }
    return -1;
}

INT CTexCtrl::ChangeFPSUvScroll(CHAR* nITexName, INT nInFps) {
    SLContainerT<CRsrcData>* pContainer;
    TexMemScroll* pScr;

    for (pContainer = CRsrcDB::getScrollTexList().getHead(); pContainer; pContainer = pContainer->getNext()) {
        CRsrcTex* pTex = (CRsrcTex*)pContainer->getValue();
        if (strcmp(pTex->getName(), nITexName) == 0) {
            pScr = pTex->getTexMemScroll();
            if (pScr) {
                pScr->m_UvFps = nInFps;
                return 0;
            }
            return -1;
        }
    }
    return -1;
}

static void _transPrefixTex(INT nRsrcType) {
    SLContainerT<CRsrcData>* pContainer;
    TexInfo* pInfo;

    if (!(CPackMan::getEnableDraw() & 2)) {
        return;
    }
    for (pContainer = CRsrcDB::getRsrcList(nRsrcType).getHead(); pContainer; pContainer = pContainer->getNext()) {
        pInfo = _getTexInfo(pContainer->getValue());
        if (!pInfo) {
            continue;
        }
        if (pInfo->isFix()) {
            if (pInfo->isFixTex() && pInfo->_TexInfo._pImage) {
                CPackMan::pkRefLoadImage(pInfo->_TexInfo);
            }
            if (pInfo->isFixClut() && pInfo->_ClutInfo._pImage) {
                CPackMan::pkRefLoadImage(pInfo->_ClutInfo);
            }
            if (pInfo->_pTexAnm) {
                pInfo->_pTexAnm->sendTexture(pInfo->_TexInfo._nDistPos, pInfo->_TexInfo, pInfo->_ClutInfo._nDistPos,
                                             pInfo->_ClutInfo);
            }
        }
    }
}

void CTexCtrl::transPrefixTex() {
    _transPrefixTex(2);
    _transPrefixTex(16);
}

void CTexCtrl::addFixTexAnm(CTexAnm* pTexAnm, INT nTexAnm) {
    SLContainerT<CRsrcData>* pContainer;
    TexInfo* pInfo;
    INT i;
    CHAR* pTName;

    if (!pTexAnm || nTexAnm <= 0) {
        return;
    }
    for (pContainer = CRsrcDB::getRsrcList(2).getHead(); pContainer; pContainer = pContainer->getNext()) {
        pInfo = _getTexInfo(pContainer->getValue());
        if (!pInfo) {
            continue;
        }
        if (!pInfo->isFix()) {
            continue;
        }
        for (i = 0; i < nTexAnm; i++) {
            pTName = pTexAnm[i].getDstTexName();
            if (!pTName) {
                TRACE("\x82\xa8\x82\xa2\x93\x5d\x91\x97\x90\xe6\x82\xcc\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x82\xaa\x82\xcb\x81\x5b\x82\xbc\x21\n");
                continue;
            }
            if (strcmp(pTName, pInfo->_pName) == 0) {
                pInfo->_pTexAnm = &pTexAnm[i];
                break;
            }
        }
    }
}

void CTexCtrl::delFixTexAnm(CTexAnm* pTexAnm, INT nTexAnm) {
    SLContainerT<CRsrcData>* pContainer;
    TexInfo* pInfo;
    INT i;
    CHAR* pTName;

    for (pContainer = CRsrcDB::getRsrcList(2).getHead(); pContainer; pContainer = pContainer->getNext()) {
        pInfo = _getTexInfo(pContainer->getValue());
        if (!pInfo) {
            continue;
        }
        if (!pInfo->isFix()) {
            continue;
        }
        if (!pTexAnm || nTexAnm <= 0) {
            pInfo->_pTexAnm = NULL;
            continue;
        }
        for (i = 0; i < nTexAnm; i++) {
            pTName = pTexAnm[i].getDstTexName();
            if (!pTName) {
                TRACE("\x82\xa8\x82\xa2\x93\x5d\x91\x97\x90\xe6\x82\xcc\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x82\xaa\x82\xcb\x81\x5b\x82\xbc\x21\n");
                continue;
            }
            if (strcmp(pTName, pInfo->_pName) == 0) {
                pInfo->_pTexAnm = NULL;
                break;
            }
        }
    }
}
// SOURCE DATA: .rodata
