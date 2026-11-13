/* libmpeg ext.c (Sony EE SDK): MPEG-2 header and extension parsing */
#include "mpegdec.h"

#define IPU_CMD 0x10002000
#define IPU_BP 0x10002020
#define D4_MADR 0x1000B410
#define UNCACHED_ACCEL(p) (((unsigned int)(p) & 0x0FFFFFFF) | 0x20000000)

int DIntr(void);
int EIntr(void);
void _sceMpegError(_sceMpegDecoder *d, const char *msg);
void _sceMpegError1(_sceMpegDecoder *d, const char *fmt, int arg);
void _sceMpegAlalcFree(_sceMpegAlalc *a);
unsigned char *_sceMpegAlalcAlloc(_sceMpegDecoder *d, _sceMpegAlalc *a, int size, unsigned int align);
int _sceMpegDispatchMpegCallback(sceMpeg *mp, sceMpegCbData *cbdata);
unsigned int _sceMpegNextBit(_sceMpegDecoder *d, int n);
unsigned int _sceMpegPeepBit(_sceMpegDecoder *d, int n);
void _sceMpegFlushBuf(_sceMpegDecoder *d, int n);
void _sceMpegWaitIpuIdle(_sceMpegDecoder *d);
int _sceMpegMbAddressIncrement(_sceMpegDecoder *d);

typedef struct {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
} _sceMpegSliceState;

/* a value that is read but not used (an argument: the read is no tail call) */
static inline void _unused(unsigned int v)
{
}

static void _initSeq(sceMpeg *mp);
static void _sequenceHeader(_sceMpegDecoder *d);
static void _groupOfPicturesHeader(_sceMpegDecoder *d);
static void _pictureHeader(_sceMpegDecoder *d);
static void _extensionAndUserData(_sceMpegDecoder *d);
static void _waitIpuIdle(_sceMpegDecoder *d);
static void _flushBuf(_sceMpegDecoder *d, int n);
static inline void flushBuf32(_sceMpegDecoder *d);
static inline void flushByteBoundary(_sceMpegDecoder *d);

static unsigned char _defIQM[64] = {
    0x08, 0x10, 0x10, 0x13, 0x10, 0x13, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x1A, 0x18, 0x1A, 0x1B,
    0x1B, 0x1B, 0x1A, 0x1A, 0x1A, 0x1A, 0x1B, 0x1B, 0x1B, 0x1D, 0x1D, 0x1D, 0x22, 0x22, 0x22, 0x1D,
    0x1D, 0x1D, 0x1B, 0x1B, 0x1D, 0x1D, 0x20, 0x20, 0x22, 0x22, 0x25, 0x26, 0x25, 0x23, 0x23, 0x22,
    0x23, 0x26, 0x26, 0x28, 0x28, 0x28, 0x30, 0x30, 0x2E, 0x2E, 0x38, 0x38, 0x3A, 0x45, 0x45, 0x53,
};

static unsigned char _defNIQM[64] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};

/* next_start_code() */
static inline void _nextStartCode(_sceMpegDecoder *d)
{
    flushByteBoundary(d);
    while (_sceMpegPeepBit(d, 24) != 1 && !d->abort) {
        _flushBuf(d, 8);
    }
}

int _sceMpegSliceA0(_sceMpegDecoder *d, void *unused, int *mbaddr, int *mbinc, _sceMpegSliceState *st)
{
    unsigned int code;
    int inc;

    d->x12C = 0;
    _nextStartCode(d);
    code = _sceMpegPeepBit(d, 32);
    if (code - 0x101 >= 0xAF) {
        _sceMpegError1(d, "slice_start_code(0x%08x) out of range", code);
        return 2;
    }
    flushBuf32(d);
    d->x1C4 = _sceMpegNextBit(d, 5);
    if (_sceMpegNextBit(d, 1)) {
        _sceMpegNextBit(d, 1);
        _flushBuf(d, 7);
        while (_sceMpegNextBit(d, 1)) {
            _flushBuf(d, 8);
        }
    }
    inc = _sceMpegMbAddressIncrement(d);
    *mbinc = inc;
    if (d->x12C) {
        _sceMpegError(d, "_sceMpegSliceA0(): error happens");
        return 1;
    }
    *mbaddr = ((code & 0xFF) - 1) * d->x13C + inc - 1;
    *mbinc = 1;
    d->x1C0 = 1;
    st->x14 = 0;
    st->x10 = 0;
    st->x4 = 0;
    st->x0 = 0;
    st->x1C = 0;
    st->x18 = 0;
    st->xC = 0;
    st->x8 = 0;
    return 0;
}

