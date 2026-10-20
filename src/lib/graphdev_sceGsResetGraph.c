/* libgraph (Sony EE SDK): reset the GS and set the video mode. */

typedef struct {
    short sceGsInterMode;
    short sceGsOutMode;
    short sceGsFFMode;
    short sceGsVersion;
    volatile int (*sceGsVSCfunc)(int);
    int sceGsVSCid;
} sceGsGParam;

#define GS_CSR ((volatile unsigned long *)0x12001000)

unsigned long GsPutIMR(unsigned long imr);
int DisableIntc(int cause);
int RemoveIntcHandler(int cause, int hid);
void SetGsCrt(short interlace, short omode, short ffmode);

static char __ps2_libinfo__[16] = "PsIIlibgraph3100";

sceGsGParam *sceGsGetGParam(void);

void sceGsResetGraph(short mode, short inter, short omode, short ffmode)
{
    sceGsGParam *gp;

    switch (mode) {
    case 0:
        gp = sceGsGetGParam();
        *GS_CSR = 0x200;
        gp->sceGsInterMode = inter;
        gp->sceGsOutMode = omode;
        gp->sceGsVersion = (unsigned short)((*GS_CSR >> 16) & 0xff);
        GsPutIMR(0xff00);
        gp->sceGsFFMode = ffmode != 0;
        if (gp->sceGsVSCfunc) {
            DisableIntc(2);
            RemoveIntcHandler(2, gp->sceGsVSCid);
            gp->sceGsVSCfunc = 0;
            gp->sceGsVSCid = 0;
        }
        SetGsCrt(inter & 1, omode & 0xff, ffmode & 1);
        break;
    case 1:
        *GS_CSR = 0x100;
        break;
    case 5:
        gp = sceGsGetGParam();
        gp->sceGsFFMode = ffmode != 0;
        gp->sceGsInterMode = inter;
        gp->sceGsOutMode = omode;
        gp->sceGsVersion = (unsigned short)((*GS_CSR >> 16) & 0xff);
        SetGsCrt(inter & 1, omode & 0xff, ffmode & 1);
        break;
    }
}

sceGsGParam *sceGsGetGParam(void)
{
    static sceGsGParam gp = { 1, 2, 1, 3, 0, 0 };

    return &gp;
}
// SOURCE DATA: .data
