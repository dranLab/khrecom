#include "include_asm.h"

/* libgraph (Sony EE SDK): default display environment; newer consoles (ROM after 2001-06-08) report per-mode
   display offsets through a syscall. */

typedef struct {
    short sceGsInterMode;
    short sceGsOutMode;
    short sceGsFFMode;
    short sceGsVersion;
    volatile int (*sceGsVSCfunc)(int);
    int sceGsVSCid;
} sceGsGParam;

typedef struct {
    unsigned long pmode;
    unsigned long smode2;
    unsigned long dispfb;
    unsigned long display;
    unsigned long bgcolor;
} sceGsDispEnv;

/* MATCH: the OR order the original's DISPLAY values are combined in (sceGsSetDefDispEnv's tree
   ((MAGH | DW) | (DX | DH)) | DY comes out of this order only; Sony's libgraph.h order gives another). */
#define SCE_GS_SET_DISPLAY(dx, dy, magh, magv, dw, dh)                                                     \
    (((unsigned long)(magh) << 23) | ((unsigned long)(magv) << 27) | ((unsigned long)(dh) << 44) | (unsigned long)(dx) \
     | ((unsigned long)(dy) << 12) | ((unsigned long)(dw) << 32))

#define SCE_GS_NTSC 0x02
#define SCE_GS_PAL 0x03
#define SCE_GS_DTV480P 0x50

int sceOpen(const char *filename, int flag, ...);
int sceClose(int fd);
int sceRead(int fd, void *buf, int nbyte);
int atoi(const char *s);
void scePrintf(const char *fmt, ...);
sceGsGParam *sceGsGetGParam(void);
void _GetGsDxDyOffset(int mode, int *dx1, int *dy1, int *dx2, int *dy2);

static int checkModelVersion(void)
{
    char buf[256];
    char *p;
    unsigned int i;
    int fd;

    if ((fd = sceOpen("rom0:ROMVER", 1)) < 0) {
        return -1;
    }
    for (i = 0, p = buf; i < 256; i++) {
        sceRead(fd, p, 1);
        if (*p++ == 0) {
            break;
        }
    }
    sceClose(fd);
    return atoi(&buf[i - 9]) > 20010608;
}

/* Syscall 0x80 (GetGsDxDyOffset). */
asm(".text\n"
    "\t.align 3\n"
    "\t.ent _GetGsDxDyOffset\n"
    "_GetGsDxDyOffset:\n"
    "\t.set noreorder\n"
    "\tli $3, 0x80\n"
    "\tsyscall\n"
    "\tjr $31\n"
    "\tnop\n"
    "\t.set reorder\n"
    "\t.end _GetGsDxDyOffset\n");

/* Parked: 25 of 228 lines (2026-11-09): only the two non-interlaced blocks differ (NTSC falls into PAL's tail): the original issues h - 1 before the off[0]/off[1] loads (prio 7 vs 10 in our sched2: as if the loads were not ready at t=2) and allocates DY v1, sx a1, DH a3, dx*magh a2 (ours a1, a0, a2, a3); interlaced, DTV480P, dispfb and size match. Levers that got here from 96: magh/sy/sx block-local per arm (no move after mflo, sx dies into DX), magh * w, the DISPLAY OR order M|V|H|X|Y|W (120 orders searched), psm before fbw in dispfb, DTV off[0] + 232 + dx * 2; tried: statement order of magh/sy/sx/dh in the non-interlaced blocks (all equal), a dh local */
#if 0
void sceGsSetDefDispEnv(sceGsDispEnv *disp, short psm, short w, short h, short dx, short dy)
{
    sceGsGParam *gp = sceGsGetGParam();
    int off[4];

    if (!(gp->sceGsOutMode == SCE_GS_NTSC || gp->sceGsOutMode == SCE_GS_PAL) && checkModelVersion()) {
        _GetGsDxDyOffset(gp->sceGsOutMode, &off[0], &off[1], &off[2], &off[3]);
    } else {
        off[3] = 0;
        off[2] = 0;
        off[1] = 0;
        off[0] = 0;
    }
    disp->pmode = 0x66;
    if (gp->sceGsInterMode != 0) {
        if (gp->sceGsFFMode) {
            disp->smode2 = 3;
        } else {
            disp->smode2 = 1;
        }
    } else {
        disp->smode2 = 2;
    }
    disp->dispfb = (unsigned long)(psm & 0xf) << 15 | (unsigned long)(((w + 63) >> 6) & 0x3f) << 9;
    if (gp->sceGsOutMode == SCE_GS_NTSC) {
        if (gp->sceGsInterMode == 1) {
            int magh = (w + 0x9ff) / w;
            int sy = (dy + off[1] + 0x32) & 0xfff;
            long sx = (long)off[0] + 0x27c;
            disp->display = SCE_GS_SET_DISPLAY((dx * magh + sx) & 0xfff, sy,
                                               magh - 1, 0, magh * w - 1, gp->sceGsFFMode ? h * 2 - 1 : h - 1);
        } else {
            int magh = (w + 0x9ff) / w;
            int sy = (dy + off[1] + 0x19) & 0xfff;
            long sx = (long)off[0] + 0x27c;
            disp->display = SCE_GS_SET_DISPLAY((dx * magh + sx) & 0xfff, sy,
                                               magh - 1, 0, magh * w - 1, h - 1);
        }
    } else if (gp->sceGsOutMode == SCE_GS_PAL) {
        if (gp->sceGsInterMode == 1) {
            int magh = (w + 0x9ff) / w;
            int sy = (dy + off[1] + 0x48) & 0xfff;
            long sx = (long)off[0] + 0x290;
            disp->display = SCE_GS_SET_DISPLAY((dx * magh + sx) & 0xfff, sy,
                                               magh - 1, 0, magh * w - 1, gp->sceGsFFMode ? h * 2 - 1 : h - 1);
        } else {
            int magh = (w + 0x9ff) / w;
            int sy = (dy + off[1] + 0x24) & 0xfff;
            long sx = (long)off[0] + 0x290;
            disp->display = SCE_GS_SET_DISPLAY((dx * magh + sx) & 0xfff, sy,
                                               magh - 1, 0, magh * w - 1, h - 1);
        }
    } else if (gp->sceGsOutMode == SCE_GS_DTV480P) {
        disp->display = SCE_GS_SET_DISPLAY(((720 - w) / 2 * 2 + (long)off[0] + 232 + dx * 2) & 0xfff,
                                           (dy + off[1] + 0x23) & 0xfff, 1, 0, w * 2 - 1, h - 1);
    } else {
        scePrintf("sceGsDefDispEnv:Not support displaymode for 0x%x!!\n", gp->sceGsOutMode);
    }
    disp->bgcolor = 0;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/graphdev_checkModelVersion", sceGsSetDefDispEnv);
#endif

/* Keeps sceGsSetDefDispEnv's string in .rodata while the function is assembly (never emitted). */
static inline void sceGsSetDefDispEnv_strings(void)
{
    scePrintf("sceGsDefDispEnv:Not support displaymode for 0x%x!!\n", 0);
}

// SOURCE DATA: .rodata