static inline void _setPictSize(_sceMpegPicture *p, int w, int h)
{
    p->width = w;
    p->height = h;
    p->mbw = w >> 4;
    p->mbh = h >> 4;
}

static void _initSeq(sceMpeg *mp)
{
    _sceMpegDecoder *d = mp->sys;
    int w;
    int h;
    unsigned int size;
    int off;

    if (d->x858 == 0) {
        d->x184 = 3;
        d->x14C = 1;
        d->x150 = 1;
        d->x198 = 1;
        d->x18C = 1;
        d->x154 = 5;
    }
    d->x13C = (d->x134 + 15) >> 4;
    d->x140 = (d->x858 && !d->x14C) ? ((d->x138 + 31) >> 5) << 1 : (d->x138 + 15) >> 4;
    w = d->x13C << 4;
    h = d->x140 << 4;
    if (w == mp->width && h == mp->height) {
        return;
    }
    mp->width = w;
    mp->height = h;
    size = (unsigned int)(w * 384 * h) >> 8;
    _sceMpegAlalcFree(&d->alalc);
    d->x10C = (int)_sceMpegAlalcAlloc(d, &d->alalc, size, 64);
    d->x110 = (int)_sceMpegAlalcAlloc(d, &d->alalc, size, 64);
    d->x114 = (int)_sceMpegAlalcAlloc(d, &d->alalc, size, 64);
    if (d->buffType == 0) {
        off = mp->width * mp->height / 512;
        d->pict[0].buf = UNCACHED_ACCEL(d->x10C);
        d->pict[1].buf = UNCACHED_ACCEL(d->x110);
        d->pict[3].buf = UNCACHED_ACCEL(d->x10C);
        d->pict[4].buf = UNCACHED_ACCEL(d->x110);
        d->pict[2].buf = UNCACHED_ACCEL(d->x114);
        d->pict[5].buf = UNCACHED_ACCEL(d->x114);
        d->pict[6].buf = UNCACHED_ACCEL(d->x10C) + off * 384;
        d->pict[7].buf = UNCACHED_ACCEL(d->x110) + off * 384;
        d->pict[8].buf = UNCACHED_ACCEL(d->x114) + off * 384;
    } else if (d->buffType == 1) {
        off = mp->width * mp->height / 512;
        d->pict[0].buf = d->x10C;
        d->pict[1].buf = d->x110;
        d->pict[3].buf = d->x10C;
        d->pict[4].buf = d->x110;
        d->pict[2].buf = d->x114;
        d->pict[5].buf = d->x114;
        d->pict[6].buf = d->x10C + off * 384;
        d->pict[7].buf = d->x110 + off * 384;
        d->pict[8].buf = d->x114 + off * 384;
    }
    _setPictSize(&d->pict[0], w, h);
    _setPictSize(&d->pict[1], w, h);
    _setPictSize(&d->pict[2], w, h);
    _setPictSize(&d->pict[3], w, h / 2);
    _setPictSize(&d->pict[4], w, h / 2);
    _setPictSize(&d->pict[5], w, h / 2);
    _setPictSize(&d->pict[6], w, h / 2);
    _setPictSize(&d->pict[7], w, h / 2);
    _setPictSize(&d->pict[8], w, h / 2);
}

