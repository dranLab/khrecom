/* SIF RPC on the EE: client calls/binds/data transfers to IOP servers and the EE-side server queues. */
#include "sdk/eekernel.h"
#include "sdk/sifrpc.h"
#include "include_asm.h"

void sceSifInitCmd(void);
void sceSifExitCmd(void);
unsigned int sceSifGetReg(unsigned int reg);
int sceSifSetReg(unsigned int reg, unsigned int value);
int sceSifGetSreg(int reg);
void sceSifAddCmdHandler(int cid, void (*func)(void *pkt, void *data), void *data);
int sceSifSendCmd(int cid, void *pkt, int pktsize, void *src, void *dest, int size);
int isceSifSendCmd(int cid, void *pkt, int pktsize, void *src, void *dest, int size);
void sceSifWriteBackDCache(void *ptr, int size);
int iSetTimerAlarm(unsigned long clock, unsigned long (*handler)(int, unsigned long, unsigned long, void *), void *arg);
int iWakeupThread(int tid);
void SleepThread(void);

typedef struct {
    unsigned int data;
    unsigned int addr;
    unsigned int size;
    unsigned int mode;
} sceSifDmaData;

unsigned int sceSifSetDma(sceSifDmaData *sdd, int len);

#define SIF_CMD_INIT_CMD 0x80000002
#define SIF_CMD_RPC_END 0x80000008
#define SIF_CMD_RPC_BIND 0x80000009
#define SIF_CMD_RPC_CALL 0x8000000A
#define SIF_CMD_RPC_RDATA 0x8000000C
#define SIF_SYSREG_RPCINIT 0x80000002
#define SIF_RPCM_NOWAIT 1
#define SIF_RPCM_NOWBDC 2
#define PACKET_F_ALLOC 1
#define UNCACHED(p) ((void *)((unsigned int)(p) | 0x20000000))

typedef struct {
    unsigned int psize : 8;
    unsigned int dsize : 24;
    unsigned int dest;
    int cid;
    unsigned int opt;
} sceSifCmdHdr;

/* the RPC packets (0x40 bytes each) */
typedef struct {
    sceSifCmdHdr hdr;
    unsigned int rec_id;
    void *pkt_addr;
    int rpc_id;
} RpcPktHeader;

typedef struct {
    RpcPktHeader rpc;
    sceSifClientData *client;
    unsigned int cid;
    sceSifServeData *server;
    void *buff;
    void *cbuff;
} RpcRendPkt;

typedef struct {
    RpcPktHeader rpc;
    sceSifReceiveData *receive;
    unsigned int cid;
    void *src;
    void *dest;
    int size;
} RpcRdataRendPkt;

typedef struct {
    RpcPktHeader rpc;
    sceSifReceiveData *receive;
    void *src;
    void *dest;
    int size;
} RpcOtherDataPkt;

typedef struct {
    RpcPktHeader rpc;
    sceSifClientData *client;
    unsigned int sid;
} RpcBindPkt;

typedef struct {
    RpcPktHeader rpc;
    sceSifClientData *client;
    unsigned int fno;
    int ssize;
    void *receive;
    int rsize;
    int rmode;
    sceSifServeData *server;
} RpcCallPkt;

struct rpc_data {
    int pid;
    void *pkt_table;
    int pkt_table_len;
    int unused1;
    int unused2;
    void *rdata_table;
    int rdata_table_len;
    void *client_table;
    int client_table_len;
    int rdata_table_idx;
    sceSifQueueData *active_queue;
};

static int _sceSifInitCheck = 0;

void *_sceRpcGetPacket(struct rpc_data *rd);

static unsigned char _packet_buffer[0x800] __attribute__((aligned(64)));
static unsigned char _free_buffer[0x800] __attribute__((aligned(64)));
static unsigned char _free_buffer2[0x800] __attribute__((aligned(64)));
static struct rpc_data _data_table __attribute__((aligned(64)));

static inline int GetGP(void)
{
    int gp;

    asm volatile ("move\t%0, $gp" : "=r" (gp));
    return gp;
}

