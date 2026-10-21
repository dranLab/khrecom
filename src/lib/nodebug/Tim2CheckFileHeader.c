/* SCE sample library tim2.c (TIM2 texture files) */
#include "include_asm.h"
#include "Tim2CheckFileHeader.h"

/* .rodata stays asm until every function is C: its strings and tables by label */
extern char D_004FF688[]; /* "TIM2" (in .sdata) */
extern char D_004FF690[]; /* "CLT2" (in .sdata) */
extern char D_004FC9C0[]; /* "Tim2CheckFileHeaer: TIM2 is broken %02X,%02X,%02X,%02X\n" */
extern char D_004FC9F8[]; /* "Tim2CheckFileHeaer: TIM2 is broken (2)\n" */
extern char D_004FCA20[]; /* "Tim2GetPictureHeader: Illegal image no.(%d)\n" */
extern char D_004FCAF0[]; /* "Illegal clut and texture combination. ($%02X,$%02X)\n" */
extern char D_004FCB28[]; /* "Illegal pixel format.\n" */
extern char D_004FCB40[]; /* "file create failure.\n" */
extern const char mmsize_24[] __asm__("mmsize.24");
extern const char mmsize_28[] __asm__("mmsize.28");

extern int printf(const char *, ...);

typedef struct {
    u_long128 _data[6];
} sceGsLoadImage;
typedef struct {
    u_long128 _data[7];
} sceGsStoreImage;
#define WRITEBACK_DCACHE 0
extern int sceGsSetDefLoadImage(sceGsLoadImage *lp, short dbp, short dbw, short dpsm, short x, short y, short w, short h);
extern int sceGsExecLoadImage(sceGsLoadImage *lp, u_long128 *srcaddr);
extern int sceGsSetDefStoreImage(sceGsStoreImage *sp, short sbp, short sbw, short spsm, short x, short y, short w, short h);
extern int sceGsExecStoreImage(sceGsStoreImage *sp, u_long128 *dstaddr);
extern int sceGsSyncPath(int mode, u_short timeout);
extern void FlushCache(int);
extern void *memset(void *, int, size_t);
#define SCE_WRONLY 0x0002
#define SCE_CREAT 0x0200
extern int sceOpen(const char *filename, int flag, ...);
extern int sceClose(int fd);
extern int sceWrite(int fd, const void *buf, int nbyte);

typedef struct {
    u_int FBP : 9;
    u_int FBW : 6;
    u_int PSM : 5;
    u_int pad00 : 12;
    u_int DBX : 11;
    u_int DBY : 11;
    u_int pad01 : 10;
} sceGsDispfb;

typedef struct {
    u_int DX : 12;
    u_int DY : 11;
    u_int MAGH : 4;
    u_int MAGV : 2;
    u_int pad00 : 3;
    u_int DW : 12;
    u_int DH : 11;
    u_int pad01 : 9;
} sceGsDisplay;

typedef struct {
    u_int pmode[2]; /* tGS_PMODE */
    u_int smode2[2]; /* tGS_SMODE2 */
    sceGsDispfb dispfb;
    sceGsDisplay display;
    u_int bgcolor[2]; /* tGS_BGCOLOR */
} sceGsDispEnv;

static int Tim2CalcBufWidth(int psm, int w);
static int Tim2CalcBufSize(int psm, int w, int h);
static int Tim2GetLog2(int n);
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

#ifdef TIM2_C_JT /* jump table: needs the data from source */
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
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2GetMipMapPictureSize);
#endif

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