/* loads a default quantizer matrix from memory into the IPU */
static inline void _setIQ(_sceMpegDecoder *d, unsigned char *mat, unsigned int cmd)
{
    sceMpegCbData cb;
    int old;

    cb.type = 2;
    _sceMpegDispatchMpegCallback(d->mp, &cb);
    _waitIpuIdle(d);
    REG(IPU_CMD) = 0; _waitIpuIdle(d);
    old = DIntr();
    REG(D4_MADR) = (unsigned int)mat & 0x0FFFFFFF;
    REG(D4_QWC) = 4;
    REG(D4_CHCR) = 0x101; if (old) {
        EIntr();
    }
    REG(IPU_CMD) = cmd;
    d->x82C = 0x50000000;
    d->x828 = 1;
    _waitIpuIdle(d);
    cb.type = 3;
    _sceMpegDispatchMpegCallback(d->mp, &cb);
}

static void _sequenceHeader(_sceMpegDecoder *d)
{
    unsigned int v;

    d->xE8 = 0;
    v = _sceMpegNextBit(d, 32);
    d->x134 = v >> 20;
    d->x138 = (v >> 8) & 0xFFF;
    if (d->x138 > 2800) {
        _sceMpegError(d, "vertical size > 2800");
    }
    v = _sceMpegNextBit(d, 30);
    d->x144 = v >> 12;
    d->x148 = (v >> 1) & 0x3FF;
    if ((d->x850 = _sceMpegNextBit(d, 1)) != 0) {
        _waitIpuIdle(d);
        REG(IPU_CMD) = 0x50000000;
        d->x82C = 0x50000000;
        d->x828 = 1;
        _waitIpuIdle(d);
    } else {
        _setIQ(d, _defIQM, 0x50000000);
    }
    if ((d->x854 = _sceMpegNextBit(d, 1)) != 0) {
        _waitIpuIdle(d);
        REG(IPU_CMD) = 0x58000000;
        d->x82C = 0x50000000;
        d->x828 = 1;
        _waitIpuIdle(d);
    } else {
        _setIQ(d, _defNIQM, 0x58000000);
    }
    _extensionAndUserData(d);
    _initSeq(d->mp);
}

static void _groupOfPicturesHeader(_sceMpegDecoder *d)
{
    d->xFC = 0;
    d->x85C = d->x860 + 1;
    d->x864 = 1;
    _sceMpegNextBit(d, 1);
    _sceMpegNextBit(d, 5);
    _sceMpegNextBit(d, 6);
    _sceMpegNextBit(d, 1);
    _sceMpegNextBit(d, 6);
    _sceMpegNextBit(d, 6);
    d->x1B4 = _sceMpegNextBit(d, 1);
    d->x1B8 = _sceMpegNextBit(d, 1);
    _extensionAndUserData(d);
}

static void _pictureHeader(_sceMpegDecoder *d)
{
    int tr;
    int wrap;
    int ref;

    tr = _sceMpegNextBit(d, 10);
    d->x160 = _sceMpegNextBit(d, 3);
    _sceMpegNextBit(d, 16);
    if (d->x160 == 2 || d->x160 == 3) {
        d->x164 = _sceMpegNextBit(d, 1);
        d->x168 = _sceMpegNextBit(d, 3);
    }
    if (d->x160 == 3) {
        d->x16C = _sceMpegNextBit(d, 1);
        d->x170 = _sceMpegNextBit(d, 3);
    }
    while (_sceMpegNextBit(d, 1)) {
        _flushBuf(d, 8);
    }
    _extensionAndUserData(d);
    wrap = 0;
    ref = 0;
    if (d->x160 != 3 && tr != 0) {
        if (tr < 0) {
            wrap = !d->x864;
        }
        d->x864 = 0;
        ref = tr;
    }
    d->x1BC = d->x85C + tr;
    if (wrap && !(ref < tr)) {
        d->x1BC += 0x400;
    }
    d->x860 = (d->x860 < d->x1BC) ? d->x1BC : d->x860;
}

