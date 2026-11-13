/* libmpeg mpeg.c (Sony EE SDK): decoder setup, the picture loop and the callbacks (the library has no type
   information: the decoder's fields are named by offset) */

#include "mpegdec.h"

void *memset(void *s, int c, unsigned int n);
int scePrintf(const char *fmt, ...);
void FlushCache(int op);
int DIntr(void);
int EIntr(void);
void sceIpuInit(void);
int sceIpuSync(int mode, unsigned short timeout);
void _sceMpegError(_sceMpegDecoder *d, const char *msg);
void _sceMpegError1(_sceMpegDecoder *d, const char *fmt, int arg);
void _sceMpegAlalcInit(_sceMpegAlalc *a, unsigned char *start, int size);
void _sceMpegAlalcSetDynamic(_sceMpegAlalc *a);
unsigned char *_sceMpegAlalcAlloc(_sceMpegDecoder *d, _sceMpegAlalc *a, int size, unsigned int align);
int _sceMpegDefaultStopDMA(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData);
int _sceMpegDefaultRestartDMA(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData);
int _sceMpegNextHeader(_sceMpegDecoder *d);
int _sceMpegPictureData0(_sceMpegDecoder *d);
int _sceMpegFlush(sceMpeg *mp);
void _sceMpegOutputFrame(_sceMpegDecoder *d, int n, int output);

int sceMpegReset(sceMpeg *mp);
int sceMpegClearRefBuff(sceMpeg *mp);
static void _clearEach(_sceMpegDecoder *d);
static int _getpic(sceMpeg *mp);
static int _decodeOrSkip(sceMpeg *mp, int count, int mode);
static inline int _decPicture(_sceMpegDecoder *d);

typedef char _check_size[sizeof(_sceMpegDecoder) == 0x19A0 ? 1 : -1];
typedef char _check_pict[(unsigned int)&((_sceMpegDecoder *)0)->pict == 0x1F8 ? 1 : -1];
typedef char _check_5a0[(unsigned int)&((_sceMpegDecoder *)0)->spr == 0x5A0 ? 1 : -1];
typedef char _check_878[(unsigned int)&((_sceMpegDecoder *)0)->abort == 0x878 ? 1 : -1];
typedef char _check_128[(unsigned int)&((_sceMpegDecoder *)0)->x128 == 0x128 ? 1 : -1];

static char __ps2_libinfo__[16] = "PsIIlibmpeg 3100";

static inline int _dispatchCbNodata(sceMpeg *mp)
{
    sceMpegCbData cbdata;
    _sceMpegDecoder *d;
    sceMpegCallback func;
    void *gp;

    cbdata.type = 1;
    if (mp != 0 && (d = mp->sys) != 0 && (func = d->cb[1].func) != 0) {
        gp = SET_GP(d->cb[1].gp);
        func(mp, &cbdata, d->cb[cbdata.type].data);
        SET_GP(gp);
    }
    return 1;
}

static void _clearOnce(_sceMpegDecoder *d)
{
    REG(IPU_CTRL) = (REG(IPU_CTRL) & ~0x800000) | 0x800000;
    if (d->buffType == 0) {
        unsigned char *spr = (unsigned char *)0x70000000;

        d->x830 = (unsigned int)(spr + 0x3600);
        d->spr[0].sadr = (unsigned int)spr;
        d->spr[0].buf = (unsigned int)(spr + 0x1800);
        d->spr[1].sadr = (unsigned int)(spr + 0x1B00);
        d->spr[1].buf = (unsigned int)(spr + 0x3300);
        d->x820 = 0;
    } else if (d->buffType == 1) {
        d->x830 = (unsigned int)d->xE80;
        d->spr[0].buf = ((unsigned int)d->bstag & 0x0FFFFFFF) | 0x30000000;
        d->spr[0].sadr = 0;
        d->spr[1].buf = ((unsigned int)d->idct & 0x0FFFFFFF) | 0x30000000;
        d->spr[1].sadr = 0;
        d->x820 = 0;
    } else {
        _sceMpegError(d, "Unknown buff type.");
    }
}

