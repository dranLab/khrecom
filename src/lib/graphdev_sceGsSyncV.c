/* libgraph (Sony EE SDK): wait for the next vertical blank. */

typedef struct {
    short sceGsInterMode;
    short sceGsOutMode;
    short sceGsFFMode;
    short sceGsVersion;
    volatile int (*sceGsVSCfunc)(int);
    int sceGsVSCid;
} sceGsGParam;

#define GS_CSR ((volatile unsigned long *)0x12001000)

sceGsGParam *sceGsGetGParam(void);
void VSync(void);
long VSync2(void);

int sceGsSyncV(int mode)
{
    sceGsGParam *p = sceGsGetGParam();
    long r;

    if (p->sceGsVSCfunc == 0) {
        VSync();
        if (p->sceGsInterMode == 1) {
            return (*GS_CSR >> 13) & 1;
        }
    } else {
        r = (VSync2() >> 13) & 1;
        if (p->sceGsInterMode == 1) {
            return r;
        }
    }
    return 1;
}
