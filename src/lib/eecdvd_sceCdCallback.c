/* libcdvd eecdvd.c (Sony EE SDK): initialisation, callbacks, the N/S command semaphores and RPC binds,
   search file and disk ready. The RPC buffers are read through the uncached segment (0x20000000). */
#include "sdk/eekernel.h"
#include "sdk/sifrpc.h"

#define UNCACHED(p) ((unsigned int)(p) | 0x20000000)

typedef struct {
    unsigned int lsn;
    unsigned int size;
    char name[16];
    unsigned char date[8];
    unsigned int flag;
} sceCdlFILE;

typedef struct {
    sceCdlFILE file;
    char name[256];
    void *addr;
    int layer;
} SearchFileArg;

/* filled by the IOP: data the read interrupt copies to unaligned buffer heads and tails */
typedef struct {
    int size1;
    int size2;
    unsigned char *dst1;
    unsigned char *dst2;
    unsigned char buf1[64];
    unsigned char buf2[64];
} ReadIntrData;

void scePrintf(const char *fmt, ...); /* MATCH: void (no v0 result): sceCdLayerSearchFile reloads SCE_CD_debug into v0 after each call */
void *memcpy(void *dst, const void *src, unsigned int n);
int SignalSema(int sema);
int PollSema(int sema);
int StartThread(int thid, void *arg);
void ExitDeleteThread(void);
int DelayThread(unsigned int usec);
void sceSifInitRpc(unsigned int mode);
int sceSifCheckStatRpc(sceSifClientData *cd);
void sceSifWriteBackDCache(void *ptr, int size);
void *sceSifAddCmdHandler(unsigned int cid, void (*func)(void *), void *data);
void sceSifRemoveCmdHandler(unsigned int cid);

int sceCdSync(int mode);
void _sceCd_cbLoop(void);
int sceCdSyncS(int mode);
static void cmd_sem_init(void);
static int PowerOffCB(void);

static char __ps2_libinfo__[16] = "PsIIlibcdvd 3100";
int SCE_CD_debug = 0;
static int cb_thid = 0;
static int scmd_sema_keep_cmd = 0;
static int ncmd_sema_keep_cmd = 0;
static volatile int cb_semid = -1;
static volatile int Init_seq = 0;
volatile int _sceCd_ncmd_semid = -1;
volatile int _sceCd_scmd_semid = -1;
volatile int _sceCd_rcmd_semid = -1;
volatile int _sceCd_c_cb_sem = 0;
volatile int _sceCd_ee_read_mode = 0;
static int _ncmd_bind = -1;
int _sceCd_old_module = 1;
int _sceCd_CdInit_ON = 0;
static int _icmd_bind = -1;
static int _sf_bind = -1;
static int _scmd_dr_bind = -1;
static int _scmd_bind = -1;
static int _it_bind = -1;

/* defined at the end of the file (emitted after sceCdInit's static) */
extern volatile int sceCdCbfunc_num;
extern volatile int sceCdCbfunc_number;
extern unsigned int _sceCd_ncmdrdata[];
extern unsigned int _sceCd_ncmdsdata[];
extern ReadIntrData _sceCd_rd_intr_data;
extern int _sceCd_Read_cur_pos[];
extern sceSifClientData _sceCd_cd_ncmd;
extern unsigned int _sceCd_scmdrdata[];
extern unsigned int _sceCd_scmdsdata[];
extern unsigned int _sceCd_rcmdrdata[];
extern sceSifClientData _sceCd_cd_scmd;

static void (*cb_func)(int);
static void *cb_gp;
static void (*poff_func)(void *);
static void *poff_gp;
static void *poff_arg;
static int cb_flag;
static struct ThreadParam cb_thparam;
static SearchFileArg search_arg __attribute__((aligned(64)));
static unsigned int search_rdata[16] __attribute__((aligned(64)));
static sceSifClientData cd_sf;
static sceSifClientData cd_init;
static sceSifClientData cd_dr;
static unsigned int init_mode __attribute__((aligned(64)));
static unsigned int dr_mode __attribute__((aligned(16)));

