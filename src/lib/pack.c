/* libmpeg pack.c (Sony EE SDK): MPEG-2 program stream demultiplexing (pack, system and PES headers) */
#include "include_asm.h"
#include "mpegdec.h"

/* bit reader over a ring buffer: buf holds the next bits MSB first, count of them valid */
typedef struct {
    unsigned long buf; /* 0x00 */
    long pos; /* 0x08: bits consumed */
    unsigned char *start; /* 0x10 */
    unsigned char *ptr; /* 0x14 */
    int count; /* 0x18 */
    unsigned char *ring; /* 0x1C */
    unsigned char *end; /* 0x20 */
    int size; /* 0x24 */
    int _pad28[2];
} _sceMpegBs;

typedef struct {
    int scrExt; /* 0x00 */
    unsigned int scr; /* 0x04 */
    unsigned int scrHi; /* 0x08 */
    int hasSystemHeader; /* 0x0C */
    int _pad10[2];
} _sceMpegPackHeader;

typedef struct {
    unsigned long id; /* 0x00 */
    int _pad08[2];
    long pts; /* 0x10 */
    long dts; /* 0x18 */
    int data; /* 0x20: bit offset of the payload */
    int len; /* 0x24 */
    int header; /* 0x28: bit offset of the packet */
    int _pad2C;
} _sceMpegPes;

typedef struct {
    _sceMpegPackHeader ph;
    _sceMpegPes pes;
} _sceMpegHdr;

typedef struct {
    int type;
    unsigned char *header;
    unsigned char *data;
    unsigned int len;
    long pts;
    long dts;
} sceMpegCbDataStr;

typedef struct {
    unsigned long id; /* 0x00 */
    unsigned long mask; /* 0x08 */
    sceMpegCallback func; /* 0x10 */
    int data; /* 0x14 */
    void *gp; /* 0x18 */
    int _pad1C;
} _sceMpegStrCb;

typedef struct {
    unsigned long id;
    unsigned long mask;
} _sceMpegStrMap;

int sceMpegDemuxPssRing(sceMpeg *mp, unsigned char *pss, int size, unsigned char *ring, int ringsize);
static int _PES_packet(_sceMpegDecoder *d, _sceMpegBs *bs, _sceMpegPes *pes);

static _sceMpegStrMap _strmap[10] = {
    { 0xE000000000, 0xFF00000000 },
    { 0xBDFFC00000, 0xFFFFFFFFFF },
    { 0xBDFFA00000, 0xFFFFFFFFFF },
    { 0xBDFFA10000, 0xFFFFFFFFFF },
    { 0xBDFF900000, 0xFFFFFFFFFF },
    { 0xC000000000, 0xFF00000000 },
    { 0xBD80000000, 0xFFFF000000 },
    { 0xBDA0000000, 0xFFFF000000 },
    { 0xBD88000000, 0xFFFF000000 },
    { 0xBD90000000, 0xFFFF000000 },
};

/* drops n bits and refills the buffer from the ring */
static inline void _flushBits(_sceMpegBs *bs, int n)
{
    bs->buf <<= n;
    bs->count -= n;
    while (bs->count <= 56) {
        bs->buf |= (unsigned long)*bs->ptr++ << (56 - bs->count);
        if (bs->ptr >= bs->end) {
            bs->ptr = bs->ring;
        }
        bs->count += 8;
    }
    bs->pos += n;
}

static inline unsigned int _showBits(_sceMpegBs *bs, int n)
{
    return bs->buf >> (64 - n);
}

static inline unsigned int _getBits(_sceMpegBs *bs, int n)
{
    unsigned int v = bs->buf >> (64 - n);

    _flushBits(bs, n);
    return v;
}

