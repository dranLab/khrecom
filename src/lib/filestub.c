/* File I/O for the EE (sceOpen/sceRead/...): RPC calls to the IOP's file service (FILEIO), replies through SIF commands. */
#include "sdk/eekernel.h"
#include "sdk/sifrpc.h"
#include "sdk/sifdev.h"
#include "sdk/stdarg.h"
#include "include_asm.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long s64;
typedef unsigned long u64;
typedef volatile int vs32;
#define NULL ((void *)0)
#define ARRAY_COUNT(a) ((int)(sizeof(a) / sizeof((a)[0])))
#define ARRAY_COUNTU(a) (sizeof(a) / sizeof((a)[0]))

#define EBADF 9
#define EAGAIN 11
#define ENOMEM 12
#define EBUSY 16
#define ENODEV 19
#define EINVAL 22
#define E2BIG 7
#define SCE_ECALLMISS 0x10001
#define SCE_EVERSIONMISS 0x10004

typedef void (*sceSifCmdHandler)(void *, void *);
void sceSifInitRpc(unsigned int mode);
void sceSifAddCmdHandler(unsigned int cid, sceSifCmdHandler func, void *data);
void sceSifWriteBackDCache(const void *ptr, int size);
int SignalSema(int sema);
int DelayThread(int usec);
void *memcpy(void *dst, const void *src, unsigned int n);
void *memset(void *dst, int c, unsigned int n);
int memcmp(const void *a, const void *b, unsigned int n);


typedef struct _sceFsIob {
    s32 i_fd;
    u32 i_flag;
    s32 i_errno;
    void *i_private;
}_sceFsIob;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    s32 flag;
    u32 mode;
    char name[1024];
    s32 ee_fds;
} _sceFsOpenData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    s32 fd;
    s32 ee_fds;
} _sceFsCloseData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    s32 fd;
    s32 offset;
    s32 how;
    s32 ee_fds;
} _sceFsLseekData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    s32 fd;
    u32 addr;
    s32 size;
    u32 s32r_addr;
    s32 ee_fds;
} _sceFsReadData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    s32 fd;
    u32 addr;
    s32 size;
    s32 psize;
    u8 pdata[16];
    s32 ee_fds;
} _sceFsWriteData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    s32 fd;
    s32 cmd;
    u8 arg[1024];
    void *ret_argadr;
    s32 ret_argsiz;
    s32 arglen;
} _sceFsIoctlData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    char name[1024];
} _sceFsNameData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    void *addr;
} _sceFsAddrData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    void *addr;
    u8 name[1024];
} _sceFsGStatData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    u32 cbit;
    struct sce_stat stat;
    u8 name[1024];
} _sceFsCStatData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    char path[1024];
    char blkdevname[1024];
    u8 arg[1024];
    s32 arglen;
} _sceFsFormatData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    char oldpath[1024];
    char newpath[1024];
} _sceFsRenameData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    s32 fd;
    s32 flag;
    char path[1024];
} _sceFsSyncData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    s32 flag;
    char name[1024];
} _sceFsMkdirData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    char fsdevname[1024];
    char blkdevname[1024];
    u8 arg[1024];
    s32 flag;
    s32 arglen;
} _sceFsMountData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    s32 fd;
    s64 offset;
    s32 how;
    s32 ee_fds;
} _sceFsLseek64Data;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    char path[1024];
    u8 arg_buf[1024];
    s32 cmd;
    s32 arglen;
    void *ret_argadr;
    s32 ret_arglen;
} _sceFsDevctlData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    char existing[1024];
    char new[1024];
} _sceFsSymlinkData;

typedef struct {
    s32 ee_semid;
    u32 ee_retadr;
    u32 ee_retsiz;
    u32 bufsize;
    u32 bufaddr;
    char path[1024];
} _sceFsReadlinkData;

typedef struct {
    s32 psize;
    s32 ssize;
    u32 paddr;
    u32 saddr;
    u8 pdata[64];
    u8 sdata[64];
} _sceFsReadIntrData;

typedef struct {
    s32 ee_semid;
    u32 ee_retmod;
    u32 ee_retadr;
    u32 ee_retsiz;
    s32 data_top[4];
} _sceFsIntrRcvData;

typedef struct {
    s32 ee_semid;
    u32 ee_retmod;
    u32 ee_retadr;
    u32 ee_retsiz;
    s32 ee_ret;
    u32 ee_dentadr;
    struct sce_dirent dent;
} _sceFsIntrRcvDirData;

typedef struct {
    s32 ee_semid;
    u32 ee_retmod;
    u32 ee_retadr;
    u32 ee_retsiz;
    s32 ee_ret;
    u32 ee_readladr;
    u32 ee_readlsiz;
    char linkname[1024];
} _sceFsIntrRcvReadLData;

typedef struct {
    s32 ee_semid;
    u32 ee_retmod;
    u32 ee_retadr;
    u32 ee_retsiz;
    s32 ee_ret;
    u32 ret_argadr;
    u32 ret_argsiz;
    u8 arg[1024];
} _sceFsIntrRcvIoctlData;

typedef struct {
    s32 ee_semid;
    u32 ee_retmod;
    u32 ee_retadr;
    u32 ee_retsiz;
    s32 ee_ret;
    u32 ret_argadr;
    u32 ret_argsiz;
    u8 arg[1024];
} _sceFsIntrRcvDevctlData;

typedef struct {
    void (*sceFsPoffCbfunc)(void *);
    void *sceFsPoffCbdata;
} _sceFsPoffData;

// inferred
typedef struct {
    u32 bufsize;
    u32 bufnum;
} _sceFsIopBufData;

typedef union {
    _sceFsIopBufData iopBufData;
    _sceFsOpenData openData;
    _sceFsCloseData closeData;
    _sceFsLseekData lSeekData;
    _sceFsReadData readData;
    _sceFsWriteData writeData;
    _sceFsIoctlData ioctlData;
    _sceFsNameData nameData;
    _sceFsAddrData addrData;
    _sceFsGStatData gStatData;
    _sceFsCStatData cStatData;
    _sceFsFormatData formatData;
    _sceFsRenameData renameData;
    _sceFsSyncData syncData;
    _sceFsMkdirData mkdirData;
    _sceFsMountData mountData;
    _sceFsLseek64Data lSeek64Data;
    _sceFsDevctlData devctlData;
    _sceFsSymlinkData symlinkData;
    _sceFsReadlinkData readLinkData;
    _sceFsReadIntrData readIntrData;
} _sceFsData;

// inferred
typedef union {
    _sceFsReadIntrData readIntrData;
    _sceFsIntrRcvData rcvData;
    _sceFsIntrRcvDirData rcvDirData;
    _sceFsIntrRcvReadLData rcvReadLdata;
    _sceFsIntrRcvIoctlData rcvIoctlData;
    _sceFsIntrRcvDevctlData rcvDevctlData;
} _sceFsIntrData;



#define MAX_IOB_COUNT 32
#define MAX_ARG_SIZE 1024
#define UNCACHED(p) (((u32)(p) | 0x20000000))

typedef union _sceFsIntrBuf {
    _sceFsIntrData d;
} __attribute__((aligned(64))) _sceFsIntrBuf;

