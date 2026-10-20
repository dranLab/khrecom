/* SIF commands: EE <-> IOP packets over SIF DMA (channel 5 receives), with handler tables and soft registers. */
#include "sdk/eekernel.h"
#include "include_asm.h"

void *memcpy(void *dst, const void *src, unsigned int n);
unsigned int sceSifGetReg(unsigned int reg);
int sceSifSetReg(unsigned int reg, unsigned int value);
void sceSifSetDChain(void);
void isceSifSetDChain(void);
int AddDmacHandler(int channel, int (*handler)(int), int next);
int RemoveDmacHandler(int channel, int id);
int EnableDmac(int channel);
int DisableDmac(int channel);

typedef struct {
    unsigned int data;
    unsigned int addr;
    unsigned int size;
    unsigned int mode;
} sceSifDmaData;

unsigned int _sceSifSetDma(sceSifDmaData *sdd, int len);
unsigned int _isceSifSetDma(sceSifDmaData *sdd, int len);

typedef struct {
    unsigned int psize : 8;
    unsigned int dsize : 24;
    unsigned int dest;
    int cid;
    unsigned int opt;
} sceSifCmdHdr;

typedef void (*sceSifCmdHandler)(void *pkt, void *data);

typedef struct {
    sceSifCmdHandler func;
    void *data;
    int gp;
} sceSifCmdData;

typedef struct {
    void *pktbuf;
    void *sendbuf;
    void *iopbuf;
    sceSifCmdData *sys_cmd_handlers;
    int nr_sys_handlers;
    sceSifCmdData *usr_cmd_handlers;
    int nr_usr_handlers;
    int *sregs;
} sceSifCmdDataTable;

typedef struct {
    sceSifCmdHdr hdr;
    void *buff;
} sceSifCmdCSData;

#define SIF_REG_SUBADDR 2
#define SIF_REG_SMFLAG 4
#define SIF_STAT_CMDINIT 0x20000
#define SIF_SYSREG_SUBADDR 0x80000000
#define SIF_SYSREG_MAINADDR 0x80000001
#define SIF_CMD_CHANGE_SADDR 0x80000000
#define SIF_CMD_INIT_CMD 0x80000002
#define UNCACHED(p) ((void *)((unsigned int)(p) | 0x20000000))

/* the "set soft register" header that ends every DMA chain sent while commands are up */
static const sceSifCmdHdr _Sch = { 16, 0, 0, 0x80000004, 0 };
static int _cmd_init_check = 0;

static unsigned char _pckt_buffer[0x80] __attribute__((aligned(64)));
static unsigned char _send_buffer[0x40] __attribute__((aligned(64)));
static sceSifCmdCSData _csdata;
static int sif0_handleid;
static sceSifCmdDataTable _data_table;
static sceSifCmdData _sys_buffer[32] __attribute__((aligned(64)));
static int soft_reg[32];

static inline int GetGP(void)
{
    int gp;

    asm volatile ("move\t%0, $gp" : "=r" (gp) : : "memory");
    return gp;
}

static inline int SetGP(int gp)
{
    int old;

    asm volatile ("move\t%0, $gp\n\tmove\t$gp, %1" : "=&r" (old) : "r" (gp) : "memory");
    return old;
}

typedef struct {
    sceSifCmdHdr hdr;
    int index;
    int value;
} SetSregPkt;

typedef struct {
    sceSifCmdHdr hdr;
    void *buff;
} ChangeAddrPkt;

static void _set_sreg(SetSregPkt *pkt, sceSifCmdDataTable *table)
{
    table->sregs[pkt->index] = pkt->value;
}

static void _change_addr(ChangeAddrPkt *pkt, sceSifCmdDataTable *table)
{
    table->iopbuf = pkt->buff;
}

int sceSifGetSreg(int reg)
{
    return soft_reg[reg];
}

int sceSifSetSreg(int reg, int value)
{
    soft_reg[reg] = value;
    return value;
}

