/* libdbc (Sony EE SDK): device basic control, the EE side of the IOP's dbcman.irx (sockets for controllers and
   other DBC devices), all through SIF RPC. */
#include "sdk/eekernel.h"
#include "sdk/sifrpc.h"

typedef struct {
    unsigned int option;
    int number;
    int p1;
    int p2;
    int p3;
    char name[16];
} sceDbcSocketParam;

/* The RPC buffer (one layout per call). */
typedef struct {
    int socket;
    int result;
    int size;
    unsigned char data[0x80];
    int ret;
} DbcData;

typedef struct {
    int socket;
    int cmd;
    int ssize;
    int rsize;
    unsigned char sdata[0x80];
    unsigned char rdata[0x80];
    int ret;
} DbcSRData;

typedef struct {
    unsigned int option;
    int number;
    int p1;
    int p2;
    int p3;
    char name[16];
    int ret;
    void *buf1;
    void *buf2;
} DbcCreateSocket;

typedef struct {
    int socket;
    int cmd;
    int size;
    unsigned char data[0x480];
} DbcData3;

typedef struct {
    unsigned char b[0x40];
} DbcConnection;

int scePrintf(const char *fmt, ...);
void SyncDCache(void *start, void *end);
void sceSifInitRpc(unsigned int mode);
int sceSifCheckStatRpc(sceSifClientData *cd);
int SignalSema(int sema);

int sceDbcSetWorkAddr(void *addr);
static void DPRINT(const char *fmt, ...);

static char __ps2_libinfo__[16] = "PsIIlibdbc  3100";
static int isInit = 0;

static sceSifClientData cd __attribute__((aligned(64)));
static sceSifClientData cd_unused;
static sceSifClientData cd3;
static sceSifClientData cd2[2];
static unsigned int rpc_buf[0x24] __attribute__((aligned(64)));
static DbcData3 rpc_buf3 __attribute__((aligned(64)));
static unsigned int rpc_buf2[2][0x30] __attribute__((aligned(64)));
static unsigned char dma_buf[0x80] __attribute__((aligned(64)));
static int connection[16];
static int sema_id;

static int sceDbcGetModVersion(void)
{
    sceSifCallRpc(&cd, 0x80001363, 0, rpc_buf, 0x90, rpc_buf, 0x90, 0, 0);
    return rpc_buf[0];
}

int sceDbcInit(void)
{
    struct SemaParam sp;
    int i;
    int j;
    int ver;

    if (isInit == 1) {
        return 1;
    }
    sema_id = 0;
    sceSifInitRpc(0);
    while (1) {
        if (sceSifBindRpc(&cd, 0x80001300, 0) < 0) {
            DPRINT("libdbc: bind failed\n");
            return 0;
        }
        if (cd.serve != 0) {
            break;
        }
        i = 0x10000;
        while (i--)
            ;
    }
    ver = sceDbcGetModVersion();
    if ((ver >> 4) != 0x32) {
        scePrintf("libdbc: Module version mismatch ");
        scePrintf("[libdbc.a = %d.%02x, dbcman.irx = %d.%02x]\n", 3, 0x20, ver >> 8, ver & 0xFF);
        return 0;
    }
    while (1) {
        if (sceSifBindRpc(&cd3, 0x8000131C, 0) < 0) {
            DPRINT("libdbc: bind failed\n");
            return 0;
        }
        if (cd3.serve != 0) {
            break;
        }
        i = 0x10000;
        while (i--)
            ;
    }
    for (j = 0; j < 2; j++) {
        while (1) {
            if (sceSifBindRpc(&cd2[j], 0x8000131E + j, 0) < 0) {
                DPRINT("libdbc: bind failed\n");
                return 0;
            }
            if (cd2[j].serve != 0) {
                break;
            }
            i = 0x10000;
            while (i--)
                ;
        }
    }
    sp.initCount = 1;
    sp.maxCount = 0x7F;
    sp.option = (unsigned int)"sceDbc_sema";
    if ((sema_id = CreateSema(&sp)) < 0) {
        return 0;
    }
    for (i = 15; i >= 0; i--) {
        connection[i] = 0;
    }
    DPRINT("libdbc: SifDmaAddr %08x\n", dma_buf);
    sceDbcSetWorkAddr(dma_buf);
    isInit = 1;
    return 1;
}

int sceDbcEnd(void)
{
    if (!isInit) {
        return 0;
    }
    isInit = 0;
    if (sema_id >= 0) {
        DeleteSema(sema_id);
    }
    return 1;
}

static void *sceDbcGetIopDmabuff(void)
{
    if (sceSifCallRpc(&cd, 0x80001305, 0, rpc_buf, 0x90, rpc_buf, 0x90, 0, 0) < 0) {
        return 0;
    }
    return (void *)rpc_buf[1];
}

