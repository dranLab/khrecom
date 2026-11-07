#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: Gprim.h strings in .rodata use the full path */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/TexUtil.h"
#include "System/Buf.h"
#include "System/VramMan.h"
#include "System/FileIO.h"
#include "System/Math3D.h"
#include "System/Debug.h"
#include "include_asm.h"
#include <string.h>



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
    __asm__ volatile("pextlb %0, $0, %1\n"
            "pextlh %0, $0, %0\n"
            : "=r"(ivCol)
            : "r"(iCol));
    return ivCol;
}

static inline UINT _vecToCol(u_long128 iCol) {
    UINT ivCol;
    __asm__ volatile("ppach %0, $0, %1\n"
            "ppacb %0, $0, %0\n"
            : "=r"(ivCol)
            : "r"(iCol));
    return ivCol;
}

#include "System/Graphic/TexInfo.h"



































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

/* Parked: 72 of 280 lines, 4 bytes short - structure as the original (default display env -> width/height/PSM
 * via (INT)(u_long >> n) & mask, TIM2 file and picture headers, per-line StoreImage + writeFile). Left: the
 * reserved-bytes loop (pointer + own counter in the original), the GsTex0 TBW/PSM/TW/TH writes (the original
 * goes through a pointer and a 64-bit shift) and the header store order. Bitfield/shift forms, a separate loop
 * variable and permuter 6 min tried. */