vs32 _sceFs_q[MAX_IOB_COUNT] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};
static s32 ee_retbuf_sel = 0;
static s32 _fs_init = 0;
static s32 _fs_rcv_bufdbl = 0;
static s32 _fs_semid = -1;
static s32 _fs_iob_semid = -1;
static s32 _fs_fsq_semid = -1;

static _sceFsData _send_data __attribute__((aligned(64)));
static s32 _rcv_data_rpc[2] __attribute__((aligned(64)));
static _sceFsIntrBuf _rcv_data_cmd[2];
static _sceFsIob _iob[MAX_IOB_COUNT];
static sceSifClientData _cd;
static char _fsversion[4];
static _sceFsPoffData _sif_FsRcv_Data __attribute__((aligned(64)));
static _sceFsPoffData _sif_FsPoff_Data __attribute__((aligned(64)));
static void *sceCbPoff_Gp __attribute__((aligned(64)));

static void _sceFsIobSemaMK(void) {
    struct SemaParam param;

    if (_fs_iob_semid == -1) {
        param.initCount = 1;
        param.maxCount = 1;
        param.option = (u32)"SceStdioIobSema";
        _fs_iob_semid = CreateSema(&param);
        param.option = (u32)"SceStdioQueSema";
        _fs_fsq_semid = CreateSema(&param);
    }
}

static int use_iob_cnt(void) {
    _sceFsIob* io;
    int cnt = 0;

    _sceFsIobSemaMK();
    WaitSema(_fs_iob_semid);
    for (io = &_iob[0]; io < &_iob[MAX_IOB_COUNT]; io++) {
        if (io->i_flag != 0) {
            cnt++;
        }
    }
    SignalSema(_fs_iob_semid);
    return cnt;
}

static _sceFsIob* new_iob(void) {
    _sceFsIob* io;

    _sceFsIobSemaMK();
    WaitSema(_fs_iob_semid);

    for (io = &_iob[0]; io < &_iob[MAX_IOB_COUNT]; io++) {
        if (io->i_flag == 0) {
            io->i_flag = 0x10000000;
            SignalSema(_fs_iob_semid);
            return io;
        }
    }

    SignalSema(_fs_iob_semid);
    return NULL;
}

static int sceFsSifCallRpc(sceSifClientData *cd, unsigned int fno, unsigned int mode, void *send, int ssize,
                           void *receive, int rsize, sceSifEndFunc end_func, void *end_para, int nowait) {
    int i;
    int delay = 1;
    int err;

    i = 0;
    do {
        err = sceSifCallRpc(cd, fno, mode, send, ssize, receive, rsize, end_func, end_para) < 0;
        if (!err || nowait) {
            break;
        }
        DelayThread(delay * 1000);
        if (delay < 127) {
            delay *= 2;
        }
        i++;
    } while (i <= 100);
    return err;
}

static _sceFsIob* get_iob(s32 fd) {
    _sceFsIob* ret;

    _sceFsIobSemaMK();
    WaitSema(_fs_iob_semid);

    if (ARRAY_COUNTU(_iob) <= fd) {
        SignalSema(_fs_iob_semid);
        return 0;
    }

    ret = &_iob[fd];
    SignalSema(_fs_iob_semid);
    return ret;
}

#if 0
void _sceFs_Rcv_Intr(void* pkt, void* data) {
    s32 ee_semid;
    u32 ee_retmod;
    void* ee_retadr;
    u32 ee_retsiz;
    s32 i;
    u8* cp;
    u8* ucp;
    _sceFsReadIntrData* ridp;
    void* r_addr;
    u32 r_size;
    u32 rp;
    s32 sel;

    sel = 0;
    if (_fs_rcv_bufdbl) {
        sel = ((s32*)pkt)[3];
    }
    ee_retbuf_sel = sel;
    rp = (u32)&_rcv_data_cmd[sel];
    rp |= 0x20000000;
    memcpy(&ee_semid, (void*)(rp + 0x0), sizeof(s32));
    memcpy(&ee_retmod, (void*)(rp + 0x4), sizeof(s32));
    memcpy(&ee_retadr, (void*)(rp + 0x8), sizeof(s32));
    memcpy(&ee_retsiz, (void*)(rp + 0xc), sizeof(s32));

    if (ee_semid >= 0) {
        memcpy(ee_retadr, (void*)(rp + 0x10), ee_retsiz);
    }

    switch (ee_retmod) {
        case 0x2:
            ridp = (_sceFsReadIntrData *)(rp + 0x14);
            if (ridp->psize > 0) {
                cp = (u8*)ridp->paddr;
                ucp = (u8*)UNCACHED(cp);
                for (i = 0; i < ridp->psize; i++) {
                    cp[i] = ucp[i] = ridp->pdata[i];
                }
            }
            if (ridp->ssize > 0) {
                cp = (u8*)ridp->saddr;
                ucp = (u8*)UNCACHED(cp);
                for (i = 0; i < ridp->ssize; i++) {
                    cp[i] = ucp[i] = ridp->sdata[i];
                }
            }
            break;
        case 0xb:
            if (*(s32*)ee_retadr > 0) {
                memcpy(&r_addr, (void*)(rp + 0x14), sizeof(s32));
                memcpy(r_addr, (void*)(rp + 0x18), sizeof(struct sce_dirent));
            }
            break;
        case 0xc:
            memcpy(&r_addr, (void*)(rp + 0x14), sizeof(s32));
            memcpy(r_addr, (void*)(rp + 0x18), 0x40);
            break;
        case 0x17:
        case 0x19:
        case 0x1a:
            memcpy(&r_addr, (void*)(rp + 0x14), sizeof(s32));
            memcpy(&r_size, (void*)(rp + 0x18), sizeof(s32));
            if (r_size > 0x400) {
                r_size = 0x400;
            }
            memcpy(r_addr, (void*)(rp + 0x1c), r_size);
            break;
    }

    if (ee_semid < 0) {
        ee_semid = -ee_semid;
        for (i = 0; i < ARRAY_COUNT(_sceFs_q); i++) {
            if (_sceFs_q[i] == ee_semid) {
                _sceFs_q[i] = -1;
                break;
            }
        }
    } else {
        iSignalSema(ee_semid);
    }
}
#endif
void _sceFs_Rcv_Intr(void* pkt, void* data);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/filestub", _sceFs_Rcv_Intr);
/* _sceFs_Rcv_Intr's jump table (the function is still assembly). */
asm(".rdata\n.align 3\njtbl_004FEEE0:\n"
    ".word .L00388848\n.word .L00388AB0\n.word .L00388AB0\n.word .L00388AB0\n.word .L00388AB0\n"
    ".word .L00388AB0\n.word .L00388AB0\n.word .L00388AB0\n.word .L00388AB0\n.word .L003888F0\n"
    ".word .L003889CC\n.word .L00388AB0\n.word .L00388AB0\n.word .L00388AB0\n.word .L00388AB0\n"
    ".word .L00388AB0\n.word .L00388AB0\n.word .L00388AB0\n.word .L00388AB0\n.word .L00388AB0\n"
    ".word .L00388AB0\n.word .L00388A68\n.word .L00388AB0\n.word .L00388A68\n.word .L00388A68\n"
    ".text\n");

