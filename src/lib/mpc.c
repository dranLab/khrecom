/* libmpeg mpc.c (Sony EE SDK): macroblock decoding and motion compensation through the IPU; the
   reference image copies (_rix_*, _ri0_*, _copy*RefImage) are hand-written MMI assembly */
#include "include_asm.h"
#include "mpegdec.h"

void _sceMpegWaitIpuIdle(_sceMpegDecoder *d);
void _sceMpegFlushBuf(_sceMpegDecoder *d, int n);

#define IPU_CMD 0x10002000
#define IPU_BP 0x10002020
#define IPU_TOP 0x10002030
#define D3_MADR 0x1000B010
#define D9_CHCR 0x1000D400
#define D9_TADR 0x1000D430
#define D9_SADR 0x1000D480

int DIntr(void);
int EIntr(void);
void _sceMpegError(_sceMpegDecoder *d, const char *msg);
void _sceMpegError1(_sceMpegDecoder *d, const char *fmt, int arg);
int _sceMpegDispatchMpegCbNodata(sceMpeg *mp);
void _sceMpegStopIpuDma(void);
void _sceMpegErrorBdec(_sceMpegDecoder *d);
static int _slice0(_sceMpegDecoder *d, int n);
int _sceMpegSliceA0(_sceMpegDecoder *d, int n, int *mbaddr, int *mbinc, int (*pmv)[2][2]);
static int _decMB0(_sceMpegDecoder *d, int *mbtype, int *motion_type, int *dct_type, int (*pmv)[2][2], int (*mvfs)[2],
                   int *dmv);
static int _skipMB0(_sceMpegDecoder *d, int (*pmv)[2][2], int *motion_type, int (*mvfs)[2], int *mbtype);
unsigned long _sceMpegWaitIpuIdle64(_sceMpegDecoder *d);
unsigned int _sceMpegNextBit(_sceMpegDecoder *d, int n);
unsigned int _sceMpegPeepBit(_sceMpegDecoder *d, int n);
static void _waitIpuIdle(_sceMpegDecoder *d);
static void _getAllRefs(_sceMpegDecoder *d, int x, int y, int mbtype, int motion_type, int (*pmv)[2][2], int (*mvfs)[2],
                        int *dmv);
static void _getRef0(_sceMpegDecoder *d, _sceMpegPicture *pic, int sel, int dblk, int yblk, int h, int x, int y,
                     int mvx, int mvy, int fld, int avg);
static void dmaRefImage(_sceMpegDecoder *d);
void _copyRefImage(unsigned int dst, unsigned int src);
void _copyAddRefImage(unsigned int dst, unsigned int src, unsigned int buf);
static void _flushBuf(_sceMpegDecoder *d, int n);
static void receiveDataFromIPU(unsigned int addr, int size);

/* issues an IPU command; the bit buffer is kept valid only across BDEC, VDEC and FDEC */
#define _setIpuCmd(d, cmd)                                                              \
    ({                                                                                  \
        unsigned int __type;                                                            \
        unsigned int __cmd = (cmd); /* MATCH: value before the address (loop.c keeps IPU_CMD per pass) */ \
                                                                                        \
        (void)(REG(IPU_CMD) = __cmd);                                                   \
        __type = __cmd & 0xF0000000;                                                    \
        (d)->x82C = __type;                                                             \
        if (__type == 0x20000000 || __type == 0x30000000 || __type == 0x40000000) {     \
            (d)->x828 = 0;                                                              \
        } else {                                                                        \
            (d)->x828 = 1;                                                              \
        }                                                                               \
    })

/* VDEC: decodes one VLC (the table in cmd) and refills the bit buffer */
#define _getVlc(d, cmd)                                                                 \
    ({                                                                                  \
        unsigned long __v;                                                              \
        unsigned int __bp;                                                              \
        long __top;                                                                     \
                                                                                        \
        _waitIpuIdle(d);                                                                \
        _setIpuCmd(d, cmd);                                                             \
        __v = _sceMpegWaitIpuIdle64(d);                                                 \
        __bp = REG(IPU_BP);                                                             \
        __top = *(volatile long *)IPU_TOP;                                              \
        (d)->x848 = (int)__top;                                                         \
        if (__top < 0) {                                                                \
            (d)->x84C = (32 - (__bp & 0x1F)) & 0x1F;                                    \
        } else {                                                                        \
            (d)->x84C = 32;                                                             \
        }                                                                               \
        (d)->x12C = (int)__v == 0;                                                      \
        (short)__v;                                                                     \
    })

void _rix_000(void *p);
void _rix_001(void *p);
void _rix_010(void *p);
void _rix_011(void *p);
void _rix_100(void *p);
void _rix_101(void *p);
void _rix_110(void *p);
void _rix_111(void *p);
void _ri0_000(void *p);
void _ri0_001(void *p);
void _ri0_010(void *p);
void _ri0_011(void *p);
void _ri0_100(void *p);
void _ri0_101(void *p);
void _ri0_110(void *p);
void _ri0_111(void *p);

