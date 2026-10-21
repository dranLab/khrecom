/* libmc (Sony EE SDK): the memory card library, a SIF RPC client of mcserv.irx; the read fast path also
   registers an RPC server of its own (ECC-checked slices). Cross-reference: kh1's (older) libmc.c. */
#include "sdk/eekernel.h"
#include "sdk/sifrpc.h"

#define UNCACHED(p) ((unsigned int)(p) | 0x20000000)

typedef struct {
    int fd; /* 0x00 */
    int port;
    int slot;
    int size;
    int offset;
    unsigned int mode;
    void *buffer;
    void *rdbuf;
    unsigned char data[16]; /* 0x20 */
} McSifParams;

typedef struct {
    int port; /* 0x000 */
    int slot;
    int mode;
    int maxent;
    void *table;
    char name[0x3FF];
    char term; /* 0x413 */
} McStatus;

typedef struct {
    unsigned char head[0x20];
    char EntryName[0x20];
} McTblGetDir;

/* the read callbacks' data (filled by the IOP): bytes for unaligned buffer heads and tails */
typedef struct {
    int size1;
    int size2;
    unsigned char *dst1;
    unsigned char *dst2;
    unsigned char buf1[64];
    unsigned char buf2[64];
    unsigned char pad[0x30];
} McRdFix;

typedef struct {
    int x0;
    int ret;
    int slice;
    int total;
    int count;
    int stride;
} McSlice;

int scePrintf(const char *fmt, ...);
char *strncpy(char *dst, const char *src, unsigned int n);
unsigned int strlen(const char *s);
void *memcpy(void *dst, const void *src, unsigned int n);
int PollSema(int sema);
int SignalSema(int sema);
int StartThread(int thid, void *arg);
int TerminateThread(int thid);
int DeleteThread(int thid);
int DelayThread(unsigned int usec);
void FlushCache(int op);
void sceSifInitRpc(unsigned int mode);
void sceSifWriteBackDCache(void *ptr, int size);
void sceSifSetRpcQueue(sceSifQueueData *qd, int thid);
void sceSifRegisterRpc(sceSifServeData *sd, unsigned int command, sceSifRpcFunc func, void *buff,
                       sceSifRpcFunc cfunc, void *cbuff, sceSifQueueData *qd);
void sceSifRpcLoop(sceSifQueueData *qd);
sceSifServeData *sceSifRemoveRpc(sceSifServeData *sd, sceSifQueueData *qd);
sceSifQueueData *sceSifRemoveRpcQueue(sceSifQueueData *qd);
extern char _gp[];

int sceMcSync(int mode, int *cmd, int *result);
static int mceRegistSifFunc(void);
static void mceSifRpcEndFunc(void *data);
static void mceIntrReadFixAlign(void *data);
static void mceGetInfoApdx(void *data);
static void mceStorePwd(void *data);
static void *mceSifEntry(unsigned int fno, void *data, int size);