static void _sequenceExtension(_sceMpegDecoder *d)
{
    unsigned int v;
    unsigned int pl;
    unsigned int hext;
    unsigned int vext;
    unsigned int brext;
    unsigned int vbvext;

    d->x858 = 1;
    REG(IPU_CTRL) &= ~0x800000; v = _sceMpegNextBit(d, 28);
    brext = (v >> 1) & 0xFFF;
    vext = (v >> 13) & 3;
    hext = (v >> 15) & 3;
    if ((d->x150 = (v >> 17) & 3) != 1) {
        _sceMpegError(d, "_chroma_format needs to be 1: 420");
    }
    d->x14C = (v >> 19) & 1;
    pl = v >> 20;
    vbvext = _sceMpegNextBit(d, 16) >> 8;
    if (pl != 0x48 && pl != 0x58 && pl != 0x44) {
        _sceMpegError(d, "Unsupported profile/level");
    }
    d->x134 = (hext << 12) | (d->x134 & 0xFFF);
    d->x138 = (vext << 12) | (d->x138 & 0xFFF);
    d->x144 += brext << 18;
    d->x148 += vbvext << 10;
}

static void _quantMatrixExtension(_sceMpegDecoder *d)
{
    if ((d->x850 = _sceMpegNextBit(d, 1)) != 0) {
        _waitIpuIdle(d);
        REG(IPU_CMD) = 0x50000000;
        d->x82C = 0x50000000;
        d->x828 = 1;
        _waitIpuIdle(d);
    }
    if ((d->x854 = _sceMpegNextBit(d, 1)) != 0) {
        _waitIpuIdle(d);
        REG(IPU_CMD) = 0x58000000;
        d->x82C = 0x50000000;
        d->x828 = 1;
        _waitIpuIdle(d);
    }
    if (_sceMpegNextBit(d, 1)) {
        _sceMpegError(d, "load_chroma_intra_quantizer_matrix == 1");
    }
    if (_sceMpegNextBit(d, 1)) {
        _sceMpegError(d, "load_chroma_non_intra_quantizer_matrix == 1");
    }
}

static void _sequenceScalableExtension(_sceMpegDecoder *d)
{
    _sceMpegError(d, "_sequenceScalableExtension() is not implemented");
}

static void _unknown_extension(_sceMpegDecoder *d)
{
    _sceMpegError(d, "Unknown Extension");
}

static void _pictureCodingExtension(_sceMpegDecoder *d)
{
    d->x174 = _sceMpegNextBit(d, 4);
    d->x178 = _sceMpegNextBit(d, 4);
    d->x17C = _sceMpegNextBit(d, 4);
    d->x180 = _sceMpegNextBit(d, 4);
    REG(IPU_CTRL) = (REG(IPU_CTRL) & ~0x30000) | (_sceMpegNextBit(d, 2) << 16); d->x184 = _sceMpegNextBit(d, 2);
    if (d->xE8 == 0) {
        d->xE8 = d->x184;
    }
    d->x188 = _sceMpegNextBit(d, 1);
    d->x18C = _sceMpegNextBit(d, 1);
    d->x190 = _sceMpegNextBit(d, 1);
    REG(IPU_CTRL) = (REG(IPU_CTRL) & ~0x400000) | (_sceMpegNextBit(d, 1) << 22); REG(IPU_CTRL) = (REG(IPU_CTRL) & ~0x200000) | (_sceMpegNextBit(d, 1) << 21); REG(IPU_CTRL) = (REG(IPU_CTRL) & ~0x100000) | (_sceMpegNextBit(d, 1) << 20); d->x194 = _sceMpegNextBit(d, 1);
    _sceMpegNextBit(d, 1);
    d->x198 = _sceMpegNextBit(d, 1);
    if (_sceMpegNextBit(d, 1)) {
        _sceMpegNextBit(d, 1);
        _sceMpegNextBit(d, 3);
        _sceMpegNextBit(d, 1);
        _sceMpegNextBit(d, 7);
        _unused(_sceMpegNextBit(d, 8));
    }
}

static void _pictureSpatialScalableExtension(_sceMpegDecoder *d)
{
    _sceMpegError(d, "_pictureSpatialScalableExtension is not supported");
}

