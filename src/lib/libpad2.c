/* libpad2 (Sony EE SDK): controllers through the DBC (dbcman) driver; each socket's driver DMAs its state into one of
   two 128-byte buffers, the one with the higher count being the newest. */

typedef struct {
    unsigned long cmd : 14;
    unsigned long mode : 2;
    unsigned long type : 8;
    unsigned long size : 8;
    unsigned long pad : 32;
} sceDbcCmd;

typedef struct {
    char s[16];
} Name16;

typedef struct {
    unsigned int option;
    int p1;
    int p2;
    int p3;
    Name16 name;
} scePad2SocketParam;

typedef struct {
    unsigned int option;
    int number;
    int p1;
    int p2;
    int p3;
    Name16 name;
} sceDbcSocketParam;

/* The driver's DMA buffer (two per socket). */
typedef struct {
    unsigned char state;
    unsigned char b1;
    unsigned char len;
    unsigned char len2;
    int stat;
    int pad8[5];
    unsigned char data[0x60];
    int count;
} Pad2DmaBuf;

typedef struct {
    int valid;
    int type;
    int byte;
    int bit;
} Pad2ButtonInfo;

typedef struct {
    int valid;
    int linked;
    int dmaCount;
    int dep;
    Pad2DmaBuf *buf;
    Pad2ButtonInfo binfo[50];
} Pad2Info;

#include "include_asm.h"

void scePrintf(const char *fmt, ...);
void *memcpy(void *dst, const void *src, unsigned int n);
void *memset(void *dst, int c, unsigned int n);
char *strcpy(char *dst, const char *src);
void SyncDCache(void *start, void *end);
int sceDbcCreateSocket(sceDbcSocketParam *param, void *buf1, void *buf2);
int sceDbcDeleteSocket(int socket);
int sceDbcGetDepNumber(int socket);
int sceDbcReceiveData(int socket, int cmd, void *rdata, void *data);

int scePad2DeleteSocket(int socket);
int scePad2GetState(int socket);
static void scePad2InitDmaDBuff(int socket);
static int scePad2LinkDriver(int socket);
static Pad2DmaBuf *scePad2GetSide(int socket);
static Pad2DmaBuf *scePad2GetSide2(int socket);
static int scePad2CheckDma(int socket);
static int scePad2SetButtonOrder(unsigned char *p, Pad2ButtonInfo *bi);

static char __ps2_libinfo__[16] = "PsIIlibpad2 3100";
static int isInit = 0;
static long count_old = 0;
static Pad2Info pad2_info[16];

int scePad2Init(int mode)
{
    int i;

    isInit = 1;
    for (i = 0; i < 16; i++) {
        pad2_info[i].valid = 0;
        pad2_info[i].linked = 0;
        pad2_info[i].dmaCount = 0;
        pad2_info[i].dep = 0;
    }
    return 1;
}

int scePad2End(void)
{
    int i;
    int ret;

    isInit = 0;
    for (i = 0; i < 16; i++) {
        if (pad2_info[i].valid) {
            if ((ret = scePad2DeleteSocket(i)) < 0) {
                return ret;
            }
        }
    }
    return 1;
}

int scePad2CreateSocket(scePad2SocketParam *param, void *buf)
{
    sceDbcSocketParam dp;
    int socket;

    if ((unsigned int)buf & 0x3f) {
        scePrintf("libpad2: buffer addr is not 64 byte align. %08x\n", buf);
        return -1;
    }
    if (param) {
        dp.option = param->option;
        dp.p1 = param->p1;
        dp.p2 = param->p2;
        dp.p3 = param->p3;
        dp.name = param->name;
    } else {
        dp.option = 0;
        dp.p1 = 0;
        dp.p2 = 0;
        dp.p3 = 0;
        dp.name.s[0] = 0;
    }
    dp.number = 1;
    dp.option |= 1;
    socket = sceDbcCreateSocket(&dp, buf, (char *)buf + 0x80);
    if (socket < 0) return socket; /* MATCH: early return (an if around the stores schedules the pad2_info lui first) */
    pad2_info[socket].valid = 1;
    pad2_info[socket].buf = buf;
    scePad2InitDmaDBuff(socket);
    return socket;
}

int scePad2DeleteSocket(int socket)
{
    int ret = sceDbcDeleteSocket(socket);

    if (ret < 0) {
        return ret;
    }
    pad2_info[socket].valid = 0;
    pad2_info[socket].linked = 0;
    pad2_info[socket].dep = 0;
    return 1;
}

int scePad2Read(int socket, unsigned char *data)
{
    Pad2DmaBuf *side;

    if (!pad2_info[socket].valid) {
        return -1;
    }
    if (!pad2_info[socket].linked && scePad2LinkDriver(socket) < 0) {
        return -1;
    }
    side = scePad2GetSide(socket);
    if (side->len && side->data) {
        memcpy(data, side->data, side->len);
        scePad2SetButtonOrder(&side->data[side->len], pad2_info[socket].binfo);
    }
    if (side->stat == 0) {
        return -1;
    }
    return side->len;
}

