#include "sdk/libipu.h"
#include "sdk/libmpeg.h"

/* libmpeg's internal state (mp->sys), as far as this file uses it. */
typedef struct {
    char pad[0x68];
    sceIpuDmaEnv dmaEnv;
} _sceMpeg;

int _sceMpegDefaultStopDMA(sceMpeg *mp)
{
    sceIpuStopDMA(&((_sceMpeg *)mp->sys)->dmaEnv);
    return 1;
}

int _sceMpegDefaultRestartDMA(sceMpeg *mp)
{
    sceIpuRestartDMA(&((_sceMpeg *)mp->sys)->dmaEnv);
    return 1;
}
