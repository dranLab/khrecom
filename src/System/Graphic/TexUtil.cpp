#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: Gprim.h strings in .rodata use the full path */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/TexUtil.h"
#include "System/Graphic/TexCtrl.h"
#include "System/Buf.h"
#include "System/VramMan.h"
#include "System/FileIO.h"
#include "System/Math3D.h"
#include "System/Debug.h"
#include <string.h>
extern "C" {
#include "System/Graphic/Effect/libppp/partMng.h"
#include "System/Graphic/Effect/libppp/draw/draw_w.h"
}



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

struct tGS_PMODE { /* libgraph (eestruct.h) */
    unsigned int EN1 : 1;
    unsigned int EN2 : 1;
    unsigned int CRTMD : 3;
    unsigned int MMOD : 1;
    unsigned int AMOD : 1;
    unsigned int SLBG : 1;
    unsigned int ALP : 8;
    unsigned int p0 : 16;
    unsigned int p1;
};
struct tGS_SMODE2 { /* libgraph (eestruct.h) */
    unsigned int INT : 1;
    unsigned int FFMD : 1;
    unsigned int DPMS : 2;
    unsigned int p0 : 28;
    unsigned int p1;
};
struct tGS_DISPFB2 { /* libgraph (eestruct.h) */
    unsigned int FBP : 9;
    unsigned int FBW : 6;
    unsigned int PSM : 5;
    unsigned int p0 : 12;
    unsigned int DBX : 11;
    unsigned int DBY : 11;
    unsigned int p1 : 10;
};
struct tGS_DISPLAY2 { /* libgraph (eestruct.h) */
    unsigned int DX : 12;
    unsigned int DY : 11;
    unsigned int MAGH : 4;
    unsigned int MAGV : 2;
    unsigned int p0 : 3;
    unsigned int DW : 12;
    unsigned int DH : 11;
    unsigned int p1 : 9;
};
struct tGS_BGCOLOR { /* libgraph (eestruct.h) */
    unsigned int R : 8;
    unsigned int G : 8;
    unsigned int B : 8;
    unsigned int p0 : 8;
    unsigned int p1;
};
struct sceGsDispEnv { /* libgraph */
    tGS_PMODE pmode;
    tGS_SMODE2 smode2;
    tGS_DISPFB2 dispfb;
    tGS_DISPLAY2 display;
    tGS_BGCOLOR bgcolor;
};
extern "C" int sceGsSetDefDispEnv(sceGsDispEnv* disp, short psm, short w, short h, short dx, short dy);
extern "C" int sceGsSetDefStoreImage(sceGsStoreImage* sp, short sbp, short sbw, short spsm, short x, short y, short w, short h);
extern "C" int sceGsExecStoreImage(sceGsStoreImage* sp, u_long128* dstaddr);
extern "C" void FlushCache(int operation);
extern INT SCREEN_HEIGHT;
BOOL CTexUtil::takeSnapShot(const CHAR* lpszInFile) {
    sceGsDispEnv disp;
    INT nWidth;
    INT nHeight;
    INT nFormat;
    INT nBytes;
    INT nImageType;
    INT hFile;
    TIM2_FILEHEADER fhdr;
    TIM2_PICTUREHEADER phdr;
    INT nImageSize;

    sceGsSetDefDispEnv(&disp, 1, 0x200, SCREEN_HEIGHT, 0, 0);
    nWidth = (disp.display.DW + 1) / (disp.display.MAGH + 1);
    nHeight = disp.display.DH + 1;
    nFormat = disp.dispfb.PSM;
    nBytes = nWidth * 3;
    nImageType = 2;
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
    for (INT i = 0; i < 8; i++) fhdr.pad[i] = 0;
    nImageSize = nBytes * nHeight;
    phdr.TotalSize = nImageSize + 0x30;
    phdr.ClutSize = 0;
    phdr.ImageSize = nImageSize;
    phdr.HeaderSize = 0x30;
    phdr.ClutColors = 0;
    phdr.PictFormat = 0;
    phdr.MipMapTextures = 1;
    phdr.ClutType = 0;
    phdr.ImageType = nImageType;
    phdr.ImageWidth = nWidth;
    phdr.ImageHeight = nHeight;
    sceGsTex0* pTex0 = (sceGsTex0*)&phdr.GsTex0;
    phdr.GsTex0 = 0;
    pTex0->TBW = (nWidth + 63) / 64;
    pTex0->PSM = nFormat;
    pTex0->TW = CTexUtil::getBitDepth(nWidth);
    pTex0->TH = CTexUtil::getBitDepth(nHeight);
    phdr.GsTex1 = 0;
    phdr.GsTexaFbaPabe = 0;
    phdr.GsTexClut = 0;
    CFileIO::writeFile(hFile, &fhdr, 0x10);
    CFileIO::writeFile(hFile, &phdr, 0x30);
    sceGsSyncPath(0, 0);
    TRACE("Start SnapShot\n");
    for (INT i = 0; i < nHeight; i++) {
        unsigned char pBuf[4096];
        sceGsStoreImage si;
        memset(pBuf, 0, 0x1000);
        TRACE(".");
        sceGsSetDefStoreImage(&si, VRAM::_FRAME0_START, disp.dispfb.FBW, nFormat, disp.dispfb.DBX, disp.dispfb.DBY + i, nWidth, 1);
        FlushCache(0);
        sceGsExecStoreImage(&si, (u_long128*)pBuf);
        sceGsSyncPath(0, 0);
        CFileIO::writeFile(hFile, pBuf, nBytes);
    }
    TRACE("\nEnd SnapShot!!\n");
    CFileIO::closeFile(hFile);
    return TRUE;
}

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