sceSifCmdDataTable *sceSifGetDataTable(void)
{
    return &_data_table;
}

/* Append the soft-register header to the chain. */
static unsigned int sceSifSetDmaInternal(sceSifDmaData *sdd, int len, int intr)
{
    sceSifDmaData dma[32];

    if (len < 32) {
        memcpy(dma, sdd, len * sizeof(sceSifDmaData));
        dma[len].data = (unsigned int)&_Sch;
        dma[len].addr = (unsigned int)_data_table.iopbuf;
        dma[len].size = sizeof(_Sch);
        dma[len].mode = 0x44;
        len++;
        if (!intr) {
            return _sceSifSetDma(dma, len);
        }
        return _isceSifSetDma(dma, len);
    }
    return 0;
}

unsigned int sceSifSetDma(sceSifDmaData *sdd, int len)
{
    int eie;

    eie = DIntr();
    if (_cmd_init_check) {
        if (eie) {
            EIntr();
        }
        return sceSifSetDmaInternal(sdd, len, 0);
    }
    if (eie) {
        EIntr();
    }
    return _sceSifSetDma(sdd, len);
}

unsigned int isceSifSetDma(sceSifDmaData *sdd, int len)
{
    if (_cmd_init_check) {
        return sceSifSetDmaInternal(sdd, len, 1);
    }
    return _isceSifSetDma(sdd, len);
}

int _sceSifCmdIntrHdlr(int channel);
int sceSifSendCmd(int cid, void *pkt, int pktsize, void *src, void *dest, int size);

void sceSifInitCmd(void)
{
    int i;

    DIntr();
    if (_cmd_init_check) {
        EIntr();
        return;
    }
    _cmd_init_check = 1;
    _data_table.pktbuf = UNCACHED(_pckt_buffer);
    _data_table.sendbuf = UNCACHED(_send_buffer);
    _data_table.nr_sys_handlers = 32;
    _data_table.iopbuf = 0;
    _data_table.sys_cmd_handlers = _sys_buffer;
    _data_table.usr_cmd_handlers = 0;
    _data_table.nr_usr_handlers = 0;
    _data_table.sregs = soft_reg;
    for (i = 0; i < 32; i++) {
        _sys_buffer[i].func = 0;
        _sys_buffer[i].data = 0;
    }
    for (i = 0; i < 32; i++) {
        soft_reg[i] = 0;
    }
    _sys_buffer[0].func = (sceSifCmdHandler)_change_addr;
    _sys_buffer[1].func = (sceSifCmdHandler)_set_sreg;
    _sys_buffer[0].data = &_data_table;
    _sys_buffer[1].data = &_data_table;
    EIntr();
    FlushCache(0);
    if (*(volatile unsigned int *)0x1000E010 & 0x20) {  /* D_STAT: channel 5 interrupt pending */
        *(volatile unsigned int *)0x1000E010 = 0x20;
    }
    if (!(*(volatile unsigned int *)0x1000C000 & 0x100)) {  /* D5_CHCR: not running */
        sceSifSetDChain();
    }
    sif0_handleid = AddDmacHandler(5, _sceSifCmdIntrHdlr, 0);
    EnableDmac(5);
    if ((_data_table.iopbuf = (void *)sceSifGetReg(SIF_SYSREG_SUBADDR)) != 0) {
        _csdata.buff = _pckt_buffer;
        sceSifSendCmd(SIF_CMD_CHANGE_SADDR, &_csdata, sizeof(_csdata), 0, 0, 0);
        return;
    }
    while (!(sceSifGetReg(SIF_REG_SMFLAG) & SIF_STAT_CMDINIT)) {
    }
    _data_table.iopbuf = (void *)sceSifGetReg(SIF_REG_SUBADDR);
    sceSifSetReg(SIF_SYSREG_SUBADDR, (unsigned int)_data_table.iopbuf);
    sceSifSetReg(SIF_SYSREG_MAINADDR, (unsigned int)&_data_table);
    _csdata.buff = _pckt_buffer;
    _csdata.hdr.opt = 0;
    sceSifSendCmd(SIF_CMD_INIT_CMD, &_csdata, sizeof(_csdata), 0, 0, 0);
}

