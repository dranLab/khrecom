#ifndef SYSTEM_GRAPHIC_TEXUTIL_H
#define SYSTEM_GRAPHIC_TEXUTIL_H

#include "khtypes.h"

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

struct VTransInfo;
struct TexInfo;
class CBuf;

class CTexUtil {
public:
    static void getTransSize(const TIM2_PICTUREHEADER& inData, INT& nOutTexSize, INT& nOutClutSize);
    static INT getTransInfo(const TIM2_PICTUREHEADER& inTim2Head, VTransInfo& outTex, VTransInfo& outClut, INT nInTexPos,
                            INT nInClutPos);
    static INT getTransInfo(const void* pInTim2, VTransInfo& outTex, VTransInfo& outClut, INT nInTexPos, INT nInClutPos);
    static BOOL setupTexInfo(TexInfo& outInfo, const BYTE* pInTim2, INT nAllocType);
    static BOOL setupTexInfo(TexInfo& outInfo, const BYTE* pInTex, INT nWidth, INT nHeight);
    static BOOL setupTexInfoImd(TexInfo& outInfo, const BYTE* pInImd, INT nAllocType);
    static void setHalfClutAlpha(BYTE* pInClut, INT nInClutSize);
    static void setHalfClutAlpha(BYTE* pInTim2);
    static void copyClutData(CBuf& outBuf, const void* pInTim2);
    static INT getBitDepth(UINT nValue) {
        UINT nBit = 0x400;
        INT i;
        for (i = 10; i > 0; i--) {
            if (nValue & nBit) break;
            nBit >>= 1;
        }
        return i + (nBit < nValue);
    }
    static BOOL isTim2Data(const void* pInData) { return pInData && *(const UINT*)pInData == 0x324D4954; }
    static void resetTmpTexTransInfo();
    static INT getTmpTexTransInfo(const void* pInTim2, VTransInfo& outTex, VTransInfo& outClut);
    static INT getFixTexTransInfo(const void* pInData, VTransInfo& outTex, VTransInfo& outClut, INT nInType);
    static BOOL sumupTmpTexSize(const TexInfo& inInfo, SHORT& nOutTex, SHORT& nOutClut);
    static INT getClutSize(const BYTE* pInTim2);
    static BOOL takeSnapShot(const CHAR* lpszInFile);
    static void makeCustomClut(ULONGLONG* inSrc, ULONGLONG* inDest, INT inType, INT inColNum);
    static void makeMonochromeClut(const TexInfo* inpInfo, INT inBufIdx, FLOAT inRate);
    static void makeMonochromeClut(const TexInfo* inpInfo, INT inSrcBufIdx, INT inOutBufIdx, FLOAT inRate);
    static void makeWhiteClut(const TexInfo* inpInfo, INT inOutBufIdx, FLOAT inRate);
    static void makeWhiteClut(const TexInfo* inpInfo, INT inSrcBufIdx, INT inOutBufIdx, FLOAT inRate);
};

#endif // SYSTEM_GRAPHIC_TEXUTIL_H
