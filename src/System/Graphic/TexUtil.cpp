#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/TexUtil.h"
#include "System/Buf.h"
#include "System/VramMan.h"
#include "System/FileIO.h"
#include "System/Math3D.h"
#include "System/Debug.h"
#include "include_asm.h"
#include <string.h>

typedef unsigned char TIM2_UCHAR8;
typedef unsigned int TIM2_UINT32;
typedef unsigned short TIM2_UINT16;
typedef unsigned long TIM2_UINT64;

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
    union {
        struct {
            FLOAT fSOfs;
            FLOAT fTOfs;
            unsigned int pad[4];
        };
        struct {
            TIM2_UINT64 GsTex0;
            TIM2_UINT64 GsTex1;
            TIM2_UINT32 GsTexaFbaPabe;
            TIM2_UINT32 GsTexClut;
        };
    };
};

extern "C" {
TIM2_PICTUREHEADER* Tim2GetPictureHeader(const void* pTim2, int imgno);
void* Tim2GetImage(const TIM2_PICTUREHEADER* ph, int mipmap);
void* Tim2GetClut(const TIM2_PICTUREHEADER* ph);
}

void ITOF0Vector(FVECTOR& outVec, const IVECTOR& inVec);
void FTOI0Vector(IVECTOR& outVec, const FVECTOR& inVec);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

static inline u_long128 _colToVec(UINT iCol) {
    u_long128 ivCol;
    __asm__("pextlb %0, $0, %1\n"
            "pextlh %0, $0, %0\n"
            : "=r"(ivCol)
            : "r"(iCol));
    return ivCol;
}