static void _sceFsSemInit(void) {
    struct SemaParam param;

    if (_fs_semid == -1) {
        param.initCount = 1;
        param.maxCount = 1;
        param.option = (u32)"SceStdioFioSema";
        _fs_semid = CreateSema(&param);
    }
}

static s32 _sceFsWaitS(s32 fno) {
    _sceFsSemInit();
    WaitSema(_fs_semid);
    return 0;
}

static void _sceFsSigSema(void) {
    SignalSema(_fs_semid);
}

void *scePowerOffHandler(void (*func)(void*), void* addr) {
    void *ret;
    _sceFsPoffData* pd;
    int oldei;
    register void *gp asm("$28");

    pd = &_sif_FsPoff_Data;
    _sceFsWaitS(0x1b);
    if (_fs_init == 0) {
        sceFsInit();
    }
    oldei = DIntr();
    ret = (void *)_sif_FsPoff_Data.sceFsPoffCbfunc;
    pd->sceFsPoffCbdata = addr;
    pd->sceFsPoffCbfunc = func;
    sceCbPoff_Gp = gp;
    if (oldei) {
        EIntr();
    }
    _sceFsSigSema();
    return ret;
}

static void _sceFs_Poff_Intr(void* pkt, void* data) {
    _sceFsPoffData* p;
    void *gp;

    p = (_sceFsPoffData*)data;
    if (p->sceFsPoffCbfunc != NULL) {
        asm volatile("move %0, $gp\n\tlw $gp, %1" : "=r"(gp) : "m"(sceCbPoff_Gp));
        p->sceFsPoffCbfunc(p->sceFsPoffCbdata);
        asm volatile("move $gp, %0" : : "r"(gp));
    }
    asm volatile("sync.l\n\tei");
}

int sceFsInit(void) {
    static u32 rcv_adr[2];
    s32 i;
    s32 oldei;
    _sceFsIob* io;
    _sceFsPoffData* pd;
    s32 bufmode;

    pd = &_sif_FsPoff_Data;
    sceSifInitRpc(0);
    pd->sceFsPoffCbfunc = NULL;
    pd->sceFsPoffCbdata = NULL;
    oldei = DIntr();
    sceSifAddCmdHandler(0x80000011, _sceFs_Rcv_Intr, &_sif_FsRcv_Data);
    sceSifAddCmdHandler(0x80000013, _sceFs_Poff_Intr, pd);
    if (oldei) {
        EIntr();
    }

    while (1) {
        if (sceSifBindRpc(&_cd, 0x80000001, 0) < 0) {
            return -EINVAL;
        }
        if (_cd.serve != NULL) {
            break;
        }
        for (i = 0x100000; i != -1; i--) {
        }
    }

    _sceFsIobSemaMK();
    WaitSema(_fs_iob_semid);
    for (io = &_iob[0]; io < &_iob[MAX_IOB_COUNT]; io++) {
        io->i_flag = 0;
    }
    SignalSema(_fs_iob_semid);

    rcv_adr[0] = (u32)&_rcv_data_cmd[0];
    rcv_adr[1] = (u32)&_rcv_data_cmd[1];
    if (sceFsSifCallRpc(&_cd, 0xff, 0, rcv_adr, sizeof(rcv_adr), _rcv_data_rpc, sizeof(_rcv_data_rpc), NULL, NULL, 0) < 0) {
        return -SCE_ECALLMISS;
    }
    memcpy(_fsversion, (void*)UNCACHED(&_rcv_data_rpc[0]), sizeof(_fsversion));
    memcpy(&bufmode, (void*)UNCACHED(&_rcv_data_rpc[1]), sizeof(bufmode));
    _fs_rcv_bufdbl = bufmode == 2;
    _fs_init = 1;
    return 0;
}

static char* _fswildcard = "....";

extern char __ps2_klibinfo__[];

static s32 _fs_version(void) {
    s32 ret = 0;
    char* libver;

    libver = &__ps2_klibinfo__[12];
    if (memcmp(_fsversion, libver, 4) != 0 && memcmp(_fsversion, _fswildcard, 4) != 0) {
        ret = memcmp(libver, _fswildcard, 4) != 0;
    }
    return ret;
}

s32 sceFsReset(void) {
    _fs_init = 0;
    memset(_fsversion, 0, sizeof(_fsversion));
    return 0;
}

int sceFsSetIopBuf(unsigned int bufsize, unsigned int bufnum) {
    int ret;

    _sceFsWaitS(0xfe);
    if (_fs_init == 0) {
        sceFsInit();
    }
    if (_fs_version() != 0) {
        _sceFsSigSema();
        return -SCE_EVERSIONMISS;
    }
    if (use_iob_cnt() != 0) {
        _sceFsSigSema();
        return -EBUSY;
    }
    if ((bufsize & 0x3fff) != 0 || bufsize == 0 || bufsize > 0xfffff || bufnum == 0 || bufnum > 32) {
        _sceFsSigSema();
        return -EINVAL;
    }
    _send_data.iopBufData.bufsize = bufsize;
    _send_data.iopBufData.bufnum = bufnum;
    if (sceFsSifCallRpc(&_cd, 0xfe, 0, &_send_data, sizeof(_sceFsIopBufData), _rcv_data_rpc, 4, NULL, NULL, 0) < 0) {
        _sceFsSigSema();
        return -EAGAIN;
    }
    ret = *(s32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    return ret;
}

int sceFsSetIopPrio(int prio) {
    int ret;

    _sceFsWaitS(0xfd);
    if (_fs_init == 0) {
        sceFsInit();
    }
    if (_fs_version() != 0) {
        _sceFsSigSema();
        return -SCE_EVERSIONMISS;
    }
    if (use_iob_cnt() != 0) {
        _sceFsSigSema();
        return -EBUSY;
    }
    _send_data.iopBufData.bufsize = prio;
    if (sceFsSifCallRpc(&_cd, 0xfd, 0, &_send_data, sizeof(_sceFsIopBufData), _rcv_data_rpc, 4, NULL, NULL, 0) < 0) {
        _sceFsSigSema();
        return -EAGAIN;
    }
    ret = *(s32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    return ret;
}

int sceOpen(const char* filename, int flag, ...) {
    u32 mode;
    s32 nsize;
    s32 ret;
    s32 retfd;
    _sceFsOpenData* od;
    _sceFsIob* io;
    struct SemaParam sparam;
    s32 i;
    va_list arg;
    s32 semaId;

    od = &_send_data.openData;

    _sceFsWaitS(0);
    if (_fs_init == 0) {
        sceFsInit();
    }
    if (_fs_version() != 0) {
        _sceFsSigSema();
        return -SCE_EVERSIONMISS;
    }
    io = new_iob();
    if (io == NULL) {
        _sceFsSigSema();
        return -ENODEV;
    }

    va_start(arg, flag);
    mode = va_arg(arg, int);
    va_end(arg);

    for (i = 0; i < MAX_ARG_SIZE && (od->name[i] = filename[i]) != 0; i++) {
    }
    if (i == MAX_ARG_SIZE) {
        od->name[MAX_ARG_SIZE - 1] = 0;
    }

    nsize = io - _iob;
    od->flag = flag & 0x7fffffff;
    od->ee_fds = nsize;
    od->mode = mode;
    sparam.maxCount = 1;
    sparam.initCount = 0;
    sparam.option = (u32)"SceStdioOpenSema";
    semaId = CreateSema(&sparam);
    od->ee_semid = semaId;
    od->ee_retadr = (u32)&retfd;
    od->ee_retsiz = sizeof(retfd);

    if (sceFsSifCallRpc(&_cd, 0, 0, &_send_data, sizeof(_sceFsOpenData), _rcv_data_rpc, 4, NULL, NULL, 0) < 0) {
        DeleteSema(semaId);
        _sceFsSigSema();
        return -EAGAIN;
    }

    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0) {
        DeleteSema(semaId);
        return -EAGAIN;
    }

    WaitSema(semaId);
    DeleteSema(semaId);
    if (retfd < 0) {
        WaitSema(_fs_iob_semid);
        io->i_flag = 0;
        SignalSema(_fs_iob_semid);
        return retfd;
    }

    ret = nsize;
    WaitSema(_fs_iob_semid);
    io->i_fd = retfd;
    io->i_flag |= flag;
    SignalSema(_fs_iob_semid);
    return ret;
}

int sceClose(int fd) {
    _sceFsCloseData* cd;
    _sceFsIob* io;
    s32 ret;
    s32 nsize;
    s32 ret_close;
    struct SemaParam sparam;
    s32 semaid;

    cd = &_send_data.closeData;
    
    io = get_iob(fd);
    _sceFsWaitS(0x1);
    if (_fs_init == 0 || io == NULL || io->i_flag == 0) {
        _sceFsSigSema();
        return -EBADF;
    }
    
    nsize = io - _iob;
    cd->fd = io->i_fd;
    cd->ee_fds = nsize;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioCloseSema";
    semaid = CreateSema(&sparam);
    cd->ee_semid = semaid;
    cd->ee_retadr = (u32)&ret_close;
    cd->ee_retsiz = sizeof(ret_close);
    ret = sceFsSifCallRpc(&_cd, 0x1, 0x0, &_send_data, sizeof(_sceFsCloseData), _rcv_data_rpc, 4, NULL, NULL, 0);
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    io->i_flag = 0x0;
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }
        
    WaitSema(semaid);
    DeleteSema(semaid);
    if (ret_close < 0) {
        return ret_close;
    }
    
    return 0; // SCE_OK
}