void sceSifExitCmd(void)
{
    DisableDmac(5);
    RemoveDmacHandler(5, sif0_handleid);
    _cmd_init_check = 0;
}

sceSifCmdData *sceSifSetCmdBuffer(sceSifCmdData *buf, int num)
{
    sceSifCmdData *old = _data_table.usr_cmd_handlers;

    _data_table.usr_cmd_handlers = buf;
    _data_table.nr_usr_handlers = num;
    return old;
}

sceSifCmdData *sceSifSetSysCmdBuffer(sceSifCmdData *buf, int num)
{
    sceSifCmdData *old = _data_table.sys_cmd_handlers;

    _data_table.sys_cmd_handlers = buf;
    _data_table.nr_sys_handlers = num;
    return old;
}

/* cid with the top bit set: a system command. */
void sceSifAddCmdHandler(int cid, sceSifCmdHandler func, void *data)
{
    if (cid < 0) {
        _data_table.sys_cmd_handlers[cid & 0x7FFFFFFF].func = func;
        _data_table.sys_cmd_handlers[cid & 0x7FFFFFFF].data = data;
        _data_table.sys_cmd_handlers[cid & 0x7FFFFFFF].gp = GetGP();
    } else {
        _data_table.usr_cmd_handlers[cid].func = func;
        _data_table.usr_cmd_handlers[cid].data = data;
        _data_table.usr_cmd_handlers[cid].gp = GetGP();
    }
}

void sceSifRemoveCmdHandler(int cid)
{
    if (cid < 0) {
        _data_table.sys_cmd_handlers[cid & 0x7FFFFFFF].func = 0;
    } else {
        _data_table.usr_cmd_handlers[cid].func = 0;
    }
}

void sceSifWriteBackDCache(void *ptr, int size);

static unsigned int _sceSifSendCmd(int cid, int mode, sceSifCmdHdr *pkt, int pktsize, void *src, void *dest, int size)
{
    sceSifDmaData dma[2];
    int n;

    if ((unsigned int)(pktsize - 16) > 0x60) {
        return 0;
    }
    n = 0;
    if (size > 0) {
        n = 1;
        pkt->dsize = size;
        dma[0].data = (unsigned int)src;
        dma[0].addr = (unsigned int)dest;
        pkt->dest = (unsigned int)dest;
        dma[0].size = size;
        dma[0].mode = 0;
        if (mode & 4) {
            sceSifWriteBackDCache(src, size);
        }
    } else {
        pkt->dsize = 0;
        pkt->dest = 0;
    }
    pkt->cid = cid;
    pkt->psize = pktsize;
    dma[n].data = (unsigned int)pkt;
    dma[n].addr = (unsigned int)_data_table.iopbuf;
    dma[n].size = pktsize;
    dma[n].mode = 0x44;
    sceSifWriteBackDCache(pkt, pktsize);
    n++;
    if (mode & 1) {
        return _isceSifSetDma(dma, n);
    }
    return _sceSifSetDma(dma, n);
}

int sceSifSendCmd(int cid, void *pkt, int pktsize, void *src, void *dest, int size)
{
    return _sceSifSendCmd(cid, 0, pkt, pktsize, src, dest, size);
}

int isceSifSendCmd(int cid, void *pkt, int pktsize, void *src, void *dest, int size)
{
    return _sceSifSendCmd(cid, 1, pkt, pktsize, src, dest, size);
}

/* DMA channel 5: a command packet arrived in _pckt_buffer; copy it out, restart the chain, run its handler.
 * Still the original assembly: the C below has the original's shape and size, but its registers differ (pkt/size
 * in a2/a0 instead of a3/a1, the scaled index kept apart from id). */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/sifcmd", _sceSifCmdIntrHdlr);
