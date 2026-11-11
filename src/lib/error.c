/* libmpeg error reporting: through the stream's error callback, or printed. */
#include "sdk/eekernel.h"
#include "sdk/libmpeg.h"

#define D_ENABLER ((volatile unsigned int *)0x1000F520)
#define D_ENABLEW ((volatile unsigned int *)0x1000F590)
#define D3_CHCR ((volatile unsigned int *)0x1000B000)
#define D3_QWC ((volatile unsigned int *)0x1000B020)
#define D4_CHCR ((volatile unsigned int *)0x1000B400)
#define D4_QWC ((volatile unsigned int *)0x1000B420)
#define IPU_CTRL ((volatile unsigned int *)0x10002010)

typedef struct {
    int type;
    char *errMessage;
} sceMpegCbDataError;

typedef struct {
    int type;
    long pts;
    long dts;
} sceMpegCbDataTimeStamp;

typedef union {
    int type;
    sceMpegCbDataError error;
    sceMpegCbDataTimeStamp ts;
} sceMpegCbData;

/* The decoder's internal state (sceMpeg.sys): only the fields used here. */
typedef struct {
    char pad0[0xC];
    void *cbError;
    char pad10[0x868 - 0x10];
    sceMpeg *mp;
} MpegSys;

void scePrintf(const char *fmt, ...);
int sceSnprintf(char *buf, int size, const char *fmt, ...);
int _sceMpegDispatchMpegCallback(sceMpeg *mp, void *cbdata);
void _sceMpegError(MpegSys *sys, char *msg);

static const char bdec_msg[] = "Error code detected(BDEC)";

static void _ErrMessage(char *msg)
{
    scePrintf("[MPEG ERROR]%s\n", msg);
}

void _sceMpegStopIpuDma(void)
{
    int oldei = DIntr();

    *D_ENABLEW = *D_ENABLER | 0x10000;
    *D3_CHCR = 0;
    *D4_CHCR = 0;
    *D_ENABLEW = *D_ENABLER & ~0x10000;
    if (oldei) {
        EIntr();
    }
    *D3_QWC = 0;
    *D4_QWC = 0;
    *IPU_CTRL = 0x40000000;
}

void _sceMpegErrorBdec(MpegSys *sys)
{
    sceMpegCbData cb;
    int oldei;

    _sceMpegError(sys, (char *)bdec_msg);
    cb.type = 2;
    _sceMpegDispatchMpegCallback(sys->mp, &cb);
    *IPU_CTRL = 0x40000000;
    cb.type = 3;
    _sceMpegDispatchMpegCallback(sys->mp, &cb);
    oldei = DIntr();
    *D_ENABLEW = *D_ENABLER | 0x10000;
    *D3_CHCR = 0;
    *D_ENABLEW = *D_ENABLER & ~0x10000;
    if (oldei) {
        EIntr();
    }
    *D3_QWC = 0;
}

void _sceMpegError(MpegSys *sys, char *msg)
{
    sceMpegCbDataError cb;

    if (sys->mp && sys && sys->cbError) {
        cb.errMessage = msg;
        cb.type = 0;
        _sceMpegDispatchMpegCallback(sys->mp, &cb);
    } else {
        _ErrMessage(msg);
    }
}

void _sceMpegError1(MpegSys *sys, const char *fmt, int arg)
{
    char buf[256];

    sceSnprintf(buf, sizeof(buf), fmt, arg);
    _sceMpegError(sys, buf);
}
// SOURCE DATA: .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
