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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2LoadClut);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2TakeSnapshot);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2LoadTexture);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/Tim2CheckFileHeader", Tim2StoreTexture);

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