static inline int SetGP(int gp)
{
    int old;

    asm volatile ("move\t%0, $gp\n\tmove\t$gp, %1" : "=&r" (old) : "r" (gp) : "memory");
    return old;
}

void _request_end(RpcRendPkt *rend, struct rpc_data *rd);
void _request_bind(RpcBindPkt *bind, struct rpc_data *rd);
void _request_call(RpcCallPkt *call, struct rpc_data *rd);
void _request_rdata(RpcOtherDataPkt *rdata, struct rpc_data *rd);

/* Parked: 62 lines differ (permuter score) */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/sifrpc", sceSifInitRpc);
#if 0
void sceSifInitRpc(unsigned int mode)
{
    DIntr();
    if (_sceSifInitCheck) {
        EIntr();
        return;
    }
    _sceSifInitCheck = 1;
    EIntr();
    sceSifInitCmd();
    DIntr();
    _data_table.client_table_len = 32;
    _data_table.pid = 1;
    _data_table.rdata_table = UNCACHED(_free_buffer);
    _data_table.pkt_table = UNCACHED(_packet_buffer);
    _data_table.client_table = UNCACHED(_free_buffer2);
    _data_table.pkt_table_len = 32;
    _data_table.unused1 = 0;
    _data_table.unused2 = 0;
    _data_table.rdata_table_len = 32;
    _data_table.rdata_table_idx = 0;
    sceSifAddCmdHandler(SIF_CMD_RPC_END, (void *)_request_end, &_data_table);
    sceSifAddCmdHandler(SIF_CMD_RPC_BIND, (void *)_request_bind, &_data_table);
    sceSifAddCmdHandler(SIF_CMD_RPC_CALL, (void *)_request_call, &_data_table);
    sceSifAddCmdHandler(SIF_CMD_RPC_RDATA, (void *)_request_rdata, &_data_table);
    EIntr();
    if (sceSifGetReg(SIF_SYSREG_RPCINIT) == 0) {
        ((sceSifCmdHdr *)(_packet_buffer + 0x40))->opt = 1;
        sceSifSendCmd(SIF_CMD_INIT_CMD, _packet_buffer + 0x40, sizeof(sceSifCmdHdr), 0, 0, 0);
        while (!sceSifGetSreg(0)) {
        }
        sceSifSetReg(SIF_SYSREG_RPCINIT, 1);
    }
}
#endif

void sceSifExitRpc(void)
{
    sceSifExitCmd();
    _sceSifInitCheck = 0;
}

/* Parked: 1 line, 4 bytes short - in the pid == 1 arm the original also sets v1 = 1 (`li v1,1` after the old + 2): pid
   is written again for the rpc_id store after the join. `pid = 1` in the arm folds away, before or after rd->pid++, also
   across an empty asm or through a local; an asm output keeps it but disturbs the load (6 lines). */
static void *_sceRpcGetPacket(struct rpc_data *rd)
{
    RpcPktHeader *packet;
    int len, rid;
    register int pid asm("$3"); /* MATCH: in v1, so the arm's pid = 1 stays (li v1,1) */

    DIntr();
    len = rd->pkt_table_len;
    packet = rd->pkt_table;
    for (rid = 0; rid < len; rid++, packet = (RpcPktHeader *)((char *)packet + 0x40)) {
        if (packet->rec_id & PACKET_F_ALLOC) {
            continue;
        }
        packet->rec_id = (rid << 16) | 4 | PACKET_F_ALLOC;
        {
            int old = rd->pid;
            pid = old + 1;
            rd->pid = pid;
            if (pid == 1) {
                rd->pid = old + 2;
                pid = 1;
            }
        }
        packet->pkt_addr = packet;
        packet->rpc_id = pid;
        EIntr();
        return packet;
    }
    EIntr();
    return 0;
}

static void _sceRpcFreePacket(RpcPktHeader *packet)
{
    packet->rpc_id = 0;
    packet->rec_id &= ~PACKET_F_ALLOC;
}

