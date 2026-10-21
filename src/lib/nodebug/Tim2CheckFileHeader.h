/* SCE sample library tim2.h */
#ifndef NODEBUG_TIM2_H
#define NODEBUG_TIM2_H

#define NULL 0
typedef unsigned int size_t;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef int u_long128 __attribute__((mode(TI)));

/* libgraph pixel formats */
#define SCE_GS_PSMCT32 0
#define SCE_GS_PSMCT24 1
#define SCE_GS_PSMCT16 2
#define SCE_GS_PSMCT16S 10
#define SCE_GS_PSMT8 19
#define SCE_GS_PSMT4 20
#define SCE_GS_PSMT8H 27
#define SCE_GS_PSMT4HL 36
#define SCE_GS_PSMT4HH 44
#define SCE_GS_PSMZ32 48
#define SCE_GS_PSMZ24 49
#define SCE_GS_PSMZ16 50
#define SCE_GS_PSMZ16S 58

typedef struct {
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
} sceGsTex0;

enum TIM2_gattr_type {
    TIM2_NONE = 0, /* no CLUT (for ImageType) */
    TIM2_RGB16, /* 16 bit color (both ClutType and ImageType) */
    TIM2_RGB24, /* 24 bit color (only ImageType) */
    TIM2_RGB32, /* 32 bit color (both ClutType and ImageType) */
    TIM2_IDTEX4, /* 16 colors texture (only ImageType) */
    TIM2_IDTEX8 /* 256 colors texture (only ImageType) */
};

typedef struct {
    u_char FileId[4]; /* ID of the file ('T','I','M','2' or 'C','L','T','2') */
    u_char FormatVersion; /* version number of the format */
    u_char FormatId; /* ID of the format */
    u_short Pictures; /* number of picture data */
    u_char Reserved[8]; /* padding */
} TIM2_FILEHEADER;

typedef struct {
    u_int TotalSize; /* total size of the picture data in bytes */
    u_int ClutSize; /* CLUT data size in bytes */
    u_int ImageSize; /* image data size in bytes */
    u_short HeaderSize; /* header size in bytes */
    u_short ClutColors; /* total color number in CLUT */
    u_char PictFormat; /* ID of the picture format (must be 0) */
    u_char MipMapTextures; /* number of MIPMAP texture */
    u_char ClutType; /* type of the CLUT data */
    u_char ImageType; /* type of the image data */
    u_short ImageWidth; /* width of the picture */
    u_short ImageHeight; /* height of the picture */
    u_long GsTex0; /* TEX0 */
    u_long GsTex1; /* TEX1 */
    u_int GsRegs; /* TEXA, FBA, PABE */
    u_int GsTexClut; /* TEXCLUT */
} TIM2_PICTUREHEADER;

typedef struct {
    u_long GsMiptbp1; /* MIPTBP1 */
    u_long GsMiptbp2; /* MIPTBP2 */
    u_int MMImageSize[0]; /* image size of the MIPMAP texture in bytes */
} TIM2_MIPMAPHEADER;

typedef struct {
    u_char ExHeaderId[4]; /* ID of the extended comment header ('e','X','t','\0') */
    u_int UserSpaceSize; /* size of the user space */
    u_int UserDataSize; /* size of the user data */
    u_int Reserved; /* reserved */
} TIM2_EXHEADER;

u_int Tim2LoadImage(TIM2_PICTUREHEADER *ph, u_int tbp);
u_int Tim2LoadClut(TIM2_PICTUREHEADER *ph, u_int cbp);
int Tim2LoadTexture(int psm, u_int tbp, int tbw, int w, int h, u_long128 *pImage);
void *Tim2GetImage(TIM2_PICTUREHEADER *ph, int mipmap);
void *Tim2GetClut(TIM2_PICTUREHEADER *ph);
int Tim2GetMipMapPictureSize(TIM2_PICTUREHEADER *ph, int mipmap, int *pWidth, int *pHeight);
u_int Tim2GetTexel(TIM2_PICTUREHEADER *ph, int mipmap, int x, int y);
u_int Tim2GetClutColor(TIM2_PICTUREHEADER *ph, int clut, int no);

#endif
