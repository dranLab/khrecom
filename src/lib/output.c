/* libmpeg output.c: hands decoded pictures to the display (fields named by offset: the library has no
   type information) */
#include "include_asm.h"

typedef struct _sceMpegPicture _sceMpegPicture;

struct _sceMpegPicture {
    unsigned int image; /* 0x00 */
    int w; /* 0x04 */
    int h; /* 0x08 */
    int rows; /* 0x0C */
    int width; /* 0x10 */
    int _pad14; /* 0x14 */
    long pts; /* 0x18 */
    long dts; /* 0x20 */
    int pictType; /* 0x28 */
    int flag2C; /* 0x2C */
    int flag30; /* 0x30 */
    int flag34; /* 0x34 */
    int flag38; /* 0x38 */
    int flag3C; /* 0x3C */
    int flag40; /* 0x40 */
    int info[6]; /* 0x44 */
    int v5C; /* 0x5C */
    int v60; /* 0x60 */
};

typedef struct {
    char _pad00[0x4];
    int outputPending; /* 0x04 */
    int state; /* 0x08 */
    char _pad0C[0x8C - 0xC];
    int hasPts; /* 0x8C */
    volatile long ptsStep; /* 0x90 */
    volatile int lastPts; /* 0x98 */
    volatile int ptsField; /* 0x9C */
    volatile long showCount; /* 0xA0 */
    char _padA8[0xC0 - 0xA8];
    int outputBase; /* 0xC0 */
    int useCsc; /* 0xC4 */
    int info[6]; /* 0xC8 */
    int vE0; /* 0xE0 */
    int vE4; /* 0xE4 */
    char _padE8[0xEC - 0xE8];
    unsigned int refBase; /* 0xEC */
    int bufW; /* 0xF0 */
    int width; /* 0xF4 */
    int stride; /* 0xF8 */
    char _padFC[0x100 - 0xFC];
    long nextPts; /* 0x100 */
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
    char _pad1F8[0x834 - 0x1F8];
    int outputReady; /* 0x834 */
    char _pad838[0x868 - 0x838];
    long *tags; /* 0x868 */
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
extern int sceSnprintf(char *buf, int n, const char *fmt, ...);
extern void _sceMpegCscStoreRefImage(_sceMpegDecoder *d, _sceMpegPicture *pict);
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
/* Parked: 24 of 189 lines, 4 bytes long (2026-11-06, first attempt). Worked: ptsStep, lastPts, ptsField and showCount are
   volatile (reloaded across __muldi3, a full ld before the truncation), one embedded read of ptsStep, the flag word's
   or tree (96 forms searched). Left: the original reloads tags in the beqz delay slot, filled by GNU as, not gcc (as only
   swaps the insn right before the branch, so the lw has to follow the `and`; hasPts/outputMode/nextPts look volatile
   too: gcc must not fill that slot from the target, and _dispRefImageField reloads hasPts after the same store).
   Tried: volatile tags (the lw lands above the `and`, 24-30 lines), tags through a union (order right, slot still
   nop, 32), all on one line for the $LM rule. `*pPts = pict->pts` goes through s5 there; the size test's flag is in s0.
   _dispRefImageField is this body twice: wait for this one. */
#if 0
static void _dispRefImage(_sceMpegDecoder *d, _sceMpegPicture *pict, int n)
{
    char buf[256];
    long *tags = d->tags;
    long *pPts = &tags[2];
    long *pDts = &tags[3];
    long *pFlags = &tags[4];
    int ok;

    if (d->hasPts) {
        long v = pict->pts;
        if (v < 0 && d->lastPts >= 0) {
            int n = d->showCount;
            long odd = n & 1;
            long step;
            int adj = odd & (d->ptsField & 1) & ((step = d->ptsStep) & 1);
            tags[2] = d->lastPts + ((int)((step * n) >> 1) + adj);
            odd &= d->ptsStep;
            tags = d->tags;
            if (odd) d->ptsField++;
        } else {
            *pPts = v;
        }
    } else {
        *pPts = pict->pts;
    }
    if (d->outputMode == 2 && d->nextPts >= 0) {
        *pPts = d->nextPts;
        d->outputMode = 0;
        d->nextPts = -1;
    }
    *pDts = pict->dts;
    *pFlags = (((long)pict->flag3C << 6) | ((long)pict->flag40 << 5)) | ((pict->flag2C | ((long)pict->flag34 << 8)) | (((long)pict->flag30 << 3) | ((long)pict->flag38 << 7)));
    d->lastPts = tags[2];
    d->showCount = (unsigned int)_showCount[(int)(tags[4] >> 5) & 0xF];
    d->vE0 = pict->v5C;
    d->vE4 = pict->v60;
    d->info[0] = pict->info[0];
    d->info[1] = pict->info[1];
    d->info[2] = pict->info[2];
    d->info[3] = pict->info[3];
    d->info[4] = pict->info[4];
    d->info[5] = pict->info[5];
    if (d->width) {
        ok = d->bufW >= pict->w && d->width >= pict->h;
    } else {
        ok = d->stride >= pict->rows * pict->width;
    }
    if (!ok) {
        sceSnprintf(buf, 256, "Too small buffer size for %dx%d picture\n", pict->w, pict->h);
        _sceMpegError(d, buf);
        if (!ok) return;
    }
    if (pict->pictType == 1) {
        if (d->useCsc) {
            _sceMpegCscStoreRefImage(d, pict);
        } else {
            _cpr8(d, pict);
        }
        if (d->state != 2) {
            d->state = 2;
            d->outputBase = d->frameCount;
        }
        d->outputReady = 1;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/output", _dispRefImage);
#endif
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
