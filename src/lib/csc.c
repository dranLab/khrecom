/* libmpeg (Sony EE SDK): colour space conversion of a decoded reference picture by the IPU (CSC command), with DMA
   channel 4 feeding macroblocks and channel 3 returning RGB; large pictures go in chunks of 1023 macroblocks driven by
   DMA interrupt handlers. */
#include "sdk/eekernel.h"
#include "sdk/libmpeg.h"

#define D_STAT ((volatile unsigned int *)0x1000E010)
#define D3_CHCR ((volatile unsigned int *)0x1000B000)
#define D3_MADR ((volatile unsigned int *)0x1000B010)
#define D3_QWC ((volatile unsigned int *)0x1000B020)
#define D4_CHCR ((volatile unsigned int *)0x1000B400)
#define D4_MADR ((volatile unsigned int *)0x1000B410)
#define D4_QWC ((volatile unsigned int *)0x1000B420)
#define IPU_CMD ((volatile unsigned int *)0x10002000)
#define IPU_CTRL ((volatile int *)0x10002010)

#define IPU_WAIT() \
    while (*IPU_CTRL < 0) { \
    }
#define ExitHandler() asm volatile("sync.l\n\tei")

#define MB_CHUNK 1023

typedef struct {
    unsigned int DIR : 1;
    unsigned int p0 : 1;
    unsigned int MOD : 2;
    unsigned int ASP : 2;
    unsigned int TTE : 1;
    unsigned int TIE : 1;
    unsigned int STR : 1;
    unsigned int p1 : 7;
    unsigned int TAG : 16;
} tD_CHCR;

typedef union {
    int type;
    struct {
        int type;
        unsigned char *header;
        unsigned char *data;
        unsigned int len;
        long pts;
        long dts;
    } str;
} sceMpegCbData;

/* The decoder's internal state (sceMpeg.sys): only the fields used here. */
typedef struct {
    char pad0[0xec];
    void *rgbBuf;
    char padF0[0x828 - 0xf0];
    int ipuBusy;
    unsigned int ipuCmd;
    char pad830[0x868 - 0x830];
    sceMpeg *mp;
} MpegSys;

typedef struct {
    void *mbBuf;
    int pad4[2];
    int width;
    int height;
} MpegRefImage;

typedef struct {
    volatile int count;
    int error;
    int mbnum;
    unsigned int addr;
    int nchunk;
} CscIntrData;

typedef struct {
    unsigned int qwc;
    unsigned int addr;
} Ch4IntrData;

int _sceMpegDispatchMpegCallback(sceMpeg *mp, void *cbdata);
void _sceMpegError(MpegSys *sys, char *msg);
int AddDmacHandler2(int cause, int (*handler)(int, void *, void *), int next, void *arg);
int RemoveDmacHandler(int cause, int hid);
int EnableDmac(int cause);
int DisableDmac(int cause);

static int _ch3dmaCSC(int ch, void *arg, void *addr);
static int _ch4dma(int ch, void *arg, void *addr);

static void _doCSC2(MpegSys *sys, void *rgb, int mbnum)
{
    sceMpegCbData cb;
    CscIntrData data;
    int hid;
    int oldei;

    data.nchunk = mbnum / MB_CHUNK + 1;
    data.addr = ((unsigned int)rgb + MB_CHUNK * 0x400) & 0x0fffffff;
    data.mbnum = mbnum;
    data.error = 0;
    data.count = 0;
    IPU_WAIT();
    hid = AddDmacHandler2(3, _ch3dmaCSC, 0, &data);
    *D_STAT = 8; EnableDmac(3); /* MATCH: one line: the assembler moves the store into the jal slot */
    do { /* MATCH: the interrupt-disabled DMA kick in a do-while(0) (a macro body?): its registers */
        oldei = DIntr();
        *D3_MADR = (unsigned int)rgb & 0x0fffffff;
        *D3_QWC = MB_CHUNK * 0x40;
        *D3_CHCR = 0x100;
        if (oldei) {
            EIntr();
        }
    } while (0);
    *IPU_CMD = 0x70000000 | MB_CHUNK; /* CSC */
    cb.type = 4;
    _sceMpegDispatchMpegCallback(sys->mp, &cb);
    while (data.count < data.nchunk) {
    }
    if (data.error) {
        _sceMpegError(sys, "CSC handler error\n");
    }
    IPU_WAIT();
    DisableDmac(3);
    RemoveDmacHandler(3, hid);
}

static inline void _sceMpegIpuCmd(MpegSys *sys, unsigned int cmd)
{
    *IPU_CMD = cmd;
    cmd &= 0xf0000000;
    sys->ipuCmd = cmd;
    if (cmd == 0x20000000 || cmd == 0x30000000 || cmd == 0x40000000) {
        sys->ipuBusy = 0;
    } else {
        sys->ipuBusy = 1;
    }
}