#define WAIT_LOOP()            \
    {                          \
        int __i = 0x100000;    \
        while (__i--)          \
            ;                  \
    }

void *sceCdCallback(void (*func)(int))
{
    void (*old)(int);
    int oldi;

    if (sceCdSync(1)) {
        return 0;
    }
    oldi = DIntr();
    old = cb_func;
    cb_func = func;
    __asm__ volatile("sw $gp,%0" : "=m"(cb_gp));
    if (oldi) {
        EIntr();
    }
    return (void *)old;
}

void _sceCd_cd_callback(void *p)
{
    sceCdCbfunc_num = *(int *)p;
    sceCdCbfunc_number = sceCdCbfunc_num;
    if (sceCdCbfunc_num == 11) {
        sceCdCbfunc_num = 0;
        _sceCd_c_cb_sem = 0;
        return;
    }
    iSignalSema(_sceCd_ncmd_semid);
    if (cb_thid && cb_func) {
        iSignalSema(cb_semid);
    } else {
        _sceCd_c_cb_sem = 0;
    }
    sceCdCbfunc_num = 0;
}

void _sceCd_cbLoop(void)
{
    void *old;

    while (1) {
        WaitSema(cb_semid);
        if (sceCdCbfunc_num == -1) {
            /* MATCH: the barriers hold the four stores in the original's order, cb_flag in the jal slot */
            _sceCd_c_cb_sem = 0;
            __asm__ volatile("");
            sceCdCbfunc_num = 0;
            cb_thid = 0;
            __asm__ volatile("");
            cb_flag = 0;
            ExitDeleteThread();
        }
        if (SCE_CD_debug > 0) {
            scePrintf("sceCdCbfunc= %d sceCdCbfunc_num= %d\n", cb_func, sceCdCbfunc_number);
        }
        if (cb_func && sceCdCbfunc_number) {
            __asm__ volatile("move %0,$gp\n\tlw $gp,%1" : "=&r"(old) : "m"(cb_gp));
            cb_func(sceCdCbfunc_number);
            __asm__ volatile("move $gp,%0" : : "r"(old));
        }
        _sceCd_c_cb_sem = 0;
    }
}

int sceCdInitEeCB(int prio, void *stack, int stacksize)
{
    int ret = 1;

    if (cb_thid == 0) {
        cb_thparam.stackSize = stacksize;
        cb_thparam.entry = (void (*)(void *))_sceCd_cbLoop;
        cb_thparam.stack = stack;
        cb_thparam.initPriority = prio;
        cb_thparam.option = (unsigned int)"SceCdCallbackThread";
        cb_thparam.gpReg = 0;
        cb_thid = CreateThread(&cb_thparam);
        StartThread(cb_thid, 0);
    } else {
        ChangeThreadPriority(cb_thid, prio);
        ret = 0;
    }
    return ret;
}

void _sceCd_cd_read_intr(void *data)
{
    ReadIntrData *p = (ReadIntrData *)UNCACHED(data);
    unsigned char *dst;
    unsigned char *udst;
    unsigned char c;
    int i;

    if (p->size1 > 0) {
        udst = (unsigned char *)UNCACHED(p->dst1);
        dst = p->dst1;
        for (i = 0; i < p->size1; i++) {
            dst[i] = udst[i] = p->buf1[i];
        }
    }
    if (p->size2 > 0) {
        udst = (unsigned char *)UNCACHED(p->dst2);
        dst = p->dst2;
        for (i = 0; i < p->size2; i++) {
            dst[i] = udst[i] = p->buf2[i];
        }
    }
    _sceCd_cd_callback((void *)&sceCdCbfunc_num);
}

static void cmd_sem_init(void)
{
    struct SemaParam sp;

    if (_sceCd_ncmd_semid == -1 || _sceCd_scmd_semid == -1 || _sceCd_rcmd_semid == -1) {
        sp.option = (unsigned int)"SceCdNcmdSema";
        sp.initCount = 1;
        sp.maxCount = 1;
        _sceCd_ncmd_semid = CreateSema(&sp);
        sp.option = (unsigned int)"SceCdScmdSema";
        _sceCd_scmd_semid = CreateSema(&sp);
        sp.option = (unsigned int)"SceCdRcmdSema";
        _sceCd_rcmd_semid = CreateSema(&sp);
        sp.option = (unsigned int)"SceCdCallbackSema";
        sp.initCount = 0;
        cb_semid = CreateSema(&sp);
        _sceCd_c_cb_sem = 0;
    }
}