static void (*_rix[8])(void *) = { _rix_000, _rix_001, _rix_010, _rix_011, _rix_100, _rix_101, _rix_110, _rix_111 };
static void (*_ri0[8])(void *) = { _ri0_000, _ri0_001, _ri0_010, _ri0_011, _ri0_100, _ri0_101, _ri0_110, _ri0_111 };

/* dmaRefImage's static (while it is asm) */
static unsigned long _sprtagdata[64] __asm__("_sprtagdata.51") __attribute__((aligned(64)));

static void _dualPrimeVector(_sceMpegDecoder *d, int *out, int *dmv, int mvx, int mvy)
{
    if (d->x184 == 3) {
        if (d->x188) {
            out[0] = ((mvx > 0) ? (mvx + 1) >> 1 : mvx >> 1) + dmv[0];
            out[1] = ((mvy > 0) ? (mvy + 1) >> 1 : mvy >> 1) + dmv[1] - 1;
            out[2] = ((mvx * 3 + (mvx > 0)) >> 1) + dmv[0];
            out[3] = ((mvy * 3 + (mvy > 0)) >> 1) + dmv[1] + 1;
        } else {
            out[0] = ((mvx * 3 + (mvx > 0)) >> 1) + dmv[0];
            out[1] = ((mvy * 3 + (mvy > 0)) >> 1) + dmv[1] - 1;
            out[2] = ((mvx > 0) ? (mvx + 1) >> 1 : mvx >> 1) + dmv[0];
            out[3] = ((mvy > 0) ? (mvy + 1) >> 1 : mvy >> 1) + dmv[1] + 1;
        }
    } else {
        out[0] = ((mvx > 0) ? (mvx + 1) >> 1 : mvx >> 1) + dmv[0];
        out[1] = ((mvy > 0) ? (mvy + 1) >> 1 : mvy >> 1) + dmv[1];
        if (d->x184 == 1) {
            out[1]--;
        } else {
            out[1]++;
        }
    }
}

static void _getAllRefs(_sceMpegDecoder *d, int x, int y, int mbtype, int motion_type, int (*pmv)[2][2], int (*mvfs)[2],
                        int *dmv)
{
    int dmvec[4];
    _sceMpegPicture *refs[2][2];
    int avg;
    int bf;
    int sel;
    int two;

    sel = 1;
    d->spr[d->x820].count = 0;
    avg = 0;
    if ((mbtype & 8) || d->x160 == 2) {
        if (d->x184 == 3) {
            if (motion_type == 2 || !(mbtype & 8)) {
                _getRef0(d, d->frame[0], 0, 0, 0, 16, x, y, pmv[0][0][0], pmv[0][0][1], 0, 0);
            } else if (motion_type == 1) {
                _getRef0(d, d->frame[0], mvfs[0][0], 0, 0, 8, x, y, pmv[0][0][0], pmv[0][0][1] >> 1, 1, 0);
                _getRef0(d, d->frame[0], mvfs[1][0], 1, 0, 8, x, y, pmv[1][0][0], pmv[1][0][1] >> 1, 1, 0);
            } else if (motion_type == 3) {
                _dualPrimeVector(d, dmvec, dmv, pmv[0][0][0], pmv[0][0][1] >> 1);
                _getRef0(d, d->frame[0], 0, 0, 0, 8, x, y, pmv[0][0][0], pmv[0][0][1] >> 1, 1, 0);
                _getRef0(d, d->frame[0], 1, 0, 0, 8, x, y, dmvec[0], dmvec[1], 1, 1);
                _getRef0(d, d->frame[0], 1, 1, 0, 8, x, y, pmv[0][0][0], pmv[0][0][1] >> 1, 1, 0);
                _getRef0(d, d->frame[0], 0, 1, 0, 8, x, y, dmvec[2], dmvec[3], 1, 1);
            } else {
                _sceMpegError1(d, "(a) invalid motion_type(%d)-0", motion_type);
            }
        } else {
            two = 2; /* MATCH: constant in a variable: its li is scheduled before the stores */
            refs[0][0] = d->top[0];
            refs[0][1] = d->bottom[0];
            refs[1][0] = d->top[1];
            refs[1][1] = d->bottom[1];
            bf = d->x184 == two;
            sel = 0;
            if (d->x160 == 2 && d->x130) {
                sel = bf != mvfs[0][0];
            }
            if (motion_type == 1 || !(mbtype & 8)) {
                _getRef0(d, refs[sel][mvfs[0][0]], 0, 0, 0, 16, x, y, pmv[0][0][0], pmv[0][0][1], 0, 0);
            } else if (motion_type == 2) {
                _getRef0(d, refs[sel][mvfs[0][0]], 0, 0, 0, 8, x, y, pmv[0][0][0], pmv[0][0][1], 0, 0);
                sel = 0;
                if (d->x160 == 2 && d->x130) {
                    if (bf ^ mvfs[1][0]) {
                        sel = 1;
                    } else {
                        sel = 0;
                    }
                }
                _getRef0(d, refs[sel][mvfs[1][0]], 0, 0, 8, 8, x, y, pmv[1][0][0], pmv[1][0][1], 0, 0);
            } else if (motion_type == 3) {
                if (d->x130) {
                    sel = 1;
                } else {
                    sel = 0;
                }
                _dualPrimeVector(d, dmvec, dmv, pmv[0][0][0], pmv[0][0][1]);
                _getRef0(d, refs[0][bf], 0, 0, 0, 16, x, y, pmv[0][0][0], pmv[0][0][1], 0, 0);
                _getRef0(d, refs[sel][!bf], 0, 0, 0, 16, x, y, dmvec[0], dmvec[1], 0, 1);
            } else {
                _sceMpegError1(d, "(b) invalid motion_type(%d)-1", motion_type);
            }
        }
        avg = 1;
    }
    if (mbtype & 4) {
        if (d->x184 == 3) {
            sel = 1;
            if (motion_type == 2) {
                _getRef0(d, d->frame[1], 0, 0, 0, 16, x, y, pmv[0][1][0], pmv[0][1][1], 0, avg);
            } else {
                _getRef0(d, d->frame[1], mvfs[0][1], 0, 0, 8, x, y, pmv[0][1][0], pmv[0][1][1] >> 1, 1, avg);
                _getRef0(d, d->frame[1], mvfs[1][1], 1, 0, 8, x, y, pmv[1][1][0], pmv[1][1][1] >> 1, 1, avg);
            }
        } else if (motion_type == 1) {
            _getRef0(d, mvfs[0][1] ? d->bottom[1] : d->top[1], 0, 0, 0, 16, x, y, pmv[0][1][0], pmv[0][1][1], 0, avg);
        } else if (motion_type == 2) {
            _getRef0(d, mvfs[0][1] ? d->bottom[1] : d->top[1], 0, 0, 0, 8, x, y, pmv[0][1][0], pmv[0][1][1], 0, avg);
            _getRef0(d, mvfs[1][1] ? d->bottom[1] : d->top[1], 0, 0, 8, 8, x, y, pmv[1][1][0], pmv[1][1][1], 0, avg);
        } else {
            _sceMpegError1(d, "(c) invalid motion_type(%d)-2", motion_type);
        }
    }
}


