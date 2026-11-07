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
    long ptsStep; /* 0x90 */
    int lastPts; /* 0x98 */
    int ptsField; /* 0x9C */
    long showCount; /* 0xA0 */
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
/* MATCH: the union store has alias set 0, so it conflicts with every field: d->tags, ptsStep and ptsField reload after
   it (strict aliasing folds them after a plain long store). The flag word's three locals keep their pairs, which one
   expression does not: gcc 2.9 re-pairs the terms. */
static void _dispRefImage(_sceMpegDecoder *d, _sceMpegPicture *pict, int n)
{
    char buf[256];
    long *tags = d->tags;
    long *pPts = &tags[2];
    long *pDts = &tags[3];
    long *pFlags = &tags[4];
    int ok;

    if (d->hasPts && pict->pts < 0 && d->lastPts >= 0) {
        int n = d->showCount;
        long odd = n & 1;
        long step;
        int r;
        int ia;
        int q;
        long adj = odd & (d->ptsField & 1);
        { long sb = (step = d->ptsStep) & 1; adj &= sb; }
        ia = adj;
        q = (step * n) >> 1;
        asm volatile("" : : : "memory"); /* MATCH: lastPts reloads after __muldi3 (a const libcall), behind the shift */
        r = q + ia;
        ((union { long l; int i; } *)&tags[2])->l = d->lastPts + r;
        odd &= d->ptsStep; tags = d->tags; if (odd) d->ptsField++; } else {
        *pPts = pict->pts;
    }
    if (d->outputMode == 2 && d->nextPts >= 0) {
        *pPts = d->nextPts;
        d->outputMode = 0;
        d->nextPts = -1;
    }
    *pDts = pict->dts;
    {
        long a = ((long)pict->flag40 << 5) | ((long)pict->flag3C << 6);
        long b = ((long)pict->flag34 << 8) | (long)pict->flag2C;
        long c = ((long)pict->flag38 << 7) | ((long)pict->flag30 << 3);
        *pFlags = (b | a) | c;
    }
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
        asm volatile("" : "=r"(ok) : "0"(ok)); /* MATCH: ok stays live across the call; cse would fold this test from the branch above */
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
/* Parked: 13 of 272 lines, same size, _dispRefImage's body twice with showCount known to be 1 (the second __muldi3 folds).
   Left: in the second body the value is in a1 for v1, step in v1 for a0, sb in a0 for a1 (a local for the value only
   adds a truncation; six shapes of that body, 13-16); `lw t1` (hasPts, a PRE copy) is issued before the addiu: it is
   the only insn ready that cycle, and nothing natural delays it. */
#if 0
static void _dispRefImageField(_sceMpegDecoder *d, _sceMpegPicture *top, _sceMpegPicture *bottom, int n)
{
    char buf[256];
    int bit = 0;
    _sceMpegPicture *first;
    _sceMpegPicture *second;
    long *tags;
    long *pPts;
    long *pDts;
    long *pFlags;
    long *pPts2;
    long *pDts2;
    long *pFlags2;
    int ok;

    if (d->pictStruct == 2) {
        first = top;
        second = bottom;
        bit = 0x40;
    } else {
        first = bottom;
        second = top;
    }
    tags = d->tags;
    pPts = &tags[2];
    pDts = &tags[3];
    pFlags = &tags[4];
    if (d->hasPts && first->pts < 0 && d->lastPts >= 0) {
        int n = d->showCount;
        long odd = n & 1;
        long step;
        int r;
        int ia;
        int q;
        long adj = odd & (d->ptsField & 1);
        { long sb = (step = d->ptsStep) & 1; adj &= sb; }
        ia = adj;
        q = (step * n) >> 1;
        asm volatile("" : : : "memory"); /* MATCH: as in _dispRefImage */
        r = q + ia;
        ((union { long l; int i; } *)&tags[2])->l = d->lastPts + r;
        odd &= d->ptsStep; tags = d->tags; if (odd) d->ptsField++; } else {
        *pPts = first->pts;
    }
    if (d->outputMode == 2 && d->nextPts >= 0) {
        *pPts = d->nextPts;
        d->outputMode = 0;
        d->nextPts = -1;
    }
    pPts2 = &tags[5];
    pDts2 = &tags[6];
    pFlags2 = &tags[7];
    *pDts = first->dts;
    {
        long a = ((long)first->flag40 << 5) | ((long)first->flag3C << 6);
        long b = ((long)first->flag34 << 8) | (long)first->flag2C;
        long c = ((long)first->flag38 << 7) | ((long)first->flag30 << 3);
        *pFlags = (b | a) | c;
    }
    d->lastPts = tags[2];
    d->showCount = 1;
    if (d->hasPts && second->pts < 0 && d->lastPts >= 0) {
        int n = d->showCount;
        long odd = n & 1;
        long step;
        int r;
        int ia;
        int q;
        long adj = odd & (d->ptsField & 1);
        { long sb = (step = d->ptsStep) & 1; adj &= sb; }
        ia = adj;
        q = (step * n) >> 1;
        r = q + ia;
        ((union { long l; int i; } *)&tags[5])->l = d->lastPts + r;
        odd &= d->ptsStep; tags = d->tags; if (odd) d->ptsField++; } else {
        *pPts2 = second->pts;
    }
    if (d->outputMode == 2 && d->nextPts >= 0) {
        *pPts2 = d->nextPts;
        d->outputMode = 0;
        d->nextPts = -1;
    }
    *pDts2 = second->dts;
    {
        long a = ((long)second->flag40 << 5) | ((long)second->flag3C << 6);
        long b = ((long)second->flag34 << 8) | (long)second->flag2C;
        long c = ((long)second->flag38 << 7) | ((long)second->flag30 << 3);
        *pFlags2 = (b | a) | c;
    }
    d->lastPts = tags[5];
    d->showCount = 1;
    tags[4] |= bit;
    tags[7] |= bit;
    d->vE0 = first->v5C;
    d->vE4 = first->v60;
    d->info[0] = first->info[0];
    d->info[1] = second->info[1];
    d->info[3] = first->info[3];
    d->info[4] = second->info[4];
    if (d->width) {
        ok = d->bufW >= top->w && d->width >= top->h;
    } else {
        ok = d->stride >= top->rows * top->width;
    }
    if (!ok) {
        sceSnprintf(buf, 256, "Too small buffer size for %dx%d picture\n", top->w, top->h);
        _sceMpegError(d, buf);
        asm volatile("" : "=r"(ok) : "0"(ok));
        if (!ok) return;
    }
    if (top->pictType == 1 && bottom->pictType == 1) {
        top->width <<= 1;
        if (d->useCsc) {
            _sceMpegCscStoreRefImage(d, top);
        } else {
            _cpr8(d, top);
        }
        top->width >>= 1;
        if (d->state != 2) {
            d->state = 2;
            d->outputBase = d->frameCount;
        }
        d->outputReady = 1;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/output", _dispRefImageField);
#endif
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