u_int Tim2GetClutColor(TIM2_PICTUREHEADER *ph, int clut, int no)
{
    unsigned char *pClut;
    int n;
    unsigned char r, g, b, a;

    pClut = Tim2GetClut(ph);
    if (pClut == NULL) {
        return (0);
    }

    /* which color entry */
    switch (ph->ImageType) {
    case TIM2_IDTEX4:
        n = clut * 16 + no;
        break;
    case TIM2_IDTEX8:
        n = clut * 256 + no;
        break;
    default:
        return (0); /* illegal pixel color */
    }
    if (n >= ph->ClutColors) {
        return (0);
    }

    /* reorder when the CLUT is stored in CSM1 order */
    switch ((ph->ClutType << 8) | ph->ImageType) {
    case (((TIM2_RGB16 | 0x40) << 8) | TIM2_IDTEX4):
    case (((TIM2_RGB24 | 0x40) << 8) | TIM2_IDTEX4):
    case (((TIM2_RGB32 | 0x40) << 8) | TIM2_IDTEX4):
    case ((TIM2_RGB16 << 8) | TIM2_IDTEX8):
    case ((TIM2_RGB24 << 8) | TIM2_IDTEX8):
    case ((TIM2_RGB32 << 8) | TIM2_IDTEX8):
        if ((n & 31) >= 8) {
            if ((n & 31) < 16) {
                n += 8; /* +8..15 to +16..23 */
            } else if ((n & 31) < 24) {
                n -= 8; /* +16..23 to +8..15 */
            }
        }
        break;
    }

    /* the color data, by the CLUT's pixel format */
    switch (ph->ClutType & 0x3F) {
    case TIM2_RGB16:
        r = (unsigned char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) << 3) & 0xF8);
        g = (unsigned char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) >> 2) & 0xF8);
        b = (unsigned char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) >> 7) & 0xF8);
        a = (unsigned char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) >> 8) & 0x80);
        break;

    case TIM2_RGB24:
        r = pClut[n * 3];
        g = pClut[n * 3 + 1];
        b = pClut[n * 3 + 2];
        a = 0x80;
        break;

    case TIM2_RGB32:
        r = pClut[n * 4];
        g = pClut[n * 4 + 1];
        b = pClut[n * 4 + 2];
        a = pClut[n * 4 + 3];
        break;

    default:
        r = 0;
        g = 0;
        b = 0;
        a = 0;
        break;
    }
    return ((a << 24) | (b << 16) | (g << 8) | r);
}

u_int Tim2SetClutColor(TIM2_PICTUREHEADER *ph, int clut, int no, u_int newcolor)
{
    unsigned char *pClut;
    unsigned char r, g, b, a;
    int n;

    pClut = Tim2GetClut(ph);
    if (pClut == NULL) {
        return (0);
    }

    /* which color entry */
    switch (ph->ImageType) {
    case TIM2_IDTEX4:
        n = clut * 16 + no;
        break;
    case TIM2_IDTEX8:
        n = clut * 256 + no;
        break;
    default:
        return (0); /* illegal pixel color */
    }
    if (n > ph->ClutColors) {
        return (0);
    }

    /* reorder when the CLUT is stored in CSM1 order */
    switch ((ph->ClutType << 8) | ph->ImageType) {
    case (((TIM2_RGB16 | 0x40) << 8) | TIM2_IDTEX4):
    case (((TIM2_RGB24 | 0x40) << 8) | TIM2_IDTEX4):
    case (((TIM2_RGB32 | 0x40) << 8) | TIM2_IDTEX4):
    case ((TIM2_RGB16 << 8) | TIM2_IDTEX8):
    case ((TIM2_RGB24 << 8) | TIM2_IDTEX8):
    case ((TIM2_RGB32 << 8) | TIM2_IDTEX8):
        if ((n & 31) >= 8) {
            if ((n & 31) < 16) {
                n += 8; /* +8..15 to +16..23 */
            } else if ((n & 31) < 24) {
                n -= 8; /* +16..23 to +8..15 */
            }
        }
        break;
    }

    /* the old color is returned, the new one stored, by the CLUT's pixel format */
    switch (ph->ClutType & 0x3F) {
    case TIM2_RGB16: {
        unsigned char rr, gg, bb, aa;
        r = (unsigned char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) << 3) & 0xF8);
        g = (unsigned char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) >> 2) & 0xF8);
        b = (unsigned char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) >> 7) & 0xF8);
        a = (unsigned char)((((pClut[n * 2 + 1] << 8) | pClut[n * 2]) >> 8) & 0x80);

        rr = (unsigned char)((newcolor >> 3) & 0x1F);
        gg = (unsigned char)((newcolor >> 11) & 0x1F);
        bb = (unsigned char)((newcolor >> 19) & 0x1F);
        aa = (unsigned char)((newcolor >> 31) & 1);

        pClut[n * 2] = (unsigned char)((((aa << 15) | (bb << 10) | (gg << 5) | rr)) & 0xFF);
        pClut[n * 2 + 1] = (unsigned char)((((aa << 15) | (bb << 10) | (gg << 5) | rr) >> 8) & 0xFF);
    } break;

    case TIM2_RGB24:
        r = pClut[n * 3];
        g = pClut[n * 3 + 1];
        b = pClut[n * 3 + 2];
        a = 0x80;
        pClut[n * 3] = newcolor;
        pClut[n * 3 + 1] = newcolor >> 8;
        pClut[n * 3 + 2] = newcolor >> 16;
        break;

    case TIM2_RGB32:
        r = pClut[n * 4];
        g = pClut[n * 4 + 1];
        b = pClut[n * 4 + 2];
        a = pClut[n * 4 + 3];
        pClut[n * 4] = newcolor;
        pClut[n * 4 + 1] = newcolor >> 8;
        pClut[n * 4 + 2] = newcolor >> 16;
        pClut[n * 4 + 3] = newcolor >> 24;
        break;

    default:
        r = 0;
        g = 0;
        b = 0;
        a = 0;
        break;
    }
    return ((a << 24) | (g << 16) | (b << 8) | r);
}