static int _motionComp0(_sceMpegDecoder *d, int mbaddr, int dct_type, int mbtype, int motion_type, int (*pmv)[2][2],
                        int (*mvfs)[2], int *dmv)
{
    int intra = mbtype & 1;
    int mby = mbaddr / d->x13C;
    int mbx = mbaddr % d->x13C;
    int x = mbx << 4;
    int y = mby << 4;
    _sceMpegPicture *p;

    if (intra) {
        while ((REG(D9_CHCR) >> 8) & 1)
            ;
        d->spr[d->x820].mc = 0;
    } else {
        if ((unsigned int)(motion_type - 1) >= 3) {
            _sceMpegError1(d, "Invalid modion type -- ignored(%d)", motion_type);
            d->x12C = 1;
            return 0;
        }
        _getAllRefs(d, x, y, mbtype, motion_type, pmv, mvfs, dmv);
        while ((REG(D9_CHCR) >> 8) & 1)
            ;
        dmaRefImage(d);
        d->spr[d->x820].mc = 1;
    }
    if (dct_type == 1 && (mbtype & 2)) {
        d->spr[d->x820].x134 = dct_type;
    } else {
        d->spr[d->x820].x134 = 0;
    }
    d->spr[d->x820].intra = intra;
    if (d->x184 == 3) {
        d->spr[d->x820].dst = d->frame[2]->buf + (mbx * d->frame[2]->mbh + mby) * 0x180;
    } else {
        p = (d->x184 == 2) ? d->bottom[2] : d->top[2];
        d->spr[d->x820].dst = p->buf + (mbx * p->mbh + mby) * 0x180;
    }
    return 1;
}