int _sceMpegId2type(int *type, int *ch, unsigned long id)
{
    unsigned int i;
    int found = 0;
    unsigned long mask;
    unsigned long mid;
    unsigned long m;
    unsigned long cm;

    for (i = 0; i < 10 && !found; i++) {
        mask = _strmap[i].mask;
        switch (mask) {
        case 0xFFFFFFFFFF:
            m = id & 0xFFFFFF0000;
            if (m == _strmap[i].id) {
                *type = i;
                *ch = (int)(id & 0xFFFF);
                found = 1;
            }
            break;
        case 0xFFFF000000:
            mid = _strmap[i].id;
            switch (mid) {
            case 0xBD20000000:
                m = id & 0xFFE0000000;
                cm = 0x1F;
                break;
            case 0xBD80000000:
            case 0xBD88000000:
            case 0xBD90000000:
            case 0xBDA0000000:
                m = id & 0xFFF8000000;
                cm = 7;
                break;
            default:
                m = id & 0xFFFF000000;
                cm = 0;
                break;
            }
            if (m == mid) {
                *type = i;
                *ch = (int)((id >> 24) & cm);
                found = 1;
            }
            break;
        case 0xFF00000000:
            mid = _strmap[i].id;
            if (mid == 0xE000000000) {
                m = id & 0xF000000000;
                cm = 0xF;
            } else if (mid == 0xC000000000) {
                m = id & 0xE000000000;
                cm = 0x1F;
            } else {
                m = id & mask;
                cm = 0;
            }
            if (m == mid) {
                *type = i;
                *ch = (int)((id >> 32) & cm);
                found = 1;
            }
            break;
        }
    }
    return found;
}

static int _system_header(_sceMpegBs *bs, _sceMpegPackHeader *ph)
{
    _flushBits(bs, 56);
    _flushBits(bs, 40);
    while (_showBits(bs, 1) == 1) {
        _flushBits(bs, 24);
    }
    return 1;
}

static int _pack_header(_sceMpegBs *bs, _sceMpegPackHeader *ph)
{
    unsigned int hi;
    unsigned int mid;
    unsigned int lo;
    unsigned int stuffing;
    unsigned int i;

    _flushBits(bs, 34);
    hi = _getBits(bs, 3);
    _flushBits(bs, 1);
    mid = _getBits(bs, 15);
    _flushBits(bs, 1);
    lo = _getBits(bs, 15);
    _flushBits(bs, 1);
    ph->scrExt = _getBits(bs, 9);
    _flushBits(bs, 30);
    stuffing = _getBits(bs, 3);
    ph->scr = (hi << 30) | (mid << 15) | lo;
    ph->scrHi = hi >> 2;
    for (i = 0; i < stuffing; i++) {
        _flushBits(bs, 8);
    }
    if (_showBits(bs, 32) == 0x1BB) {
        ph->hasSystemHeader = 1;
        _system_header(bs, ph);
    } else {
        ph->hasSystemHeader = 0;
    }
    return 1;
}

/* _PES_packet's constants */
__asm__(".rdata\n"
        ".align 3\n.byte 0x00,0x10,0x08,0x18,0x08,0x18,0x10,0x20,0x18,0x28,0x20,0x30,0x20,0x30,0x28,0x38\n"
        ".align 3\n.asciz \"pack_header_field_flag needs to be '0' in PS\\n\"\n"
        ".text\n");

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/pack", _PES_packet);

static inline void _bsInit(_sceMpegBs *bs, unsigned char *pss, unsigned char *ring, int ringsize)
{
    bs->start = pss;
    bs->ptr = pss;
    bs->size = ringsize;
    bs->pos = 0;
    bs->ring = ring;
    bs->end = ring + ringsize;
    bs->buf = 0;
    bs->count = 0;
    do {
        bs->buf |= (unsigned long)*bs->ptr++ << (56 - bs->count);
        if (bs->ptr >= bs->end) {
            bs->ptr = bs->ring;
        }
        bs->count += 8;
    } while (bs->count <= 56);
    bs->pos = 0;
}

/* address of a bit offset in the ring */
static inline unsigned char *_bsAddr(_sceMpegBs *bs, int bits)
{
    unsigned char *p = bs->start + (bits >> 3);

    if (p >= bs->end) {
        p -= bs->size;
    }
    return p;
}

/* calls a stream callback with its own $gp */
#define _callStrCallback(r_, mp, bs, h, cbs, func_, data_, gp_)                                 \
    ({                                                                                          \
        int __old;                                                                              \
        unsigned char *__p;                                                                     \
                                                                                                \
        __p = (bs)->start + ((h)->pes.header >> 3);                                             \
        (cbs)->type = 6;                                                                        \
        if (__p >= (bs)->end) {                                                                 \
            __p -= (bs)->size;                                                                  \
        }                                                                                       \
        (cbs)->header = __p;                                                                    \
        (cbs)->data = _bsAddr(bs, (h)->pes.data);                                               \
        (cbs)->len = (h)->pes.len;                                                              \
        (cbs)->pts = (h)->pes.pts;                                                              \
        (cbs)->dts = (h)->pes.dts;                                                              \
        __asm__ volatile("move %0, $gp\n\tmove $gp, %1" : "=&r"(__old) : "r"(gp_));             \
        r_ = (func_)(mp, (sceMpegCbData *)(cbs), (void *)(data_));                             \
        __asm__ volatile("move %0, $gp\n\tmove $gp, %1" : "=&r"(__old) : "r"(__old));           \
    })