static void *_sceRpcGetFPacket(struct rpc_data *rd)
{
    int index = rd->rdata_table_idx % rd->rdata_table_len;
    void *p = (char *)rd->rdata_table + index * 0x40;

    rd->rdata_table_idx = index + 1;
    return p;
}

static void *_sceRpcGetFPacket2(struct rpc_data *rd, int rid)
{
    if (rid < 0 || rid >= rd->client_table_len) {
        return _sceRpcGetFPacket(rd);
    }
    return (char *)rd->client_table + rid * 0x40;
}

/* Parked: 11 lines (47 before the client/buff locals, decomp-permuter) - the original tests 0x8000000A first with a
   range check (more cases in its switch?), ours 0x80000009 */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/sifrpc", _request_end);
#if 0
static void _request_end(RpcRendPkt *rend, struct rpc_data *rd)
{
    int gp;

    switch (rend->cid) {
    case SIF_CMD_RPC_CALL:
        if (rend->client->func) {
            sceSifClientData *cd = rend->client;
            gp = SetGP((int)cd->gp);
            cd->func(cd->para);
            SetGP(gp);
        }
        break;
    case SIF_CMD_RPC_BIND:
        rend->client->serve = rend->server;
        {
            void *buff = rend->buff;
            rend->client->buff = buff;
        }
        break;
    }
    if (rend->client->rpcd.tid >= 0) {
        iSignalSema(rend->client->rpcd.tid);
    }
    {
        sceSifClientData *cd = rend->client;
        _sceRpcFreePacket(cd->rpcd.paddr);
        cd->rpcd.paddr = 0;
    }
}
#endif

static unsigned long _alarm_rdata(int id, unsigned long schedule, unsigned long actual, void *arg)
{
    RpcRdataRendPkt *rend = arg;

    if (isceSifSendCmd(SIF_CMD_RPC_END, rend, 0x40, rend->src, rend->dest, rend->size)) {
        return 0;
    }
    return 0x800;
}

/* Parked: 7 of 59 lines (2026-11-07, was 10): the strict load/store order is stores of alias set 0: through a union, every
   later read reloads, also the call's arguments (int casts on both sides do not: cse keeps those). pkt_addr's store is
   plain: receive loads ahead of it. Left: v0/v1 alternate the other way through the three last copies, a sched1 tie. */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/sifrpc", _request_rdata);
#if 0
static void _request_rdata(RpcOtherDataPkt *rdata, struct rpc_data *rd)
{
    RpcRdataRendPkt *rend;

    if (rdata->rpc.rec_id & 4) {
        rend = _sceRpcGetFPacket2(rd, rdata->rpc.rec_id >> 16);
    } else {
        rend = _sceRpcGetFPacket(rd);
    }
#define _COPY(f, g) (((union { int i; } *)&rend->f)->i = (int)rdata->g)
    rend->rpc.pkt_addr = rdata->rpc.pkt_addr;
    _COPY(receive, receive);
    rend->cid = SIF_CMD_RPC_RDATA;
    _COPY(src, src);
    _COPY(dest, dest);
    _COPY(size, size);
    if (!isceSifSendCmd(SIF_CMD_RPC_END, rend, 0x40, rdata->src, rdata->dest, rdata->size)) {
        iSetTimerAlarm(0x800, _alarm_rdata, rend);
    }
}
#endif

