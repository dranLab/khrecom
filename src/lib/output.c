/* libmpeg output.c: hands decoded pictures to the display (fields named by offset: the library has no
   type information) */
#include "include_asm.h"

typedef struct _sceMpegPicture _sceMpegPicture;

struct _sceMpegPicture {
    unsigned int image; /* 0x00 */
    char _pad04[0xC - 0x4];
    int rows; /* 0x0C */
    int width; /* 0x10 */
};

typedef struct {
    char _pad00[0x4];
    int outputPending; /* 0x04 */
    int state; /* 0x08 */
    char _pad0C[0xC0 - 0xC];
    int outputBase; /* 0xC0 */
    char _padC4[0xEC - 0xC4];
    unsigned int refBase; /* 0xEC */
    char _padF0[0xF4 - 0xF0];
    int width; /* 0xF4 */
    int stride; /* 0xF8 */
    char _padFC[0x108 - 0xFC];
    int outputMode; /* 0x108 */
    char _pad10C[0x128 - 0x10C];
    int frameCount; /* 0x128 */
    char _pad12C[0x130 - 0x12C];
    int fieldMissing; /* 0x130 */
    char _pad134[0x160 - 0x134];
    int pictStructNext; /* 0x160 */
    char _pad164[0x184 - 0x164];
    int pictStruct; /* 0x184 */
    char _pad188[0x1C8 - 0x188];
    _sceMpegPicture *frame[4]; /* 0x1C8 */
    _sceMpegPicture *top[4]; /* 0x1D8 */
    _sceMpegPicture *bottom[4]; /* 0x1E8 */
} _sceMpegDecoder;

typedef struct {
    char _pad00[0x8];
    int frameCount; /* 0x08 */
    char _pad0C[0x40 - 0xC];
    _sceMpegDecoder *sys; /* 0x40 */
} sceMpeg;

#define FRAME_PICTURE 3

extern void _sceMpegError(_sceMpegDecoder *d, const char *msg);
static void _dispRefImage(_sceMpegDecoder *d, _sceMpegPicture *pict, int n);
static void _dispRefImageField(_sceMpegDecoder *d, _sceMpegPicture *top, _sceMpegPicture *bottom, int n);

/* fields shown per picture, by picture structure and repeat flags */
static int _showCount[16] = { 2, 0, 2, 0, 2, 3, 2, 3, 0, 0, 0, 0, 2, 4, 0, 6 };

/* keeps _dispRefImage's string in .rodata while it is asm */
static inline const char *_dispRefImage_string(void)
{
    return "Too small buffer size for %dx%d picture\n";
}

#define D8_CHCR ((volatile unsigned int *)0x1000D000)
#define D8_MADR ((volatile unsigned int *)0x1000D010)
#define D8_QWC ((volatile unsigned int *)0x1000D020)
#define D8_SADR ((volatile unsigned int *)0x1000D080)
#define D9_CHCR ((volatile unsigned int *)0x1000D400)
#define D9_MADR ((volatile unsigned int *)0x1000D410)
#define D9_QWC ((volatile unsigned int *)0x1000D420)
#define D9_SADR ((volatile unsigned int *)0x1000D480)

static void _cpr8(_sceMpegDecoder *d, _sceMpegPicture *pict)
{
    int fields;
    unsigned int dst0;
    unsigned int src;
    int srcStep;
    int dstStep;
    int qwc;
    int i, j;

    src = pict->image & 0x0FFFFFFF;
    dst0 = d->refBase & 0x0FFFFFFF;
    if (d->pictStruct == FRAME_PICTURE || d->width == 0) {
        srcStep = pict->width * 0x180;
        qwc = srcStep >> 4;
        if (d->width) {
            dstStep = (d->width >> 4) * 0x180;
        } else {
            dstStep = srcStep;
        }
        fields = 1;
    } else {
        dstStep = (d->width >> 4) * 0xC0;
        srcStep = (pict->width >> 1) * 0x180;
        fields = 2;
        qwc = srcStep >> 4;
    }
    for (i = 0; i < fields; i++) {
        unsigned int dst = dst0;
        for (j = 0; j < pict->rows; j++) {
            int ie;
            unsigned int nextSrc, nextDst;

            ie = DIntr();
            *D9_SADR = 0;
            *D9_MADR = src;
            *D9_QWC = qwc;
            *D9_CHCR = 0x101;
            if (ie) EIntr();
            nextSrc = src + srcStep;
            nextDst = dst + dstStep;
            while (*D9_CHCR & 0x100) {
            }
            ie = DIntr();
            *D8_SADR = 0;
            *D8_MADR = dst;
            *D8_QWC = qwc;
            *D8_CHCR = 0x100;
            if (ie) EIntr();
            while (*D8_CHCR & 0x100) {
            }
            while (*D8_QWC) {
            }
            dst = nextDst;
            src = nextSrc;
        }
        dst0 += d->stride * 0xC0;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/output", _dispRefImage);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/output", _dispRefImageField);
/* gcc still thinks it is in .rdata (the next string follows without a section change) */
__asm__(".rdata");

int _sceMpegFlush(sceMpeg *mp)
{
    _sceMpegDecoder *d = mp->sys;
    int ret = 0;

    if (d->outputPending && d->state) {
        int n = d->frameCount;

        if (d->fieldMissing)
            _sceMpegError(d, "the second field is missing");
        else if (d->pictStruct == FRAME_PICTURE)
            _dispRefImage(d, d->frame[1], n - 1);
        else
            _dispRefImageField(d, d->top[1], d->bottom[1], n - 1);
        d->fieldMissing = 0;
        mp->frameCount = d->frameCount - d->outputBase;
        d->outputPending = 0;
        ret = 1;
    }
    return ret;
}

void _sceMpegOutputFrame(_sceMpegDecoder *d, int n, int output)
{
    if (output) {
        if (d->pictStruct == FRAME_PICTURE) {
            _dispRefImage(d, d->pictStructNext == FRAME_PICTURE ? d->frame[3] : d->frame[0], n - 1);
        } else {
            _sceMpegPicture *top, *bottom;

            if (d->pictStructNext == FRAME_PICTURE) {
                top = d->top[3];
                bottom = d->bottom[3];
            } else {
                top = d->top[0];
                bottom = d->bottom[0];
            }
            _dispRefImageField(d, top, bottom, n - 1);
        }
    }
    if (d->outputMode == 1)
        d->outputMode = 2;
}
// SOURCE DATA: .data .rodata