#ifdef TIM2_C_JT /* jump tables: need the data from source */
u_int Tim2GetTexel(TIM2_PICTUREHEADER *ph, int mipmap, int x, int y)
{
    unsigned char *pImage;
    int t;
    int w, h;

    pImage = Tim2GetImage(ph, mipmap);
    if (pImage == NULL) {
        return (0);
    }
    Tim2GetMipMapPictureSize(ph, mipmap, &w, &h);
    if (x > w || y > h) {
        return (0);
    }

    t = y * w + x;
    switch (ph->ImageType) {
    case TIM2_RGB16:
        return ((pImage[t * 2 + 1] << 8) | pImage[t * 2]);

    case TIM2_RGB24:
        return ((pImage[t * 3 + 2] << 16) | (pImage[t * 3 + 1] << 8) | pImage[t * 3]);

    case TIM2_RGB32:
        return ((pImage[t * 4 + 3] << 24) | (pImage[t * 4 + 2] << 16) | (pImage[t * 4 + 1] << 8) | pImage[t * 4]);

    case TIM2_IDTEX4:
        if (x & 1) {
            return (pImage[t / 2] >> 4);
        } else {
            return (pImage[t / 2] & 0x0F);
        }

    case TIM2_IDTEX8:
        return (pImage[t]);
    }
    return (0);
}

u_int Tim2SetTexel(TIM2_PICTUREHEADER *ph, int mipmap, int x, int y, u_int newcolor)
{
    unsigned char *pImage;
    int t;
    int w, h;
    u_int oldcolor;

    pImage = Tim2GetImage(ph, mipmap);
    if (pImage == NULL) {
        return (0);
    }
    Tim2GetMipMapPictureSize(ph, mipmap, &w, &h);
    if (x > w || y > h) {
        return (0);
    }

    t = y * w + x;
    switch (ph->ImageType) {
    case TIM2_RGB16:
        oldcolor = (pImage[t * 2 + 1] << 8) | pImage[t * 2];
        pImage[t * 2] = newcolor;
        pImage[t * 2 + 1] = newcolor >> 8;
        return (oldcolor);

    case TIM2_RGB24:
        oldcolor = (pImage[t * 3 + 2] << 16) | (pImage[t * 3 + 1] << 8) | pImage[t * 3];
        pImage[t * 3] = newcolor;
        pImage[t * 3 + 1] = newcolor >> 8;
        pImage[t * 3 + 2] = newcolor >> 16;
        return (oldcolor);

    case TIM2_RGB32:
        oldcolor = (pImage[t * 4 + 3] << 24) | (pImage[t * 4 + 2] << 16) | (pImage[t * 4 + 1] << 8) | pImage[t * 4];
        pImage[t * 4] = newcolor;
        pImage[t * 4 + 1] = newcolor >> 8;
        pImage[t * 4 + 2] = newcolor >> 16;
        pImage[t * 4 + 3] = newcolor >> 24;
        return (oldcolor);

    case TIM2_IDTEX4:
        if (x & 1) {
            oldcolor = pImage[t / 2] >> 4;
            pImage[t / 2] = (newcolor << 4) | oldcolor;
        } else {
            oldcolor = pImage[t / 2] & 0x0F;
            pImage[t / 2] = (oldcolor << 4) | newcolor;
        }
        return (oldcolor);

    case TIM2_IDTEX8:
        oldcolor = pImage[t];
        pImage[t] = newcolor;
        return (oldcolor);
    }
    return (0);
}