int scePad2GetButtonProfile(int socket, unsigned char *profile)
{
    Pad2DmaBuf *side;

    scePad2GetState(socket);
    if (!pad2_info[socket].linked && scePad2LinkDriver(socket) < 0) {
        return -1;
    }
    side = scePad2GetSide2(socket);
    if (side->len2 && side->data) {
        memcpy(profile, &side->data[side->len], side->len2);
        scePad2SetButtonOrder(&side->data[side->len], pad2_info[socket].binfo);
    }
    if (side->stat == 0) {
        return -1;
    }
    return side->len2;
}

int scePad2GetState(int socket)
{
    unsigned char state;
    int rdata;
    union {
        sceDbcCmd b;
        long l;
    } cmd;
    Pad2DmaBuf *side;

    if (!pad2_info[socket].linked && scePad2LinkDriver(socket) < 0) {
        return 0;
    }
    if (scePad2CheckDma(socket)) {
        side = scePad2GetSide2(socket);
        if (!side->stat && scePad2LinkDriver(socket) < 0) {
            goto fail;
        }
        state = side->state;
    } else {
        cmd.b.cmd = 0xc;
        cmd.b.mode = 2;
        cmd.b.type = 1;
        cmd.b.size = 1;
        if (sceDbcReceiveData(socket, (int)cmd.l, &rdata, &state) < 0) {
        fail:
            pad2_info[socket].linked = 0;
            scePad2InitDmaDBuff(socket);
            return 0;
        }
    }
    return state;
}

/* Parked: 5 lines - button type kept in a1 (orig a3), movz instead of movn for the digital case. */
#if 0
int scePad2GetButtonInfo(int socket, unsigned char *data, int button)
{
    Pad2ButtonInfo bi = pad2_info[socket].binfo[button];
    int ret;
    unsigned char v;

    if (bi.valid) {
    switch (bi.type) {
    case 1:
        ret = ((data[bi.byte] >> bi.bit) & 1) ? 0 : bi.type;
        break;
    case 8:
        ret = data[bi.byte] >> bi.bit; /* ret as the first temporary (decomp-permuter): 12 -> 5 lines */
        v = ret;
        if (bi.bit) {
            bi.byte++;
            v |= data[bi.byte] << (bi.type - bi.bit);
        }
        ret = v;
        break;
    default:
        ret = -1;
        break;
    }
    return ret;
    }
    return -1;
}
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libpad2", scePad2GetButtonInfo);

static void scePad2InitDmaDBuff(int socket)
{
    Pad2DmaBuf *buf = pad2_info[socket].buf;
    int i;

    for (i = 0; i < 2; i++, buf++) {
        buf->state = 0;
        buf->count = 0;
        buf->b1 = 0;
        buf->len2 = 0;
        buf->len = 0;
        buf->stat = 0;
        memset(buf->data, 0xff, 0x20);
    }
}

static int scePad2LinkDriver(int socket)
{
    int dep = sceDbcGetDepNumber(socket);

    if (dep < 0) {
        return dep;
    }
    pad2_info[socket].dep = dep;
    pad2_info[socket].linked = 1;
    return dep;
}

static Pad2DmaBuf *scePad2GetSide(int socket)
{
    Pad2DmaBuf *side[2];

    side[0] = pad2_info[socket].buf;
    side[1] = side[0] + 1;
    SyncDCache(side[0], (char *)side[0] + 0xff);
    return side[side[0]->count < side[1]->count];
}

static Pad2DmaBuf *scePad2GetSide2(int socket)
{
    Pad2DmaBuf *side[2];

    side[0] = pad2_info[socket].buf;
    side[1] = side[0] + 1;
    SyncDCache(side[0], (char *)side[0] + 0x100);
    return side[side[0]->count < side[1]->count];
}

static int scePad2CheckDma(int socket)
{
    Pad2DmaBuf *side = scePad2GetSide(socket);

    if (side->count == 0 || side->count == pad2_info[socket].dmaCount) {
        return 0;
    }
    pad2_info[socket].dmaCount = side->count;
    return 1;
}

static int scePad2SetButtonOrder(unsigned char *p, Pad2ButtonInfo *bi)
{
    int bit = 0;
    int byte = 0;
    int i;
    int n = 0;

    for (i = 0; i < 40; i++, bi++) {
        if ((*p >> bit) & 1) {
            bi->valid = 1;
            bi->byte = byte;
            bi->bit = n;
            if ((unsigned int)(i - 16) < 16 || (unsigned int)(i - 35) < 4) {
                bi->type = 8;
                byte++;
            } else {
                n++;
                bi->type = 1;
                if ((n & 7) == 0) {
                    byte++;
                    n = 0;
                }
            }
        } else {
            bi->valid = 0;
            bi->byte = 0;
            bi->bit = 0;
        }
        bit++;
        if ((bit & 7) == 0) {
            p++;
            bit = 0;
        }
    }
    return 1;
}

static char *Pad2StateStr[] = { "NOLINK", "STABLE", "EXECCMD", "ERROR" };

void scePad2StateIntToStr(unsigned int state, char *str)
{
    if (state < 4) {
        strcpy(str, Pad2StateStr[state]);
        return;
    }
    strcpy(str, "");
}
// SOURCE DATA: .data .rodata .bss