static inline int _checkAlignment(_sceMpegDecoder *d)
{
    int a;

    if ((a = (unsigned int)d->bstag & 0x3F) != 0) {
        _sceMpegError1(d, "internal alignment error. _bstag:%d", a);
        return 0;
    }
    if ((a = (unsigned int)d->idct & 0x3F) != 0) {
        _sceMpegError1(d, "internal alignment error. _idct:%d", a);
        return 0;
    }
    return 1;
}

int sceMpegCreate(sceMpeg *mp, unsigned char *work_area, int work_area_size)
{
    _sceMpegDecoder *d;
    int size;

    memset(work_area, 0, work_area_size);
    d = (_sceMpegDecoder *)((((unsigned int)work_area + 3) >> 2) << 2);
    size = work_area_size - ((unsigned char *)d - work_area);
    if (size < (int)sizeof(_sceMpegDecoder)) {
        _sceMpegError(d, "The size of work area is too small");
        return 0;
    }
    mp->sys = d;
    _sceMpegAlalcInit(&d->alalc, (unsigned char *)d + sizeof(_sceMpegDecoder), size - sizeof(_sceMpegDecoder));
    mp->width = 0;
    mp->height = 0;
    mp->frameCount = 0;
    mp->pts = -1;
    mp->dts = -1;
    mp->flags = 0;
    mp->pts2nd = -1;
    mp->dts2nd = -1;
    mp->flags2nd = 0;
    d->xC8 = 0;
    d->xCC = 0;
    d->xD0 = 0;
    d->xD4 = 0;
    d->xD8 = 0;
    d->xDC = 0;
    d->xE0 = 0;
    d->xE4 = 0;
    d->xE8 = 0;
    d->xEC = 0;
    d->xF0 = 0;
    d->xF4 = 0;
    d->xF8 = 0;
    d->xFC = 0;
    d->x100 = -1;
    d->x108 = 0;
    d->cb[0].func = 0;
    d->cb[1].func = 0;
    d->cb[2].func = _sceMpegDefaultStopDMA;
    d->cb[3].func = _sceMpegDefaultRestartDMA;
    d->cb[4].func = 0;
    d->cb[5].func = 0;
    d->cb[6].func = 0;
    d->x60 = _sceMpegAlalcAlloc(d, &d->alalc, 0x800, 8);
    d->x64 = 0;
    d->x10C = 0;
    d->x110 = 0;
    d->x114 = 0;
    d->x8C = 0;
    d->x90 = 0;
    d->x98 = -1;
    d->xA0 = 0;
    d->x9C = 0;
    d->xC0 = 0;
    d->xA8 = -1;
    d->xAC = -1;
    d->xB0 = -1;
    d->mp = mp;
    d->xC4 = 1;
    _clearOnce(d);
    sceMpegReset(mp);
    sceMpegClearRefBuff(mp);
    d->frame[0] = &d->pict[0];
    d->frame[1] = &d->pict[1];
    d->frame[3] = &d->pict[2];
    d->top[0] = &d->pict[3];
    d->top[1] = &d->pict[4];
    d->top[3] = &d->pict[5];
    d->bottom[0] = &d->pict[6];
    d->bottom[1] = &d->pict[7];
    d->bottom[3] = &d->pict[8];
    _sceMpegAlalcSetDynamic(&d->alalc);
    d->x860 = -1;
    d->x85C = 0;
    d->x864 = 0;
    return _checkAlignment(d);
}

static void _sendDataToIPU(_sceMpegDecoder *d, unsigned char *data, int size)
{
    unsigned long *tag = (unsigned long *)((((unsigned int)d->dmatag + 15) >> 4) << 4);
    unsigned long *p = tag;
    int n;
    unsigned int addr;
    int qwc;
    int id;
    int old;

    while (size > 0) {
        n = (size > 0xFFF40) ? 0xFFF40 : size;
        addr = (unsigned int)data & 0x0FFFFFFF;
        size -= n;
        id = size ? 3 : 0;
        qwc = (n + 15) / 16;
        *p = ((unsigned long)addr << 32) | ((unsigned long)id << 28) | (unsigned int)qwc;
        __asm__ volatile("sync");
        data += n;
        p += 2;
    }
    FlushCache(0);
    old = DIntr();
    REG(D4_TADR) = (unsigned int)tag & 0x0FFFFFFF;
    REG(D4_QWC) = 0;
    REG(D4_CHCR) = 0x105;
    if (old) {
        EIntr();
    }
}