static char __ps2_libinfo__[16] = "PsIIlibmc   3100";
int mcRunCmdNo = 0;
int semaidRegFunc = -1;
int semaid_timerfunc = -1;
int mceSifThreadId = -1;
unsigned char eccTbl[256] = {
    0x00, 0x87, 0x96, 0x11, 0xA5, 0x22, 0x33, 0xB4, 0xB4, 0x33, 0x22, 0xA5, 0x11, 0x96, 0x87, 0x00,
    0xC3, 0x44, 0x55, 0xD2, 0x66, 0xE1, 0xF0, 0x77, 0x77, 0xF0, 0xE1, 0x66, 0xD2, 0x55, 0x44, 0xC3,
    0xD2, 0x55, 0x44, 0xC3, 0x77, 0xF0, 0xE1, 0x66, 0x66, 0xE1, 0xF0, 0x77, 0xC3, 0x44, 0x55, 0xD2,
    0x11, 0x96, 0x87, 0x00, 0xB4, 0x33, 0x22, 0xA5, 0xA5, 0x22, 0x33, 0xB4, 0x00, 0x87, 0x96, 0x11,
    0xE1, 0x66, 0x77, 0xF0, 0x44, 0xC3, 0xD2, 0x55, 0x55, 0xD2, 0xC3, 0x44, 0xF0, 0x77, 0x66, 0xE1,
    0x22, 0xA5, 0xB4, 0x33, 0x87, 0x00, 0x11, 0x96, 0x96, 0x11, 0x00, 0x87, 0x33, 0xB4, 0xA5, 0x22,
    0x33, 0xB4, 0xA5, 0x22, 0x96, 0x11, 0x00, 0x87, 0x87, 0x00, 0x11, 0x96, 0x22, 0xA5, 0xB4, 0x33,
    0xF0, 0x77, 0x66, 0xE1, 0x55, 0xD2, 0xC3, 0x44, 0x44, 0xC3, 0xD2, 0x55, 0xE1, 0x66, 0x77, 0xF0,
    0xF0, 0x77, 0x66, 0xE1, 0x55, 0xD2, 0xC3, 0x44, 0x44, 0xC3, 0xD2, 0x55, 0xE1, 0x66, 0x77, 0xF0,
    0x33, 0xB4, 0xA5, 0x22, 0x96, 0x11, 0x00, 0x87, 0x87, 0x00, 0x11, 0x96, 0x22, 0xA5, 0xB4, 0x33,
    0x22, 0xA5, 0xB4, 0x33, 0x87, 0x00, 0x11, 0x96, 0x96, 0x11, 0x00, 0x87, 0x33, 0xB4, 0xA5, 0x22,
    0xE1, 0x66, 0x77, 0xF0, 0x44, 0xC3, 0xD2, 0x55, 0x55, 0xD2, 0xC3, 0x44, 0xF0, 0x77, 0x66, 0xE1,
    0x11, 0x96, 0x87, 0x00, 0xB4, 0x33, 0x22, 0xA5, 0xA5, 0x22, 0x33, 0xB4, 0x00, 0x87, 0x96, 0x11,
    0xD2, 0x55, 0x44, 0xC3, 0x77, 0xF0, 0xE1, 0x66, 0x66, 0xE1, 0xF0, 0x77, 0xC3, 0x44, 0x55, 0xD2,
    0xC3, 0x44, 0x55, 0xD2, 0x66, 0xE1, 0xF0, 0x77, 0x77, 0xF0, 0xE1, 0x66, 0xD2, 0x55, 0x44, 0xC3,
    0x00, 0x87, 0x96, 0x11, 0xA5, 0x22, 0x33, 0xB4, 0xB4, 0x33, 0x22, 0xA5, 0x11, 0x96, 0x87, 0x00,
};

static sceSifClientData mcClientID;
static int *mcInfoType;
static int *mcInfoFree;
static int *mcInfoFormat;
static McTblGetDir mcFileTable __attribute__((aligned(64)));
static McSifParams mcSifParams;
static McStatus mcStatus;
static McRdFix mcRdFix __attribute__((aligned(64)));
static char mcPwd[0x1000];
static int mcRetVal[16] __attribute__((aligned(64)));
static sceSifQueueData mcQd;
static sceSifServeData mcSd;
static unsigned char mcRpcBuf[0x80] __attribute__((aligned(64)));
static unsigned char mcRecvBuf[0x1000] __attribute__((aligned(64)));
static unsigned char mcSliceBuf[0x1000];
static unsigned char *mcDstPtr;
static unsigned char mcThreadStack[0x800] __attribute__((aligned(16)));