/* sets up the copies of one reference block: luminance (_rix_*) and chrominance (_ri0_*) */
static void _getRef0(_sceMpegDecoder *d, _sceMpegPicture *pic, int sel, int dblk, int yblk, int h, int x, int y,
                     int mvx, int mvy, int fld, int avg)
{
    int n;
    unsigned int dst;
    _sceMpegRiArg *ra;
    _sceMpegRiArg *ra0;
    int lx;
    int ly;
    int mbx;
    int mby;
    int cbx;
    int cby;
    int cyb;
    int cmy; /* MATCH: declared before cmx: equal global-alloc priority, the lower pseudo gets the register first */
    int ch;
    int cmx;
    int hx;
    int hy;
    int c;
    int li;
    int off;
    unsigned char *src0;
    unsigned char *src1;
    unsigned char *base;
    unsigned char *p;

    n = d->spr[d->x820].count;
    dst = d->x830;
    ra = &d->spr[d->x820].rixArg[n];
    ra0 = &d->spr[d->x820].ri0Arg[n];

    lx = (mvx >> 1) + x;
    if (fld) {
        ly = (mvy >> 1) * 2 + (y + (yblk + sel));
    } else {
        ly = (mvy >> 1) + (y + (yblk + sel));
    }
    mbx = lx >> 4;
    mby = ly >> 4;
    src0 = (unsigned char *)pic->buf + (mbx * pic->mbh + mby) * 0x180;
    src1 = (unsigned char *)pic->buf + (mbx * pic->mbh + mby + pic->mbh) * 0x180;
    ly -= mby << 4;
    hx = mvx & 1;
    hy = mvy & 1;
    d->spr[d->x820].addr0[n] = src0;
    d->spr[d->x820].addr1[n] = src1;
    ra->shift = lx - (mbx << 4);
    ra->dst = dst + (dblk + yblk) * 32;
    if (hy) {
        if (ly + (h << fld) < 16) {
            ra->count = h;
            ra->count2 = 0;
        } else {
            c = (16 >> fld) - (ly >> fld) - 1;
            ra->count = c;
            ra->count2 = h - c;
        }
    } else {
        if (ly + (h << fld) > 16) {
            c = (16 >> fld) - (ly >> fld);
            ra->count = c;
            ra->count2 = h - c;
        } else {
            ra->count = h;
            ra->count2 = 0;
        }
    }
    if (d->buffType == 0) {
        base = (unsigned char *)d->spr[d->x820].sadr + n * 0x600;
        ra->src0 = base + ly * 16;
        ra->src1 = base + (ly * 16 + 0x300);
    } else {
        base = src0;
        ra->src0 = src0 + ly * 16;
        ra->src1 = src1 + ly * 16;
    }
    ra->stride = 16 << fld;
    li = (avg << 2) | (hx << 1) | hy;
    cmx = mvx / 2;
    cmy = mvy / 2;
    ch = h >> 1;
    cyb = yblk >> 1;
    lx = (cmx >> 1) + (x >> 1);
    if (fld) {
        ly = (cmy >> 1) * 2 + ((y >> 1) + (cyb + sel));
    } else {
        ly = (cmy >> 1) + ((y >> 1) + (cyb + sel));
    }
    cbx = lx >> 3;
    cby = ly >> 3;
    ly -= cby << 3;
    hx = cmx & 1;
    hy = cmy & 1;
    ra0->shift = lx - (cbx << 3);
    ra0->dst = dst + 0x200 + (dblk + cyb) * 16;
    if (hy) {
        if (ly + (ch << fld) < 8) {
            ra0->count = ch;
            ra0->count2 = 0;
        } else {
            c = (8 >> fld) - (ly >> fld) - 1;
            ra0->count = c;
            ra0->count2 = ch - c;
        }
    } else {
        if (ly + (ch << fld) > 8) {
            c = (8 >> fld) - (ly >> fld);
            ra0->count = c;
            ra0->count2 = ch - c;
        } else {
            ra0->count = ch;
            ra0->count2 = 0;
        }
    }
    off = ((cbx - mbx) * 2 + (cby - mby)) * 0x180;
    p = base + off;
    if (d->buffType == 0) {
        ra0->src0 = p + (ly * 8 + 0x100);
        ra0->src1 = p + (ly * 8 + 0x400);
    } else {
        /* MATCH: the row offset as a statement expression: fold would merge its 0x100 into the compare */
        if ((unsigned int)(off + ({ ly * 8 + 0x100; })) <= 0x300) {
            ra0->src0 = src0 + off + (ly * 8 + 0x100);
        } else {
            ra0->src0 = src1 + off + (ly * 8 + 0x100) - 0x300;
        }
        ra0->src1 = src1 + off + (ly * 8 + 0x100);
    }
    ra0->stride = 8 << fld;
    d->spr[d->x820].rix[n] = _rix[li];
    d->spr[d->x820].ri0[n] = _ri0[(avg << 2) | (hx << 1) | hy];
    d->spr[d->x820].count++;
}


static void _doMC(_sceMpegDecoder *d, int idx)
{
    int i;

    if (d->spr[idx].mc) {
        for (i = 0; i < d->spr[idx].count; i++) {
            d->spr[idx].rix[i](&d->spr[idx].rixArg[i]);
            d->spr[idx].ri0[i](&d->spr[idx].ri0Arg[i]);
        }
    }
    if (d->spr[idx].intra && d->spr[idx].skip) {
        _sceMpegError(d, "intra && skip MB");
    }
    if (d->spr[idx].intra) {
        _copyRefImage(d->spr[idx].dst, d->spr[idx].buf);
    } else if (d->spr[idx].skip) {
        _copyRefImage(d->spr[idx].dst, d->x830);
    } else {
        _copyAddRefImage(d->spr[idx].dst, d->x830, d->spr[idx].buf);
    }
}