static void _clearEach(_sceMpegDecoder *d)
{
    int old;

    d->x828 = 1;
    d->x1C0 = 0;
    old = DIntr();
    REG(D_ENABLEW) = REG(D_ENABLER) | 0x10000;
    REG(D3_CHCR) = 0;
    REG(D4_CHCR) = 0;
    REG(D9_CHCR) = 0;
    REG(D_ENABLEW) = REG(D_ENABLER) & ~0x10000;
    if (old) {
        EIntr();
    }
    REG(D3_QWC) = 0;
    REG(D4_QWC) = 0;
    REG(D9_QWC) = 0;
    REG(IPU_CTRL) = 0x40000000;
    sceIpuSync(0, 0);
}

static int _getpic(sceMpeg *mp)
{
    _sceMpegDecoder *d = mp->sys;
    int ret = 0;
    int hdr = 1;

    d->isEnd = 0;
    if (d->xEC & 0x3F) {
        _sceMpegError1(d, "image buffer needs to be aligned to 64byte boundary(0x%08x)", d->xEC);
        return -1;
    }
    if (d->abort) {
        scePrintf("Need to re-setup libipu since sceMpegGetPicture was aborted\n");
        return -1;
    }
    d->x834 = 0;
    do {
        if (ret != -1) {
            do {
                if ((hdr = _sceMpegNextHeader(d)) < 0) {
                    return -1;
                }
            } while (hdr != 0 && d->x184 != d->xE8 && d->x858 != 0);
        }
        switch (hdr) {
        case 0:
            _sceMpegFlush(mp);
            d->isEnd = 1;
            break;
        case 1:
            d->xBC = 0;
            d->xB8 = 0;
            d->xB4 = 0;
            ret = _decodeOrSkip(mp, d->xB4, d->xA8);
            d->xB4++;
            break;
        case 2:
            ret = _decodeOrSkip(mp, d->xB8, d->xAC);
            d->xB8++;
            break;
        case 3:
        case 4:
            ret = _decodeOrSkip(mp, d->xBC, d->xB0);
            d->xBC++;
            break;
        }
        if (d->abort) {
            scePrintf("sceMpegGetPicture is aborted\n");
            return -1;
        }
        if (d->x834) {
            return 1;
        }
    } while (!d->isEnd);
    return 1;
}