BOOL CTexUtil::setupTexInfoImd(TexInfo& outInfo, const BYTE* pInImd, INT nAllocType) {
    INT nTexSize = 0;
    INT nClutSize = 0;
    TIM2_PICTUREHEADER tim2Header;
    INT nTexPos;
    INT nClutPos;

    if (((IMD_HEADER*)pInImd)->tex_cnv) {
        tim2Header.ImageType = 3;
        switch (((IMD_HEADER*)pInImd)->tex_psm) {
        case 20:
            nClutSize = 1;
            tim2Header.ImageWidth = ((IMD_HEADER*)pInImd)->tex_w >> 1;
            tim2Header.ImageHeight = ((IMD_HEADER*)pInImd)->tex_h >> 2;
            outInfo._ClutInfo._qwImage = 4;
            outInfo._ClutInfo._nDistW = 8;
            outInfo._ClutInfo._nDistH = 2;
            break;
        case 19:
            nClutSize = 4;
            tim2Header.ImageWidth = ((IMD_HEADER*)pInImd)->tex_w >> 1;
            tim2Header.ImageHeight = ((IMD_HEADER*)pInImd)->tex_h >> 1;
            outInfo._ClutInfo._qwImage = 64;
            outInfo._ClutInfo._nDistW = 16;
            outInfo._ClutInfo._nDistH = 16;
            break;
        }
        nTexSize = _align(_align(tim2Header.ImageWidth, 64) * tim2Header.ImageHeight, 64) / 64;
    } else {
        tim2Header.ImageType = ((IMD_HEADER*)pInImd)->tex_type;
        tim2Header.ImageWidth = ((IMD_HEADER*)pInImd)->tex_w;
        tim2Header.ImageHeight = ((IMD_HEADER*)pInImd)->tex_h;
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
    { /* block-local references: the debug info (and so the ref) has no entry for this block's locals */
        VTransInfo& tex = outInfo._TexInfo;
        VTransInfo& clut = outInfo._ClutInfo;
        CTexUtil::getTransInfo(tim2Header, tex, clut, nTexPos, nClutPos);
        tex._pImage = (ULONGLONG*)(pInImd + ((IMD_HEADER*)pInImd)->tex_ofs);
        clut._pImage = (ULONGLONG*)(pInImd + ((IMD_HEADER*)pInImd)->clut_ofs);
        tex._nTransSize = nTexSize;
        clut._nTransSize = nClutSize;
        outInfo._nTPage = SCE_GS_SET_TEX0(nTexPos, ((IMD_HEADER*)pInImd)->tex_bw, ((IMD_HEADER*)pInImd)->tex_psm,
                                          ((IMD_HEADER*)pInImd)->tex_tw, ((IMD_HEADER*)pInImd)->tex_th, 1, 0,
                                          nClutPos, clut._nFormat, 0, 0, 1);
    }
    return TRUE;
}
// SOURCE DATA: .data .rodata