u_int Tim2GetTextureColor(TIM2_PICTUREHEADER *ph, int mipmap, int clut, int x, int y)
{
    u_int t;

    if (Tim2GetImage(ph, mipmap) == NULL) {
        return (0);
    }
    t = Tim2GetTexel(ph, mipmap, (x >> mipmap), (y >> mipmap));
    switch (ph->ImageType) {
    case TIM2_RGB16:
        {
            u_int r, g, b, a;
            r = (t << 3) & 0xF8;
            g = (t >> 2) & 0xF8;
            b = (t >> 7) & 0xF8;
            a = (t >> 8) & 0x80;
            return ((a << 24) | (g << 16) | (b << 8) | r);
        }

    case TIM2_RGB24:
        return ((t & 0x00FFFFFF) | 0x80000000);

    case TIM2_RGB32:
        return (t);

    case TIM2_IDTEX4:
    case TIM2_IDTEX8:
        return (Tim2GetClutColor(ph, clut, t));
    }
    return (0);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2GetTexel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2SetTexel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2GetTextureColor);
#endif
#ifdef TIM2_C_JT /* psmtbl: needs the data from source */
u_int Tim2LoadPicture(TIM2_PICTUREHEADER *ph, u_int tbp, u_int cbp)
{
    tbp = Tim2LoadImage(ph, tbp);
    Tim2LoadClut(ph, cbp);
    return (tbp);
}

u_int Tim2LoadImage(TIM2_PICTUREHEADER *ph, u_int tbp)
{
    /* CLUT2 files have no image */
    if (ph->MipMapTextures > 0) {
        static const int psmtbl[] = { SCE_GS_PSMCT16, SCE_GS_PSMCT24, SCE_GS_PSMCT32, SCE_GS_PSMT4, SCE_GS_PSMT8 };
        int psm;
        u_long128 *pImage;
        int w, h;
        int tbw;

        psm = psmtbl[ph->ImageType - 1]; /* pixel format */
        ((sceGsTex0 *)&ph->GsTex0)->PSM = psm;

        w = ph->ImageWidth; /* image width */
        h = ph->ImageHeight; /* image height */

        /* address of the image data */
        pImage = (u_long128 *)((char *)ph + ph->HeaderSize);

        /* override tbp, tbw of the header's GsTex0 */
        tbw = Tim2CalcBufWidth(psm, w);
        ((sceGsTex0 *)&ph->GsTex0)->TBP0 = tbp;
        ((sceGsTex0 *)&ph->GsTex0)->TBW = tbw;
        Tim2LoadTexture(psm, tbp, tbw, w, h, pImage); /* send the texture to the GS */

        /* add the size of the level 0 texture */
        tbp += Tim2CalcBufSize(psm, w, h);
    }
    return (tbp);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2LoadPicture);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2LoadImage);