int sceSifGetOtherData(sceSifReceiveData *rd, void *src, void *dest, int size, unsigned int mode)
{
    RpcOtherDataPkt *pkt;
    struct SemaParam sp;

    pkt = _sceRpcGetPacket(&_data_table);
    if (!pkt) {
        return -1;
    }
    rd->rpcd.paddr = pkt;
    rd->rpcd.pid = pkt->rpc.rpc_id;
    pkt->src = src;
    pkt->dest = dest;
    pkt->size = size;
    pkt->rpc.pkt_addr = pkt;
    pkt->receive = rd;
    if (!(mode & SIF_RPCM_NOWAIT)) {
        sp.maxCount = 1;
        sp.option = (unsigned int)"SceSifrpcGod";
        sp.initCount = 0;
        rd->rpcd.tid = CreateSema(&sp);
        if (rd->rpcd.tid < 0) {
            _sceRpcFreePacket(&pkt->rpc);
            return -3;
        }
        if (!sceSifSendCmd(SIF_CMD_RPC_RDATA, pkt, 0x40, 0, 0, 0)) {
            _sceRpcFreePacket(&pkt->rpc);
            DeleteSema(rd->rpcd.tid);
            return -2;
        }
        WaitSema(rd->rpcd.tid);
        DeleteSema(rd->rpcd.tid);
        return 0;
    }
    rd->rpcd.tid = -1;
    if (sceSifSendCmd(SIF_CMD_RPC_RDATA, pkt, 0x40, 0, 0, 0)) {
        return 0;
    }
    _sceRpcFreePacket(&pkt->rpc);
    return -2;
}

static sceSifServeData *_search_svdata(unsigned int sid, struct rpc_data *rd)
{
    sceSifQueueData *q;
    sceSifServeData *s;

    for (q = rd->active_queue; q; q = q->next) {
        for (s = q->link; s; s = s->link) {
            if (s->command == sid) {
                return s;
            }
        }
    }
    return 0;
}

static unsigned long _alarm_bind(int id, unsigned long schedule, unsigned long actual, void *arg)
{
    if (isceSifSendCmd(SIF_CMD_RPC_END, arg, 0x40, 0, 0, 0)) {
        return 0;
    }
    return 0x800;
}

/* Parked: the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/sifrpc", _request_bind);
#if 0
static void _request_bind(RpcBindPkt *bind, struct rpc_data *rd)
{
    RpcRendPkt *rend;
    sceSifServeData *sd;

    rend = _sceRpcGetFPacket(rd);
    rend->client = bind->client;
    rend->rpc.pkt_addr = bind->rpc.pkt_addr;
    rend->cid = SIF_CMD_RPC_BIND;
    sd = _search_svdata(bind->sid, rd);
    if (sd) {
        rend->server = sd;
        rend->buff = sd->buff;
    } else {
        rend->server = 0;
        rend->buff = 0;
    }
    if (!isceSifSendCmd(SIF_CMD_RPC_END, rend, 0x40, 0, 0, 0)) {
        iSetTimerAlarm(0x800, _alarm_bind, rend);
    }
}
#endif

int sceSifBindRpc(sceSifClientData *cd, unsigned int request, unsigned int mode)
{
    RpcBindPkt *bind;
    struct SemaParam sp;

    cd->command = 0;
    cd->serve = 0;
    bind = _sceRpcGetPacket(&_data_table);
    if (!bind) {
        return -1;
    }
    cd->rpcd.paddr = bind;
    cd->rpcd.pid = bind->rpc.rpc_id;
    bind->sid = request;
    bind->rpc.pkt_addr = bind;
    bind->client = cd;
    if (!(mode & SIF_RPCM_NOWAIT)) {
        sp.maxCount = 1;
        sp.option = (unsigned int)"SceSifrpcBind";
        sp.initCount = 0;
        cd->rpcd.tid = CreateSema(&sp);
        if (cd->rpcd.tid < 0) {
            _sceRpcFreePacket(&bind->rpc);
            return -3;
        }
        if (!sceSifSendCmd(SIF_CMD_RPC_BIND, bind, 0x40, 0, 0, 0)) {
            _sceRpcFreePacket(&bind->rpc);
            DeleteSema(cd->rpcd.tid);
            return -2;
        }
        WaitSema(cd->rpcd.tid);
        DeleteSema(cd->rpcd.tid);
        return 0;
    }
    cd->rpcd.tid = -1;
    if (sceSifSendCmd(SIF_CMD_RPC_BIND, bind, 0x40, 0, 0, 0)) {
        return 0;
    }
    _sceRpcFreePacket(&bind->rpc);
    return -2;
}

/* A call for an EE server: queue it for its thread. */
/* Parked: 8 lines, 4 bytes short (45 before the do-while(0) around the fno copy, decomp-permuter); the rest of the
   field copies still let the scheduler hoist loads */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/sifrpc", _request_call);