static int _updateRefImage(_sceMpegDecoder *d, int second)
{
    _sceMpegPicture *p = 0;
    _sceMpegPicture *q;
    _sceMpegPicture *tmp;
    int ret = 0;
    int n = (d->x184 == 3) ? 2 : 4;

    if (d->x160 == 3) {
        d->frame[2] = d->frame[3];
        d->top[2] = d->top[3];
        d->bottom[2] = d->bottom[3];
        if (d->xB4 + d->xB8 >= n) {
            d->xFC = 0;
            d->x1B8 = 0;
            d->x1B4 = 0;
        }
        if ((d->xFC || d->x1B8) && !d->x1B4) {
            d->frame[0]->x28 = 0;
            d->top[0]->x28 = 0;
            d->bottom[0]->x28 = 0;
        }
        d->xFC = 0;
        d->x1B8 = 0;
        if (d->x184 == 3) {
            if (d->frame[0]->x28 == 1 || d->x1B4) {
                if (d->frame[1]->x28 == 1) {
                    ret = 1;
                }
            }
        } else {
            if ((d->top[0]->x28 == 1 && d->bottom[0]->x28 == 1) || d->x1B4) {
                if (d->top[1]->x28 == 1 && d->bottom[1]->x28 == 1) {
                    ret = 1;
                }
            }
        }
    } else {
        if (!second) {
            tmp = d->frame[0];
            d->frame[0] = d->frame[1];
            d->frame[1] = tmp;
            tmp = d->top[0];
            d->top[0] = d->top[1];
            d->top[1] = tmp;
            tmp = d->bottom[0];
            d->bottom[0] = d->bottom[1];
            d->bottom[1] = tmp;
        }
        d->frame[2] = d->frame[1];
        d->top[2] = d->top[1];
        d->bottom[2] = d->bottom[1];
        if (d->x184 == 3) {
            if (d->x160 != 2 || d->frame[0]->x28 == 1) {
                ret = 1;
            }
        } else {
            q = (d->x184 == 1) ? d->bottom[1] : d->top[1];
            if (d->x160 != 2 || (second && q->x28 == 1) || (d->top[0]->x28 == 1 && d->bottom[0]->x28 == 1)) {
                ret = 1;
            }
        }
    }
    switch (d->x184) {
    case 3:
        p = d->frame[2];
        break;
    case 1:
        p = d->top[2];
        break;
    case 2:
        p = d->bottom[2];
        break;
    }
    p->x28 = 0;
    p->x18 = d->x838;
    p->x2C = d->x160;
    p->x20 = d->x840;
    p->x30 = d->x184;
    p->x34 = d->x14C;
    p->x38 = d->x198;
    p->x3C = d->x188;
    p->x40 = d->x194;
    p->x44 = d->hoff[0];
    p->x48 = d->hoff[1];
    p->x4C = d->hoff[2];
    p->x50 = d->voff[0];
    p->x54 = d->voff[1];
    p->x58 = d->voff[2];
    p->x5C = d->x158;
    p->x60 = d->x15C;
    return ret;
}

static int _decodeOrSkipFrame(sceMpeg *mp, int count, int mode)
{
    _sceMpegDecoder *d = mp->sys;
    int skipped = 0;
    int ret;

    if (mode == -1 || count < mode) {
        if (d->x8 == 0) {
            mp->frameCount = 0;
            d->x8 = 1;
        }
        {
            int upd = _updateRefImage(d, 0);
            int dec = 0;

            if (upd) {
                dec = _decPicture(d) != 0;
            }
            ret = dec;
        }
    } else {
        ret = _updateRefImage(d, 0);
        skipped = _dispatchCbNodata(mp);
    }
    if (d->abort) {
        return 0;
    }
    _sceMpegOutputFrame(d, d->x128, d->x4);
    if (d->x184 != 3 && !skipped) {
        d->x130 = !d->x130;
    }
    mp->frameCount = d->x128 - d->xC0;
    if (!d->x130) {
        d->x128++;
        d->x4++;
    }
    return ret;
}

static int _decodeOrSkipField(sceMpeg *mp, int count, int mode)
{
    _sceMpegDecoder *d = mp->sys;
    int decode = 0;
    int ret;
    int upd;
    int hdr;

    d->x130 = 0;
    if (mode == -1 || count < mode) {
        decode = 1;
    }
    if (d->x8 == 0) {
        mp->frameCount = 0;
        d->x8 = 1;
    }
    if (_updateRefImage(d, 0) && decode) {
        _decPicture(d);
    }
    if (d->abort) {
        return 0;
    }
    d->x130 = 1;
    if ((hdr = _sceMpegNextHeader(d)) == 0) {
        _sceMpegFlush(mp);
        d->isEnd = 1;
        return 0;
    }
    upd = 2;
    if (d->xE8 != 1) {
        upd = 1;
    }
    if (d->x184 != upd) {
        return -1;
    }
    if ((hdr = _updateRefImage(d, 1)) != 0) {
        hdr = 1;
    }
    ret = 0;
    if (hdr && decode) {
        if (_decPicture(d)) {
            ret = 1;
        }
    }
    if (d->abort) {
        return 0;
    }
    _sceMpegOutputFrame(d, d->x128, d->x4);
    d->x130 = 0;
    mp->frameCount = d->x128 - d->xC0;
    d->x128++;
    d->x4++;
    if (!decode) {
        _dispatchCbNodata(mp);
    }
    return ret;
}