int sceMcInit(void)
{
    struct SemaParam sp;
    int i = 0;
    int ret;

    if (semaidRegFunc < 0) {
        sp.initCount = 1;
        sp.option = (unsigned int)"sceMc_sema_regs";
        sp.maxCount = 1;
        if ((semaidRegFunc = CreateSema(&sp)) == -1) {
            return -101;
        }
        sp.maxCount = 1;
        sp.initCount = 0;
        sp.option = (unsigned int)"sceMc_sema_timer";
        if ((semaid_timerfunc = CreateSema(&sp)) == -1) {
            DeleteSema(semaidRegFunc);
            semaidRegFunc = -1;
            return -101;
        }
    }
    sceMcSync(0, 0, 0);
    WaitSema(semaidRegFunc);
    sceSifInitRpc(0);
    while (1) {
        if (sceSifBindRpc(&mcClientID, 0x80000400, 0) < 0) {
            scePrintf("bind error libmc \n");
            while (1)
                ;
        }
        if (mcClientID.serve != 0) {
            break;
        }
        DelayThread(10000);
        if (++i >= 0x200) {
            scePrintf("bind error libmc (retry max)\n");
            return -91;
        }
    }
    if ((mceSifThreadId = mceRegistSifFunc()) < 0) {
        DeleteSema(semaidRegFunc);
        semaidRegFunc = -1;
        DeleteSema(semaid_timerfunc);
        semaid_timerfunc = -1;
        return -101;
    }
    ret = sceSifCallRpc(&mcClientID, 0xFE, 0, &mcSifParams, sizeof(mcSifParams), mcRetVal, 12, 0, 0);
    if (ret < 0) {
        mcClientID.serve = 0;
        DeleteSema(semaidRegFunc);
        semaidRegFunc = -1;
        DeleteSema(semaid_timerfunc);
        semaid_timerfunc = -1;
        return -91;
    }
    if (mcRetVal[1] < 0x20A) {
        scePrintf("libmc: too old release of mcserv.irx\n");
        mcClientID.serve = 0;
        DeleteSema(semaidRegFunc);
        semaidRegFunc = -1;
        DeleteSema(semaid_timerfunc);
        semaid_timerfunc = -1;
        return -120;
    }
    if (mcRetVal[2] < 0x20E) {
        scePrintf("libmc: too old release of mcman.irx\n");
        mcClientID.serve = 0;
        DeleteSema(semaidRegFunc);
        semaidRegFunc = -1;
        DeleteSema(semaid_timerfunc);
        semaid_timerfunc = -1;
        return -121;
    }
    SignalSema(semaidRegFunc);
    return mcRetVal[0];
}

int sceMcEnd(void)
{
    if (semaidRegFunc >= 0) {
        sceMcSync(0, 0, 0);
        if (mceSifThreadId >= 0) {
            TerminateThread(mceSifThreadId);
            DeleteThread(mceSifThreadId);
            mceSifThreadId = -1;
            sceSifRemoveRpc(&mcSd, &mcQd);
            sceSifRemoveRpcQueue(&mcQd);
        }
        DeleteSema(semaidRegFunc);
        semaidRegFunc = -1;
        DeleteSema(semaid_timerfunc);
    }
    return 1;
}

static void mceSifRpcEndFunc(void *data)
{
    iSignalSema(semaid_timerfunc);
}

int sceMcChangeThreadPriority(int level)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    mcSifParams.mode = level;
    if (sceSifCallRpc(&mcClientID, 0x14, 1, &mcSifParams, sizeof(mcSifParams), mcRetVal, 4, mceSifRpcEndFunc, 0)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 0x14;
    return 0;
}

int sceMcGetSlotMax(int port)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    mcSifParams.port = port;
    if (sceSifCallRpc(&mcClientID, 0x15, 0, &mcSifParams, sizeof(mcSifParams), mcRetVal, 4, 0, 0)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    SignalSema(semaidRegFunc);
    return mcRetVal[0];
}

int sceMcOpen(int port, int slot, const char *name, int mode)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    if (name == 0 || *name == 0) {
        SignalSema(semaidRegFunc);
        return -210;
    }
    strncpy(mcStatus.name, name, sizeof(mcStatus.name));
    mcStatus.port = port;
    mcStatus.mode = mode;
    mcStatus.slot = slot;
    mcStatus.term = 0;
    if (sceSifCallRpc(&mcClientID, 2, 1, &mcStatus, sizeof(mcStatus), mcRetVal, 4, mceSifRpcEndFunc, 0)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 2;
    return 0;
}

int sceMcMkdir(int port, int slot, const char *name)
{
    int ret = sceMcOpen(port, slot, name, 0x40);

    if (ret == 0) {
        mcRunCmdNo = 0xB;
    }
    return ret;
}