int sceDbcSetWorkAddr(void *addr)
{
    rpc_buf[1] = (unsigned int)addr;
    if (sceSifCallRpc(&cd, 0x80001304, 0, rpc_buf, 0x90, rpc_buf, 0x90, 0, 0) < 0) {
        DPRINT("sceDbcSetWorkAddr: rpc error\n");
        return 0;
    }
    return rpc_buf[0];
}

int sceDbcCreateSocket(sceDbcSocketParam *param, void *buf1, void *buf2)
{
    DbcCreateSocket *p;
    int i;
    int ret;

    if (!isInit) {
        return 0;
    }
    WaitSema(sema_id);
    p = (DbcCreateSocket *)rpc_buf;
    p->option = param->option;
    p->buf1 = buf1;
    p->buf2 = buf2;
    p->number = param->number;
    p->p1 = param->p1;
    p->p2 = param->p2;
    p->p3 = param->p3;
    for (i = 0; i < 16; i++) {
        p->name[i] = param->name[i];
    }
    if (sceSifCallRpc(&cd, 0x80001301, 0, rpc_buf, 0x90, rpc_buf, 0x90, 0, 0) < 0) {
        DPRINT("sceDbcCreateSocket: rpc error\n");
        SignalSema(sema_id);
        return 0;
    }
    ret = p->ret;
    SignalSema(sema_id);
    return ret;
}

int sceDbcDeleteSocket(int socket)
{
    int ret;

    if (!isInit) {
        return 0;
    }
    WaitSema(sema_id);
    rpc_buf[0] = socket;
    if (sceSifCallRpc(&cd, 0x80001302, 0, rpc_buf, 0x90, rpc_buf, 0x90, 0, 0) < 0) {
        DPRINT("sceDbcDeleteSocket: rpc error\n");
        SignalSema(sema_id);
        return 0;
    }
    ret = rpc_buf[1];
    SignalSema(sema_id);
    return ret;
}

int sceDbcGetDepNumber(int socket)
{
    int ret;

    SyncDCache(dma_buf, dma_buf + 0x80);
    DIntr();
    *(DbcConnection *)connection = *(DbcConnection *)dma_buf;
    EIntr();
    if (connection[socket] != 1) {
        return -12;
    }
    WaitSema(sema_id);
    rpc_buf[0] = socket;
    if (sceSifCallRpc(&cd, 0x80001303, 0, rpc_buf, 0x90, rpc_buf, 0x90, 0, 0) < 0) {
        DPRINT("sceDbcGetDepNumber: rpc error\n");
        SignalSema(sema_id);
        return 0;
    }
    ret = rpc_buf[1];
    SignalSema(sema_id);
    return ret;
}

int sceDbcGetConnection(int socket)
{
    SyncDCache(dma_buf, dma_buf + 0x80);
    DIntr();
    *(DbcConnection *)connection = *(DbcConnection *)dma_buf;
    EIntr();
    if (connection[socket] == 1) {
        return 1;
    }
    return 3;
}

int sceDbcInitSocket(int socket)
{
    int ret;

    WaitSema(sema_id);
    rpc_buf[0] = socket;
    if (sceSifCallRpc(&cd, 0x80001315, 0, rpc_buf, 0x90, rpc_buf, 0x90, 0, 0) < 0) {
        DPRINT("sceDbcInitSocket: rpc error\n");
        SignalSema(sema_id);
        return 0;
    }
    ret = rpc_buf[1];
    SignalSema(sema_id);
    return ret;
}

int sceDbcResetSocket(int socket)
{
    int ret;

    WaitSema(sema_id);
    rpc_buf[0] = socket;
    if (sceSifCallRpc(&cd, 0x80001316, 0, rpc_buf, 0x90, rpc_buf, 0x90, 0, 0) < 0) {
        DPRINT("sceDbcResetSocket: rpc error\n");
        SignalSema(sema_id);
        return 0;
    }
    ret = rpc_buf[1];
    SignalSema(sema_id);
    return ret;
}

int sceDbcGetDeviceStatus(int socket)
{
    int ret;

    WaitSema(sema_id);
    rpc_buf[0] = socket;
    if (sceSifCallRpc(&cd, 0x80001317, 0, rpc_buf, 0x90, rpc_buf, 0x90, 0, 0) < 0) {
        DPRINT("sceDbcGetDeviceStatus: rpc error\n");
        SignalSema(sema_id);
        return 0;
    }
    ret = rpc_buf[1];
    SignalSema(sema_id);
    return ret;
}

int sceDbcSearchPortSpecific(int *param)
{
    int ret;

    WaitSema(sema_id);
    rpc_buf[2] = 2;
    rpc_buf[4] = param[2];
    rpc_buf[5] = param[3];
    if (sceSifCallRpc(&cd, 0x8000131D, 0, rpc_buf, 0x90, rpc_buf, 0x90, 0, 0) < 0) {
        DPRINT("sceDbcGetDeviceStatus: rpc error\n");
        SignalSema(sema_id);
        return -1;
    }
    ret = rpc_buf[1];
    SignalSema(sema_id);
    return ret;
}