int sceLseek(int fd, int offset, int how) {
    _sceFsLseekData* ld;
    s32 ret_lseek;
    s32 ret1;
    s32 i;
    s32 flag;
    _sceFsIob* io;
    s32 ret;
    struct SemaParam sparam;
    s32 semaid;

    ld = &_send_data.lSeekData;
    
    io = get_iob(fd);
    _sceFsWaitS(0x4);
    if (_fs_init == 0 || io == NULL || io->i_flag == 0) {
        _sceFsSigSema();
        return -EBADF;
    }
    
    flag = io->i_flag;
    ld->fd = io->i_fd;
    ld->offset = offset;
    ld->how = how;
    ld->ee_fds = io - _iob;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioLseekSema";
    semaid = CreateSema(&sparam);
    ld->ee_semid = semaid;
    ld->ee_retadr = (u32)&ret_lseek;
    ld->ee_retsiz = sizeof(ret_lseek);
        
    if ((flag & 0x8000) != 0) {
        WaitSema(_fs_fsq_semid);
        for (i = 0; i < 32; i++) {
            if (_sceFs_q[i] == -1) {
                _sceFs_q[i] = ld->ee_semid;
                ld->ee_semid = -ld->ee_semid;
                break;
            }
        }
        SignalSema(_fs_fsq_semid);
    }
    
    ret = sceFsSifCallRpc(&_cd, 0x4, 0x0, &_send_data, sizeof(_sceFsLseekData), _rcv_data_rpc, 4, NULL, NULL, flag & 0x8000);
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }

    if ((flag & 0x8000) != 0) {
        DeleteSema(semaid);
        return 0; // SCE_OK
    } 
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_lseek;
}

int sceRead(int fd, void* addr, int size) {
    _sceFsReadData* rd;
    _sceFsIob* io;
    s32 ret;
    s32 ret_read;
    s32 cnt0;
    s32 i;
    s32 flag;
    struct SemaParam sparam;
    s32 semaid;

    rd = &_send_data.readData;
    
    io = get_iob(fd);
    _sceFsWaitS(0x2);
    if (_fs_init == 0 || io == NULL || io->i_flag == 0) {
        _sceFsSigSema();
        return -EBADF;
    }
    
    flag = io->i_flag;
    rd->fd = io->i_fd;
    rd->addr = (u32)addr;
    rd->size = size;
    rd->ee_fds = io - _iob;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioReadSema";
    semaid = CreateSema(&sparam);
    rd->ee_semid = semaid;
    rd->ee_retadr = (u32)&ret_read;
    rd->ee_retsiz = sizeof(ret_read);
        
    if ((flag & 0x8000) != 0) {
        WaitSema(_fs_fsq_semid);
        for (i = 0; i < 32; i++) {
            if (_sceFs_q[i] == -1) {
                _sceFs_q[i] = rd->ee_semid;
                rd->ee_semid = -rd->ee_semid;
                break;
            }
        }
        SignalSema(_fs_fsq_semid);
    }
    if ((flag & 0x20000000) == 0x0) {
        sceSifWriteBackDCache(addr, size);
    }
    sceSifWriteBackDCache(rd, sizeof(_sceFsReadData));
    ret = sceFsSifCallRpc(&_cd, 0x2, 0x0, &_send_data, sizeof(_sceFsReadData), _rcv_data_rpc, 4, NULL, NULL, flag & 0x8000);
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }

    if ((flag & 0x8000) != 0) {
        DeleteSema(semaid);
        return 0; // SCE_OK
    } 
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_read;
}