/* waits until the IPU output DMA has drained; FALSE on abort or a decoding error */
static inline int _waitIpuDma(_sceMpegDecoder *d)
{
    unsigned int bp;
    long top;
    int ret = 1;

    while (REG(D3_QWC) != 0 && !(REG(IPU_CTRL) & 0x4000)) {
        if (REG(D4_QWC) == 0 && !(REG(D4_CHCR) & 0x100)) {
            _sceMpegDispatchMpegCbNodata(d->mp);
        }
        if (d->abort) {
            _sceMpegStopIpuDma();
            return 0;
        }
    }
    bp = REG(IPU_BP);
    top = *(volatile long *)IPU_TOP;
    d->x848 = (int)top;
    if (top < 0) {
        d->x84C = (bp & 0x1F) ? 32 - (bp & 0x1F) : 0;
    } else {
        d->x84C = 32;
    }
    if (REG(IPU_CTRL) & 0x4000) {
        _sceMpegErrorBdec(d);
        ret = 0;
    }
    return ret;
}

int _sceMpegPictureData0(_sceMpegDecoder *d)
{
    int n;
    int r;

    d->x820 = 0;
    d->x824 = 0;
    n = d->x13C * d->x140;
    if (d->x184 != 3) {
        n >>= 1;
    }
    while ((r = _slice0(d, n)) == 1 || r == 3);
    _waitIpuIdle(d);
    _waitIpuIdle(d);
    if (!_waitIpuDma(d)) {
        if (d->abort) {
            return 4;
        }
        r = 2;
    }
    while ((REG(D9_CHCR) >> 8) & 1);
    if (r == 0) {
        _doMC(d, d->x820 == 0);
    }
    if (r == 1 || r == 2) {
        _sceMpegError(d, "= Skip to the next picture =");
    }
    return r == 0;
}

static int _slice0(_sceMpegDecoder *d, int n)
{
    int pmv[2][2][2];
    int mvfs[2][2];
    int dmv[4];
    int mbaddr;
    int incr;
    int mbtype;
    int motion_type;
    int dct_type;
    int r;

    mbaddr = 0;
    incr = 0;
    if ((r = _sceMpegSliceA0(d, n, &mbaddr, &incr, pmv)) != 0) {
        return r;
    }
    d->x12C = 0;
    for (;;) {
        if (mbaddr >= n) {
            return 0;
        }
        d->spr[d->x820].skip = 0;
        _waitIpuIdle(d);
        if (!_waitIpuDma(d)) {
            if (d->abort) {
                return 4;
            }
            return 2;
        }
        if (incr == 0) {
            if (_sceMpegPeepBit(d, 23) == 0 || d->x12C) {
                d->x12C = 0;
                return 3;
            }
            incr = _sceMpegMbAddressIncrement(d);
            if (d->x12C) {
                d->x12C = 0;
                return 1;
            }
        }
        if (mbaddr >= n) {
            _sceMpegError(d, "Too many macroblocks in picture");
            return 2;
        }
        if (incr == 1) {
            if (!_decMB0(d, &mbtype, &motion_type, &dct_type, pmv, mvfs, dmv)) {
                d->x12C = 0;
                return 1;
            }
        } else {
            if (!_skipMB0(d, pmv, &motion_type, mvfs, &mbtype)) {
                d->x12C = 0;
                return 2;
            }
        }
        if (!_motionComp0(d, mbaddr, incr, mbtype, motion_type, pmv, mvfs, dmv)) {
            d->x12C = 0;
            return 2;
        }
        if (mbaddr) {
            _doMC(d, d->x820 ^ 1);
        }
        mbaddr++;
        d->x820 ^= 1;
        incr--;
    }
}

/* _skipMB0 is emitted at the end of the file, its string here */
__asm__(".rdata\n"
        ".align 3\n_skipMB0_msg:\n.asciz \"skiped macroblock in I picure is not allowed\"\n"
        ".text\n");
extern const char _skipMB0_msg[];

/* decodes one motion vector component into *pred */
static inline void _mvComponent(int *pred, int r_size, int code, int residual, int full_pel)
{
    int vec = *pred;
    int f = 16 << r_size;

    if (full_pel) {
        vec >>= 1;
    }
    if (code > 0) {
        vec += ((code - 1) << r_size) + residual + 1;
        if (vec >= f) {
            vec -= f << 1;
        }
    } else if (code < 0) {
        vec -= ((-code - 1) << r_size) + residual + 1;
        if (vec < -f) {
            vec += f << 1;
        }
    }
    *pred = full_pel ? vec << 1 : vec;
}

static void _motionVector(_sceMpegDecoder *d, int *pmv, int *dmvector, int h_r_size, int v_r_size, int dmv,
                          int mvscale, int full_pel)
{
    int code;
    int residual;

    code = _getVlc(d, 0x38000000);
    residual = (h_r_size && code) ? _sceMpegNextBit(d, h_r_size) : 0;
    _mvComponent(&pmv[0], h_r_size, code, residual, full_pel);
    if (dmv) {
        dmvector[0] = _getVlc(d, 0x3C000000);
    }
    code = _getVlc(d, 0x38000000);
    residual = (v_r_size && code) ? _sceMpegNextBit(d, v_r_size) : 0;
    if (mvscale) {
        pmv[1] >>= 1;
    }
    _mvComponent(&pmv[1], v_r_size, code, residual, full_pel);
    if (mvscale) {
        pmv[1] <<= 1;
    }
    if (dmv) {
        dmvector[1] = _getVlc(d, 0x3C000000);
    }
}