static void cdvd_exit(void)
{
    int old;

    if (cb_thid) {
        sceCdCbfunc_num = -1;
        SignalSema(cb_semid);
    }
    DeleteSema(_sceCd_ncmd_semid);
    DeleteSema(_sceCd_scmd_semid);
    DeleteSema(_sceCd_rcmd_semid); DeleteSema(cb_semid);
    old = DIntr();
    sceSifRemoveCmdHandler(0x80000012);
    if (old) {
        EIntr();
    }
}

void *sceCdPOffCallback(void (*func)(void *), void *addr)
{
    void (*old)(void *);
    int oldi;

    if (_icmd_bind < 0) {
        PowerOffCB();
    }
    oldi = DIntr();
    old = poff_func;
    poff_arg = addr;
    poff_func = func;
    __asm__ volatile("sw $gp,%0" : "=m"(poff_gp));
    if (oldi) {
        EIntr();
    }
    return (void *)old;
}

static void _sceCd_Poff_Intr(void *data)
{
    void *old;

    if (poff_func && !Init_seq) {
        __asm__ volatile("move %0,$gp\n\tlw $gp,%1" : "=&r"(old) : "m"(poff_gp));
        poff_func(poff_arg);
        __asm__ volatile("move $gp,%0" : : "r"(old));
    }
}

static int PowerOffCB(void)
{
    int old;

    Init_seq = 1; old = DIntr();
    sceSifAddCmdHandler(0x80000012, _sceCd_Poff_Intr, 0);
    if (old) {
        EIntr();
    }
    Init_seq = 0;
    _icmd_bind = 1;
    return 1;
}

int sceCdLayerSearchFile(sceCdlFILE *fp, const char *name, int layer)
{
    int i;
    int ret;

    cmd_sem_init();
    if (_sceCd_ncmd_semid != PollSema(_sceCd_ncmd_semid)) {
        return 0;
    }
    ncmd_sema_keep_cmd = 1;
    if (sceCdSync(1)) {
        SignalSema(_sceCd_ncmd_semid);
        return 0;
    }
    sceSifInitRpc(0);
    if (_sf_bind < 0) {
        while (1) {
            if (sceSifBindRpc(&cd_sf, 0x80000597, 0) < 0) {
                if (SCE_CD_debug > 0) {
                    scePrintf("Libcdvd bind err CdSearchFile\n");
                }
                WAIT_LOOP()
                continue;
            }
            if (cd_sf.serve != 0) {
                _sf_bind = 0;
                break;
            }
            WAIT_LOOP()
        }
    }
    for (i = 0; i < 256; i++) {
        if ((search_arg.name[i] = name[i]) == 0) {
            break;
        }
    }
    if (i == 256) {
        search_arg.name[255] = 0;
    }
    if (SCE_CD_debug > 0) {
        scePrintf("ee call cmd search %s\n", search_arg.name);
    }
    search_arg.layer = layer;
    search_arg.addr = &search_arg;
    sceSifWriteBackDCache(&search_arg, sizeof(search_arg));
    if (sceSifCallRpc(&cd_sf, 0, 0, &search_arg, sizeof(search_arg), search_rdata, 4, 0, 0) < 0) {
        SignalSema(_sceCd_ncmd_semid);
        return 0;
    }
    memcpy(fp, (void *)UNCACHED(&search_arg), sizeof(sceCdlFILE));
    if (SCE_CD_debug > 0) {
        scePrintf("search name %s\n", fp->name);
    }
    if (SCE_CD_debug > 0) {
        scePrintf("search size %d\n", fp->size);
    }
    if (SCE_CD_debug > 0) {
        scePrintf("search loc lbn %d\n", fp->lsn);
    }
    ret = *(int *)UNCACHED(search_rdata);
    SignalSema(_sceCd_ncmd_semid);
    return ret;
}