#endif
u_int Tim2LoadClut(TIM2_PICTUREHEADER *ph, u_int cbp)
{
    int i;
    sceGsLoadImage li;
    u_long128 *pClut;
    int cpsm;

    /* the CLUT's pixel format */
    if (ph->ClutType == TIM2_NONE) {
        /* no CLUT data */
        return (1);
    } else if ((ph->ClutType & 0x3F) == TIM2_RGB16) {
        cpsm = SCE_GS_PSMCT16;
    } else if ((ph->ClutType & 0x3F) == TIM2_RGB24) {
        cpsm = SCE_GS_PSMCT24;
    } else {
        cpsm = SCE_GS_PSMCT32;
    }
    ((sceGsTex0 *)&ph->GsTex0)->CPSM = cpsm; /* CLUT pixel format */
    ((sceGsTex0 *)&ph->GsTex0)->CSM = 0; /* CLUT storage mode (always CSM1) */
    ((sceGsTex0 *)&ph->GsTex0)->CSA = 0; /* CLUT entry offset (always 0) */
    ((sceGsTex0 *)&ph->GsTex0)->CLD = 1; /* CLUT buffer load control (always load) */

    if (cbp == -1) {
        /* no cbp given: take it from the header's GsTex0 */
        cbp = ((sceGsTex0 *)&ph->GsTex0)->CBP;
    } else {
        /* cbp given: override the header's GsTex0 */
        ((sceGsTex0 *)&ph->GsTex0)->CBP = cbp;
    }

    /* address of the CLUT data */
    pClut = (u_long128 *)((char *)ph + ph->HeaderSize + ph->ImageSize);

    /* send the CLUT data to the GS local memory; its layout depends on the CLUT and texture formats */
    switch ((ph->ClutType << 8) | ph->ImageType) {
    case (((TIM2_RGB16 | 0x40) << 8) | TIM2_IDTEX4):
    case (((TIM2_RGB24 | 0x40) << 8) | TIM2_IDTEX4):
    case (((TIM2_RGB32 | 0x40) << 8) | TIM2_IDTEX4):
    case ((TIM2_RGB16 << 8) | TIM2_IDTEX8):
    case ((TIM2_RGB24 << 8) | TIM2_IDTEX8):
    case ((TIM2_RGB32 << 8) | TIM2_IDTEX8):
        /* already in CSM1 order: send as is */
        Tim2LoadTexture(cpsm, cbp, 1, 16, (ph->ClutColors / 16), pClut);
        break;

    case ((TIM2_RGB16 << 8) | TIM2_IDTEX4):
    case ((TIM2_RGB24 << 8) | TIM2_IDTEX4):
    case ((TIM2_RGB32 << 8) | TIM2_IDTEX4):
    case (((TIM2_RGB16 | 0x80) << 8) | TIM2_IDTEX4):
    case (((TIM2_RGB24 | 0x80) << 8) | TIM2_IDTEX4):
    case (((TIM2_RGB32 | 0x80) << 8) | TIM2_IDTEX4):
    case (((TIM2_RGB16 | 0x80) << 8) | TIM2_IDTEX8):
    case (((TIM2_RGB24 | 0x80) << 8) | TIM2_IDTEX8):
    case (((TIM2_RGB32 | 0x80) << 8) | TIM2_IDTEX8):
        /* linear order: reorder to CSM1 while sending */
        for (i = 0; i < (ph->ClutColors / 16); i++) {
            sceGsSetDefLoadImage(&li, cbp, 1, cpsm, (i & 1) * 8, (i >> 1) * 2, 8, 2);
            FlushCache(WRITEBACK_DCACHE);
            sceGsExecLoadImage(&li, pClut);
            sceGsSyncPath(0, 0);

            /* next 16 colors */
            if ((ph->ClutType & 0x3F) == TIM2_RGB16) {
                pClut = (u_long128 *)((char *)pClut + 2 * 16);
            } else if ((ph->ClutType & 0x3F) == TIM2_RGB24) {
                pClut = (u_long128 *)((char *)pClut + 3 * 16);
            } else {
                pClut = (u_long128 *)((char *)pClut + 4 * 16);
            }
        }
        break;

    default:
        printf(D_004FCAF0, ph->ClutType, ph->ImageType);
        return (0);
    }
    return (1);
}
int Tim2TakeSnapshot(sceGsDispEnv *d0, sceGsDispEnv *d1, char *pszFname)
{
    int i;

    int h; /* file handle */
    int nWidth, nHeight; /* image size */
    int nImageType; /* image type */
    int psm; /* pixel format */
    int nBytes; /* bytes per raster */

    /* image size and pixel format */
    nWidth = d0->display.DW / (d0->display.MAGH + 1);
    nHeight = d0->display.DH + 1;
    psm = d0->dispfb.PSM;

    /* bytes per raster and TIM2 image type from the pixel format */
    switch (psm) {
    case SCE_GS_PSMCT32:
        nBytes = nWidth * 4;
        nImageType = TIM2_RGB32;
        break;
    case SCE_GS_PSMCT24:
        nBytes = nWidth * 3;
        nImageType = TIM2_RGB24;
        break;
    case SCE_GS_PSMCT16:
        nBytes = nWidth * 2;
        nImageType = TIM2_RGB16;
        break;
    case SCE_GS_PSMCT16S:
        nBytes = nWidth * 2;
        nImageType = TIM2_RGB16;
        break;
    default:
        /* illegal pixel format (GS_PSGPU24 is not supported) */
        printf(D_004FCB28);
        return (0);
    }

    /* open the TIM2 file */
    h = sceOpen(pszFname, SCE_WRONLY | SCE_CREAT);
    if (h == -1) {
        printf(D_004FCB40);
        return (0);
    }

    /* the file header */
    {
        TIM2_FILEHEADER fhdr;

        fhdr.FileId[0] = 'T';
        fhdr.FileId[1] = 'I';
        fhdr.FileId[2] = 'M';
        fhdr.FileId[3] = '2';
        fhdr.FormatVersion = 3;
        fhdr.FormatId = 0; /* 16 byte alignment */
        fhdr.Pictures = 1;
        for (i = 0; i < 8; i++) {
            fhdr.Reserved[i] = 0;
        }

        sceWrite(h, &fhdr, sizeof(TIM2_FILEHEADER));
    }

    /* the picture header */
    {
        TIM2_PICTUREHEADER phdr;
        int nImageSize = nBytes * nHeight;

        phdr.TotalSize = sizeof(TIM2_PICTUREHEADER) + nImageSize;
        phdr.ClutSize = 0;
        phdr.ImageSize = nImageSize;
        phdr.HeaderSize = sizeof(TIM2_PICTUREHEADER);
        phdr.ClutColors = 0;
        phdr.PictFormat = 0;
        phdr.MipMapTextures = 1;
        phdr.ClutType = TIM2_NONE;
        phdr.ImageType = nImageType;
        phdr.ImageWidth = nWidth;
        phdr.ImageHeight = nHeight;

        /* all GS registers 0 */
        phdr.GsTex0 = 0;
        ((sceGsTex0 *)&phdr.GsTex0)->TBW = Tim2CalcBufWidth(psm, nWidth);
        ((sceGsTex0 *)&phdr.GsTex0)->PSM = psm;
        ((sceGsTex0 *)&phdr.GsTex0)->TW = Tim2GetLog2(nWidth);
        ((sceGsTex0 *)&phdr.GsTex0)->TH = Tim2GetLog2(nHeight);
        phdr.GsTex1 = 0;
        phdr.GsRegs = 0;
        phdr.GsTexClut = 0;

        sceWrite(h, &phdr, sizeof(TIM2_PICTUREHEADER));
    }

    /* the image data */
    for (i = 0; i < nHeight; i++) {
        u_char buf[4096]; /* raster buffer */
        sceGsStoreImage si;

        memset(buf, 0, sizeof(buf));
        if (d1) {
            /* interlaced */
            if (!(i & 1)) {
                /* even raster */
                sceGsSetDefStoreImage(&si, d0->dispfb.FBP * 32, d0->dispfb.FBW, psm, d0->dispfb.DBX,
                                      (d0->dispfb.DBY + i / 2), nWidth, 1);
            } else {
                /* odd raster */
                sceGsSetDefStoreImage(&si, d1->dispfb.FBP * 32, d1->dispfb.FBW, psm, d1->dispfb.DBX,
                                      (d1->dispfb.DBY + i / 2), nWidth, 1);
            }
        } else {
            /* non-interlaced */
            sceGsSetDefStoreImage(&si, d0->dispfb.FBP * 32, d0->dispfb.FBW, psm, d0->dispfb.DBX,
                                  (d0->dispfb.DBY + i), nWidth, 1);
        }
        FlushCache(WRITEBACK_DCACHE);

        sceGsExecStoreImage(&si, (u_long128 *)buf); /* request the VRAM transfer */
        sceGsSyncPath(0, 0); /* wait for it */

        sceWrite(h, buf, nBytes); /* write one raster */
    }

    sceClose(h);
    return (1);
}
#ifdef TIM2_C_JT /* jump tables: need the data from source */
void Tim2LoadTexture(int psm, u_int tbp, int tbw, int w, int h, u_long128 *pImage)
{
    sceGsLoadImage li;
    int i, l, n;
    u_long128 *p;

    switch (psm) {
    case SCE_GS_PSMZ32:
    case SCE_GS_PSMCT32:
        n = w * 4;
        break;

    case SCE_GS_PSMZ24:
    case SCE_GS_PSMCT24:
        n = w * 3;
        break;

    case SCE_GS_PSMZ16:
    case SCE_GS_PSMZ16S:
    case SCE_GS_PSMCT16:
    case SCE_GS_PSMCT16S:
        n = w * 2;
        break;

    case SCE_GS_PSMT8H:
    case SCE_GS_PSMT8:
        n = w;
        break;

    case SCE_GS_PSMT4HL:
    case SCE_GS_PSMT4HH:
    case SCE_GS_PSMT4:
        n = w / 2;
        break;

    default:
        return;
    }

    /* send in pieces below the 512KB DMA limit */
    l = 32764 * 16 / n;
    for (i = 0; i < h; i += l) {
        p = (u_long128 *)((char *)pImage + n * i);
        if ((i + l) > h) {
            l = h - i;
        }

        sceGsSetDefLoadImage(&li, tbp, tbw, psm, 0, i, w, l);
        FlushCache(WRITEBACK_DCACHE);
        sceGsExecLoadImage(&li, p);
        sceGsSyncPath(0, 0);
    }
}