static void _motionVectors(_sceMpegDecoder *d, int (*pmv)[2][2], int *dmvector, int (*mvfs)[2], int s, int mvcount,
                           int mvformat, int h_r_size, int v_r_size, int dmv, int mvscale)
{
    if (mvcount == 1) {
        if (mvformat == 0 && dmv == 0) {
            mvfs[1][s] = mvfs[0][s] = _sceMpegNextBit(d, 1);
        }
        _motionVector(d, pmv[0][s], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);
        pmv[1][s][0] = pmv[0][s][0];
        pmv[1][s][1] = pmv[0][s][1];
    } else {
        mvfs[0][s] = _sceMpegNextBit(d, 1);
        _motionVector(d, pmv[0][s], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);
        mvfs[1][s] = _sceMpegNextBit(d, 1);
        _motionVector(d, pmv[1][s], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);
    }
}

static int _decMB0(_sceMpegDecoder *d, int *mbtype, int *motion_type, int *dct_type, int (*pmv)[2][2], int (*mvfs)[2],
                   int *dmv)
{
    int mvcount;
    int mvformat;
    int dmvflag;
    int mvscale;
    unsigned int cmd;
    unsigned int dtdcr;

    REG(IPU_CTRL) = (REG(IPU_CTRL) & ~0x07000000) | (d->x160 << 24);
    *mbtype = _getVlc(d, 0x34000000);
    if (*mbtype == 0) {
        _sceMpegError(d, "Invalid macroblock_type code: 0");
        d->x12C = 1;
        return 0;
    }
    if (*mbtype & 0xC) {
        if (d->x184 == 3 && d->x18C) {
            *motion_type = 2;
        } else {
            *motion_type = _sceMpegNextBit(d, 2);
        }
    } else if (*mbtype & 1) {
        if (d->x190) {
            *motion_type = (d->x184 == 3) ? 2 : 1;
        }
    }
    if (d->x184 == 3) {
        mvcount = (*motion_type == 1) ? 2 : 1;
        mvformat = *motion_type == 2;
    } else {
        mvcount = (*motion_type == 2) ? 2 : 1;
        mvformat = 0;
    }
    dmvflag = *motion_type == 3;
    mvscale = 0;
    if (!mvformat) {
        mvscale = d->x184 == 3;
    }
    *dct_type = (d->x184 == 3 && !d->x18C && (*mbtype & 3)) ? _sceMpegNextBit(d, 1) : 0;
    if (*mbtype & 0x10) {
        d->x1C4 = _sceMpegNextBit(d, 5);
    }
    if ((*mbtype & 8) || ((*mbtype & 1) && d->x190)) {
        if (d->x858) {
            _motionVectors(d, pmv, dmv, mvfs, 0, mvcount, mvformat, d->x174 - 1, d->x178 - 1, dmvflag, mvscale);
        } else {
            _motionVector(d, pmv[0][0], dmv, d->x168 - 1, d->x168 - 1, 0, 0, d->x164);
        }
    }
    if (d->x12C) {
        return 0;
    }
    if (*mbtype & 4) {
        if (d->x858) {
            _motionVectors(d, pmv, dmv, mvfs, 1, mvcount, mvformat, d->x17C - 1, d->x180 - 1, 0, mvscale);
        } else {
            _motionVector(d, pmv[0][1], dmv, d->x170 - 1, d->x170 - 1, 0, 0, d->x16C);
        }
    }
    if (d->x12C) {
        return 0;
    }
    if ((*mbtype & 1) && d->x190) {
        _flushBuf(d, 1);
    }
    if (*mbtype & 3) {
        receiveDataFromIPU(d->spr[d->x820].buf, 0x300);
        _waitIpuIdle(d);
        dtdcr = (*dct_type << 25) | (d->x1C0 << 26);
        cmd = ((*mbtype & 1) << 27) | dtdcr | 0x20000000 | (d->x1C4 << 16);
        _setIpuCmd(d, cmd);
    } else {
        d->spr[d->x820].skip = 1;
    }
    d->x1C0 = 0;
    if (d->x12C) {
        return 0;
    }
    if (!(*mbtype & 1)) {
        d->x1C0 = 1;
    }
    if ((*mbtype & 1) && !d->x190) {
        pmv[1][0][1] = 0;
        pmv[1][0][0] = 0;
        pmv[0][0][1] = 0;
        pmv[0][0][0] = 0;
        pmv[1][1][1] = 0;
        pmv[1][1][0] = 0;
        pmv[0][1][1] = 0;
        pmv[0][1][0] = 0;
    }
    if (d->x160 == 2 && !(*mbtype & 9)) {
        pmv[1][0][1] = 0;
        pmv[1][0][0] = 0;
        pmv[0][0][1] = 0;
        pmv[0][0][0] = 0;
        if (d->x184 == 3) {
            *motion_type = 2;
        } else {
            *motion_type = 1;
            mvfs[0][0] = d->x184 == 2;
        }
    }
    return 1;
}