int sceCdSearchFile(sceCdlFILE *fp, const char *name)
{
    return sceCdLayerSearchFile(fp, name, 0);
}

int _sceCd_ncmd_prechk(int cmd)
{
    cmd_sem_init();
    if (_sceCd_ncmd_semid != PollSema(_sceCd_ncmd_semid)) {
        if (SCE_CD_debug > 0) {
            scePrintf("Ncmd fail sema cur_cmd:%d keep_cmd:%d\n", cmd, ncmd_sema_keep_cmd);
        }
        return 0;
    }
    ncmd_sema_keep_cmd = cmd;
    if (sceCdSync(1)) {
        SignalSema(_sceCd_ncmd_semid);
        return 0;
    }
    sceSifInitRpc(0);
    if (_ncmd_bind < 0) {
        while (1) {
            if (sceSifBindRpc(&_sceCd_cd_ncmd, 0x80000595, 0) < 0) {
                if (SCE_CD_debug > 0) {
                    scePrintf("Libcdvd bind err N CMD\n");
                }
                WAIT_LOOP()
                continue;
            }
            if (_sceCd_cd_ncmd.serve != 0) {
                break;
            }
            WAIT_LOOP()
        }
        _ncmd_bind = 0;
    }
    return 1;
}

int sceCdNcmdDiskReady(void)
{
    int ret;

    if (!_sceCd_ncmd_prechk(2)) {
        return 0;
    }
    if (sceSifCallRpc(&_sceCd_cd_ncmd, 0xE, 0, 0, 0, _sceCd_ncmdrdata, 4, 0, 0) < 0) {
        SignalSema(_sceCd_ncmd_semid);
        return 0;
    }
    ret = *(int *)UNCACHED(_sceCd_ncmdrdata);
    SignalSema(_sceCd_ncmd_semid);
    return ret;
}

int sceCdSync(int mode)
{
    if (mode == 0) {
        if (SCE_CD_debug > 0) {
            scePrintf("N cmd wait\n");
        }
        while (_sceCd_c_cb_sem || sceSifCheckStatRpc(&_sceCd_cd_ncmd)) {
            DelayThread(4000);
        }
        return 0;
    }
    if (_sceCd_c_cb_sem || sceSifCheckStatRpc(&_sceCd_cd_ncmd)) {
        return 1;
    }
    return 0;
}

int sceCdSyncS(int mode)
{
    if (mode == 0) {
        if (SCE_CD_debug > 0) {
            scePrintf("S cmd wait\n");
        }
        while (sceSifCheckStatRpc(&_sceCd_cd_scmd)) {
            DelayThread(4000);
        }
        return 0;
    }
    return sceSifCheckStatRpc(&_sceCd_cd_scmd);
}

int _sceCd_scmd_prechk(int cmd)
{
    cmd_sem_init();
    if (_sceCd_scmd_semid != PollSema(_sceCd_scmd_semid)) {
        if (SCE_CD_debug > 0) {
            scePrintf("Scmd fail sema cur_cmd:%d keep_cmd:%d\n", cmd, scmd_sema_keep_cmd);
        }
        return 0;
    }
    scmd_sema_keep_cmd = cmd;
    if (sceCdSyncS(1)) {
        SignalSema(_sceCd_scmd_semid);
        return 0;
    }
    sceSifInitRpc(0);
    if (_scmd_bind < 0) {
        while (1) {
            if (sceSifBindRpc(&_sceCd_cd_scmd, 0x80000593, 0) < 0) {
                if (SCE_CD_debug > 0) {
                    scePrintf("Libcdvd bind err S cmd\n");
                }
                WAIT_LOOP()
                continue;
            }
            if (_sceCd_cd_scmd.serve != 0) {
                break;
            }
            WAIT_LOOP()
        }
        _scmd_bind = 0;
    }
    return 1;
}