static inline void _doCSC1(MpegSys *sys, int mbnum)
{
    sceMpegCbData cb;
    void *rgb = sys->rgbBuf;
    unsigned int qwc = mbnum * 0x40; /* MATCH: in a local: rgb/qwc registers */
    int oldei;

    IPU_WAIT();
    oldei = DIntr();
    *D3_MADR = (unsigned int)rgb & 0x0fffffff;
    *D3_QWC = qwc;
    *D3_CHCR = 0x100;
    if (oldei) {
        EIntr();
    }
    _sceMpegIpuCmd(sys, mbnum | 0x70000000);
    cb.type = 4;
    _sceMpegDispatchMpegCallback(sys->mp, &cb);
    while ((*D3_CHCR >> 8) & 1) {
    }
    IPU_WAIT();
}

void _sceMpegCscStoreRefImage(MpegSys *sys, MpegRefImage *ref)
{
    sceMpegCbData cb;
    Ch4IntrData d4;
    int mbnum = ref->width * ref->height;
    int small;
    int hid;
    int oldei;

    cb.type = 2;
    _sceMpegDispatchMpegCallback(sys->mp, &cb);
    if (*IPU_CTRL & 0x4000) {
        *IPU_CTRL = 0x40000000;
    }
    small = mbnum < 1024;
    IPU_WAIT();
    *IPU_CMD = 0; /* BCLR */
    sys->ipuBusy = 1;
    sys->ipuCmd = 0;
    IPU_WAIT();
    FlushCache(0);
    d4.qwc = mbnum * 24;
    d4.addr = (unsigned int)ref->mbBuf & 0x0fffffff;
    if (d4.qwc > 0xffff) {
        hid = AddDmacHandler2(4, _ch4dma, 0, &d4);
        *D_STAT = 0x10; EnableDmac(4); /* MATCH: one line: the store goes in the jal slot */
        oldei = DIntr();
        *D4_MADR = d4.addr;
        *D4_QWC = 0xffff;
        *D4_CHCR = 0x101;
        if (oldei) {
            EIntr();
        }
        d4.addr = (d4.addr + 0xffff0) & 0x0fffffff;
        d4.qwc -= 0xffff;
        if (small) {
            _doCSC1(sys, mbnum);
        } else {
            _doCSC2(sys, sys->rgbBuf, mbnum);
        }
        DisableDmac(4);
        RemoveDmacHandler(4, hid);
    } else {
        oldei = DIntr();
        *D4_MADR = (unsigned int)ref->mbBuf & 0x0fffffff;
        *D4_QWC = d4.qwc;
        *D4_CHCR = 0x101; if (oldei) { /* MATCH: one line (delay slot fill) */
            EIntr();
        }
        d4.qwc = 0;
        if (small) {
            _doCSC1(sys, mbnum);
        } else {
            _doCSC2(sys, sys->rgbBuf, mbnum);
        }
    }
    cb.type = 3;
    _sceMpegDispatchMpegCallback(sys->mp, &cb);
}

static int _ch3dmaCSC(int ch, void *arg, void *addr)
{
    CscIntrData *data = arg;

    *D_STAT = 8;
    data->count++;
    if (*D3_QWC != 0 || (*D3_CHCR & 0x100)) {
        data->error = 1;
        ExitHandler();
        return 0;
    }
    if (data->count < data->nchunk - 1) {
        *D3_MADR = data->addr;
        *D3_QWC = MB_CHUNK * 0x40;
        *D3_CHCR = 0x100;
        *IPU_CMD = 0x70000000 | MB_CHUNK;
        data->addr = (data->addr + MB_CHUNK * 0x400) & 0x0fffffff;
    } else if (data->count == data->nchunk - 1) {
        data->mbnum -= data->count * MB_CHUNK;
        *D3_MADR = data->addr;
        *D3_QWC = data->mbnum << 6;
        *D3_CHCR = 0x100;
        *IPU_CMD = data->mbnum | 0x70000000;
    }
    ExitHandler();
    return 0;
}

static int _ch4dma(int ch, void *arg, void *addr)
{
    Ch4IntrData *d4 = arg;

    *D_STAT = 0x10;
    if (d4->qwc == 0) {
        ExitHandler();
        return 1;
    }
    if (d4->qwc > 0xffff) {
        *D4_MADR = d4->addr;
        *D4_QWC = 0xffff;
        *D4_CHCR = 0x101;
        d4->addr = (d4->addr + 0xffff0) & 0x0fffffff;
        d4->qwc -= 0xffff;
    } else {
        *D4_MADR = d4->addr;
        *D4_QWC = d4->qwc;
        *D4_CHCR = 0x101;
        d4->qwc = 0;
    }
    ExitHandler();
    return 0;
}
// SOURCE DATA: .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