int _sceMpegMbAddressIncrement(_sceMpegDecoder *d)
{
    int inc = 0;
    unsigned int code;
    unsigned int peep;
    int cont;

    do {
        code = _getVlc(d, 0x30000000);
        switch (code) {
        case 0x22:
            cont = 1;
            break;
        case 0x23:
            cont = 1;
            inc += 0x21;
            break;
        case 0:
            peep = _sceMpegPeepBit(d, 11);
            if (d->x858 && peep == 0xF) {
                _flushBuf(d, 11);
                cont = 1;
                break;
            }
            _sceMpegError1(d, "Invalid macroblock_address_increment code(0x%08x)", code);
            d->x12C = 1;
            return 1;
        default:
            inc += code;
            cont = 0;
            break;
        }
    } while (cont);
    return inc;
}

void _sceMpegWaitIpuIdle(_sceMpegDecoder *d)
{
    unsigned int bp;
    unsigned int type;
    unsigned int n;
    int cnt = 0;

    while ((REG(IPU_CTRL) & 0x80004000) == 0x80000000) {
            bp = REG(IPU_BP);
            type = d->x82C;
            n = ((bp & 0xFF00) >> 1) + ((bp & 0x30000) >> 9) - (bp & 0x7F);
            if ((type == 0x20000000 || type == 0x30000000 || type == 0x40000000) && n < 32 && REG(D4_QWC) == 0) {
                _sceMpegDispatchMpegCbNodata(d->mp);
                if (d->abort) {
                    goto stop;
                }
                cnt = 0;
            }
            if (cnt++ > 5000) {
                _sceMpegDispatchMpegCbNodata(d->mp);
                if (d->abort) {
                stop:
                    _sceMpegStopIpuDma();
                    break;
                }
                cnt = 0;
            }
    }
    d->x82C = 0;
}

unsigned long _sceMpegWaitIpuIdle64(_sceMpegDecoder *d)
{
    unsigned long cmd;
    unsigned int bp;
    unsigned int type;
    unsigned int n;
    int cnt = 0;

    while ((long)(cmd = *(volatile unsigned long *)IPU_CMD) < 0 && !(REG(IPU_CTRL) & 0x4000)) {
            bp = REG(IPU_BP);
            type = d->x82C;
            n = ((bp & 0xFF00) >> 1) + ((bp & 0x30000) >> 9) - (bp & 0x7F);
            if ((type == 0x20000000 || type == 0x30000000 || type == 0x40000000) && n < 32 && REG(D4_QWC) == 0) {
                _sceMpegDispatchMpegCbNodata(d->mp);
                if (d->abort) {
                    goto stop;
                }
                cnt = 0;
            }
            if (cnt++ > 500) {
                _sceMpegDispatchMpegCbNodata(d->mp);
                if (d->abort) {
                stop:
                    _sceMpegStopIpuDma();
                    break;
                }
                cnt = 0;
            }
    }
    d->x82C = 0;
    return cmd;
}

unsigned int _sceMpegNextBit(_sceMpegDecoder *d, int n)
{
    unsigned int v;
    unsigned int cmd;

    _waitIpuIdle(d);
    if (d->x828 || d->x84C < n) {
        _setIpuCmd(d, 0x40000000);
        d->x848 = (int)_sceMpegWaitIpuIdle64(d);
    }
    v = (unsigned int)d->x848 >> (32 - n);
    d->x84C = 32;
    cmd = n | 0x40000000;
    _setIpuCmd(d, cmd);
    d->x848 = (int)_sceMpegWaitIpuIdle64(d);
    return v;
}

unsigned int _sceMpegPeepBit(_sceMpegDecoder *d, int n)
{
    if (d->x828 || d->x84C < n) {
        _waitIpuIdle(d);
        _setIpuCmd(d, 0x40000000);
        d->x848 = (int)_sceMpegWaitIpuIdle64(d);
        d->x84C = 32;
    }
    return (unsigned int)d->x848 >> (32 - n);
}

void _sceMpegFlushBuf(_sceMpegDecoder *d, int n)
{
    unsigned long v;

    _waitIpuIdle(d);
    _setIpuCmd(d, n | 0x40000000);
    v = _sceMpegWaitIpuIdle64(d);
    d->x848 = (int)v;
    d->x84C = 32;
}

static void _waitIpuIdle(_sceMpegDecoder *d)
{
    if ((REG(IPU_CTRL) & 0x80004000) == 0x80000000) {
        _sceMpegWaitIpuIdle(d);
    }
}

static void _flushBuf(_sceMpegDecoder *d, int n)
{
    _sceMpegFlushBuf(d, n);
}


static int _skipMB0(_sceMpegDecoder *d, int (*pmv)[2][2], int *motion_type, int (*mvfs)[2], int *mbtype)
{
    int ret = 1;

    d->spr[d->x820].skip = 1;
    d->x1C0 = 1;
    if (d->x160 == 2) {
        pmv[1][0][1] = 0;
        pmv[1][0][0] = 0;
        pmv[0][0][1] = 0;
        pmv[0][0][0] = 0;
    }
    if (d->x184 == 3) {
        *motion_type = 2;
    } else {
        *motion_type = 1;
        mvfs[0][0] = mvfs[0][1] = d->x184 == 2;
    }
    if (d->x160 == 1) {
        _sceMpegError(d, _skipMB0_msg);
        ret = 0;
    }
    *mbtype &= ~1;
    return ret;
}

