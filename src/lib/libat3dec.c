/* libat3dec.c (ATRAC3 decoder core, middleware) */
#include "include_asm.h"

void *memcpy(void *dst, const void *src, unsigned int n);

/* joint stereo weights of the previous (0) and the current (1) frame */
typedef struct {
    unsigned short *w[4]; /* per band: weights of the two channels */
    unsigned short *g; /* gains */
    int n; /* bands below n use gain 0x8000 */
} At3Joint;

/* decoder work area (no type information: fields named by use) */
typedef struct {
    At3Joint joint[2]; /* 0x0000 */
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
extern signed char huf_maxlen[7][2];
extern signed char *huf_prefixtbl[7][2];
extern signed char *huf_lentbl[7][2];
extern int *huf_deqtbl[3][7];
extern const int scaleDiv[] __asm__("scaleDiv.0");

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

/* close: loop.c bases the inner loop on the channel 0 samples (the original on channel 1) and strength-reduces the
   band offset (the original recomputes (3 * band) * 136 from a counter) */
#if 0
static void channel_conv_joint(At3Work *w)
{
    int band;
    int k;
    unsigned short w0, w1, g0, g1;
    unsigned short n0, n1, h0, h1;
    unsigned short dw0, dw1, dg0, dg1;
    int cw0, cw1, cg0, cg1;
    int ndw0, ndw1, ndg0, ndg1;
    unsigned short *pw;
    unsigned short *nw;
    int (*sp)[3320];
    int idx;
    int *pr;
    int x;
    int y;
    int m;

    sp = (int (*)[3320])w->ch;
    for (band = 0; band < 4; band++) {
        g1 = 0x8000;
        g0 = 0x8000;
        h1 = 0x8000;
        h0 = 0x8000;
        if (band >= w->joint[1].n) {
            h1 = w->joint[1].g[1];
            h0 = w->joint[1].g[0];
        }
        if (band >= w->joint[0].n) {
            g1 = w->joint[0].g[1];
            g0 = w->joint[0].g[0];
        }
        pw = w->joint[0].w[band];
        nw = w->joint[1].w[band];
        w0 = pw[0];
        w1 = pw[1];
        dw0 = (nw[0] - w0) >> 3;
        dw1 = (nw[1] - w1) >> 3;
        dg0 = (short)(h0 - g0) >> 3;
        dg1 = (short)(h1 - g1) >> 3;
        cw0 = (dw0 << 3) + w0;
        cw1 = (dw1 << 3) + w1;
        cg0 = (dg0 << 3) + g0;
        cg1 = (dg1 << 3) + g1;
        idx = band * 408 + 35;
        ndw0 = -dw0;
        ndw1 = -dw1;
        ndg0 = -dg0;
        ndg1 = -dg1;
        pr = &((int (*)[136])(w->ch[1]))[band * 3][23];
        for (k = 0; k < 256; k++) {
            n0 = w0;
            h0 = g0;
            n1 = w1;
            h1 = g1;
            if (k - 8 < 0) {
                n0 = cw0;
                h0 = cg0;
                n1 = cw1;
                h1 = cg1;
            }
            cw0 += ndw0;
            cw1 += ndw1;
            cg0 += ndg0;
            cg1 += ndg1;
            x = pr[k - 3320];
            y = pr[k];
            m = n0 * (x >> 4) + n1 * (y >> 4);
            pr[k - 3320] = ((m >> 8) * h0) >> 6;
            pr[k] = (((x - m) >> 8) * h1) >> 6;
        }
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", channel_conv_joint);
#endif

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

static int unpack_specs(char *ch, unsigned int sel, int cb, int start, int end, int q, At3Work *w)
{
    int bits = huf_maxlen[cb - 1][sel];
    signed char *len = huf_lentbl[cb - 1][sel];
    signed char *prefix = huf_prefixtbl[cb - 1][sel];
    int shift;
    int *deq;
    int *p;
    int c;
    int c2;

    if (sel >= 2) {
        return -1;
    }
    if ((unsigned int)(cb - 1) >= 7) {
        return -1;
    }
    if (end == start) {
        return -1;
    }
    shift = scaleDiv[q];
    p = (int *)(ch + 0x1A40) + start;
    deq = huf_deqtbl[q - (21 - shift) * 3][cb - 1];
    end -= start;
    if (cb == 1) {
        do {
            c = prefix[get_unpack_from_msb(bits, w)];
            w->bitpos += len[c];
            *p++ += deq[c * 2] >> shift;
            if (end != 1) {
                *p++ += deq[c * 2 + 1] >> shift;
            }
            end -= 2;
        } while (end > 0);
    } else {
        do {
            c2 = prefix[get_unpack_from_msb(bits, w)];
            end--;
            w->bitpos += len[c2];
            *p++ += deq[c2] >> shift;
        } while (end > 0);
    }
    return 0;
}

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

