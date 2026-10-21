/* SCE sample library tim2.c (TIM2 texture files) */
#include "include_asm.h"
#include "Tim2CheckFileHeader.h"

/* .rodata stays asm until every function is C: its strings and tables by label */
extern char D_004FF688[]; /* "TIM2" (in .sdata) */
extern char D_004FF690[]; /* "CLT2" (in .sdata) */
extern char D_004FC9C0[]; /* "Tim2CheckFileHeaer: TIM2 is broken %02X,%02X,%02X,%02X\n" */
extern char D_004FC9F8[]; /* "Tim2CheckFileHeaer: TIM2 is broken (2)\n" */
extern char D_004FCA20[]; /* "Tim2GetPictureHeader: Illegal image no.(%d)\n" */
extern const char mmsize_24[] __asm__("mmsize.24");
extern const char mmsize_28[] __asm__("mmsize.28");

extern int printf(const char *, ...);
extern int strncmp(const char *, const char *, size_t);

int Tim2CheckFileHeader(void *pTim2)
{
    TIM2_FILEHEADER *pFileHdr = (TIM2_FILEHEADER *)pTim2;
    int i;

    /* check the TIM2 signature */
    if (strncmp((char *)pFileHdr->FileId, D_004FF688, 4) == 0) {
        i = 1; /* TIM2 */
    } else if (strncmp((char *)pFileHdr->FileId, D_004FF690, 4) == 0) {
        i = 2; /* CLUT2 */
    } else {
        printf(D_004FC9C0, pFileHdr->FileId[0], pFileHdr->FileId[1], pFileHdr->FileId[2], pFileHdr->FileId[3]);
        return (0);
    }

    /* check the format version and id */
    if (!(pFileHdr->FormatVersion == 0x03 ||
          (pFileHdr->FormatVersion == 0x04 && (pFileHdr->FormatId == 0x00 || pFileHdr->FormatId == 0x01)))) {
        printf(D_004FC9F8);
        return (0);
    }
    return (i);
}

TIM2_PICTUREHEADER *Tim2GetPictureHeader(void *pTim2, int imgno)
{
    TIM2_FILEHEADER *pFileHdr = (TIM2_FILEHEADER *)pTim2;
    TIM2_PICTUREHEADER *pPictHdr;
    int i;

    if (imgno >= pFileHdr->Pictures) {
        printf(D_004FCA20, imgno);
        return (NULL);
    }

    if (pFileHdr->FormatId == 0x00) {
        pPictHdr = (TIM2_PICTUREHEADER *)((char *)pTim2 + sizeof(TIM2_FILEHEADER));
    } else {
        pPictHdr = (TIM2_PICTUREHEADER *)((char *)pTim2 + 0x80);
    }

    for (i = 0; i < imgno; i++) {
        pPictHdr = (TIM2_PICTUREHEADER *)((char *)pPictHdr + pPictHdr->TotalSize);
    }
    return (pPictHdr);
}

int Tim2IsClut2(TIM2_PICTUREHEADER *ph)
{
    if (ph->MipMapTextures == 0) {
        return (1);
    } else {
        return (0);
    }
}

/* its jump table would be in this object's .rodata: asm until the data comes from source */
#if 0
int Tim2GetMipMapPictureSize(TIM2_PICTUREHEADER *ph, int mipmap, int *pWidth, int *pHeight)
{
    int w, h, n;

    w = ph->ImageWidth >> mipmap;
    h = ph->ImageHeight >> mipmap;
    if (pWidth) {
        *pWidth = w;
    }
    if (pHeight) {
        *pHeight = h;
    }

    n = w * h;
    switch (ph->ImageType) {
    case TIM2_RGB16:
        n *= 2;
        break;
    case TIM2_RGB24:
        n *= 3;
        break;
    case TIM2_RGB32:
        n *= 4;
        break;
    case TIM2_IDTEX4:
        n /= 2;
        break;
    case TIM2_IDTEX8:
        break;
    }

    n = (n + 15) & ~15;
    return (n);
}
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2GetMipMapPictureSize);