int sceWrite(int fd, const void* buf, int nbyte) {
    _sceFsWriteData* wd;
    _sceFsIob* io;
    s32 i;
    s32 cnt0;
    s32 psize;
    s32 ret;
    s32 ret_write;
    struct SemaParam sparam;
    s32 flag;
    s32 semaid;
    void* p;

    wd = &_send_data.writeData;
    
    io = get_iob(fd);
    _sceFsWaitS(0x3);
    if (_fs_init == 0 || io == NULL || io->i_flag == 0) {
        _sceFsSigSema();
        return -EBADF;
    }
    
    flag = io->i_flag;
    wd->fd = io->i_fd;
    wd->size = nbyte;
    wd->addr = (u32)buf;
    wd->ee_fds = io - _iob;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioWriteSema";
    semaid = CreateSema(&sparam);
    wd->ee_semid = semaid;
    wd->ee_retadr = (u32)&ret_write;
    wd->ee_retsiz = sizeof(ret_write);
    
    if ((flag & 0x8000) != 0) {
        WaitSema(_fs_fsq_semid);
        for (cnt0 = 0; cnt0 < 32; cnt0++) {
            if (_sceFs_q[cnt0] == -1) {
                _sceFs_q[cnt0] = wd->ee_semid;
                wd->ee_semid = -wd->ee_semid;
                break;
            }
        }
        SignalSema(_fs_fsq_semid);
    }

    if (((u32)buf & 15) == 0x0) {
        psize = 0x0;
    } else {
        int temp_v1;
        temp_v1 = ((u32)buf - 0x10);
        psize = ((u32)buf / 16) * 16 - temp_v1;
    }
    
    if (nbyte < psize) {
        psize = nbyte;
    }
    if ((flag & 0x20000000) == 0x0) {
        sceSifWriteBackDCache(buf, nbyte);
    }
    
    wd->psize = psize;
    buf = (void*)UNCACHED(buf);
    for (i = 0; i < psize; i++) {
        wd->pdata[i] = *(char*)(buf + i);
    }    
    
    ret = sceFsSifCallRpc(&_cd, 0x3, 0x0, &_send_data, sizeof(_sceFsWriteData), _rcv_data_rpc, 4, NULL, NULL, flag & 0x8000);
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }

    if ((flag & 0x8000) != 0) {
        DeleteSema(semaid);
        return 0; // SCE_OK
    } 
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_write;
}

int sceIoctl(int fd, int cmd, void* arg) {
    static u32* ip0;
    _sceFsIoctlData* id;
    _sceFsIob* io;
    s32 ret;
    s32 ret_ioctl;
    s32 cnt0;
    s32 wait;
    struct SemaParam sparam;
    s32 i;
    s32 semaid;
    s32 sz;
    s32 temp;

    ret = 1;
    id = &_send_data.ioctlData;

    io = get_iob(fd);
    _sceFsWaitS(5);
    ip0 = arg;
    if (_fs_init == 0x0) {
        sceFsInit();
    }

    if ((io == NULL) || (io->i_flag == 0x0)) {
        _sceFsSigSema();
        return -EBADF;
    }

    id->ret_argadr = 0;
    id->ret_argsiz = 0;
    
    switch(cmd) {
        case 1:
            WaitSema(_fs_fsq_semid);
            i = 0;
            for (; i < 32 && (_sceFs_q[i] == -1); i++) {
            }
            if (i == 32) {
                *ip0 = 0x0;
            }
            else {
                *ip0 = 0x1;
            }
            SignalSema(_fs_fsq_semid);
            _sceFsSigSema();
            return 0x0;
        break;
        case 2:
            *(u32*)arg = *(u32*)UNCACHED(&_rcv_data_cmd[ee_retbuf_sel].d.rcvIoctlData.ee_ret);
            _sceFsSigSema();
            return 0x0;
        break;
        case 3:
            *(u64*)arg = *(u64*)UNCACHED(&_rcv_data_cmd[ee_retbuf_sel].d.rcvIoctlData.ee_ret);
            _sceFsSigSema();
            return 0x0;
        break;
    }
    
    id->fd = io->i_fd;
    id->cmd = cmd;
    
    if (arg == NULL) {
        id->arglen = 0;
    } else {
        id->arglen = 0x400;
        memcpy(&id->arg, arg, 0x400);
    }
    
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioIoctlSema";
    semaid = CreateSema(&sparam);
    id->ee_semid = semaid;
    id->ee_retadr = (u32)&ret_ioctl;
    id->ee_retsiz = sizeof(ret_ioctl);
    
    sceSifWriteBackDCache(&_send_data, sizeof(_sceFsIoctlData));
    ret = sceFsSifCallRpc(&_cd, 0x5, 0x0, &_send_data, sizeof(_sceFsIoctlData), _rcv_data_rpc, 4, NULL, NULL, 0);
    
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    temp = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (temp == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_ioctl;
}

int sceIoctl2(int fd, int cmd, const void* arg, unsigned int arglen, void* bufp,  unsigned int buflen) {
    _sceFsIoctlData* id;
    _sceFsIob* io;
    s32 ret;
    s32 ret_ioctl;
    struct SemaParam sparam;
    s32 semaid;
    
    id = &_send_data.ioctlData;

    io = get_iob(fd);
    _sceFsWaitS(0x1A);
    if (_fs_init == 0x0) {
        sceFsInit();
    }

    if ((io == NULL) || (io->i_flag == 0x0)) {
        _sceFsSigSema();
        return -EBADF;
    }
    
    if ((arglen > MAX_ARG_SIZE) || (buflen > MAX_ARG_SIZE)) {
        _sceFsSigSema();
        return -EINVAL;
    }
    
    if (arg == NULL) {
        id->arglen = 0;
    } else {
        memcpy(&id->arg, arg, arglen);
    }
    
    id->fd = io->i_fd;
    id->cmd = cmd;
    id->arglen = arglen;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioIoctl2Sema";
    semaid = CreateSema(&sparam);
    id->ee_semid = semaid;
    id->ee_retadr = (u32)&ret_ioctl;
    id->ee_retsiz = sizeof(ret_ioctl);
    id->ret_argadr = bufp;
    id->ret_argsiz = buflen;
    
    sceSifWriteBackDCache(&_send_data, sizeof(_sceFsIoctlData));
    ret = sceFsSifCallRpc(&_cd, 0x1a, 0x0, &_send_data, sizeof(_sceFsIoctlData), _rcv_data_rpc, 4, NULL, NULL, 0);
    
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_ioctl;
}

static s32 _sceCallCode(const char* name, u32 callcode) {
    _sceFsNameData* cc;
    s32 nsize;
    s32 ret;
    s32 ret_code;
    struct SemaParam sparam;
    s32 semaid;

    cc = &_send_data.nameData;
    
    _sceFsWaitS(callcode);
    if (_fs_init == 0x0) {
        sceFsInit();
    }
    
    for (nsize = 0; nsize < MAX_ARG_SIZE && (cc->name[nsize] = name[nsize]) != 0; nsize++) { }
    if (nsize == MAX_ARG_SIZE) {
        cc->name[MAX_ARG_SIZE-1] = 0x0;
        nsize = MAX_ARG_SIZE-1;
    }
    
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioCCodeSema";
    semaid = CreateSema(&sparam);
    cc->ee_semid = semaid;
    cc->ee_retadr = (u32)&ret_code;
    cc->ee_retsiz = sizeof(ret_code);
    
    ret = sceFsSifCallRpc(&_cd, callcode, 0x0, &_send_data, nsize + 0xC + 1, _rcv_data_rpc, 4, NULL, NULL, 0);
    
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_code;
}

/**
 * @brief Delete file
 * @param filename File pathname (includes device name and ':')
 * @return 0 on success, -1 * error code on failure 
 */
s32 sceRemove(const char* filename) {
    return _sceCallCode(filename, 6);
}

int sceMkdir(const char* name, int flag) {
    _sceFsMkdirData* mkd;
    s32 nsize;
    s32 ret;
    s32 ret_mkdir;
    struct SemaParam sparam;
    s32 semaid;

    mkd = &_send_data.mkdirData;
    
    _sceFsWaitS(0x7);
    if (_fs_init == 0x0) {
        sceFsInit();
    }

    for (nsize = 0; nsize < MAX_ARG_SIZE && (mkd->name[nsize] = name[nsize]) != 0; nsize++) { }
    if (nsize == MAX_ARG_SIZE) {
        mkd->name[MAX_ARG_SIZE-1] = 0x0;
        nsize = MAX_ARG_SIZE-1;
    }
    
    mkd->flag = flag;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioMkdirSema";
    semaid = CreateSema(&sparam);
    mkd->ee_semid = semaid;
    mkd->ee_retadr = (u32)&ret_mkdir;
    mkd->ee_retsiz = sizeof(ret_mkdir);
    
    ret = sceFsSifCallRpc(&_cd, 0x7, 0x0, &_send_data, nsize + 0x10 + 1, _rcv_data_rpc, 4, NULL, NULL, 0);
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_mkdir;
}

/**
 * @brief Delete directory
 * @param dirname Directory pathname (includes device name and ':')
 * @return 0 on success, -1 * error code on failure
 */
s32 sceRmdir(const char* dirname) {
    return _sceCallCode(dirname, 8);
}

int sceFormat(const char* path, const char* blkdevname, void* arg, int arglen) {
    _sceFsFormatData* fd;
    s32 nsize;
    s32 ret;
    s32 ret_format;
    struct SemaParam sparam;
    s32 semaid;

    fd = &_send_data.formatData;
    
    _sceFsWaitS(0xE);
    if (_fs_init == 0x0) {
        sceFsInit();
    }

    for (nsize = 0; nsize < MAX_ARG_SIZE && (fd->path[nsize] = path[nsize]) != 0; nsize++) { }
    if (nsize == MAX_ARG_SIZE) {
        fd->path[MAX_ARG_SIZE-1] = 0x0;
    }

    if (blkdevname == NULL) {
        fd->blkdevname[0] = 0x0;
    } else {   
        for (nsize = 0; nsize < MAX_ARG_SIZE && (fd->blkdevname[nsize] = blkdevname[nsize]) != 0; nsize++) { }
        if (nsize == MAX_ARG_SIZE) {
            fd->blkdevname[MAX_ARG_SIZE-1] = 0x0;
        }
    }

    if (arglen > MAX_ARG_SIZE) {
        _sceFsSigSema();
        return -E2BIG;
    }

    for (nsize = 0; nsize < arglen; nsize++) {
        fd->arg[nsize] = ((char*)arg)[nsize];
    }

    fd->arglen = arglen;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioFormatSema";
    semaid = CreateSema(&sparam);
    fd->ee_semid = semaid;
    fd->ee_retadr = (u32)&ret_format;
    fd->ee_retsiz = sizeof(ret_format);

    sceSifWriteBackDCache(&_send_data, sizeof(_sceFsFormatData));
    ret = sceFsSifCallRpc(&_cd, 0xE, 0x0, &_send_data, sizeof(_sceFsFormatData), _rcv_data_rpc, 4, NULL, NULL, 0);
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_format;
}

int sceAddDrv(void* addr) {
    _sceFsAddrData* id;
    s32 nsize;
    s32 ret;
    s32 ret_adddrv;
    struct SemaParam sparam;
    s32 semaid;

    id = &_send_data.addrData;
    
    _sceFsWaitS(0xF);
    if (_fs_init == 0x0) {
        sceFsInit();
    }

    id->addr = addr;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioAddDrvSema";
    semaid = CreateSema(&sparam);
    id->ee_semid = semaid;
    id->ee_retadr = (u32)&ret_adddrv;
    id->ee_retsiz = sizeof(ret_adddrv);

    ret = sceFsSifCallRpc(&_cd, 0xF, 0x0, &_send_data, sizeof(_sceFsAddrData), _rcv_data_rpc, 4, NULL, NULL, 0);
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_adddrv;
}

s32 sceDelDrv(const char* name) {
    return _sceCallCode(name, 16);
}

int sceDopen(const char* name) {
    _sceFsIob* io;
    s32 ret;
    
    _sceFsWaitS(0x9);
    if (_fs_init == 0x0) {
        sceFsInit();
    }
    
    _sceFsSigSema();
    io = new_iob();
    if (io == NULL) {
        return -ENODEV;
    }
    
    ret = _sceCallCode(name, 0x9);
    if (ret < 0x0) {
        WaitSema(_fs_iob_semid);
        io->i_flag = 0x0;
        SignalSema(_fs_iob_semid);
        return ret;
    }
        
    WaitSema(_fs_iob_semid);
    io->i_fd = ret;
    ret = io - _iob;
    SignalSema(_fs_iob_semid);
    
    return ret;
}

int sceDclose(int fd) {
    _sceFsCloseData* cd;
    _sceFsIob* io;
    s32 ret;
    s32 ret_dclose;
    struct SemaParam sparam;
    s32 semaid;
    s32 temp;

    io = get_iob(fd);
    cd = &_send_data.closeData;
    
    _sceFsWaitS(0xA);
    if (_fs_init == 0 || io == NULL || io->i_flag == 0) {
        _sceFsSigSema();
        return -EBADF;
    }

    cd->fd = io->i_fd;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioDcloseSema";
    semaid = CreateSema(&sparam);
    cd->ee_semid = semaid;
    cd->ee_retadr = (u32)&ret_dclose;
    cd->ee_retsiz = sizeof(ret_dclose);

    ret = sceFsSifCallRpc(&_cd, 0xA, 0x0, &_send_data, sizeof(_sceFsCloseData), _rcv_data_rpc, 4, NULL, NULL, 0);
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    io->i_flag = 0;
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }

    WaitSema(semaid);
    DeleteSema(semaid);
    temp = -1;
    if (temp < ret_dclose) {
        return 0;
    }
    return ret_dclose;
}