#if 0
struct sceGsDispEnv { /* libgraph */
    u_long pmode;
    u_long smode2;
    u_long dispfb;
    u_long display;
    u_long bgcolor;
};
struct tGS_DISPFB { /* libgraph (eestruct.h) */
    u_long FBP : 9;
    u_long FBW : 6;
    u_long PSM : 5;
    u_long p0 : 12;
    u_long DBX : 11;
    u_long DBY : 11;
    u_long p1 : 10;
};
struct tGS_DISPLAY { /* libgraph (eestruct.h) */
    u_long DX : 12;
    u_long DY : 11;
    u_long MAGH : 4;
    u_long MAGV : 2;
    u_long p0 : 3;
    u_long DW : 12;
    u_long DH : 11;
    u_long p1 : 9;
};
struct tGS_TEX0 { /* libgraph (eestruct.h) */
    u_long TBP0 : 14;
    u_long TBW : 6;
    u_long PSM : 6;
    u_long TW : 4;
    u_long TH : 4;
    u_long TCC : 1;
    u_long TFX : 2;
    u_long CBP : 14;
    u_long CPSM : 4;
    u_long CSM : 1;
    u_long CSA : 5;
    u_long CLD : 3;
};
struct sceGsStoreImage { /* libgraph */
    u_long128 data[7];
};
struct TIM2_FILEHEADER {
    TIM2_UCHAR8 FileId[4];
    TIM2_UCHAR8 FormatVersion;
    TIM2_UCHAR8 FormatId;
    TIM2_UINT16 Pictures;
    TIM2_UCHAR8 Reserved[8];
};
extern "C" int sceGsSetDefDispEnv(sceGsDispEnv* disp, short psm, short w, short h, short dx, short dy);
extern "C" int sceGsSetDefStoreImage(sceGsStoreImage* sp, short sbp, short sbw, short spsm, short x, short y, short w, short h);
extern "C" int sceGsExecStoreImage(sceGsStoreImage* sp, u_long128* dstaddr);
extern "C" int sceGsSyncPath(int mode, unsigned short timeout);
extern "C" void FlushCache(int operation);
extern INT SCREEN_HEIGHT;
BOOL CTexUtil::takeSnapShot(const CHAR* lpszInFile) {
    sceGsDispEnv disp;
    INT nWidth;
    INT nHeight;
    INT nFormat;
    INT nBytes;
    INT hFile;
    TIM2_FILEHEADER fhdr;
    TIM2_PICTUREHEADER phdr;
    INT nImageSize;
    INT i;
    unsigned char pBuf[4096];
    sceGsStoreImage si;

    sceGsSetDefDispEnv(&disp, 1, 0x200, SCREEN_HEIGHT, 0, 0);
    nWidth = (UINT)(((INT)(disp.display >> 32) & 0xFFF) + 1) / (UINT)(((INT)(disp.display >> 23) & 0xF) + 1);
    nHeight = ((INT)(disp.display >> 44) & 0x7FF) + 1;
    nFormat = (INT)(disp.dispfb >> 15) & 0x1F;
    nBytes = nWidth * 3;
    hFile = CFileIO::openFile(lpszInFile, 0x202, 0);
    if (hFile < 0) {
        TRACE("file create failure.\n");
        return FALSE;
    }
    fhdr.FileId[0] = 'T';
    fhdr.FileId[1] = 'I';
    fhdr.FileId[2] = 'M';
    fhdr.FileId[3] = '2';
    fhdr.FormatVersion = 3;
    fhdr.FormatId = 0;
    fhdr.Pictures = 1;
    for (i = 7; i >= 0; i--) fhdr.Reserved[i] = 0;
    nImageSize = nBytes * nHeight;
    phdr.TotalSize = nImageSize + 0x30;
    phdr.ClutSize = 0;
    phdr.ImageSize = nImageSize;
    phdr.HeaderSize = 0x30;
    phdr.ClutColors = 0;
    phdr.PictFormat = 0;
    phdr.MipMapTextures = 1;
    phdr.ClutType = 0;
    phdr.ImageType = 2;
    phdr.ImageWidth = nWidth;
    phdr.ImageHeight = nHeight;
    phdr.GsTex0 = ((u_long)((nWidth + 63) / 64 & 0x3F) << 14) | ((u_long)nFormat << 20);
    ((tGS_TEX0*)&phdr.GsTex0)->TW = CTexUtil::getBitDepth(nWidth);
    ((tGS_TEX0*)&phdr.GsTex0)->TH = CTexUtil::getBitDepth(nHeight);
    phdr.GsTex1 = 0;
    phdr.GsTexaFbaPabe = 0;
    phdr.GsTexClut = 0;
    CFileIO::writeFile(hFile, &fhdr, 0x10);
    CFileIO::writeFile(hFile, &phdr, 0x30);
    sceGsSyncPath(0, 0);
    TRACE("Start SnapShot\n");
    for (i = 0; i < nHeight; i++) {
        memset(pBuf, 0, 0x1000);
        TRACE(".");
        sceGsSetDefStoreImage(&si, VRAM::_FRAME0_START, (INT)(disp.dispfb >> 9) & 0x3F, nFormat, (INT)(disp.dispfb >> 32) & 0x7FF,
                              ((INT)(disp.dispfb >> 43) & 0x7FF) + i, nWidth, 1);
        FlushCache(0);
        sceGsExecStoreImage(&si, (u_long128*)pBuf);
        sceGsSyncPath(0, 0);
        CFileIO::writeFile(hFile, pBuf, nBytes);
    }
    TRACE("\nEnd SnapShot!!\n");
    CFileIO::closeFile(hFile);
    return TRUE;
}
#else
/* Placeholder: takeSnapShot's strings (INCLUDE_ASM below). */
__attribute__((section(".sdata"))) void _takeSnapShotStrings() {
    TRACE("file create failure.\n");
    TRACE("Start SnapShot\n");
    TRACE(".");
    TRACE("\nEnd SnapShot!!\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/TexUtil", takeSnapShot__8CTexUtilPCc);
#endif

INT CTexUtil::getClutSize(const BYTE* pInTim2) {
    TIM2_PICTUREHEADER* pHead = Tim2GetPictureHeader(pInTim2, 0);

    if (!pHead) return 0;
    return pHead->ClutSize;
}

void CTexUtil::makeCustomClut(ULONGLONG* inSrc, ULONGLONG* inDest, INT inType, INT inColNum) {
    UINT* pSrc;
    UINT* pDes;

    if (!inSrc || !inDest || inSrc == inDest) return;
    pSrc = (UINT*)inSrc;
    pDes = (UINT*)inDest;
    switch (inType) {
    case 0:
        memcpy(pDes, pSrc, inColNum * 4);
        break;
    case 1: {
        INT j;
        for (j = 0; j < inColNum; j++, pSrc++, pDes++) {
            UBYTE b = 0xFF - (UBYTE)*pSrc;
            UBYTE g = 0xFF - (*pSrc >> 8);
            UBYTE r = 0xFF - (*pSrc >> 16);
            b >>= 1;
            g >>= 1;
            r >>= 1;
            *pDes = (*pSrc & 0xFF000000) | b | (g << 8) | (r << 16);
        }
        break;
    }
    case 2: {
        INT j;
        FLOAT fR;
        FLOAT fG;
        FLOAT fB;
        UINT iI;
        for (j = 0; j < inColNum; j++, pSrc++, pDes++) {
            fR = (FLOAT)(*pSrc & 0xFF);
            fG = (FLOAT)((*pSrc & 0xFF00) >> 8);
            fB = (FLOAT)((*pSrc & 0xFF0000) >> 16);
            fR = fR * 0.2989f + fG * 0.5866f + fB * 0.1145f;
            iI = (UINT)fR;
            if (iI >= 0x100) iI = 0xFF;
            *pDes = (*pSrc & 0xFF000000) | (iI | (iI << 16 | iI << 8));
        }
        break;
    }
    default: {
        INT j;
        for (j = 0; j < inColNum; j++, pSrc++, pDes++) {
            *pDes = *pSrc & 0xFF000000;
        }
        break;
    }
    }
}

void CTexUtil::makeMonochromeClut(const TexInfo* inpInfo, INT inBufIdx, FLOAT inRate) {
    makeMonochromeClut(inpInfo, 0, inBufIdx, inRate);
}

void CTexUtil::makeMonochromeClut(const TexInfo* inpInfo, INT inSrcBufIdx, INT inOutBufIdx, FLOAT inRate) {
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
    pInfoSrc = NULL;
    pInfoDes = NULL;
    switch (inSrcBufIdx) {
    case 0:
        pInfoSrc = inpInfo;
        break;
    case 1:
        pInfoSrc = inpInfo->_pExtendInfo0;
        break;
    case 2:
        pInfoSrc = inpInfo->_pExtendInfo1;
        break;
    case 3:
        pInfoSrc = inpInfo->_pExtendInfo2;
        break;
    }
    switch (inOutBufIdx) {
    case 0:
        pInfoDes = inpInfo;
        break;
    case 1:
        pInfoDes = inpInfo->_pExtendInfo0;
        break;
    case 2:
        pInfoDes = inpInfo->_pExtendInfo1;
        break;
    case 3:
        pInfoDes = inpInfo->_pExtendInfo2;
        break;
    }
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

void CTexUtil::makeWhiteClut(const TexInfo* inpInfo, INT inOutBufIdx, FLOAT inRate) {
    makeWhiteClut(inpInfo, 0, inOutBufIdx, inRate);
}

void CTexUtil::makeWhiteClut(const TexInfo* inpInfo, INT inSrcBufIdx, INT inOutBufIdx, FLOAT inRate) {
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
    pInfoSrc = NULL;
    pInfoDes = NULL;
    switch (inSrcBufIdx) {
    case 0:
        pInfoSrc = inpInfo;
        break;
    case 1:
        pInfoSrc = inpInfo->_pExtendInfo0;
        break;
    case 2:
        pInfoSrc = inpInfo->_pExtendInfo1;
        break;
    case 3:
        pInfoSrc = inpInfo->_pExtendInfo2;
        break;
    }
    switch (inOutBufIdx) {
    case 0:
        pInfoDes = inpInfo;
        break;
    case 1:
        pInfoDes = inpInfo->_pExtendInfo0;
        break;
    case 2:
        pInfoDes = inpInfo->_pExtendInfo1;
        break;
    case 3:
        pInfoDes = inpInfo->_pExtendInfo2;
        break;
    }
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

BOOL CTexUtil::setupTexInfo(TexInfo& outInfo, const BYTE* pInTex, INT nWidth, INT nHeight) {
    INT nTexSize;
    INT nTexPos;
    INT nClutPos;

    nTexSize = _align(_align(nWidth, 64) / 4 * nHeight, 64) / 64;
    if (CVramMan::allocTex(nTexSize, 0, nTexPos, nClutPos, 0) < 0) return FALSE;
    VTransInfo& tex = outInfo._TexInfo;
    VTransInfo& clut = outInfo._ClutInfo;
    INT nAlignW = 64; /* as in getTransInfo; the ref omits it (a constant pseudo has no debug location) */
    INT nImageSize = nWidth * nHeight >> 2;
    tex._nDistPos = nTexPos;
    tex._nFormat = 0;
    tex._pImage = (ULONGLONG*)pInTex;
    tex._nDistX = 0;
    tex._nDistY = 0;
    tex._nDistW = nWidth;
    tex._nDistH = nHeight;
    tex._nTw = CTexUtil::getBitDepth(nWidth);
    tex._nTh = CTexUtil::getBitDepth(nHeight);
    tex._nDistBufW = _align(nWidth, nAlignW) >> 6;
    tex._qwImage = nImageSize;
    tex._nTransSize = nTexSize;
    clut._nDistPos = 0;
    clut._nFormat = 0;
    clut._pImage = NULL;
    clut._nDistX = 0;
    clut._nDistY = 0;
    clut._nDistBufW = 1;
    clut._nTransSize = 0;
    outInfo._nTPage = SCE_GS_SET_TEX0(nTexPos, tex._nDistBufW, tex._nFormat, tex._nTw, tex._nTh, 1, 0, nClutPos,
                                      clut._nFormat, 0, 0, 1);
    return TRUE;
}

/* Parked: 93 of 153 lines, 16 bytes short - control flow as the original (tex_cnv PSM 19/20 cases, getTransSize
 * else, allocTex, getTransInfo, TEX0). Left: the original keeps &outInfo._TexInfo/_ClutInfo in s1/s0 across the
 * getTransInfo call (one more saved register, frame 0x90), which renumbers most registers; it also tests tex_cnv
 * with andi 4 where ours shifts (bitfield types and ==1/!=0 tests tried). */
#if 0
BOOL CTexUtil::setupTexInfoImd(TexInfo& outInfo, const BYTE* pInImd, INT nAllocType) {
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
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/TexUtil", setupTexInfoImd__8CTexUtilR7TexInfoPCUci);
#endif
// SOURCE DATA: .data .rodata