int sceMcClose(int fd)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    mcSifParams.fd = fd;
    if (sceSifCallRpc(&mcClientID, 3, 1, &mcSifParams, sizeof(mcSifParams), mcRetVal, 4, mceSifRpcEndFunc, 0)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 3;
    return 0;
}

int sceMcSeek(int fd, int offset, int mode)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    mcSifParams.fd = fd;
    mcSifParams.offset = offset;
    mcSifParams.mode = mode;
    if (sceSifCallRpc(&mcClientID, 4, 1, &mcSifParams, sizeof(mcSifParams), mcRetVal, 4, mceSifRpcEndFunc, 0)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 4;
    return 0;
}

static void mceIntrReadFixAlign(void *data)
{
    McRdFix *p = (McRdFix *)UNCACHED(data);
    unsigned char *dst;
    unsigned char *udst;
    int i;

    if (p->size1) {
        dst = p->dst1;
        udst = (unsigned char *)UNCACHED(dst);
        for (i = 0; i < p->size1; i++) {
            *udst++ = *dst++ = p->buf1[i];
        }
    }
    if (p->size2) {
        dst = p->dst2;
        udst = (unsigned char *)UNCACHED(dst);
        for (i = 0; i < p->size2; i++) {
            *udst++ = *dst++ = p->buf2[i];
        }
    }
    iSignalSema(semaid_timerfunc);
}

int sceMcRead(int fd, void *buff, int size)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    mcSifParams.fd = fd;
    mcSifParams.buffer = buff;
    mcSifParams.size = size;
    mcSifParams.rdbuf = &mcRdFix;
    sceSifWriteBackDCache(buff, size);
    sceSifWriteBackDCache(&mcRdFix, sizeof(mcRdFix));
    if (sceSifCallRpc(&mcClientID, 5, 1, &mcSifParams, sizeof(mcSifParams), mcRetVal, 4, mceIntrReadFixAlign,
                      &mcRdFix)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 5;
    return 0;
}