int sceDread(int fd, struct sce_dirent* dp) {
    _sceFsReadData* rd;
    _sceFsIob* io;
    s32 ret;
    s32 ret_dread;
    struct SemaParam sparam;
    s32 semaid;

    io = get_iob(fd);
    rd = &_send_data.readData;
    
    _sceFsWaitS(0xB);
    if (_fs_init == 0 || io == NULL || io->i_flag == 0) {
        _sceFsSigSema();
        return -EBADF;
    }

    rd->fd = io->i_fd;
    rd->addr = (u32)dp;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioDreadSema";
    semaid = CreateSema(&sparam);
    rd->ee_semid = semaid;
    rd->ee_retadr = (u32)&ret_dread;
    rd->ee_retsiz = sizeof(ret_dread);

    ret = sceFsSifCallRpc(&_cd, 0xB, 0x0, &_send_data, sizeof(_sceFsReadData), _rcv_data_rpc, 4, NULL, NULL, 0);
    if (ret < 0x0) {
        WaitSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }

    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_dread;
}

int sceGetstat(const char* name, struct sce_stat* dp) {
    _sceFsGStatData* sd;
    s32 ret;
    s32 nsize;
    s32 ret_getstat;
    struct SemaParam sparam;
    s32 semaid;

    sd = &_send_data.gStatData;
    
    _sceFsWaitS(0xC);
    if (_fs_init == 0x0) {
        sceFsInit();
    }

    for (nsize = 0; nsize < MAX_ARG_SIZE && (sd->name[nsize] = name[nsize]) != 0; nsize++) { }
    if (nsize == MAX_ARG_SIZE) {
        sd->name[MAX_ARG_SIZE-1] = 0x0;
        nsize = MAX_ARG_SIZE-1;
    }

    sd->addr = dp;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioGetstatSema";
    semaid = CreateSema(&sparam);
    sd->ee_semid = semaid;
    sd->ee_retadr = (u32)&ret_getstat;
    sd->ee_retsiz = sizeof(ret_getstat);

    ret = sceFsSifCallRpc(&_cd, 0xC, 0x0, &_send_data, nsize + 0x10 + 1, _rcv_data_rpc, 4, NULL, NULL, 0);
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }

    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_getstat;
}