static void _pictureTemporalScalableExtension(_sceMpegDecoder *d)
{
    _sceMpegError(d, "_pictureTemporalScalableExtension is not supported");
}

static inline void _sequenceDisplayExtension(_sceMpegDecoder *d)
{
    _sceMpegNextBit(d, 3);
    if (_sceMpegNextBit(d, 1)) {
        _sceMpegNextBit(d, 8);
        _sceMpegNextBit(d, 8);
        d->x154 = _sceMpegNextBit(d, 8);
    }
    d->x158 = _sceMpegNextBit(d, 14);
    _sceMpegNextBit(d, 1);
    d->x15C = _sceMpegNextBit(d, 14);
}

static inline int _copyrightExtension(_sceMpegDecoder *d)
{
    _sceMpegNextBit(d, 1);
    _sceMpegNextBit(d, 8);
    _sceMpegNextBit(d, 1);
    _sceMpegNextBit(d, 7);
    _sceMpegNextBit(d, 1);
    _sceMpegNextBit(d, 20);
    _sceMpegNextBit(d, 1);
    _sceMpegNextBit(d, 22);
    _sceMpegNextBit(d, 1);
    _unused(_sceMpegNextBit(d, 22));
}

static inline void _pictureDisplayExtension(_sceMpegDecoder *d)
{
    int n;
    int i;

    if (d->x14C) {
        if (d->x194) {
            n = d->x188 ? 3 : 2;
        } else {
            n = 1;
        }
    } else {
        if (d->x184 != 3) {
            n = 1;
        } else {
            n = d->x194 ? 3 : 2;
        }
    }
    for (i = 0; i < n; i++) {
        d->hoff[i] = _sceMpegNextBit(d, 16);
        _sceMpegNextBit(d, 1);
        d->voff[i] = _sceMpegNextBit(d, 16);
        _sceMpegNextBit(d, 1);
    }
}

static void _extensionAndUserData(_sceMpegDecoder *d)
{
    void (*ext[11])(_sceMpegDecoder *) = {
        _unknown_extension,
        _sequenceExtension,
        _sequenceDisplayExtension,
        _quantMatrixExtension,
        (void (*)(_sceMpegDecoder *))_copyrightExtension,
        _sequenceScalableExtension,
        _unknown_extension,
        _pictureDisplayExtension,
        _pictureCodingExtension,
        _pictureSpatialScalableExtension,
        _pictureTemporalScalableExtension,
    };
    unsigned int code;
    unsigned int id;
    unsigned int max;

    _nextStartCode(d);
    while ((code = _sceMpegPeepBit(d, 32)) == 0x1B5 || code == 0x1B2) {
        if (code == 0x1B5) {
            flushBuf32(d);
            id = _sceMpegNextBit(d, 4);
            max = 10;
            if (id > max) {
                id = 0;
            }
            ext[id](d);
            _nextStartCode(d);
        } else {
            flushBuf32(d);
            _nextStartCode(d);
        }
    }
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

int _sceMpegNextHeader(_sceMpegDecoder *d)
{
    sceMpegCbData cb;
    unsigned int code;

    while (1) {
        if (d->abort) {
            return -1;
        }
        _nextStartCode(d);
        code = _sceMpegNextBit(d, 32);
        switch (code) {
        case 0x1B3:
            _sequenceHeader(d);
            break;
        case 0x1B7:
            return 0;
        case 0x1B8:
            _groupOfPicturesHeader(d);
            break;
        case 0x100:
            _pictureHeader(d);
            cb.type = 5;
            cb.pts = -1;
            cb.dts = -1;
            _sceMpegDispatchMpegCallback(d->mp, &cb);
            d->x838 = cb.pts;
            d->x840 = cb.dts;
            return d->x160;
        }
    }
}

static inline void flushBuf32(_sceMpegDecoder *d)
{
    _flushBuf(d, 32);
}

static inline void flushByteBoundary(_sceMpegDecoder *d)
{
    int n;

    _waitIpuIdle(d);
    if ((n = -(REG(IPU_BP) & 7) & 7) != 0) {
        _flushBuf(d, n);
    }
}

// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
