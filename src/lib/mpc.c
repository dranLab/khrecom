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

int DIntr(void);
int EIntr(void);
void _sceMpegError(_sceMpegDecoder *d, const char *msg);
void _sceMpegError1(_sceMpegDecoder *d, const char *fmt, int arg);
int _sceMpegDispatchMpegCbNodata(sceMpeg *mp);
void _sceMpegStopIpuDma(void);
unsigned long _sceMpegWaitIpuIdle64(_sceMpegDecoder *d);
unsigned int _sceMpegNextBit(_sceMpegDecoder *d, int n);
unsigned int _sceMpegPeepBit(_sceMpegDecoder *d, int n);
static void _waitIpuIdle(_sceMpegDecoder *d);
static void _getAllRefs(_sceMpegDecoder *d, int x, int y, int mbtype, int motion_type, int (*pmv)[2][2], int (*mvfs)[2]);
static void dmaRefImage(_sceMpegDecoder *d);
void _copyRefImage(unsigned int dst, unsigned int src);
void _copyAddRefImage(unsigned int dst, unsigned int src, unsigned int buf);
static void _flushBuf(_sceMpegDecoder *d, int n);

/* issues an IPU command; the bit buffer is kept valid only across BDEC, VDEC and FDEC */
#define _setIpuCmd(d, cmd)                                                              \
    ({                                                                                  \
        unsigned int __type;                                                            \
                                                                                        \
        (void)(REG(IPU_CMD) = (cmd));                                                   \
        __type = (cmd) & 0xF0000000;                                                    \
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

/* _getAllRefs's strings */
__asm__(".rdata\n"
        ".align 3\n.asciz \"(a) invalid motion_type(%d)-0\"\n"
        ".align 3\n.asciz \"(b) invalid motion_type(%d)-1\"\n"
        ".align 3\n.asciz \"(c) invalid motion_type(%d)-2\"\n"
        ".text\n");

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _getAllRefs);


static int _motionComp0(_sceMpegDecoder *d, int mbaddr, int dct_type, int mbtype, int motion_type, int (*pmv)[2][2],
                        int (*mvfs)[2])
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
        _getAllRefs(d, x, y, mbtype, motion_type, pmv, mvfs);
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

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _getRef0);


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

/* _sceMpegPictureData0's strings */
__asm__(".rdata\n"
        ".align 3\n.asciz \"= Skip to the next picture =\"\n"
        ".text\n");

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _sceMpegPictureData0);

/* _slice0's strings */
__asm__(".rdata\n"
        ".align 3\n.asciz \"Too many macroblocks in picture\"\n"
        ".text\n");

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _slice0);
/* _skipMB0's strings */
__asm__(".rdata\n"
        ".align 3\n.asciz \"skiped macroblock in I picure is not allowed\"\n"
        ".text\n");

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

/* _decMB0's strings */
__asm__(".rdata\n"
        ".align 3\n.asciz \"Invalid macroblock_type code: 0\"\n"
        ".text\n");

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _decMB0);


/* Parked: the loop keeps the IPU_CMD address in a saved register here (the original rebuilds it each pass) */
#if 0
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
#endif
/* _sceMpegMbAddressIncrement's strings */
__asm__(".rdata\n"
        ".align 3\n.asciz \"Invalid macroblock_address_increment code(0x%08x)\"\n"
        ".text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _sceMpegMbAddressIncrement);

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


INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _skipMB0);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _copyAddRefImage);

/* hand-written: its 0x00FF00FF constants (used by _copyAddRefImage) sit inside the code */
__asm__(".text\n"
    ".align 3\n"
    ".set noreorder\n"
    ".globl _copyRefImage\n"
    ".ent _copyRefImage\n"
    "_copyRefImage:\n"
    ".word 0x24020018\n"
    ".word 0x3C0A0037\n"
    ".word 0x254A06C0\n"
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
    ".word 0x00FF00FF\n"
    ".word 0x00FF00FF\n"
    ".word 0x00FF00FF\n"
    ".word 0x00FF00FF\n"
    ".word 0x03E00008\n"
    ".word 0\n"
    ".end _copyRefImage\n"
    ".set reorder\n");

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", dmaRefImage);

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

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", receiveDataFromIPU);


// SOURCE DATA: .data .rodata .bss