int sceMpegInit(void)
{
    int old = DIntr();

    REG(D_ENABLEW) = REG(D_ENABLER) | 0x10000;
    REG(D3_CHCR) &= ~0x100;
    REG(D4_CHCR) &= ~0x100;
    REG(D_ENABLEW) = REG(D_ENABLER) & ~0x10000;
    if (old) {
        EIntr();
    }
    REG(D3_QWC) = 0;
    REG(D4_QWC) = 0; sceIpuInit();
    return 1;
}

int sceMpegDelete(sceMpeg *mp)
{
    return 1;
}

int sceMpegAddBs(sceMpeg *mp, unsigned char *bs, int size)
{
    _sendDataToIPU(mp->sys, bs, (size + 19) / 16 * 16);
    return 1;
}

int sceMpegGetPicture(sceMpeg *mp, void *rgb32, int mbcount)
{
    _sceMpegDecoder *d = mp->sys;

    d->xC4 = 1;
    d->xEC = ((unsigned int)rgb32 & 0x0FFFFFFF) | 0x20000000;
    d->xF8 = mbcount;
    d->xF4 = 0;
    d->xF0 = 0;
    return _getpic(mp);
}

int sceMpegGetPictureRAW8(sceMpeg *mp, void *raw8, int mbcount)
{
    _sceMpegDecoder *d = mp->sys;

    d->xC4 = 0;
    d->xEC = ((unsigned int)raw8 & 0x0FFFFFFF) | 0x20000000;
    d->xF8 = mbcount;
    d->xF4 = 0;
    d->xF0 = 0;
    return _getpic(mp);
}

void sceMpegGetPictureAbort(sceMpeg *mp)
{
    mp->sys->abort = 1;
}

int sceMpegReset(sceMpeg *mp)
{
    _sceMpegDecoder *d = mp->sys;

    d->abort = 0;
    d->isEnd = 0;
    d->x4 = 0;
    d->x8 = 0;
    mp->frameCount = 0;
    d->xC0 = 0;
    d->x98 = -1;
    _clearEach(d);
    d->x128 = 0;
    d->x858 = 0;
    REG(IPU_CTRL) = (REG(IPU_CTRL) & ~0x800000) | 0x800000;
    return 1;
}

int sceMpegIsEnd(sceMpeg *mp)
{
    return mp->sys->isEnd;
}

int sceMpegIsRefBuffEmpty(sceMpeg *mp)
{
    return !mp->sys->x4;
}

void sceMpegSetDecodeMode(sceMpeg *mp, int ni, int np, int nb)
{
    _sceMpegDecoder *d = mp->sys;

    d->xA8 = ni;
    d->xAC = np;
    d->xB0 = nb;
}

void sceMpegGetDecodeMode(sceMpeg *mp, int *ni, int *np, int *nb)
{
    _sceMpegDecoder *d = mp->sys;

    *ni = d->xA8;
    *np = d->xAC;
    *nb = d->xB0;
}

sceMpegCallback sceMpegAddCallback(sceMpeg *mp, int type, sceMpegCallback func, void *anyData)
{
    _sceMpegDecoder *d = mp->sys;
    sceMpegCallback old = d->cb[type].func;
    void *gp;

    d->cb[type].func = func;
    d->cb[type].data = anyData;
    __asm__ volatile("move %0,$gp" : "=r"(gp));
    d->cb[type].gp = gp;
    return old;
}

int sceMpegSetBuffType(sceMpeg *mp, unsigned int type)
{
    _sceMpegDecoder *d = mp->sys;

    if (type >= 2) {
        return -1;
    }
    d->buffType = type;
    _clearOnce(d);
    return 1;
}

int sceMpegGetBuffType(sceMpeg *mp)
{
    return mp->sys->buffType;
}

