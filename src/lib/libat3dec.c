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
    int maxpos; /* 0x67F0: highest coefficient written */
    int nch; /* 0x67F4 */
    int mode; /* 0x67F8 */
    int frameSize; /* 0x67FC */
    unsigned char *stream; /* 0x6800 */
    unsigned char buf[0xC0]; /* 0x6804: byte-reversed second channel */
    int qoff; /* 0x68C4 */
    int bitpos; /* 0x68C8 */
} At3Work;

void iqmf(char *ch);
extern short a_wind[];
extern unsigned short chswTable[][2];
extern unsigned short chwtTable[][2];
void *memset(void *dst, int c, unsigned int n);
int imdct(char *ch, int band, int gain);
extern unsigned short subgaintable[];
extern const int a_qtstart[];
void __assert(const char *file, int line, const char *expr);

/* the unit's .rodata still comes from the original: the assert strings by their labels ("libat3dec.c",
   "(unsigned int)iqt < AT2_NQUS+1", "(unsigned int)iqt+1 < AT2_NQUS+1") */
extern const char D_004FD000[];
extern const char D_004FD010[];
extern const char D_004FD030[];
#define AT2_NQUS 32
#define QTSTART_(i, s) (((unsigned int)i < AT2_NQUS+1) ? (void)0 : __assert(D_004FD000, __LINE__, s), a_qtstart[i])
extern signed char huf_maxlen[7][2];
extern signed char *huf_prefixtbl[7][2];
extern signed char *huf_lentbl[7][2];
extern int *huf_deqtbl[3][7];
extern const int scaleDiv[] __asm__("scaleDiv.0");

typedef struct {
    int w[23];
} At3Blk23;

/* one of the 4 QMF bands of a channel (At3Chan.band) */
typedef struct {
    At3Blk23 hdr; /* 0x000 */
    int spec[233]; /* 0x05C */
    At3Blk23 save; /* 0x400 */
    int ov[128]; /* 0x45C */
    int x65C;
} At3Band;

/* the state of one channel (At3Work.ch) */
typedef struct {
    At3Band band[4]; /* 0x0000 */
    char _pad1980[0x1A40 - 0x1980];
    int spec[1024]; /* 0x1A40 */
    char _pad2A40[0x2A60 - 0x2A40];
    At3Blk23 qmfhead; /* 0x2A60 */
    char _pad2ABC[0x3260 - 0x2ABC];
    At3Blk23 qmfsave; /* 0x3260 */
    char _pad32BC[0x32D4 - 0x32BC];
    int gain[4][8][2]; /* 0x32D4 */
    char _pad33D4[0x33E0 - 0x33D4];
} At3Chan;

void h_d_make(float *a, float *b, float *c, float *d);
void h_iqmf_fb1(float *in, float *out, int odd);
void h_iqmf_fb2(float *p);

static void channel_conv_joint(At3Work *w);
static int gainc_window(char *ch, int band);
static void f2t_nodata(At3Band *bands, int band);

/* Parked: close: loop.c bases the inner loop on the channel 0 samples (the original on channel 1) and strength-reduces the
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

/* Not attempted: keeps t9 live across the calls into at3core.s with sq/lq caller-saves; parameters go
   through the stack */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", imdct);

/* Parked: 10 lines off (23 before the early `lt` compare, decomp-permuter): the original sets the return value -1 at
   the top of the loop (the conditional returns branch straight to the epilogue) */
#if 0
#define GAINC_STEP()                                                    \
    *p = ((*p >> 11) * subgaintable[sh & 7]) >> ((sh >> 3) + 4);        \
    p++;                                                                \
    sh += step