/* 6 lines off: the zero store of allData, the delay slot of the first pack header test, the register of allGp */
#if 0
int sceMpegDemuxPssRing(sceMpeg *mp, unsigned char *pss, int size, unsigned char *ring, int ringsize)
{
    _sceMpegBs bsBody;
    _sceMpegHdr hdr;
    sceMpegCbDataStr cbs;
    _sceMpegBs *bs;
    _sceMpegHdr *h = &hdr;
    _sceMpegDecoder *d = mp->sys;
    int allData;
    _sceMpegStrCb *cb = (_sceMpegStrCb *)d->x60;
    int ret;
    int allGp;
    sceMpegCallback allFunc = 0;
    int cont = 1;
    int end = size * 8;
    int i;

    ret = 0;
    allData = 0;
    allGp = 0;
    _bsInit(&bsBody, pss, ring, ringsize);
    bs = &bsBody;

    for (i = 0; !allFunc && i < d->x64; i++) {
        if (cb[i].id == 0xBDFF000000) {
            allFunc = cb[i].func;
            allData = cb[i].data;
            allGp = (int)cb[i].gp;
        }
    }
    for (;;) {
        if (_showBits(bs, 32) == 0x1BA) {
            _pack_header(bs, &h->ph);
        }
        while ((bs->buf >> 40) == 1 && _showBits(bs, 32) != 0x1BA && _showBits(bs, 32) != 0x1B9) {
            if ((int)bs->pos >= end || !cont) {
                goto done;
            }
            _PES_packet(d, bs, &h->pes);
            if ((int)bs->pos > end) {
                continue;
            }
            for (i = 0; i < d->x64; i++) {
                if (cb[i].id == (h->pes.id & cb[i].mask)) {
                    _callStrCallback(cont, mp, bs, h, &cbs, cb[i].func, cb[i].data, (int)cb[i].gp);
                    break;
                }
            }
            if (i == d->x64 && allFunc) {
                _callStrCallback(cont, mp, bs, h, &cbs, allFunc, allData, allGp);
            }
            if (cont) {
                ret = (int)(bs->pos >> 3);
            }
        }
    done:
        if ((int)bs->pos > end || _showBits(bs, 32) != 0x1BA) {
            break;
        }
    }
    return ret;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/pack", sceMpegDemuxPssRing);
#endif

int sceMpegDemuxPss(sceMpeg *mp, unsigned char *pss, int size)
{
    return sceMpegDemuxPssRing(mp, pss, size, 0, -1);
}

inline unsigned long _sceMpegType2id(unsigned int type, int ch)
{
    unsigned long id = 0;
    int shift = 0;

    if (type < 10) {
        switch (_strmap[type].mask) {
        case 0xFFFFFFFFFF:
            shift = 0;
            break;
        case 0xFFFF000000:
            shift = 24;
            break;
        case 0xFF00000000:
            shift = 32;
            break;
        default:
            return 0;
        }
        id = _strmap[type].id | ((unsigned long)ch << shift);
    }
    return id;
}

sceMpegCallback sceMpegAddStrCallback(sceMpeg *mp, unsigned int type, int ch, sceMpegCallback func, void *data)
{
    _sceMpegDecoder *d = mp->sys;
    _sceMpegStrCb *cb = (_sceMpegStrCb *)d->x60;
    sceMpegCallback old = 0;
    unsigned long id = _sceMpegType2id(type, ch);
    void *gp;
    int i;

    for (i = 0; i < d->x64; i++) {
        if (id == cb[i].id) {
            old = cb[i].func;
            break;
        }
    }
    if (i < 64) {
        cb[i].id = id;
        cb[i].func = func;
        cb[i].data = (int)data;
        cb[i].mask = _strmap[type].mask;
        __asm__ volatile("move %0, $gp" : "=r"(gp));
        cb[i].gp = gp;
        d->x64++;
    }
    return old;
}

// SOURCE DATA: .data .rodata