int sceMcWrite(int fd, const void *buff, int size)
{
    unsigned int i;
    int n;

    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    mcSifParams.fd = fd;
    if (size < 17) {
        mcSifParams.mode = size;
        mcSifParams.size = 0;
        mcSifParams.buffer = 0;
    } else {
        n = ((((unsigned int)buff - 1) & ~0xF) + 0x10) - (unsigned int)buff;
        mcSifParams.size = size - n;
        mcSifParams.mode = n;
        mcSifParams.buffer = (char *)buff + n;
    }
    for (i = 0; i < mcSifParams.mode; i++) {
        mcSifParams.data[i] = ((const unsigned char *)buff)[i];
    }
    FlushCache(0);
    if (sceSifCallRpc(&mcClientID, 6, 1, &mcSifParams, sizeof(mcSifParams), mcRetVal, 4, mceSifRpcEndFunc, 0)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 6;
    return 0;
}

int sceMcSync(int mode, int *cmd, int *result)
{
    int busy;

    if (mcRunCmdNo == 0) {
        return -1;
    }
    busy = 0;
    if (PollSema(semaid_timerfunc) < 0) {
        busy = 1;
    }
    if (mode == 0 && busy) {
        WaitSema(semaid_timerfunc);
        busy = 0;
    }
    busy = !busy;
    if (cmd) {
        *cmd = mcRunCmdNo;
    }
    if (busy) {
        mcRunCmdNo = 0;
        if (result) {
            *result = mcRetVal[0];
        }
        SignalSema(semaidRegFunc);
    }
    return busy;
}

static void mceGetInfoApdx(void *data)
{
    int *p = (int *)UNCACHED(data);

    if (mcInfoType) {
        *mcInfoType = p[0];
    }
    if (mcInfoFree) {
        *mcInfoFree = p[1];
    }
    if (mcInfoFormat) {
        *mcInfoFormat = p[0x24];
    }
    iSignalSema(semaid_timerfunc);
}

int sceMcGetInfo(int port, int slot, int *type, int *free, int *format)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    mcSifParams.port = port;
    mcSifParams.slot = slot;
    mcSifParams.rdbuf = &mcRdFix;
    if (type) {
        mcSifParams.mode = 1;
    } else {
        mcSifParams.mode = 0;
    }
    if (free) {
        mcSifParams.offset = 1;
    } else {
        mcSifParams.offset = 0;
    }
    if (format) {
        mcSifParams.size = 1;
    } else {
        mcSifParams.size = 0;
    }
    mcInfoType = type;
    mcInfoFree = free;
    mcInfoFormat = format;
    sceSifWriteBackDCache(&mcRdFix, sizeof(mcRdFix));
    if (sceSifCallRpc(&mcClientID, 1, 1, &mcSifParams, sizeof(mcSifParams), mcRetVal, 4, mceGetInfoApdx, &mcRdFix)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 1;
    return 0;
}

int sceMcUdCheckNewCard(void)
{
    if (sceSifCallRpc(&mcClientID, 0x35, 0, &mcSifParams, sizeof(mcSifParams), mcRetVal, 4, mceSifRpcEndFunc, 0)) {
        scePrintf("sceMcUdCheckNewCard RPC faild\n");
        return -91;
    }
    return mcRetVal[0];
}

int sceMcGetDir(int port, int slot, const char *name, unsigned int mode, int maxent, void *table)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    if (name == 0 || *name == 0) {
        SignalSema(semaidRegFunc);
        return -210;
    }
    mcStatus.port = port;
    mcStatus.slot = slot;
    mcStatus.mode = mode;
    mcStatus.maxent = maxent;
    mcStatus.table = table;
    strncpy(mcStatus.name, name, sizeof(mcStatus.name));
    mcStatus.term = 0;
    if (maxent >= 0) {
        sceSifWriteBackDCache(table, maxent << 6);
    }
    if (sceSifCallRpc(&mcClientID, 0xD, 1, &mcStatus, sizeof(mcStatus), mcRetVal, 4, mceSifRpcEndFunc, 0)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 0xD;
    return 0;
}

static void mceStorePwd(void *data)
{
    char *pwd = data;
    int len;

    if (pwd) {
        len = (strlen((char *)UNCACHED(mcPwd)) < 0x400) ? strlen((char *)UNCACHED(mcPwd)) : 0x3FF;
        memcpy(pwd, (char *)UNCACHED(mcPwd), len);
        pwd[len] = 0;
    }
    iSignalSema(semaid_timerfunc);
}

int sceMcChdir(int port, int slot, const char *path, char *pwd)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    if (path == 0 || *path == 0) {
        SignalSema(semaidRegFunc);
        return -210;
    }
    mcStatus.port = port;
    mcStatus.slot = slot;
    mcStatus.table = mcPwd;
    strncpy(mcStatus.name, path, sizeof(mcStatus.name));
    mcStatus.term = 0;
    sceSifWriteBackDCache(mcPwd, 0x400);
    if (sceSifCallRpc(&mcClientID, 0xC, 1, &mcStatus, sizeof(mcStatus), mcRetVal, 4, mceStorePwd, pwd)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 0xC;
    return 0;
}

int sceMcFormat(int port, int slot)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    mcSifParams.port = port;
    mcSifParams.slot = slot;
    if (sceSifCallRpc(&mcClientID, 0x10, 1, &mcSifParams, sizeof(mcSifParams), mcRetVal, 4, mceSifRpcEndFunc, 0)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 0x10;
    return 0;
}

int sceMcDelete(int port, int slot, const char *name)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    if (name == 0 || *name == 0) {
        SignalSema(semaidRegFunc);
        return -210;
    }
    strncpy(mcStatus.name, name, sizeof(mcStatus.name));
    mcStatus.port = port;
    mcStatus.slot = slot;
    mcStatus.term = 0;
    mcStatus.mode = 0;
    if (sceSifCallRpc(&mcClientID, 0xF, 1, &mcStatus, sizeof(mcStatus), mcRetVal, 4, mceSifRpcEndFunc, 0)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 0xF;
    return 0;
}

int sceMcFlush(int fd)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    mcSifParams.fd = fd;
    if (sceSifCallRpc(&mcClientID, 0xA, 1, &mcSifParams, sizeof(mcSifParams), mcRetVal, 4, mceSifRpcEndFunc, 0)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 0xA;
    return 0;
}

int sceMcSetFileInfo(int port, int slot, const char *name, const McTblGetDir *info, unsigned int valid)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    if (name == 0 || *name == 0) {
        SignalSema(semaidRegFunc);
        return -210;
    }
    valid &= 7;
    mcStatus.port = port;
    mcStatus.slot = slot;
    mcStatus.mode = valid;
    mcFileTable = *info;
    mcStatus.table = &mcFileTable;
    strncpy(mcStatus.name, name, sizeof(mcStatus.name));
    mcStatus.term = 0;
    FlushCache(0);
    if (sceSifCallRpc(&mcClientID, 0xE, 1, &mcStatus, sizeof(mcStatus), mcRetVal, 4, mceSifRpcEndFunc, 0)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 0xE;
    return 0;
}

int sceMcRename(int port, int slot, const char *org, const char *new)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    if (org == 0 || *org == 0 || new == 0) {
        SignalSema(semaidRegFunc);
        return -210;
    }
    mcStatus.port = port;
    mcStatus.slot = slot;
    mcStatus.mode = 0x10;
    strncpy(mcStatus.name, org, sizeof(mcStatus.name));
    mcStatus.term = 0;
    strncpy(mcFileTable.EntryName, new, sizeof(mcFileTable.EntryName));
    mcStatus.table = &mcFileTable;
    FlushCache(0);
    if (sceSifCallRpc(&mcClientID, 0xE, 1, &mcStatus, sizeof(mcStatus), mcRetVal, 4, mceSifRpcEndFunc, 0)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 0x13;
    return 0;
}

int sceMcUnformat(int port, int slot)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    mcSifParams.port = port;
    mcSifParams.slot = slot;
    if (sceSifCallRpc(&mcClientID, 0x11, 1, &mcSifParams, sizeof(mcSifParams), mcRetVal, 4, mceSifRpcEndFunc, 0)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 0x11;
    return 0;
}

int sceMcGetEntSpace(int port, int slot, const char *path)
{
    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    if (path == 0 || *path == 0) {
        SignalSema(semaidRegFunc);
        return -210;
    }
    mcStatus.port = port;
    mcStatus.slot = slot;
    strncpy(mcStatus.name, path, sizeof(mcStatus.name));
    mcStatus.term = 0;
    if (sceSifCallRpc(&mcClientID, 0x12, 1, &mcStatus, sizeof(mcStatus), mcRetVal, 4, mceSifRpcEndFunc, 0)) {
        SignalSema(semaidRegFunc);
        return -91;
    }
    mcRunCmdNo = 0x12;
    return 0;
}

static void regMain(void)
{
    sceSifSetRpcQueue(&mcQd, GetThreadId());
    sceSifRegisterRpc(&mcSd, 0x80000400, mceSifEntry, mcRpcBuf, 0, 0, &mcQd);
    sceSifRpcLoop(&mcQd);
}

static int mceRegistSifFunc(void)
{
    struct ThreadParam tp;
    int thid;

    tp.entry = (void (*)(void *))regMain;
    tp.stack = mcThreadStack;
    tp.stackSize = sizeof(mcThreadStack);
    tp.gpReg = _gp;
    tp.option = (unsigned int)"sceMc_read_fast_thread";
    tp.initPriority = 0;
    if ((thid = CreateThread(&tp)) < 0) {
        return -1;
    }
    StartThread(thid, 0);
    return thid;
}

static void mceEncEcc(unsigned char *data, unsigned char *ecc)
{
    int lp1 = 0;
    int lp0 = 0;
    int cp = 0;
    int i;
    unsigned char c;

    for (i = 0; i < 0x80; i++) {
        c = eccTbl[*data++];
        cp ^= c;
        if (c & 0x80) {
            lp0 ^= ~i;
            lp1 ^= i;
        }
    }
    ecc[0] = ~cp & 0x77;
    ecc[1] = ~lp0 & 0x7F;
    ecc[2] = ~lp1 & 0x7F;
}

static int mceDecEcc(unsigned char *data, unsigned char *ecc)
{
    unsigned char e[3];
    int cp;
    int c0;
    int c1;
    int c2;
    int lp;
    int i;
    int n;

    mceEncEcc(data, e);
    c0 = ecc[0] ^ e[0];
    c1 = ecc[1] ^ e[1];
    c2 = ecc[2] ^ e[2];
    cp = (c0 & 0xF) ^ (c0 >> 4);
    lp = c2 ^ c1;
    if (c2 == 0 && c1 == 0 && c0 == 0) {
        return 0;
    }
    if (lp == 0x7F && cp == 7) {
        data[c2] ^= 1 << (c0 >> 4);
        return -1;
    }
    n = 0;
    for (i = 0; i < 8; i++) {
        n += lp & 1;
        lp >>= 1;
    }
    for (i = 0; i < 4; i++) {
        n += cp & 1;
        cp >>= 1;
    }
    if (n == 1) {
        return -2;
    }
    return -3;
}

static int mceDecEccAll(int size, unsigned char *data, unsigned char *ecc)
{
    int n = size / 128;
    int i;
    int ret;

    for (i = 0; i < n; i++) {
        if ((ret = mceDecEcc(data, ecc)) < 0) {
            return ret;
        }
        data += 128;
        ecc += 3;
    }
    return 0;
}

static int mceExtractSliceData(McSlice *p)
{
    unsigned char *dst = mcSliceBuf;
    int total;
    int n;
    int i;
    int ret;

    for (i = 1; i < p->count; i++) {
        memcpy(dst, mcRecvBuf + 4 + (i << 8), p->stride);
        dst += p->stride;
    }
    total = p->total;
    dst = mcSliceBuf;
    while (total > 0) {
        if ((ret = mceDecEccAll(p->slice, dst, dst + p->slice)) != 0) {
            return ret;
        }
        n = p->slice;
        if (total < n) {
            n = total;
        }
        memcpy(mcDstPtr, dst, n);
        total -= n;
        memcpy((void *)UNCACHED(mcDstPtr), dst, n);
        mcDstPtr += n;
        dst += p->slice + p->slice / 32;
    }
    return 0;
}

static void *mceSifEntry(unsigned int fno, void *data, int size)
{
    McSlice *p = data;
    int ret = 0;

    sceSifWriteBackDCache(mcRecvBuf, sizeof(mcRecvBuf));
    switch (fno) {
    case 1:
        ret = mceExtractSliceData(p);
        break;
    case 2:
        memcpy(mcDstPtr, mcRecvBuf, p->total);
        memcpy((void *)UNCACHED(mcDstPtr), mcRecvBuf, p->total);
        mcDstPtr += p->total;
        break;
    }
    p->ret = ret;
    return data;
}

int sceMcReadFast(int fd, void *buff, int size)
{
    int ret;

    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    mcSifParams.fd = fd;
    mcSifParams.size = size;
    mcSifParams.buffer = mcRecvBuf;
    mcSifParams.rdbuf = &mcRdFix;
    mcDstPtr = buff;
    sceSifWriteBackDCache(mcRecvBuf, sizeof(mcRecvBuf));
    ret = sceSifCallRpc(&mcClientID, 0x16, 1, &mcSifParams, sizeof(mcSifParams), mcRetVal, 4, mceSifRpcEndFunc, 0);
    if (ret == 0) {
        mcRunCmdNo = 0x16;
    } else {
        SignalSema(semaidRegFunc);
    }
    return ret;
}

int sceMcWriteFast(int fd, const void *buff, int size)
{
    int ret;

    if (mcClientID.serve == 0) {
        return -100;
    }
    if (PollSema(semaidRegFunc) < 0) {
        return -200;
    }
    mcSifParams.fd = fd;
    mcSifParams.rdbuf = &mcRdFix;
    mcSifParams.buffer = (void *)buff;
    mcSifParams.size = size;
    sceSifWriteBackDCache((void *)buff, size);
    ret = sceSifCallRpc(&mcClientID, 0x1B, 1, &mcSifParams, sizeof(mcSifParams), mcRetVal, 4, mceSifRpcEndFunc, 0);
    if (ret == 0) {
        mcRunCmdNo = 0x1B;
    } else {
        SignalSema(semaidRegFunc);
    }
    return ret;
}

// SOURCE DATA: .data .rodata .bss
