/* libat3dec.c (ATRAC3 decoder core, middleware) */
#include "include_asm.h"

void *memcpy(void *dst, const void *src, unsigned int n);

/* decoder work area (no type information: fields named by use) */
typedef struct {
    char _pad0[0x30];
    char ch[2][0x33E0]; /* 0x0030: per-channel state */
    int x67F0;
    int nch; /* 0x67F4 */
    int mode; /* 0x67F8 */
    int frameSize; /* 0x67FC */
    unsigned char *stream; /* 0x6800 */
    unsigned char buf[0xC4]; /* 0x6804: byte-reversed second channel */
    int bitpos; /* 0x68C8 */
} At3Work;

int unpack_mddata(char *ch, int rev, At3Work *w);
void iqmf(char *ch);
extern short a_wind[];

typedef struct {
    int w[23];
} At3Blk23;

/* one of the 4 QMF bands of a channel */
typedef struct {
    At3Blk23 hdr; /* 0x000 */
    int spec[233]; /* 0x05C */
    At3Blk23 save; /* 0x400 */
    int ov[128]; /* 0x45C */
    int x65C;
} At3Band;
void h_d_make(float *a, float *b, float *c, float *d);
void h_iqmf_fb1(float *in, float *out, int odd);
void h_iqmf_fb2(float *p);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", channel_conv_joint);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", imdct);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", gainc_window);

void iqmf(char *ch)
{
    float tmp[4][140];
    float *s;
    float *t;
    float *q;
    int i;

    q = (float *)(ch + 0x2ABC);
    s = (float *)ch;
    t = tmp[0];
    for (i = 0; i < 0x118; i += 8) {
        h_d_make(s, s + 408, t, t + 280);
        h_d_make(s + 1224, s + 816, t + 140, t + 420);
        s += 8;
        t += 4;
    }
    t = tmp[0];
    s = tmp[2];
    do {
        h_iqmf_fb1(q++, t, 0);
        t += 4;
        h_iqmf_fb1(q, s, 1);
        s += 4;
        q += 15;
    } while (q < (float *)(ch + 0x32BC));
    q = (float *)(ch + 0x2A60);
    do {
        h_iqmf_fb2(q);
        q += 4;
    } while (q < (float *)(ch + 0x3260));
}

static unsigned int get_unpack_from_msb(int n, At3Work *w)
{
    int pos = w->bitpos;
    unsigned char *p = w->stream + (pos >> 3);
    int mask = (1 << n) - 1;

    n += pos & 7;
    return (((p[0] << 8) | p[1]) >> (16 - n)) & mask;
}

static unsigned int read_unpack_from_msb(int n, At3Work *w)
{
    unsigned int v = get_unpack_from_msb(n, w);

    w->bitpos += n;
    return v;
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", unpack_specs);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", getToneSpectrum);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", getNoiseSpectrum);

/* 5 lines off: the band address of the loop is (x << 4) + x with swapped registers */
#if 0
static void f2t_nodata(At3Band *bands, int band)
{
    int i;
    int v;
    int *p;
    int *q;

    memcpy(&bands[band].hdr, &bands[band].save, sizeof(At3Blk23));
    q = &bands[band].spec[127];
    p = bands[band].ov;
    for (i = 0; i < 128; i++) {
        v = *p;
        *q-- = v >> 4;
        *p = 0;
        p[-128] = ((v >> 14) * a_wind[i]) >> 5;
        p++;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", f2t_nodata);
#endif

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", unpack_mddata);

/* 20 lines off: the original enters the copy loop with the byte the skip loop loaded (reorg), registers differ */
#if 0
static inline int _before(unsigned char *a, unsigned char *b)
{
    return a < b;
}

int at3dec_proc(At3Work *w)
{
    int off = 0;
    int i;
    int rev;
    unsigned char *src;
    unsigned char *p;
    unsigned char *d;

    for (i = 0; i < w->nch; i++) {
        rev = 0;
        if (w->mode == 0x12) {
            rev = i == 1;
        }
        w->bitpos = off * 8;
        if (rev) {
            src = w->stream;
            d = w->buf;
            p = src + w->frameSize + off - 1;
            while (*p == 0xF8 && _before(src, p)) {
                p--;
            }
            while (src < p) {
                *d++ = *p--;
            }
            w->stream = w->buf;
            w->bitpos = 0;
        }
        if (unpack_mddata(w->ch[i], rev, w) == -1) {
            return -1;
        }
        off += w->frameSize;
        if (w->mode != 0x12 && off * 8 < w->bitpos) {
            return -1;
        }
    }
    for (i = 0; i < w->nch; i++) {
        iqmf(w->ch[i]);
    }
    return 0;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", at3dec_proc);
#endif