int _sceMpegDispatchMpegCallback(sceMpeg *mp, sceMpegCbData *cbdata)
{
    _sceMpegDecoder *d;
    sceMpegCallback func;
    void *gp;
    int ret = 0;

    if (mp != 0 && (d = mp->sys) != 0 && (func = d->cb[cbdata->type].func) != 0) {
        gp = SET_GP(d->cb[cbdata->type].gp);
        ret = func(mp, cbdata, d->cb[cbdata->type].data);
        SET_GP(gp);
    }
    return ret;
}

int _sceMpegDispatchMpegCbNodata(sceMpeg *mp)
{
    return _dispatchCbNodata(mp);
}

static int _decodeOrSkip(sceMpeg *mp, int count, int mode)
{
    if (mp->sys->x184 != 3) {
        return _decodeOrSkipField(mp, count, mode);
    }
    return _decodeOrSkipFrame(mp, count, mode);
}

int sceMpegClearRefBuff(sceMpeg *mp)
{
    _sceMpegDecoder *d = mp->sys;

    if (d->frame[0]) {
        d->frame[0]->x28 = 0;
    }
    if (d->top[0]) {
        d->top[0]->x28 = 0;
    }
    if (d->bottom[0]) {
        d->bottom[0]->x28 = 0;
    }
    if (d->frame[1]) {
        d->frame[1]->x28 = 0;
    }
    if (d->top[1]) {
        d->top[1]->x28 = 0;
    }
    if (d->bottom[1]) {
        d->bottom[1]->x28 = 0;
    }
    return 1;
}

int sceMpegGetPictureRAW8xy(sceMpeg *mp, void *raw8, int w, int h)
{
    _sceMpegDecoder *d = mp->sys;

    d->xC4 = 0;
    d->xEC = ((unsigned int)raw8 & 0x0FFFFFFF) | 0x20000000;
    d->xF8 = w * h;
    d->xF0 = w << 4;
    d->xF4 = h << 4;
    return _getpic(mp);
}

int sceMpegSetDefaultPtsGap(sceMpeg *mp, long gap)
{
    _sceMpegDecoder *d = mp->sys;

    d->x8C = 1;
    d->x90 = gap;
    return 1;
}

int sceMpegResetDefaultPtsGap(sceMpeg *mp)
{
    _sceMpegDecoder *d = mp->sys;

    d->x8C = 0;
    d->x90 = 0;
    return 1;
}

int sceMpegSetImageBuff(sceMpeg *mp, void *buf)
{
    mp->sys->xEC = (unsigned int)buf;
    return 1;
}

int sceMpegDispWidth(sceMpeg *mp)
{
    return mp->sys->xE0;
}

int sceMpegDispHeight(sceMpeg *mp)
{
    return mp->sys->xE4;
}

int sceMpegDispCenterOffX(sceMpeg *mp)
{
    return (int)&mp->sys->xC8;
}

int sceMpegDispCenterOffY(sceMpeg *mp)
{
    return (int)&mp->sys->xC8;
}

int _sceMpegSetBrokenLink(sceMpeg *mp, int pic)
{
    _sceMpegDecoder *d = mp->sys;
    int old = d->xFC;

    d->xFC = pic;
    return old;
}

int _sceMpegSetPtm(sceMpeg *mp, long ptm)
{
    _sceMpegDecoder *d = mp->sys;

    d->x100 = ptm;
    d->x108 = 1;
    return 1;
}

static inline int _decPicture(_sceMpegDecoder *d)
{
    _sceMpegPicture *p;
    int ret;

    if (d->x184 == 3 && d->x130) {
        _sceMpegError(d, "odd number of field pictures");
        d->x130 = 0;
    }
    switch (d->x184) {
    case 3:
        p = d->frame[2];
        break;
    case 1:
        p = d->top[2];
        break;
    case 2:
        p = d->bottom[2];
        break;
    default:
        p = d->frame[2];
        _sceMpegError(d, "unknown picture sutructure");
        break;
    }
    if ((ret = _sceMpegPictureData0(d)) != 0) {
        p->x28 = 1;
    }
    return ret;
}

// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