int _sceCdSC(int code, int *param)
{
    int v;

    if (code == 0x8001) goto c1;
    if (code == 0x8003) goto c3;
    if (code == 0x8002) goto c2;
    if (code == 0x8004) goto c4;
    if (code == 0x8011) goto c11;
    if (code == 0x8013) goto c13;
    if (code == 0x8012) goto c12;
    if (code != 0x8014) goto def;
    v = (int)&_sceCd_cd_scmd;
    goto end;
def:
    v = 0;
    goto end;
c12:
    v = (int)_sceCd_scmdsdata;
    goto end;
c13:
    v = (int)_sceCd_scmdrdata;
    goto end;
c11:
    v = _sceCd_scmd_semid;
    goto end;
c4:
    v = (int)&_sceCd_cd_ncmd;
    goto end;
c2:
    v = (int)_sceCd_ncmdsdata;
    goto end;
c3:
    v = (int)_sceCd_ncmdrdata;
    goto end;
c1:
    v = _sceCd_ncmd_semid;
end:
    *param = v;
    return 1;
}

int sceCdInit(int mode)
{
    static int c_cnt = 0;
    int ret;
    int r;
    int stat;
    int ver1;
    int ver2;
    unsigned int *rdata;

    if (sceCdSyncS(1)) {
        return 0;
    }
    sceSifInitRpc(0);
    /* this store order (and _it_bind = 0 first below) is what gives the original's registers: it sets the
       lifetimes sched1 leaves for local-alloc (t1/t0 for _scmd_dr_bind/_sceCd_ee_read_mode) and the
       global-alloc priority of the _it_bind/c_cnt %hi (s4/s5); sched2 then restores the original store order */
    Init_seq = 1;
    _sf_bind = -1;
    _ncmd_bind = -1;
    _scmd_bind = -1;
    _scmd_dr_bind = -1;
    _it_bind = -1;
    _icmd_bind = -1;
    _sceCd_ee_read_mode = 0;
    c_cnt++;
    while (1) {
        if ((r = sceSifBindRpc(&cd_init, 0x80000592, 0)) < 0) {
            if (SCE_CD_debug > 0) {
                scePrintf("Libcdvd bind err %d CD_Init %d\n", r, c_cnt);
            }
            WAIT_LOOP()
            continue;
        }
        if (cd_init.serve != 0) {
            _it_bind = 0;
            init_mode = mode;
            rdata = _sceCd_scmdrdata;
            sceSifWriteBackDCache(&init_mode, 4);
            if (sceSifCallRpc(&cd_init, 0, 0, &init_mode, 4, rdata, 16, 0, 0) < 0) {
                Init_seq = 0;
                return 0;
            }
            break;
        }
        WAIT_LOOP()
    }
    ver1 = *(int *)UNCACHED(&rdata[1]);
    ver2 = *(int *)UNCACHED(&rdata[2]);
    stat = *(int *)UNCACHED(&rdata[3]);
    ret = 1;
    if (stat != 0xFF) {
        if (stat == 0xFE) {
            SCE_CD_debug = 1;
        } else if (ver1 / 256 < 2 || ver2 / 256 < 2) {
            ret = 2;
        }
    }
    Init_seq = 0;
    switch (mode) {
    case 5:
        if (SCE_CD_debug > 0) {
            scePrintf("Libcdvd Exit\n");
        }
        cdvd_exit();
        _sceCd_ncmd_semid = -1;
        _sceCd_scmd_semid = -1;
        cb_semid = -1;
        _sceCd_CdInit_ON = 0;
        break;
    case 0:
    case 1:
    default:
        cmd_sem_init();
        PowerOffCB();
        _sceCd_CdInit_ON = 1;
        break;
    }
    return ret;
}