void Tim2StoreTexture(int psm, u_int tbp, int tbw, int w, int h, u_long128 *pImage)
{
    sceGsStoreImage si;
    int i, l, n;
    u_long128 *p;

    switch (psm) {
    case SCE_GS_PSMZ32:
    case SCE_GS_PSMCT32:
        n = w * 4;
        break;

    case SCE_GS_PSMZ24:
    case SCE_GS_PSMCT24:
        n = w * 3;
        break;

    case SCE_GS_PSMZ16:
    case SCE_GS_PSMZ16S:
    case SCE_GS_PSMCT16:
    case SCE_GS_PSMCT16S:
        n = w * 2;
        break;

    case SCE_GS_PSMT8H:
    case SCE_GS_PSMT8:
        n = w;
        break;

    case SCE_GS_PSMT4HL:
    case SCE_GS_PSMT4HH:
    case SCE_GS_PSMT4:
        n = w / 2;
        break;

    default:
        return;
    }

    /* read in pieces below the 512KB DMA limit */
    l = 32764 * 16 / n;
    for (i = 0; i < h; i += l) {
        p = (u_long128 *)((char *)pImage + n * i);
        if ((i + l) > h) {
            l = h - i;
        }

        sceGsSetDefStoreImage(&si, tbp, tbw, psm, 0, i, w, l);
        FlushCache(WRITEBACK_DCACHE);
        sceGsExecStoreImage(&si, p);
        sceGsSyncPath(0, 0);
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2LoadTexture);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2StoreTexture);
#endif