int sceChstat(const char* name, struct sce_stat* buf, unsigned int cbit) {
    _sceFsCStatData* cd;
    s32 ret;
    s32 nsize;
    s32 ret_chstat;
    struct SemaParam sparam;
    s32 semaid;

    cd = &_send_data.cStatData;
    
    _sceFsWaitS(0xD);
    if (_fs_init == 0x0) {
        sceFsInit();
    }

    for (nsize = 0; nsize < MAX_ARG_SIZE && (cd->name[nsize] = name[nsize]) != 0; nsize++) { }
    if (nsize == MAX_ARG_SIZE) {
        cd->name[MAX_ARG_SIZE-1] = 0x0;
        nsize = MAX_ARG_SIZE-1;
    }

    cd->stat = *buf;
    cd->cbit = cbit;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioChstatSema";
    semaid = CreateSema(&sparam);
    cd->ee_semid = semaid;
    cd->ee_retadr = (u32)&ret_chstat;
    cd->ee_retsiz = sizeof(ret_chstat);
    
    sceSifWriteBackDCache(&_send_data, sizeof(_sceFsCStatData));
    ret = sceFsSifCallRpc(&_cd, 0xD, 0x0, &_send_data, nsize + 0x50 + 1, _rcv_data_rpc, 4, NULL, NULL, 0);
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }

    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_chstat;
}

int sceRename(const char* oldname, const char* newname) {
    _sceFsRenameData* rd;
    s32 ret;
    s32 nsize;
    s32 ret_chstat;
    struct SemaParam sparam;
    s32 semaid;

    rd = &_send_data.renameData;
    
    _sceFsWaitS(0x11);
    if (_fs_init == 0x0) {
        sceFsInit();
    }

    for (nsize = 0; nsize < MAX_ARG_SIZE && (rd->oldpath[nsize] = oldname[nsize]) != 0; nsize++) { }
    if (nsize == MAX_ARG_SIZE) {
        rd->oldpath[MAX_ARG_SIZE-1] = 0x0;
    }
    
    for (nsize = 0; nsize < MAX_ARG_SIZE && (rd->newpath[nsize] = newname[nsize]) != 0; nsize++) { }
    if (nsize == MAX_ARG_SIZE) {
        rd->newpath[MAX_ARG_SIZE-1] = 0x0;
    }

    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioRenameSema";
    semaid = CreateSema(&sparam);
    rd->ee_semid = semaid;
    rd->ee_retadr = (u32)&ret_chstat;
    rd->ee_retsiz = sizeof(ret_chstat);
    
    sceSifWriteBackDCache(&_send_data, sizeof(_sceFsRenameData));
    ret = sceFsSifCallRpc(&_cd, 0x11, 0x0, &_send_data, sizeof(_sceFsRenameData), _rcv_data_rpc, 4, NULL, NULL, 0);
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }

    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_chstat;
}

/**
 * @brief Change current directory
 * @param name filepath name
 * @return 0 on success, -1 * error code on failure
 */
s32 sceChdir(const char* name) {
    return _sceCallCode(name, 18);
}

int sceSync(const char* path, int flag) {
    _sceFsSyncData* sd;
    s32 nsize;
    s32 ret;
    s32 ret_sync;
    struct SemaParam sparam;
    s32 i;
    s32 semaid;

    sd = &_send_data.syncData;
    
    _sceFsWaitS(0x13);
    if (_fs_init == 0x0) {
        sceFsInit();
    }
    
    for (i = 0; i < MAX_ARG_SIZE && (sd->path[i] = path[i]) != 0; i++) { }
    if (i == MAX_ARG_SIZE) {
        sd->path[MAX_ARG_SIZE-1] = 0x0;
    }
    
    sd->flag = flag;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioSyncSema";
    semaid = CreateSema(&sparam);
    sd->ee_semid = semaid;
    sd->ee_retadr = (u32)&ret_sync;
    sd->ee_retsiz = sizeof(ret_sync);
    
    ret = sceFsSifCallRpc(&_cd, 0x13, 0x0, &_send_data, sizeof(_sceFsSyncData), _rcv_data_rpc, 4, NULL, NULL, 0);
    
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_sync;
}

int sceMount(const char* fsdevname, const char* blkdevname, int flag, void* arg, int arglen) {
    _sceFsMountData* md;
    s32 nsize;
    s32 ret;
    s32 ret_sync;
    struct SemaParam sparam;
    s32 semaid;

    md = &_send_data.mountData;
    
    _sceFsWaitS(0x14);
    if (_fs_init == 0x0) {
        sceFsInit();
    }
    
    for (nsize = 0; nsize < MAX_ARG_SIZE && (md->fsdevname[nsize] = fsdevname[nsize]) != 0; nsize++) { }
    if (nsize == MAX_ARG_SIZE) {
        md->fsdevname[MAX_ARG_SIZE-1] = 0x0;
    }
    for (nsize = 0; nsize < MAX_ARG_SIZE && (md->blkdevname[nsize] = blkdevname[nsize]) != 0; nsize++) { }
    if (nsize == MAX_ARG_SIZE) {
        md->blkdevname[MAX_ARG_SIZE-1] = 0x0;
    }

    if (arglen > MAX_ARG_SIZE) {
        _sceFsSigSema();
        return -E2BIG;
    }

    for (nsize = 0; nsize < arglen; nsize++) {
        md->arg[nsize] = ((char*)arg)[nsize];
    }
    
    md->flag = flag;
    md->arglen = arglen;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioMountSema";
    semaid = CreateSema(&sparam);
    md->ee_semid = semaid;
    md->ee_retadr = (u32)&ret_sync;
    md->ee_retsiz = sizeof(ret_sync);
    
    sceSifWriteBackDCache(&_send_data, sizeof(_sceFsMountData));
    ret = sceFsSifCallRpc(&_cd, 0x14, 0x0, &_send_data, sizeof(_sceFsMountData), _rcv_data_rpc, 4, NULL, NULL, 0);
    
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_sync;
}

/**
 * @brief Unmount filesystem
 * @param name string specifying mounted filesystem device name and unit number
 * @return 0 on success, -1 * error code on failure
 */
s32 sceUmount(const char* name) {
    return _sceCallCode(name, 21);
}

