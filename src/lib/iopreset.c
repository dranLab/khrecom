/* IOP reset / reboot: send the reset command (with the IOP's boot arguments) over SIF. */

int sceSifExecNotifyHandler(int sys, void *data);
int sceSifStopDma(void);
unsigned int sceSifGetReg(unsigned int reg);
int sceSifSetReg(unsigned int reg, unsigned int value);
void sceSifWriteBackDCache(void *ptr, int size);
void sceResetttyinit(void);
int scePrintf(const char *format, ...);
void sceSifInitRpc(unsigned int mode);
void sceSifExitRpc(void);

typedef struct {
    unsigned int data;
    unsigned int addr;
    unsigned int size;
    unsigned int mode;
} sceSifDmaData;

unsigned int _sceSifSetDma(sceSifDmaData *sdd, int len);

typedef struct {
    unsigned int psize : 8;
    unsigned int dsize : 24;
    unsigned int dest;
    unsigned int cid;
    unsigned int opt;
} sceSifCmdHdr;

struct _iop_reset_pkt {
    sceSifCmdHdr header;
    int arglen;
    int mode;
    char arg[80];
};

#define SIF_REG_SMFLAG 4
#define SIF_STAT_SIFINIT 0x10000
#define SIF_STAT_CMDINIT 0x20000
#define SIF_STAT_BOOTEND 0x40000
#define SIF_SYSREG_SUBADDR 0x80000000
#define SIF_SYSREG_MAINADDR 0x80000001
#define SIF_SYSREG_RPCINIT 0x80000002
#define SIF_CMD_RESET 0x80000003

static struct _iop_reset_pkt rdata __attribute__((aligned(64)));

int sceSifResetIop(const char *arg, int mode)
{
    sceSifDmaData dma;
    unsigned int addr;
    int i;

    sceSifExecNotifyHandler(0, 0);
    sceSifExecNotifyHandler(1, 0);
    sceSifStopDma();
    addr = sceSifGetReg(SIF_SYSREG_SUBADDR);
    rdata.mode = mode;
    for (i = 0; arg[i] != 0; i++) {
        rdata.arg[i] = arg[i];
    }
    rdata.header.dest = 0;
    rdata.arglen = i;
    rdata.header.cid = SIF_CMD_RESET;
    rdata.header.dsize = 0;
    rdata.header.psize = sizeof(rdata);
    dma.data = (unsigned int)&rdata;
    dma.addr = addr;
    dma.size = sizeof(rdata);
    dma.mode = 0x44;
    sceSifWriteBackDCache(&rdata, sizeof(rdata));
    sceSifSetReg(SIF_REG_SMFLAG, SIF_STAT_BOOTEND);
    if (_sceSifSetDma(&dma, 1)) {
        sceSifSetReg(SIF_REG_SMFLAG, SIF_STAT_SIFINIT);
        sceSifSetReg(SIF_REG_SMFLAG, SIF_STAT_CMDINIT);
        sceSifSetReg(SIF_SYSREG_RPCINIT, 0);
        sceSifSetReg(SIF_SYSREG_SUBADDR, 0);
        return 1;
    }
    return 0;
}

int sceSifIsAliveIop(void)
{
    if (sceSifGetReg(SIF_REG_SMFLAG) & SIF_STAT_SIFINIT) {
        return 1;
    }
    return 0;
}

int sceSifSyncIop(void)
{
    if (sceSifGetReg(SIF_REG_SMFLAG) & SIF_STAT_BOOTEND) {
        sceResetttyinit();
        sceSifExecNotifyHandler(1, (void *)1);
        sceSifExecNotifyHandler(0, (void *)1);
        return 1;
    }
    return 0;
}

#define UDNL "rom0:UDNL "

/* Reboot the IOP with the image in arg (e.g. "cdrom0:\\IOPRP.IMG"). */
int sceSifRebootIop(const char *arg)
{
    char param[80];
    const char *s = UDNL;
    const char *q;
    char *p;

    for (q = arg; *q; q++) {
    }
    if ((unsigned int)(q + sizeof(UDNL) - arg) > sizeof(param)) {
        scePrintf("too long parameter '%s'\n", arg);
        return 0;
    }
    sceSifInitRpc(0);
    sceSifExitRpc();
    p = param;
    while (*s) {
        *p++ = *s++;
    }
    while (*arg) {
        *p++ = *arg++;
    }
    *p = 0;
    return sceSifResetIop(param, 0);
}
// SOURCE DATA: .rodata .bss