int sceDbcSRData(int socket, int cmd, int *ssize, unsigned char *sdata, int *rsize, unsigned char *rdata)
{
    DbcSRData *p;
    int i;
    int ret;

    WaitSema(sema_id);
    p = (DbcSRData *)rpc_buf;
    p->socket = socket;
    p->cmd = cmd;
    p->ssize = *ssize;
    p->rsize = *rsize;
    for (i = 0; i < *ssize; i++) {
        p->sdata[i] = sdata[i];
    }
    if (sceSifCallRpc(&cd, 0x80001318, 0, rpc_buf, 0x90, rpc_buf, 0x90, 0, 0) < 0) {
        DPRINT("sceDbcSRData: rpc error\n");
        SignalSema(sema_id);
        return 0;
    }
    if (p->ret >= 0) {
        *ssize = p->ssize;
        *rsize = p->rsize;
        for (i = 0; i < p->rsize; i++) {
            rdata[i] = p->rdata[i];
        }
    }
    ret = p->ret;
    SignalSema(sema_id);
    return ret;
}

int sceDbcSendData(int socket, int cmd, int *size, unsigned char *data)
{
    DbcData *p;
    int i;
    int ret;

    WaitSema(sema_id);
    p = (DbcData *)rpc_buf;
    p->socket = socket;
    p->result = cmd;
    p->size = *size;
    for (i = 0; i < *size; i++) {
        p->data[i] = data[i];
    }
    if (sceSifCallRpc(&cd, 0x80001319, 0, rpc_buf, 0x90, rpc_buf, 0x90, 0, 0) < 0) {
        DPRINT("sceDbcSendData: rpc error\n");
        SignalSema(sema_id);
        return 0;
    }
    if (p->ret >= 0) {
        *size = p->size;
    }
    ret = p->ret;
    SignalSema(sema_id);
    return ret;
}

int sceDbcSendData2(int socket, int cmd, int *size, unsigned char *data)
{
    static int ndx = 0;
    DbcData *p;
    int i;

    if (*size > 0x80) {
        DPRINT("libdbc: SendData2 data length too long [%d]\n", *size);
        return 0;
    }
    WaitSema(sema_id);
    if (++ndx >= 2) {
        ndx = 0;
    }
    p = (DbcData *)rpc_buf2[ndx];
    p->socket = socket;
    p->result = cmd;
    p->size = *size;
    for (i = 0; i < *size; i++) {
        p->data[i] = data[i];
    }
    if (sceSifCheckStatRpc(&cd2[ndx]) == 1) {
        DPRINT("dbcman : SendData2 BUSY\n");
        SignalSema(sema_id);
        return 0;
    }
    if (sceSifCallRpc(&cd2[ndx], ndx, 1, rpc_buf2[ndx], 0x90, rpc_buf2[ndx], 0x90, 0, 0) < 0) {
        DPRINT("sceDbcSendData2: rpc error\n");
        SignalSema(sema_id);
        return 0;
    }
    SignalSema(sema_id);
    return 1;
}

int sceDbcSendData3(int socket, int cmd, int *size, unsigned char *data, sceSifEndFunc func)
{
    DbcData3 *p;
    int i;

    if (*size > 0x480) {
        DPRINT("libdbc: SendData3 data length too long [%d]\n", *size);
        return 0;
    }
    WaitSema(sema_id);
    if (sceSifCheckStatRpc(&cd3) == 1) {
        DPRINT("dbcman : SendData3 BUSY\n");
        SignalSema(sema_id);
        return 0;
    }
    p = &rpc_buf3;
    p->socket = socket;
    p->cmd = cmd;
    p->size = *size;
    for (i = 0; i < *size; i++) {
        p->data[i] = data[i];
    }
    if (sceSifCallRpc(&cd3, 0x8000131C, 1, &rpc_buf3, 0x490, &rpc_buf3, 0x490, func, 0) < 0) {
        SignalSema(sema_id);
        return 0;
    }
    SignalSema(sema_id);
    return 1;
}

int sceDbcReceiveData(int socket, int cmd, int *size, unsigned char *data)
{
    DbcData *p;
    int i;
    int ret;

    WaitSema(sema_id);
    p = (DbcData *)rpc_buf;
    p->socket = socket;
    p->result = cmd;
    p->size = *size;
    if (sceSifCallRpc(&cd, 0x8000131A, 0, rpc_buf, 0x90, rpc_buf, 0x90, 0, 0) < 0) {
        DPRINT("sceDbcReceiveData: rpc error\n");
        SignalSema(sema_id);
        return 0;
    }
    if (p->ret >= 0) {
        *size = p->size;
        for (i = 0; i < p->size; i++) {
            data[i] = p->data[i];
        }
    }
    ret = p->ret;
    SignalSema(sema_id);
    return ret;
}

static void DPRINT(const char *fmt, ...)
{
}

// SOURCE DATA: .data .rodata .bss