long sceLseek64(int fd, long offset, int how) {
    _sceFsLseek64Data* ld;
    _sceFsIob* io;
    s32 cnt0;
    s32 ret;
    s64 ret_lseek64;
    s32 i;
    s32 flag;
    struct SemaParam sparam;
    s32 semaid;

    ld = &_send_data.lSeek64Data;
    
    io = get_iob(fd);
    _sceFsWaitS(0x16);
    if (_fs_init == 0 || io == NULL || io->i_flag == 0) {
        _sceFsSigSema();
        return -EBADF;
    }
    
    flag = io->i_flag;
    ld->fd = io->i_fd;
    ld->offset = offset;
    ld->how = how;
    ld->ee_fds = io - _iob;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioLseek64Sema";
    semaid = CreateSema(&sparam);
    ld->ee_semid = semaid;
    ld->ee_retadr = (u32)&ret_lseek64;
    ld->ee_retsiz = sizeof(ret_lseek64);
        
    if ((flag & 0x8000) != 0) {
        WaitSema(_fs_fsq_semid);
        for (i = 0; i < 32; i++) {
            if (_sceFs_q[i] == -1) {
                _sceFs_q[i] = ld->ee_semid;
                ld->ee_semid = -ld->ee_semid;
                break;
            }
        }
        SignalSema(_fs_fsq_semid);
    }
    
    ret = sceFsSifCallRpc(&_cd, 0x16, 0x0, &_send_data, sizeof(_sceFsLseek64Data), _rcv_data_rpc, 4, NULL, NULL, flag & 0x8000);
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }

    if ((flag & 0x8000) != 0) {
        DeleteSema(semaid);
        return 0; // SCE_OK
    } 
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_lseek64;
}

int sceDevctl(const char* devname, int cmd, const void* arg, unsigned int arglen, void* bufp, unsigned int buflen) {
    _sceFsDevctlData* dd;
    s32 nsize;
    s32 fno;
    s32 ret;
    s32 ret_devctl;
    struct SemaParam sparam;
    s32 semaid;

    dd = &_send_data.devctlData;
    
    _sceFsWaitS(0x17);
    if (_fs_init == 0x0) {
        sceFsInit();
    }

    for (nsize = 0; nsize < MAX_ARG_SIZE && (dd->path[nsize] = devname[nsize]) != 0; nsize++) { }
    if (nsize == MAX_ARG_SIZE) {
        dd->path[MAX_ARG_SIZE-1] = 0x0;
    }

    if (arglen > MAX_ARG_SIZE || buflen > MAX_ARG_SIZE) {
        _sceFsSigSema();
        return -EINVAL;
    }
    if (cmd == 0x4601) {
        if (((s32*)arg)[1] == 0) {
            _sceFsSigSema();
            return 0;
        }
        if (((s32*)arg)[3] == 0) {
            _sceFsSigSema();
            return -EINVAL;
        }
    }

    for (nsize = 0; nsize < arglen; nsize++) {
        dd->arg_buf[nsize] = ((char*)arg)[nsize];
    }
    
    dd->cmd = cmd;
    dd->arglen = arglen;
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioDevctlSema";
    semaid = CreateSema(&sparam);
    dd->ee_semid = semaid;
    dd->ee_retadr = (u32)&ret_devctl;
    dd->ee_retsiz = sizeof(ret_devctl);
    dd->ret_argadr = bufp;
    dd->ret_arglen = buflen;
    
    if (cmd == 0x4601) {
        sceSifWriteBackDCache((void*)((s32*)arg)[2], ((s32*)arg)[1] * ((s32*)arg)[3]);
        fno = 0x1c;
    } else {
        fno = 0x17;
    }
    sceSifWriteBackDCache(&_send_data, sizeof(_sceFsDevctlData));
    ret = sceFsSifCallRpc(&_cd, fno, 0x0, &_send_data, sizeof(_sceFsDevctlData), _rcv_data_rpc, 4, NULL, NULL, 0);
    
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_devctl;
}

int sceSymlink(const char* existing, const char* new) {
    _sceFsSymlinkData* rd;
    s32 nsize;
    s32 ret;
    s32 ret_link;
    struct SemaParam sparam;
    s32 semaid;

    rd = &_send_data.symlinkData;
    
    _sceFsWaitS(0x11);
    if (_fs_init == 0x0) {
        sceFsInit();
    }

    for (nsize = 0; nsize < MAX_ARG_SIZE && (rd->existing[nsize] = existing[nsize]) != 0; nsize++) { }
    if (nsize == MAX_ARG_SIZE) {
        rd->existing[MAX_ARG_SIZE-1] = 0x0;
    }
    
    for (nsize = 0; nsize < MAX_ARG_SIZE && (rd->new[nsize] = new[nsize]) != 0; nsize++) { }
    if (nsize == MAX_ARG_SIZE) {
        rd->new[MAX_ARG_SIZE-1] = 0x0;
    }

    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioSymlinkSema";
    semaid = CreateSema(&sparam);
    rd->ee_semid = semaid;
    rd->ee_retadr = (u32)&ret_link;
    rd->ee_retsiz = sizeof(ret_link);
    
    ret = sceFsSifCallRpc(&_cd, 0x18, 0x0, &_send_data, sizeof(_sceFsSymlinkData), _rcv_data_rpc, 4, NULL, NULL, 0);
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_link;
}

int sceReadlink(const char* path, char* buf, unsigned int bufsize) {
    _sceFsReadlinkData* rd;
    s32 nsize;
    s32 ret;
    s32 ret_link;
    struct SemaParam sparam;
    s32 semaid;

    rd = &_send_data.readLinkData;
    
    _sceFsWaitS(0x11);
    if (_fs_init == 0x0) {
        sceFsInit();
    }

    for (nsize = 0; nsize < MAX_ARG_SIZE && (rd->path[nsize] = path[nsize]) != 0; nsize++) { }
    
    if (nsize == MAX_ARG_SIZE) {
        rd->path[MAX_ARG_SIZE-1] = 0x0;
    }

    if (bufsize >= MAX_ARG_SIZE) {
        bufsize = MAX_ARG_SIZE-1;
    }

    rd->bufsize = bufsize;
    rd->bufaddr = (u32)buf;
    sceSifWriteBackDCache(buf, bufsize);
    sparam.maxCount = 0x1;
    sparam.initCount = 0x0;
    sparam.option = (u32)"SceStdioReadlinkSema";
    semaid = CreateSema(&sparam);
    rd->ee_semid = semaid;
    rd->ee_retadr = (u32)&ret_link;
    rd->ee_retsiz = sizeof(ret_link);
    
    ret = sceFsSifCallRpc(&_cd, 0x19, 0x0, &_send_data, sizeof(_sceFsSymlinkData), _rcv_data_rpc, 4, NULL, NULL, 0);
    if (ret < 0x0) {
        DeleteSema(semaid);
        _sceFsSigSema();
        return -EAGAIN;
    }
   
    ret = *(u32*)UNCACHED(_rcv_data_rpc);
    _sceFsSigSema();
    if (ret == 0x0) {
        DeleteSema(semaid);
        return -EAGAIN;
    }
    
    WaitSema(semaid);
    DeleteSema(semaid);
    return ret_link;
}
// SOURCE DATA: .data .rodata .bss
