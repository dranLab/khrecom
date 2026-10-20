/* libgraph (Sony EE SDK): base address of the Z buffer after the frame buffer(s). */

typedef struct {
    short sceGsInterMode;
    short sceGsOutMode;
    short sceGsFFMode;
    short sceGsVersion;
    volatile int (*sceGsVSCfunc)(int);
    int sceGsVSCid;
} sceGsGParam;

sceGsGParam *sceGsGetGParam(void);

short sceGszbufaddr(short psm, short w, short h)
{
    sceGsGParam *gp = sceGsGetGParam();
    short fbw = (w + 63) / 64;
    short fbh;

    if (psm & 2) {
        fbh = (h + 63) / 64;
    } else {
        fbh = (h + 31) / 32;
    }
    if ((*(unsigned long *)gp & 0x0000ffff0000ffffUL) == 1) { /* interlace, field mode */
        return fbw * fbh;
    }
    return fbw * fbh * 2;
}