#if 0
static void _request_call(RpcCallPkt *call, struct rpc_data *rd)
{
    sceSifServeData *sd = call->server;
    sceSifQueueData *qd = sd->base;

    if (qd->start) {
        qd->end->next = sd;
    } else {
        qd->start = sd;
    }
    qd->end = sd;
    sd->paddr = call->rpc.pkt_addr;
    sd->client = call->client;
    do {
        sd->fno = call->fno;
    } while (0);
    sd->size = call->ssize;
    sd->receive = call->receive;
    sd->rsize = call->rsize;
    sd->rmode = call->rmode;
    sd->rid = call->rpc.rec_id;
    if (qd->key >= 0 && !qd->active) {
        iWakeupThread(qd->key);
    }
}
#endif

/* Parked: the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/sifrpc", sceSifCallRpc);
#if 0
int sceSifCallRpc(sceSifClientData *cd, unsigned int fno, unsigned int mode, void *send, int ssize, void *receive,
                  int rsize, sceSifEndFunc end_func, void *end_para)
{
    RpcCallPkt *call;
    struct SemaParam sp;

    call = _sceRpcGetPacket(&_data_table);
    if (!call) {
        return -1;
    }
    cd->para = end_para;
    cd->rpcd.pid = call->rpc.rpc_id;
    cd->rpcd.paddr = call;
    cd->func = end_func;
    cd->gp = (void *)GetGP();
    call->fno = fno;
    call->ssize = ssize;
    call->receive = receive;
    call->rsize = rsize;
    call->rpc.pkt_addr = call;
    call->client = cd;
    call->server = cd->serve;
    if (!(mode & SIF_RPCM_NOWBDC)) {
        if (send == receive) {
            sceSifWriteBackDCache(send, ssize < rsize ? rsize : ssize);
        } else {
            if (ssize > 0) {
                sceSifWriteBackDCache(send, ssize);
            }
            if (rsize > 0) {
                sceSifWriteBackDCache(receive, rsize);
            }
        }
    }
    if (mode & SIF_RPCM_NOWAIT) {
        if (!end_func) {
            call->rmode = 0;
        } else {
            call->rmode = 1;
        }
        cd->rpcd.tid = -1;
        if (sceSifSendCmd(SIF_CMD_RPC_CALL, call, 0x40, send, cd->buff, ssize)) {
            return 0;
        }
        _sceRpcFreePacket(&call->rpc);
        return -2;
    }
    sp.initCount = 0;
    sp.option = (unsigned int)"SceSifrpcCall";
    sp.maxCount = 1;
    cd->rpcd.tid = CreateSema(&sp);
    if (cd->rpcd.tid < 0) {
        _sceRpcFreePacket(&call->rpc);
        return -3;
    }
    call->rmode = 1;
    if (!sceSifSendCmd(SIF_CMD_RPC_CALL, call, 0x40, send, cd->buff, ssize)) {
        DeleteSema(cd->rpcd.tid);
        _sceRpcFreePacket(&call->rpc);
        return -2;
    }
    WaitSema(cd->rpcd.tid);
    DeleteSema(cd->rpcd.tid);
    return 0;
}
#endif

int sceSifCheckStatRpc(sceSifClientData *cd)
{
    RpcPktHeader *packet = cd->rpcd.paddr;

    if (!packet || cd->rpcd.pid != packet->rpc_id || !(packet->rec_id & PACKET_F_ALLOC)) {
        return 0;
    }
    return 1;
}

void sceSifSetRpcQueue(sceSifQueueData *qd, int key)
{
    sceSifQueueData *q;

    DIntr();
    qd->key = key;
    qd->active = 0;
    qd->link = 0;
    qd->start = 0;
    qd->end = 0;
    qd->next = 0;
    if (!_data_table.active_queue) {
        _data_table.active_queue = qd;
    } else {
        for (q = _data_table.active_queue; q->next; q = q->next) {
        }
        q->next = qd;
    }
    EIntr();
}

void sceSifRegisterRpc(sceSifServeData *sd, unsigned int command, sceSifRpcFunc func, void *buff, sceSifRpcFunc cfunc,
                       void *cbuff, sceSifQueueData *qd)
{
    sceSifServeData *s;

    DIntr();
    sd->next = 0;
    sd->link = 0;
    sd->command = command;
    sd->func = func;
    sd->buff = buff;
    sd->cfunc = cfunc;
    sd->cbuff = cbuff;
    sd->base = qd;
    if (!qd->link) {
        qd->link = sd;
    } else {
        for (s = qd->link; s->link; s = s->link) {
        }
        s->link = sd;
    }
    EIntr();
}

sceSifServeData *sceSifRemoveRpc(sceSifServeData *sd, sceSifQueueData *qd)
{
    sceSifServeData *s;

    DIntr();
    s = qd->link;
    if (s == sd) {
        qd->link = s->link;
    } else if (s) {
        while (s->link != sd) {
            s = s->link;
            if (!s) {
                goto end;
            }
        }
        s->link = sd->link;
    }
end:
    EIntr();
    return s;
}

sceSifQueueData *sceSifRemoveRpcQueue(sceSifQueueData *qd)
{
    sceSifQueueData *q;

    DIntr();
    q = _data_table.active_queue;
    if (q == qd) {
        _data_table.active_queue = q->next;
    } else if (q) {
        while (q->next != qd) {
            q = q->next;
            if (!q) {
                goto end;
            }
        }
        q->next = qd->next;
    }
end:
    EIntr();
    return q;
}

sceSifServeData *sceSifGetNextRequest(sceSifQueueData *qd)
{
    sceSifServeData *sd;

    DIntr();
    sd = qd->start;
    if (sd == 0) {
        qd->active = 0;
    } else {
        qd->active = 1;
        qd->start = sd->next;
    }
    EIntr();
    return sd;
}

/* Parked: the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/sifrpc", sceSifExecRequest);
#if 0
void sceSifExecRequest(sceSifServeData *sd)
{
    sceSifDmaData dma[2];
    RpcRendPkt *rend;
    void *rec;
    int size = 0, n, i;

    rec = sd->func(sd->fno, sd->buff, sd->size);
    if (rec) {
        size = sd->rsize;
    }
    if (sd->size > 0) {
        sceSifWriteBackDCache(sd->buff, sd->size);
    }
    if (size > 0) {
        sceSifWriteBackDCache(rec, size);
    }
    DIntr();
    if (sd->rid & 4) {
        rend = _sceRpcGetFPacket2(&_data_table, sd->rid >> 16);
    } else {
        rend = _sceRpcGetFPacket(&_data_table);
    }
    EIntr();
    rend->cid = SIF_CMD_RPC_CALL;
    rend->client = sd->client;
    if (sd->rmode) {
        while (!sceSifSendCmd(SIF_CMD_RPC_END, rend, 0x40, rec, sd->receive, size)) {
        }
        return;
    }
    rend->rpc.rpc_id = 0;
    rend->rpc.rec_id = 0;
    n = 0;
    if (size > 0) {
        n = 1;
        dma[0].data = (unsigned int)rec;
        dma[0].addr = (unsigned int)sd->receive;
        dma[0].size = size;
        dma[0].mode = 0;
    }
    dma[n].data = (unsigned int)rend;
    dma[n].addr = (unsigned int)sd->paddr;
    dma[n].size = 0x40;
    dma[n].mode = 0;
    n++;
    while (!sceSifSetDma(dma, n)) {
        i = 0x100000;
        while (i--) {
        }
    }
}
#endif

void sceSifRpcLoop(sceSifQueueData *qd)
{
    sceSifServeData *sd;

    for (;;) {
        while ((sd = sceSifGetNextRequest(qd))) {
            sceSifExecRequest(sd);
        }
        SleepThread();
    }
}
/* sceSifCallRpc's semaphore name, while that function is still assembly */
static const char sifrpc_call_name[] __attribute__((aligned(8))) = "SceSifrpcCall";
// SOURCE DATA: .data .rodata .bss