static inline UINT _vecToCol(u_long128 iCol) {
    UINT ivCol;
    __asm__("ppach %0, $0, %1\n"
            "ppacb %0, $0, %0\n"
            : "=r"(ivCol)
            : "r"(iCol));
    return ivCol;
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

struct TexInfo { /* System/Graphic/modeldata.h */
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
    void* _pTexAnm;
    INT _nTexSize;
    INT _nClutSize;

    BOOL isFixTex() const { return !_TexInfo._pImage || (_nFlag & 1); }
    BOOL isFixClut() const { return !_ClutInfo._pImage || (_nFlag & 2); }
    const TexInfo* getExtendInfo(INT nIdx) const {
        switch (nIdx) {
        case 0:
            return this;
        case 1:
            return _pExtendInfo0;
        case 2:
            return _pExtendInfo1;
        case 3:
            return _pExtendInfo2;
        }
        return NULL;
    }
};

struct IMD_HEADER {
    unsigned char id[4];
    USHORT version;
    char padding[2];
    INT tex_ofs;
    INT tex_size;
    INT clut_ofs;
    INT clut_size;
    INT tex_bp;
    USHORT tex_w;
    USHORT tex_h;
    USHORT tex_tw;
    USHORT tex_th;
    USHORT tex_bw;
    USHORT tex_psm;
    INT clut_bp;
    USHORT clut_w;
    USHORT clut_h;
    USHORT clut_bw;
    USHORT clut_psm;
    USHORT tex_type;
    USHORT clut_type;
    char reserved[4];
    UINT tex_load : 1;
    UINT clut_load : 1;
    UINT tex_cnv : 1;
};

#define SCE_GS_SET_TEX0(tbp, tbw, psm, tw, th, tcc, tfx, cbp, cpsm, csm, csa, cld)                                  \
    ((ULONGLONG)(tbp) | ((ULONGLONG)(tbw) << 14) | ((ULONGLONG)(psm) << 20) | ((ULONGLONG)(tw) << 26) |               \
     ((ULONGLONG)(th) << 30) | ((ULONGLONG)(tcc) << 34) | ((ULONGLONG)(tfx) << 35) | ((ULONGLONG)(cbp) << 37) |      \
     ((ULONGLONG)(cpsm) << 51) | ((ULONGLONG)(csm) << 55) | ((ULONGLONG)(csa) << 56) | ((ULONGLONG)(cld) << 61))

namespace {
int _tblPSM[5] = { 2, 1, 0, 20, 19 };
INT _nTmpTexAddr = 0;
INT _nTmpClutAddr = 0;
} // namespace

static inline UINT _align(UINT nValue, UINT align) { return (nValue + (align - 1)) & ~(align - 1); }
#define ALIGN(x, a) _align(x, a)

void CTexUtil::getTransSize(const TIM2_PICTUREHEADER& inData, INT& nOutTexSize, INT& nOutClutSize) {
    switch (inData.ImageType) {
    case 4:
        nOutTexSize = ALIGN(ALIGN(inData.ImageWidth, 128) / 8 * inData.ImageHeight, 512) / 64;
        nOutClutSize = 1;
        break;
    case 5:
        nOutTexSize = ALIGN(ALIGN(inData.ImageWidth, 128) / 4 * inData.ImageHeight, 256) / 64;
        nOutClutSize = 4;
        break;
    case 1:
        nOutTexSize = ALIGN(ALIGN(inData.ImageWidth, 64) / 2 * inData.ImageHeight, 64) / 64;
        nOutClutSize = 0;
        break;
    case 2:
    case 3:
        nOutTexSize = ALIGN(ALIGN(inData.ImageWidth, 64) * inData.ImageHeight, 64) / 64;
        nOutClutSize = 0;
        break;
    }
}

INT CTexUtil::getTransInfo(const TIM2_PICTUREHEADER& inTim2Head, VTransInfo& outTex, VTransInfo& outClut, INT nInTexPos,
                           INT nInClutPos) {
    INT nAlignW;
    INT nImageSize;

    outTex._nDistPos = nInTexPos;
    outTex._nFormat = _tblPSM[inTim2Head.ImageType - 1];
    outTex._pImage = (ULONGLONG*)Tim2GetImage(&inTim2Head, 0);
    outTex._nDistX = 0;
    outTex._nDistY = 0;
    outTex._nDistW = inTim2Head.ImageWidth;
    outTex._nDistH = inTim2Head.ImageHeight;
    outTex._nTw = CTexUtil::getBitDepth(inTim2Head.ImageWidth);
    outTex._nTh = CTexUtil::getBitDepth(inTim2Head.ImageHeight);
    nAlignW = 64;
    nImageSize = inTim2Head.ImageWidth * inTim2Head.ImageHeight;
    switch (outTex._nFormat) {
    case 20:
        outClut._qwImage = 4;
        outClut._nDistW = 8;
        outClut._nDistH = 2;
        nAlignW = 128;
        nImageSize >>= 5;
        break;
    case 19:
        nAlignW = 128;
        outClut._qwImage = 64;
        outClut._nDistW = 16;
        nImageSize >>= 4;
        outClut._nDistH = 16;
        break;
    case 2:
        nImageSize >>= 3;
        break;
    default:
        nImageSize >>= 2;
        break;
    }
    outTex._nDistBufW = _align(inTim2Head.ImageWidth, nAlignW) >> 6;
    outTex._qwImage = nImageSize;
    outClut._nDistPos = nInClutPos;
    outClut._nFormat = 0;
    outClut._pImage = (ULONGLONG*)Tim2GetClut(&inTim2Head);
    outClut._nDistY = 0;
    outClut._nDistBufW = 1;
    outClut._nDistX = 0;
    return 0;
}

INT CTexUtil::getTransInfo(const void* pInTim2, VTransInfo& outTex, VTransInfo& outClut, INT nInTexPos, INT nInClutPos) {
    TIM2_PICTUREHEADER* pHead = Tim2GetPictureHeader(pInTim2, 0);

    if (!pHead) return -1;
    return getTransInfo(*pHead, outTex, outClut, nInTexPos, nInClutPos);
}

BOOL CTexUtil::setupTexInfo(TexInfo& outInfo, const BYTE* pInTim2, INT nAllocType) {
    TIM2_PICTUREHEADER* pHead;
    INT nTexSize;
    INT nClutSize;
    INT nTexPos;
    INT nClutPos;

    pHead = Tim2GetPictureHeader(pInTim2, 0);
    if (!pHead) return FALSE;
    nTexSize = 0;
    nClutSize = 0;
    getTransSize(*pHead, nTexSize, nClutSize);
    if (CVramMan::allocTex(nTexSize, nClutSize, nTexPos, nClutPos, nAllocType) < 0) return FALSE;
    if (nAllocType == 0) {
        outInfo._nTexSize = nTexSize;
        outInfo._nClutSize = nClutSize;
    } else {
        outInfo._nTexSize = 0;
        outInfo._nClutSize = 0;
    }
    VTransInfo& tex = outInfo._TexInfo;
    VTransInfo& clut = outInfo._ClutInfo;
    getTransInfo(*pHead, tex, clut, nTexPos, nClutPos);
    tex._nTransSize = nTexSize;
    clut._nTransSize = nClutSize;
    outInfo._nTPage = SCE_GS_SET_TEX0(nTexPos, tex._nDistBufW, tex._nFormat, tex._nTw, tex._nTh, 1, 0, nClutPos,
                                      clut._nFormat, 0, 0, 1);
    return TRUE;
}

void CTexUtil::setHalfClutAlpha(BYTE* pInClut, INT nInClutSize) {
    INT i;

    if (!pInClut) return;
    pInClut += 3;
    for (i = 0; i < nInClutSize; i += 4) {
        *pInClut = (*pInClut + 1) >> 1;
        pInClut += 4;
    }
}

void CTexUtil::setHalfClutAlpha(BYTE* pInTim2) {
    TIM2_PICTUREHEADER* pHead;

    if (!isTim2Data(pInTim2)) return;
    pHead = Tim2GetPictureHeader(pInTim2, 0);
    if (!pHead) return;
    setHalfClutAlpha((BYTE*)Tim2GetClut(pHead), pHead->ClutSize);
}

void CTexUtil::copyClutData(CBuf& outBuf, const void* pInTim2) {
    TIM2_PICTUREHEADER* pHead;
    BYTE* pClut;
    BYTE* pBuf;

    pHead = Tim2GetPictureHeader(pInTim2, 0);
    if (!pHead) return;
    pClut = (BYTE*)Tim2GetClut(pHead);
    if (outBuf.getBufPtr()) outBuf.releaseBuf(FALSE);
    pBuf = outBuf.allocBuf(pHead->ClutSize);
    if (pBuf) memcpy(pBuf, pClut, pHead->ClutSize);
}

void CTexUtil::resetTmpTexTransInfo() {
    _nTmpTexAddr = 0;
    _nTmpClutAddr = 0;
}

INT CTexUtil::getTmpTexTransInfo(const void* pInTim2, VTransInfo& outTex, VTransInfo& outClut) {
    TIM2_PICTUREHEADER* pHead;
    INT nTexSize;
    INT nClutSize;
    INT nTexPos;
    INT nNextPos;
    INT nClutPos;

    if (!isTim2Data(pInTim2)) return -1;
    pHead = Tim2GetPictureHeader(pInTim2, 0);
    if (!pHead) return -1;
    getTransSize(*pHead, nTexSize, nClutSize);
    nTexPos = -1;
    nNextPos = _nTmpTexAddr + nTexSize;
    if (nNextPos < VRAM::_TEX_TMP1_SIZE) {
        nTexPos = _nTmpTexAddr;
        _nTmpTexAddr = nNextPos;
    }
    nClutPos = -1;
    nNextPos = _nTmpClutAddr + nClutSize;
    if (nNextPos < VRAM::_CLUT_TMP1_SIZE) {
        nClutPos = _nTmpClutAddr;
        _nTmpClutAddr = nNextPos;
    }
    return getTransInfo(*pHead, outTex, outClut, nTexPos, nClutPos);
}

static inline BOOL _sumupTmpSize(SHORT& nOutPos, USHORT nSize, USHORT nMax) {
    nOutPos = nSize + nOutPos;
    if (nOutPos >= nMax) {
        nOutPos = -1;
        return FALSE;
    }
    return TRUE;
}

BOOL CTexUtil::sumupTmpTexSize(const TexInfo& inInfo, SHORT& nOutTex, SHORT& nOutClut) {
    const VTransInfo& texInfo = inInfo._TexInfo;
    const VTransInfo& clutInfo = inInfo._ClutInfo;
    BOOL bResult;

    if (inInfo.isFixTex()) {
        bResult = TRUE;
    } else {
        bResult = _sumupTmpSize(nOutTex, texInfo._nTransSize, VRAM::_TEX_TMP1_SIZE);
    }
    if (bResult && !inInfo.isFixClut()) {
        UINT nSize = (USHORT)clutInfo._nTransSize; /* MATCH: not in the ref's locals; lhu + UINT compare gives the original's movn */
        bResult = _sumupTmpSize(nOutClut, nSize > 3 ? nSize : 4, VRAM::_CLUT_TMP1_SIZE);
    }
    return bResult;
}

INT CTexUtil::getFixTexTransInfo(const void* pInData, VTransInfo& outTex, VTransInfo& outClut, INT nInType) {
    INT nRet;
    TIM2_PICTUREHEADER* pHead;
    INT nTexSize;
    INT nClutSize;
    INT nTexPos;
    INT nClutPos;

    if (!pInData) return -1;
    nRet = 0;
    if (isTim2Data(pInData)) {
        pHead = Tim2GetPictureHeader(pInData, 0);
        if (!pHead) return -1;
        getTransSize(*pHead, nTexSize, nClutSize);
        if (CVramMan::allocTex(nTexSize, nClutSize, nTexPos, nClutPos, nInType) < 0) return -2;
        getTransInfo(*pHead, outTex, outClut, nTexPos, nClutPos);
        nRet = 1;
    } else {
        outTex = ((const TexInfo*)pInData)->_TexInfo;
        outClut = ((const TexInfo*)pInData)->_ClutInfo;
    }
    return nRet;
}

/* Placeholder: takeSnapShot's strings (INCLUDE_ASM below). */
__attribute__((section(".sdata"))) void _takeSnapShotStrings() {
    TRACE("file create failure.\n");
    TRACE("Start SnapShot\n");
    TRACE(".");
    TRACE("\nEnd SnapShot!!\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/TexUtil", takeSnapShot__8CTexUtilPCc);

INT CTexUtil::getClutSize(const BYTE* pInTim2) {
    TIM2_PICTUREHEADER* pHead = Tim2GetPictureHeader(pInTim2, 0);

    if (!pHead) return 0;
    return pHead->ClutSize;
}

/* Placeholder: the C of makeCustomClut__8CTexUtilPUlT1ii (not matching yet). */
__asm__(".section .sdata\n"); /* gcc may think it is still in .sdata after the INCLUDE_ASM */
__attribute__((section(".sdata"))) static void _makeCustomClut_c(ULONGLONG* inSrc, ULONGLONG* inDest, INT inType, INT inColNum) {
    UINT* pSrc;
    UINT* pDes;

    if (!inSrc || !inDest || inSrc == inDest) return;
    pSrc = (UINT*)inSrc;
    pDes = (UINT*)inDest;
    switch (inType) {
    case 0:
        memcpy(pDes, pSrc, inColNum * 4);
        break;
    case 1:
        for (INT j = inColNum; j > 0; j--) {
            UINT iCol = *pSrc;
            *pDes = (iCol & 0xFF000000) | ((UBYTE)(0xFF - (UBYTE)iCol) >> 1) | (((0xFF - (iCol >> 8)) << 7) & 0x7F00) |
                    (((UBYTE)(0xFF - (iCol >> 16)) >> 1) << 16);
            pSrc++;
            pDes++;
        }
        break;
    case 2:
        for (INT j = inColNum; j > 0; j--) {
            UINT iCol = *pSrc;
            FLOAT fR = (FLOAT)(iCol & 0xFF);
            FLOAT fG = (FLOAT)((iCol & 0xFF00) >> 8);
            FLOAT fB = (FLOAT)((iCol & 0xFF0000) >> 16);
            UINT iI = (UINT)(fR * 0.2989f + fG * 0.5866f + fB * 0.1145f);
            if (iI >= 0x100) iI = 0xFF;
            *pDes = (iCol & 0xFF000000) | iI | (iI << 16 | iI << 8);
            pSrc++;
            pDes++;
        }
        break;
    default:
        for (INT j = inColNum; j > 0; j--) {
            *pDes = *pSrc & 0xFF000000;
            pSrc++;
            pDes++;
        }
        break;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/TexUtil", makeCustomClut__8CTexUtilPUlT1ii);

void CTexUtil::makeMonochromeClut(const TexInfo* inpInfo, INT inBufIdx, FLOAT inRate) {
    makeMonochromeClut(inpInfo, 0, inBufIdx, inRate);
}

/* Placeholder: the C of makeMonochromeClut__8CTexUtilPC7TexInfoiif (not matching yet). */
__asm__(".section .sdata\n"); /* gcc may think it is still in .sdata after the INCLUDE_ASM */
__attribute__((section(".sdata"))) static void _makeMonochromeClut_c(const TexInfo* inpInfo, INT inSrcBufIdx, INT inOutBufIdx, FLOAT inRate) {
    static const FVECTOR _cfvClampClut = { { 255.f, 255.f, 255.f, 0.f } };
    const TexInfo* pInfoSrc;
    const TexInfo* pInfoDes;
    UINT* pSrc;
    UINT* pDes;
    INT iMax;
    FLOAT fSrcRate;
    IVECTOR ivTmp;
    FVECTOR tmp1;
    FVECTOR tmp2;
    FVECTOR tmp3;
    INT i;

    if (!inpInfo || !(inpInfo->_nType & 0x40)) return;
    if (inpInfo->_nInfoType) inpInfo = inpInfo->_pExtendInfo0;
    pInfoSrc = inpInfo->getExtendInfo(inSrcBufIdx);
    pInfoDes = inpInfo->getExtendInfo(inOutBufIdx);
    if (!pInfoSrc || !pInfoDes) return;
    pSrc = (UINT*)pInfoSrc->_ClutInfo._pImage;
    pDes = (UINT*)pInfoDes->_ClutInfo._pImage;
    if (!pSrc || !pDes) return;
    iMax = pInfoSrc->_ClutInfo._nTransSize == 1 ? 16 : 256;
    if (inRate < 0.0f) inRate = 0.0f;
    if (1.0f < inRate) inRate = 1.0f;
    fSrcRate = 1.0f - inRate;
    for (i = 0; i < iMax; i++) {
        ivTmp._q = _colToVec(*pSrc);
        ITOF0Vector(tmp1, ivTmp);
        FLOAT fI = tmp1.x * 0.2989f + tmp1.y * 0.5866f + tmp1.z * 0.1145f;
        tmp3.setAll(fI);
        ScaleVectorXYZ(tmp2, tmp1, fSrcRate);
        ScaleVectorXYZ(tmp3, tmp3, inRate);
        AddVectorXYZ(tmp1, tmp2, tmp3);
        Math3D::VectorMinimize(tmp1, tmp1, _cfvClampClut);
        FTOI0Vector(ivTmp, tmp1);
        UINT iCol = _vecToCol(ivTmp._q);
        *pDes = (*pSrc & 0xFF000000) | iCol;
        pSrc++;
        pDes++;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/TexUtil", makeMonochromeClut__8CTexUtilPC7TexInfoiif);

void CTexUtil::makeWhiteClut(const TexInfo* inpInfo, INT inOutBufIdx, FLOAT inRate) {
    makeWhiteClut(inpInfo, 0, inOutBufIdx, inRate);
}

/* Placeholder: the C of makeWhiteClut__8CTexUtilPC7TexInfoiif (not matching yet). */
__asm__(".section .sdata\n"); /* gcc may think it is still in .sdata after the INCLUDE_ASM */
__attribute__((section(".sdata"))) static void _makeWhiteClut_c(const TexInfo* inpInfo, INT inSrcBufIdx, INT inOutBufIdx, FLOAT inRate) {
    static const FVECTOR _cfvWhiteClut = { { 255.f, 255.f, 255.f, 0.f } };
    const TexInfo* pInfoSrc;
    const TexInfo* pInfoDes;
    UINT* pSrc;
    UINT* pDes;
    INT iMax;
    IVECTOR ivTmp;
    FVECTOR tmp;
    INT i;

    if (!inpInfo || !(inpInfo->_nType & 0x40)) return;
    if (inpInfo->_nInfoType) inpInfo = inpInfo->_pExtendInfo0;
    pInfoSrc = inpInfo->getExtendInfo(inSrcBufIdx);
    pInfoDes = inpInfo->getExtendInfo(inOutBufIdx);
    if (!pInfoSrc || !pInfoDes) return;
    pSrc = (UINT*)pInfoSrc->_ClutInfo._pImage;
    pDes = (UINT*)pInfoDes->_ClutInfo._pImage;
    if (!pSrc || !pDes) return;
    iMax = pInfoSrc->_ClutInfo._nTransSize == 1 ? 16 : 256;
    if (inRate < 0.0f) inRate = 0.0f;
    if (1.0f < inRate) inRate = 1.0f;
    for (i = 0; i < iMax; i++) {
        ivTmp._q = _colToVec(*pSrc);
        ITOF0Vector(tmp, ivTmp);
        Math3D::VectorLerp(tmp, tmp, _cfvWhiteClut, inRate);
        Math3D::VectorMinimize(tmp, tmp, _cfvWhiteClut);
        FTOI0Vector(ivTmp, tmp);
        UINT iCol = _vecToCol(ivTmp._q);
        *pDes = (*pSrc & 0xFF000000) | iCol;
        pSrc++;
        pDes++;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/TexUtil", makeWhiteClut__8CTexUtilPC7TexInfoiif);

/* Placeholder: the C of setupTexInfo__8CTexUtilR7TexInfoPCUcii (not matching yet). */
__asm__(".section .sdata\n"); /* gcc may think it is still in .sdata after the INCLUDE_ASM */
__attribute__((section(".sdata"))) static BOOL _setupTexInfo_c(TexInfo& outInfo, const BYTE* pInTex, INT nWidth, INT nHeight) {
    INT nTexSize;
    INT nTexPos;
    INT nClutPos;

    nTexSize = _align(_align(nWidth, 64) / 4 * nHeight, 64) / 64;
    if (CVramMan::allocTex(nTexSize, 0, nTexPos, nClutPos, 0) < 0) return FALSE;
    VTransInfo& tex = outInfo._TexInfo;
    VTransInfo& clut = outInfo._ClutInfo;
    INT nImageSize = nWidth * nHeight;
    tex._nDistPos = nTexPos;
    tex._pImage = (ULONGLONG*)pInTex;
    tex._nFormat = 0;
    tex._nDistX = 0;
    tex._nDistY = 0;
    tex._nDistW = nWidth;
    tex._nDistH = nHeight;
    tex._nTw = CTexUtil::getBitDepth(nWidth);
    tex._nTh = CTexUtil::getBitDepth(nHeight);
    tex._nDistBufW = _align(nWidth, 64) >> 6;
    tex._qwImage = nImageSize >> 2;
    tex._nTransSize = nTexSize;
    clut._nDistBufW = 1;
    clut._nTransSize = 0;
    clut._nDistPos = 0;
    clut._nFormat = 0;
    clut._pImage = NULL;
    clut._nDistX = 0;
    clut._nDistY = 0;
    outInfo._nTPage = SCE_GS_SET_TEX0(nTexPos, tex._nDistBufW, tex._nFormat, tex._nTw, tex._nTh, 1, 0, nClutPos,
                                      clut._nFormat, 0, 0, 1);
    return TRUE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/TexUtil", setupTexInfo__8CTexUtilR7TexInfoPCUcii);

/* Placeholder: the C of setupTexInfoImd__8CTexUtilR7TexInfoPCUci (not matching yet). */
__asm__(".section .sdata\n"); /* gcc may think it is still in .sdata after the INCLUDE_ASM */
__attribute__((section(".sdata"))) static BOOL _setupTexInfoImd_c(TexInfo& outInfo, const BYTE* pInImd, INT nAllocType) {
    const IMD_HEADER* pImd = (const IMD_HEADER*)pInImd;
    INT nTexSize = 0;
    INT nClutSize = 0;
    TIM2_PICTUREHEADER tim2Header;
    INT nTexPos;
    INT nClutPos;

    if (pImd->tex_cnv) {
        tim2Header.ImageType = 3;
        switch (pImd->tex_psm) {
        case 20:
            tim2Header.ImageWidth = pImd->tex_w >> 1;
            tim2Header.ImageHeight = pImd->tex_h >> 2;
            nClutSize = 1;
            outInfo._ClutInfo._qwImage = 4;
            outInfo._ClutInfo._nDistW = 8;
            outInfo._ClutInfo._nDistH = 2;
            break;
        case 19:
            tim2Header.ImageWidth = pImd->tex_w >> 1;
            tim2Header.ImageHeight = pImd->tex_h >> 1;
            nClutSize = 4;
            outInfo._ClutInfo._qwImage = 64;
            outInfo._ClutInfo._nDistW = 16;
            outInfo._ClutInfo._nDistH = 16;
            break;
        }
        nTexSize = (_align(tim2Header.ImageWidth, 64) * tim2Header.ImageHeight & ~63) >> 6;
    } else {
        tim2Header.ImageType = pImd->tex_type;
        tim2Header.ImageWidth = pImd->tex_w;
        tim2Header.ImageHeight = pImd->tex_h;
        CTexUtil::getTransSize(tim2Header, nTexSize, nClutSize);
    }
    if (CVramMan::allocTex(nTexSize, nClutSize, nTexPos, nClutPos, nAllocType) < 0) return FALSE;
    if (nAllocType == 0) {
        outInfo._nTexSize = nTexSize;
        outInfo._nClutSize = nClutSize;
    } else {
        outInfo._nTexSize = 0;
        outInfo._nClutSize = 0;
    }
    CTexUtil::getTransInfo(tim2Header, outInfo._TexInfo, outInfo._ClutInfo, nTexPos, nClutPos);
    outInfo._TexInfo._pImage = (ULONGLONG*)(pInImd + pImd->tex_ofs);
    outInfo._ClutInfo._pImage = (ULONGLONG*)(pInImd + pImd->clut_ofs);
    outInfo._TexInfo._nTransSize = nTexSize;
    outInfo._ClutInfo._nTransSize = nClutSize;
    outInfo._nTPage = SCE_GS_SET_TEX0(nTexPos, pImd->tex_bw, pImd->tex_psm, pImd->tex_tw, pImd->tex_th, 1, 0, nClutPos,
                                      outInfo._ClutInfo._nFormat, 0, 0, 1);
    return TRUE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/TexUtil", setupTexInfoImd__8CTexUtilR7TexInfoPCUci);
// SOURCE DATA: .data .rodata