int sceCdDiskReady_old(int mode)
{
    int old;
    int ret;

    if (SCE_CD_debug > 0) {
        scePrintf("OLD DiskReady Call\n");
    }
    old = DIntr();
    _sceCd_old_module = 1;
    if (old) {
        EIntr();
    }
    cmd_sem_init();
    if (_sceCd_scmd_semid != PollSema(_sceCd_scmd_semid)) {
        return 6;
    }
    if (sceCdSyncS(1)) {
        SignalSema(_sceCd_scmd_semid);
        return (mode != 8) ? 6 : -1;
    }
    sceSifInitRpc(0);
    if (_scmd_dr_bind < 0) {
        while (1) {
            if (sceSifBindRpc(&cd_dr, 0x8000059A, 0) < 0) {
                if (SCE_CD_debug > 0) {
                    scePrintf("Libcdvd bind err CdDiskReady\n");
                }
                WAIT_LOOP()
                continue;
            }
            if (cd_dr.serve != 0) {
                _scmd_dr_bind = 0;
                break;
            }
            WAIT_LOOP()
        }
    }
    dr_mode = mode;
    sceSifWriteBackDCache(&dr_mode, 4);
    if (sceSifCallRpc(&cd_dr, 0, 0, &dr_mode, 4, _sceCd_scmdrdata, 4, 0, 0) < 0) {
        SignalSema(_sceCd_scmd_semid);
        return (mode != 8) ? 6 : -1;
    }
    if (SCE_CD_debug > 0) {
        scePrintf("DiskReady ended\n");
    }
    ret = *(int *)UNCACHED(_sceCd_scmdrdata);
    SignalSema(_sceCd_scmd_semid);
    return ret;
}

int sceCdDiskReady(int mode)
{
    int cnt;
    int i;
    int old;
    int ret;

    if (SCE_CD_debug > 0) {
        scePrintf("NEW DiskReady Call\n");
    }
    cmd_sem_init();
    if (_sceCd_rcmd_semid != PollSema(_sceCd_rcmd_semid)) {
        return (mode != 8) ? 6 : -1;
    }
    sceSifInitRpc(0);
    cnt = 0;
    if (_scmd_dr_bind < 0) {
        while (1) {
            if (sceSifBindRpc(&cd_dr, 0x8000059C, 0) < 0) {
                if (SCE_CD_debug > 0) {
                    scePrintf("Libcdvd bind err CdDiskReady\n");
                }
                i = 0x100000;
            while (i--)
                ;
                continue;
            }
            if (cd_dr.serve != 0) {
                _scmd_dr_bind = 0;
                break;
            }
            if (cnt > 16) {
                SignalSema(_sceCd_rcmd_semid);
                return sceCdDiskReady_old(mode);
            }
            i = 0x100000;
            while (i--)
                ;
            cnt++;
        }
    }
    old = DIntr();
    _sceCd_old_module = 0;
    if (old) {
        EIntr();
    }
    dr_mode = mode;
    sceSifWriteBackDCache(&dr_mode, 4);
    if (sceSifCallRpc(&cd_dr, 0, 0, &dr_mode, 4, _sceCd_rcmdrdata, 4, 0, 0) < 0) {
        SignalSema(_sceCd_rcmd_semid);
        return (mode != 8) ? 6 : -1;
    }
    if (SCE_CD_debug > 0) {
        scePrintf("DiskReady ended\n");
    }
    ret = *(int *)UNCACHED(_sceCd_rcmdrdata);
    SignalSema(_sceCd_rcmd_semid);
    return ret;
}

volatile int sceCdCbfunc_num = 0;
volatile int sceCdCbfunc_number = 0;
unsigned int _sceCd_ncmdrdata[24] __attribute__((aligned(64))) = { 0 };
unsigned int _sceCd_ncmdsdata[1024] __attribute__((aligned(64))) = { 0 };
ReadIntrData _sceCd_rd_intr_data __attribute__((aligned(64))) = { 0 };
int _sceCd_Read_cur_pos[4] __attribute__((aligned(64))) = { 0 };
sceSifClientData _sceCd_cd_ncmd = { { 0 } };
unsigned int _sceCd_scmdrdata[264] __attribute__((aligned(64))) = { 0 };
unsigned int _sceCd_scmdsdata[258] __attribute__((aligned(64))) = { 0 };
unsigned int _sceCd_rcmdrdata[64] __attribute__((aligned(64))) = { 0 };
sceSifClientData _sceCd_cd_scmd __attribute__((aligned(64))) = { { 0 } };

// SOURCE DATA: .data .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .data .rodata