/* Hand-written: MMI reference-copy routine */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _copyAddRefImage);

/* hand-written: its 0x00FF00FF constants (used by _copyAddRefImage) sit inside the code */
__asm__(".text\n"
    ".align 3\n"
    ".set noreorder\n"
    ".globl _copyRefImage\n"
    ".ent _copyRefImage\n"
    "_copyRefImage:\n"
    ".word 0x24020018\n"
    "lui $10, %hi($Lcopy_mask)\n" /* = 0x3C0A0037 / 0x254A06C0 at the original addresses: the mask below */
    "addiu $10, $10, %lo($Lcopy_mask)\n"
    ".word 0x79430000\n"
    ".word 0x78A80000\n"
    ".word 0x2042FFFC\n"
    ".word 0x00A0C82D\n"
    ".word 0x24A50080\n"
    ".word 0x7B290010\n"
    ".word 0x7B2A0020\n"
    ".word 0x7B2B0030\n"
    ".word 0x7B2C0040\n"
    ".word 0x7B2D0050\n"
    ".word 0x7B2E0060\n"
    ".word 0x7B2F0070\n"
    ".word 0x710341E8\n"
    ".word 0x710041C8\n"
    ".word 0x712349E8\n"
    ".word 0x712049C8\n"
    ".word 0x71284EC8\n"
    ".word 0x714351E8\n"
    ".word 0x714051C8\n"
    ".word 0x716359E8\n"
    ".word 0x716059C8\n"
    ".word 0x716A5EC8\n"
    ".word 0x718361E8\n"
    ".word 0x718061C8\n"
    ".word 0x71A369E8\n"
    ".word 0x71A069C8\n"
    ".word 0x71AC6EC8\n"
    ".word 0x71C371E8\n"
    ".word 0x71C071C8\n"
    ".word 0x71E379E8\n"
    ".word 0x71E079C8\n"
    ".word 0x71EE7EC8\n"
    ".word 0x7C890000\n"
    ".word 0x7C8B0010\n"
    ".word 0x7C8D0020\n"
    ".word 0x7C8F0030\n"
    ".word 0x1440FFDC\n"
    ".word 0x24840040\n"
    ".word 0\n"
    "$Lcopy_mask: .word 0x00FF00FF\n"
    ".word 0x00FF00FF\n"
    ".word 0x00FF00FF\n"
    ".word 0x00FF00FF\n"
    ".word 0x03E00008\n"
    ".word 0\n"
    ".end _copyRefImage\n"
    ".set reorder\n");

/* a DMA tag: id 3 (REF) or 0 (REFE) */
static inline unsigned long _dmaTag(unsigned int addr, int id, int qwc)
{
    return ((unsigned long)(addr & 0x0FFFFFFF) << 32) | ((unsigned long)id << 28) | qwc;
}

/* chains the reference blocks into the scratchpad: two REF tags per block, the last one ends the chain */
static void dmaRefImage(_sceMpegDecoder *d)
{
    unsigned long *p;
    int i;
    int n;
    int r;

    if (d->buffType == 0) {
        n = d->spr[d->x820].count;
        p = (unsigned long *)(((unsigned int)_sprtagdata & 0x0FFFFFFF) | 0x20000000);
        for (i = 0; i < n; i++) {
            p[0] = _dmaTag((unsigned int)d->spr[d->x820].addr0[i], 3, 0x30);
            p[2] = _dmaTag((unsigned int)d->spr[d->x820].addr1[i], i == n - 1 ? 0 : 3, 0x30);
            p += 4;
        }
        r = DIntr();
        __asm__ volatile("sync.l");
        REG(D9_SADR) = d->spr[d->x820].sadr;
        REG(D9_TADR) = (unsigned int)_sprtagdata;
        REG(D9_QWC) = 0;
        REG(D9_CHCR) = 0x105;
        if (r) {
            EIntr();
        }
    }
}

/* Hand-written: MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _rix_000);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _ri0_000);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _rix_001);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _ri0_001);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _rix_010);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _ri0_010);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _rix_011);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _ri0_011);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _rix_100);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _ri0_100);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _rix_101);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _ri0_101);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _rix_110);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _ri0_110);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _rix_111);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _ri0_111);

static void receiveDataFromIPU(unsigned int addr, int size)
{
    int r = DIntr();

    if ((addr >> 28) == 7) {
        addr &= 0x0FFFFFFF;
        addr |= 0x80000000;
    } else {
        addr &= 0x0FFFFFFF;
    }
    REG(D3_MADR) = addr;
    REG(D3_QWC) = size >> 4;
    REG(D3_CHCR) = 0x100;
    if (r) {
        EIntr();
    }
}


// SOURCE DATA: .data .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .data .rodata