static int gainc_window(char *ch, int band)
{
    int *g = (int *)(ch + 0x32D4) + band * 16;
    int *end = ((At3Band *)ch)[band].ov;
    int *p = ((At3Band *)ch)[band].spec;
    unsigned int sh;
    unsigned int step;

    if (band >= 4) {
        return -1;
    }
    do {
        int lt = p < ((At3Band *)ch)[band].spec + g[0] + 1;
        if (end < ((At3Band *)ch)[band].spec + g[0] + 1) {
            return -1;
        }
        sh = g[1];
        if (!lt) {
            return -1;
        }
        if (sh >= 16) {
            return -1;
        }
        do {
            *p = *p >> sh;
            p++;
        } while (p < ((At3Band *)ch)[band].spec + g[0] + 1);
        if (!(p < end)) {
            do { return 0; } while (0); /* MATCH: the loop note (decomp-permuter): 11 -> 10 lines */
        }
        g += 2;
        step = g[1] - sh;
        sh <<= 3;
        sh += step;
        GAINC_STEP();
        GAINC_STEP();
        GAINC_STEP();
        GAINC_STEP();
        GAINC_STEP();
        GAINC_STEP();
        GAINC_STEP();
    } while (p < end);
    return 0;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", gainc_window);
#endif

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

static int getToneSpectrum(char *ch, int n, int nbands, At3Work *w)
{
    int mode;
    int len;
    int mask;
    int cb;
    int sel;
    int band;
    int pos;
    int cnt;
    int q;
    int start;
    int end;

    mode = read_unpack_from_msb(2, w);
    if (mode == 2) {
        return -1;
    }
    do {
        mask = get_unpack_from_msb(4, w);
        w->bitpos += nbands;
        len = read_unpack_from_msb(3, w) + 1;
        cb = read_unpack_from_msb(3, w);
        if (cb < 2) {
            return -1;
        }
        sel = mode;
        if (mode == 3) {
            sel = read_unpack_from_msb(1, w);
        }
        band = 0;
        do {
            if (mask & (8 >> band)) {
                for (pos = 0; pos < 256; pos += 64) {
                    for (cnt = read_unpack_from_msb(3, w) - 1; cnt >= 0; cnt--) {
                        q = read_unpack_from_msb(6, w);
                        if (w->qoff > 0) {
                            q = (q > w->qoff) ? q - w->qoff : 0;
                        }
                        start = read_unpack_from_msb(6, w) + (band << 8) + pos;
                        end = len + start;
                        if (end > 1024) {
                            end = 1024;
                        }
                        if (w->maxpos < end) {
                            w->maxpos = end;
                        }
                        if (unpack_specs(ch, sel, cb, start, end, q, w) == -1) {
                            return -1;
                        }
                    }
                }
            }
        } while (++band < nbands);
    } while (--n > 0);
    return 0;
}

static int getNoiseSpectrum(char *ch, int nqus, At3Work *w)
{
    int cb[32];
    int q[32];
    int sel;
    int iqt;
    int v;

    if ((unsigned int)(nqus - 1) >= 32) {
        return -1;
    }
    sel = read_unpack_from_msb(1, w);
    iqt = 0;
    do {
        cb[iqt] = read_unpack_from_msb(3, w);
    } while (++iqt < nqus);
    iqt = 0;
    do {
        if (cb[iqt]) {
            v = read_unpack_from_msb(6, w);
            q[iqt] = v;
            if (w->qoff > 0) {
                if (v > w->qoff) {
                    q[iqt] = v - w->qoff;
                } else {
                    q[iqt] = 0;
                }
            }
        }
    } while (++iqt < nqus);
    iqt = 0;
    do {
        if (cb[iqt]) {
#line 1057 "libat3dec.c"
            if (unpack_specs(ch, sel, cb[iqt], QTSTART_(iqt, D_004FD010), QTSTART_(iqt+1, D_004FD030), q[iqt], w) == -1) {
                return -1;
            }
        }
    } while (++iqt < nqus);
#line 1063 "libat3dec.c"
    if (w->maxpos < QTSTART_(iqt, D_004FD010)) {
        w->maxpos = QTSTART_(iqt, D_004FD010);
    }
    return 0;
}

/* Parked: 5 lines off: the band address of the loop is (x << 4) + x with swapped registers */
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

/* Parked: structure right; the gain loops keep &gain[0][0][1] in a register and reload 255 (ours hoists it) */
#if 0
static int unpack_mddata(At3Chan *ch, int rev, At3Work *w)
{
    int gain[4][8][2];
    int jw[4];
    int jg;
    int nbands;
    int band;
    int n;
    int i;

    jg = 0;
    if (rev) {
        jg = read_unpack_from_msb(4, w);
        for (i = 0; i < 4; i++) {
            jw[i] = read_unpack_from_msb(2, w);
        }
        w->bitpos += 2;
    } else {
        w->bitpos += 6;
    }
    nbands = read_unpack_from_msb(2, w) + 1;
    band = 0;
    do {
        n = read_unpack_from_msb(3, w);
        for (i = 0; i < n; i++) {
            gain[band][i][1] = read_unpack_from_msb(4, w);
            gain[band][i][0] = read_unpack_from_msb(5, w) << 3;
        }
        gain[band][n][1] = 4;
        gain[band][n][0] = 0xFF;
    } while (++band < nbands);
    for (; band < 4; band++) {
        gain[band][0][1] = 4;
        gain[band][0][0] = 0xFF;
    }
    memcpy(&ch->qmfhead, &ch->qmfsave, sizeof(At3Blk23));
    memset(ch->spec, 0, sizeof(ch->spec));
    w->maxpos = nbands << 8;
    n = read_unpack_from_msb(5, w);
    if (n && getToneSpectrum((char *)ch, n, nbands, w) == -1) {
        return -1;
    }
    if (getNoiseSpectrum((char *)ch, read_unpack_from_msb(5, w) + 1, w) == -1) {
        return -1;
    }
    n = nbands;
    if (nbands << 8 < w->maxpos) {
        n = (w->maxpos + 0xF7) / 256;
    }
    band = 0;
    do {
        if (imdct((char *)ch, band, gain[band][0][0])) {
            f2t_nodata(ch->band, band);
        }
        if (gainc_window((char *)ch, band) == -1) {
            return -1;
        }
    } while (++band < n);
    for (; band < 4; band++) {
        f2t_nodata(ch->band, band);
        if (gainc_window((char *)ch, band) == -1) {
            return -1;
        }
    }
    if (rev) {
        channel_conv_joint(w);
        w->joint[1] = w->joint[0];
        w->joint[0].n = nbands;
        w->joint[0].g = chwtTable[jg];
        for (i = 0; i < 4; i++) {
            w->joint[0].w[i] = chswTable[jw[i]];
        }
    }
    memcpy(ch->gain, gain, sizeof(gain));
    return 0;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", unpack_mddata);
#endif

/* Parked: 11 lines off (20 before the 0xF8 pad byte in a variable, decomp-permuter): the original enters the copy loop
   with the byte the skip loop loaded (reorg), registers differ */
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
    unsigned char pad;

    for (i = 0; i < w->nch; i++) {
        rev = 0;
        if (w->mode == 0x12) {
            rev = i == 1;
        }
        w->bitpos = off * 8;
        if (rev) {
            src = w->stream;
            d = w->buf;
            pad = 0xF8;
            p = src + w->frameSize + off - 1;
            while (*p == pad && _before(src, p)) {
                p--;
            }
            while (src < p) {
                *d++ = *p--;
            }
            w->stream = w->buf;
            w->bitpos = 0;
        }
        if (unpack_mddata((At3Chan *)w->ch[i], rev, w) == -1) {
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