#if 0
static int _sceSifCmdIntrHdlr(int channel)
{
    u_long128 buf[7];
    sceSifCmdDataTable *tbl = &_data_table;
    volatile sceSifCmdHdr *pkt = tbl->pktbuf;
    sceSifCmdData *h;
    u_long128 *s, *d;
    sceSifCmdHandler func;
    int size, n, i, id, gp;

    size = pkt->psize;
    if (size == 0) {
        return 0;
    }
    n = (size + 15) / 16;
    pkt->psize = 0;
    s = (u_long128 *)pkt;
    d = buf;
    for (i = 0; i < n; i++) {
        *d++ = *s++;
    }
    isceSifSetDChain();
    if (((volatile sceSifCmdHdr *)buf)->cid < 0) {
        id = ((volatile sceSifCmdHdr *)buf)->cid & 0x7FFFFFFF;
        if (id >= tbl->nr_sys_handlers) {
            goto end;
        }
        h = &tbl->sys_cmd_handlers[id];
        func = h->func;
        if (func == 0) {
            goto end;
        }
        gp = SetGP(h->gp);
        h = &tbl->sys_cmd_handlers[id];
    } else {
        id = ((volatile sceSifCmdHdr *)buf)->cid;
        if (id >= tbl->nr_usr_handlers) {
            goto end;
        }
        h = &tbl->usr_cmd_handlers[id];
        func = h->func;
        if (func == 0) {
            goto end;
        }
        gp = SetGP(h->gp);
        h = &tbl->usr_cmd_handlers[id];
    }
    func(buf, h->data);
    SetGP(gp);
end:
    asm volatile ("sync");
    asm volatile ("ei");
    return 0;
}
#endif

/* Write back the data cache lines of [ptr, ptr + size) (hand-written; leaves noreorder on for the return). */
void sceSifWriteBackDCache(void *ptr, int size)
{
    asm volatile (
        ".set\tnoreorder\n"
        "\tli\t$25, 0xFFFFFFC0\n"
        "\tblez\t$5, 9f\n"
        "\taddu\t$10, $4, $5\n"
        "\tand\t$8, $4, $25\n"
        "\taddiu\t$10, $10, -1\n"
        "\tand\t$9, $10, $25\n"
        "\tsubu\t$10, $9, $8\n"
        "\tsrl\t$11, $10, 6\n"
        "\taddiu\t$11, $11, 1\n"
        "\tandi\t$9, $11, 7\n"
        "\tbeqz\t$9, 2f\n"
        "\tsrl\t$10, $11, 3\n"
        "1:\tsync\n"
        "\tcache\t0x18, 0($8)\n"
        "\tsync\n"
        "\tnop\n"
        "\taddiu\t$9, $9, -1\n"
        "\tbgtz\t$9, 1b\n"
        "\taddiu\t$8, $8, 0x40\n"
        "2:\tbeqz\t$10, 9f\n"
        "3:\taddiu\t$10, $10, -1\n"
        "\tsync\n"
        "\tcache\t0x18, 0($8)\n"
        "\tsync\n"
        "\tcache\t0x18, 0x40($8)\n"
        "\tsync\n"
        "\tcache\t0x18, 0x80($8)\n"
        "\tsync\n"
        "\tcache\t0x18, 0xC0($8)\n"
        "\tsync\n"
        "\tcache\t0x18, 0x100($8)\n"
        "\tsync\n"
        "\tcache\t0x18, 0x140($8)\n"
        "\tsync\n"
        "\tcache\t0x18, 0x180($8)\n"
        "\tsync\n"
        "\tcache\t0x18, 0x1C0($8)\n"
        "\tsync\n"
        "\tbgtz\t$10, 3b\n"
        "\taddiu\t$8, $8, 0x200\n"
        "9:\tjr\t$31\n"
        "\tnop\n");
}
// SOURCE DATA: .data .rodata .bss