TIM2_MIPMAPHEADER *Tim2GetMipMapHeader(TIM2_PICTUREHEADER *ph, int *pSize)
{
    TIM2_MIPMAPHEADER *pMmHdr;

    if (ph->MipMapTextures > 1) {
        pMmHdr = (TIM2_MIPMAPHEADER *)((char *)ph + sizeof(TIM2_PICTUREHEADER));
    } else {
        pMmHdr = NULL;
    }

    if (pSize) {
        *pSize = mmsize_24[ph->MipMapTextures];
    }
    return (pMmHdr);
}

void *Tim2GetUserSpace(TIM2_PICTUREHEADER *ph, int *pSize)
{
    void *pUserSpace;
    TIM2_EXHEADER *pExHdr;

    if (ph->HeaderSize == mmsize_28[ph->MipMapTextures]) {
        if (pSize)
            *pSize = 0;
        return (NULL);
    }

    pUserSpace = (void *)((char *)ph + mmsize_28[ph->MipMapTextures]);
    if (pSize) {
        pExHdr = (TIM2_EXHEADER *)pUserSpace;
        if (*(u_int *)pExHdr->ExHeaderId != 0x00745865) {
            *pSize = ph->HeaderSize - mmsize_28[ph->MipMapTextures];
        } else {
            *pSize = pExHdr->UserSpaceSize;
        }
    }
    return (pUserSpace);
}

void *Tim2GetUserData(TIM2_PICTUREHEADER *ph, int *pSize)
{
    void *pUserSpace;
    TIM2_EXHEADER *pExHdr;

    pUserSpace = Tim2GetUserSpace(ph, pSize);
    if (pUserSpace == NULL) {
        return (NULL);
    }

    pExHdr = (TIM2_EXHEADER *)pUserSpace;
    if (*(u_int *)pExHdr->ExHeaderId != 0x00745865) {
        return (pUserSpace);
    }

    if (pSize) {
        *pSize = pExHdr->UserDataSize;
    }
    return ((char *)pUserSpace + sizeof(TIM2_EXHEADER));
}

char *Tim2GetComment(TIM2_PICTUREHEADER *ph)
{
    void *pUserSpace;
    TIM2_EXHEADER *pExHdr;

    pUserSpace = Tim2GetUserSpace(ph, NULL);
    if (pUserSpace == NULL) {
        return (NULL);
    }

    pExHdr = (TIM2_EXHEADER *)pUserSpace;
    if (*(u_int *)pExHdr->ExHeaderId != 0x00745865) {
        return (NULL);
    }

    if (pExHdr->UserSpaceSize == ((sizeof(TIM2_EXHEADER) + pExHdr->UserDataSize))) {
        return (NULL);
    }
    return ((char *)pUserSpace + sizeof(TIM2_EXHEADER) + pExHdr->UserDataSize);
}

void *Tim2GetImage(TIM2_PICTUREHEADER *ph, int mipmap)
{
    void *pImage;
    TIM2_MIPMAPHEADER *pm;
    int i;

    if (mipmap >= ph->MipMapTextures) {
        return (NULL);
    }

    pImage = (void *)((char *)ph + ph->HeaderSize);
    if (ph->MipMapTextures == 1) {
        return (pImage);
    }

    pm = (TIM2_MIPMAPHEADER *)((char *)ph + sizeof(TIM2_PICTUREHEADER));
    for (i = 0; i < mipmap; i++) {
        pImage = (void *)((char *)pImage + pm->MMImageSize[i]);
    }
    return (pImage);
}

void *Tim2GetClut(TIM2_PICTUREHEADER *ph)
{
    void *pClut;

    if (ph->ClutColors == 0) {
        pClut = NULL;
    } else {
        pClut = (void *)((char *)ph + ph->HeaderSize + ph->ImageSize);
    }
    return (pClut);
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2GetClutColor);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2SetClutColor);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2GetTexel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2SetTexel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2GetTextureColor);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2LoadPicture);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2LoadImage);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2LoadClut);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2TakeSnapshot);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2LoadTexture);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2StoreTexture);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2CalcBufWidth);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2CalcBufSize);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2GetLog2);