#ifdef TIM2_C_JT /* jump table: needs the data from source */
/* buffer width in units of 64 pixels */
static int Tim2CalcBufWidth(int psm, int w)
{
    switch (psm) {
    case SCE_GS_PSMT8H:
    case SCE_GS_PSMT4HL:
    case SCE_GS_PSMT4HH:
    case SCE_GS_PSMCT32:
    case SCE_GS_PSMCT24:
    case SCE_GS_PSMZ32:
    case SCE_GS_PSMZ24:
    case SCE_GS_PSMCT16:
    case SCE_GS_PSMCT16S:
    case SCE_GS_PSMZ16:
    case SCE_GS_PSMZ16S:
        return ((w + 63) / 64);

    case SCE_GS_PSMT8:
    case SCE_GS_PSMT4:
        w = (w + 63) / 64;
        if (w & 1) {
            w++;
        }
        return (w);
    }
    return (0);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2CalcBufWidth);
#endif

/* buffer size in units of 256 bytes (64 words) */
static int Tim2CalcBufSize(int psm, int w, int h)
{
    return (w * h / 64);
}

/* log2, rounded up */
static int Tim2GetLog2(int n)
{
    int i;

    for (i = 31; i > 0; i--) {
        if (n & (1 << i)) {
            break;
        }
    }
    if (n > (1 << i)) {
        i++;
    }
    return (i);
}
