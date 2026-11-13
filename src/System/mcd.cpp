#include "System/mcd.h"
#include <string.h>

struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    u_int attr;
    u_int option;
};

struct ThreadParam {
    int status;
    void (*entry)(void*);
    void* stack;
    int stackSize;
    void* gpReg;
    int initPriority;
    int currentPriority;
    u_int attr;
    u_int option;
    int waitType;
    int waitId;
    int wakeupCount;
};

struct sceSifRpcData {
    void* paddr;
    unsigned int pid;
    int tid;
    unsigned int mode;
};

struct sceSifServeData;

struct sceSifClientData {
    sceSifRpcData rpcd;
    unsigned int command;
    void* buff;
    void* gp;
    void (*func)(void*);
    void* para;
    sceSifServeData* serve;
};

extern "C" {
int CreateSema(SemaParam* param);
int DeleteSema(int sid);
int SignalSema(int sid);
int WaitSema(int sid);
int ReferSemaStatus(int sid, SemaParam* param);
int CreateThread(ThreadParam* param);
int StartThread(int tid, void* arg);
void ExitDeleteThread();
void SyncDCache(void* start, void* end);
void sceSifInitRpc(unsigned int mode);
int sceSifBindRpc(sceSifClientData* cd, unsigned int request, unsigned int mode);
int sceSifCallRpc(sceSifClientData* cd, unsigned int rpc_number, unsigned int mode, void* send, int ssize,
                  void* receive, int rsize, void (*end_function)(void*), void* end_param);
int printf(const char* fmt, ...);
extern char _gp;
}

struct mcdCallbackInfo {
    int priority;
    void* stackAddr;
    int stackSize;
    void (*function)(int, void*);
    void* pContext;

    void init() {
        priority = 0;
        stackAddr = 0;
        stackSize = 0;
        function = 0;
        pContext = 0;
    }
};

struct mcdReadParam {
    int mode;
    u_int lsn;
    u_int sectors;
    void* buf;
};

static int flagMcdInitialized = 0;
static unsigned int sbuff[16];
static unsigned int rbuff[16];
static unsigned char threadStack[4096];
static sceSifClientData gCd;
static int semaMcdCall;
static int semaMcdSync;
static mcdCallbackInfo callbackInfo;
static mcdReadParam readParam;
static volatile int lastResult;

int mcdInit() {
    int r;
    SemaParam semaParam;
    int i;

    if (flagMcdInitialized) {
        DeleteSema(semaMcdCall);
        DeleteSema(semaMcdSync);
    }
    callbackInfo.init();
    sceSifInitRpc(0);
    while (1) {
        if (sceSifBindRpc(&gCd, 0xEE00, 0) < 0) {
            printf("error: sceSifBindRpc\n");
            while (1) {
            }
        }
        for (i = 10000; i > 0; i--) {
        }
        if (gCd.serve != 0) {
            break;
        }
    }
    sceSifCallRpc(&gCd, 0x101, 0, sbuff, 0x40, sbuff, 0x40, 0, 0);
    r = sbuff[0];
    semaParam.initCount = 1;
    semaParam.maxCount = 1;
    semaMcdCall = CreateSema(&semaParam);
    semaMcdSync = CreateSema(&semaParam);
    flagMcdInitialized = 1;
    return r;
}

int mcdInitEeCB(int cb_prio, void* stack_addr, int stack_size) {
    if (callbackInfo.stackAddr != 0) {
        callbackInfo.priority = cb_prio;
        return 0;
    }
    callbackInfo.priority = cb_prio;
    callbackInfo.stackAddr = stack_addr;
    callbackInfo.stackSize = stack_size;
    return 0;
}

int mcdCallback(void (*func)(int, void*), void* pContext) {
    callbackInfo.function = func;
    callbackInfo.pContext = pContext;
    return 0;
}

int mcdSearchFile(sceCdlFILE* fp, char* name) {
    int r;

    WaitSema(semaMcdCall);
    SyncDCache(rbuff, (char*)rbuff + 0x3F);
    sbuff[0] = (unsigned int)rbuff;
    strcpy((char*)&sbuff[1], name);
    sceSifCallRpc(&gCd, 0x102, 0, sbuff, 0x40, sbuff, 0x40, 0, 0);
    *fp = *(sceCdlFILE*)rbuff;
    r = sbuff[0];
    SignalSema(semaMcdCall);
    return r;
}

int mcdRead(u_int lsn, u_int sectors, void* buf) {
    int r;

    WaitSema(semaMcdCall);
    SyncDCache(buf, (char*)buf + sectors * 0x800 - 1);
    sbuff[0] = lsn;
    sbuff[1] = sectors;
    sbuff[2] = (unsigned int)buf;
    sceSifCallRpc(&gCd, 0x103, 0, sbuff, 0x40, sbuff, 0x40, 0, 0);
    r = sbuff[0];
    SignalSema(semaMcdCall);
    return r;
}

int mcdRead2(u_int lsn, u_int sectors, void* buf) {
    int r;

    WaitSema(semaMcdCall);
    SyncDCache(buf, (char*)buf + sectors * 0x800 - 1);
    sbuff[0] = lsn;
    sbuff[1] = sectors;
    sbuff[2] = (unsigned int)buf;
    sceSifCallRpc(&gCd, 0x104, 0, sbuff, 0x40, sbuff, 0x40, 0, 0);
    r = sbuff[0];
    SignalSema(semaMcdCall);
    return r;
}

static void mcdCallbackThread(void* pContext) {
    mcdReadParam* p = (mcdReadParam*)pContext;

    if (p->mode == 2) {
        lastResult = mcdRead2(p->lsn, p->sectors, p->buf);
    } else {
        lastResult = mcdRead(p->lsn, p->sectors, p->buf);
    }
    SignalSema(semaMcdSync);
    if (callbackInfo.function) {
        callbackInfo.function(0, callbackInfo.pContext);
    }
    ExitDeleteThread();
}

static int mcdAsyncReadHelper(int mode, u_int lsn, u_int sectors, void* buf) {
    SemaParam semaParam;
    ThreadParam threadParam;

    ReferSemaStatus(semaMcdSync, &semaParam);
    if (semaParam.currentCount == 0) {
        return 1;
    }
    readParam.mode = mode;
    readParam.lsn = lsn;
    readParam.sectors = sectors;
    readParam.buf = buf;
    if (callbackInfo.function) {
        threadParam.stack = callbackInfo.stackAddr;
        threadParam.stackSize = callbackInfo.stackSize;
        threadParam.initPriority = callbackInfo.priority;
    } else {
        threadParam.stack = threadStack;
        threadParam.stackSize = sizeof(threadStack);
        threadParam.initPriority = 1;
    }
    threadParam.gpReg = &_gp;
    threadParam.entry = mcdCallbackThread;
    WaitSema(semaMcdSync);
    StartThread(CreateThread(&threadParam), &readParam);
    return 0;
}

int mcdAsyncRead(u_int lsn, u_int sectors, void* buf) {
    return mcdAsyncReadHelper(0, lsn, sectors, buf);
}

int mcdAsyncRead2(u_int lsn, u_int sectors, void* buf) {
    return mcdAsyncReadHelper(2, lsn, sectors, buf);
}

int mcdSync(int mode) {
    SemaParam semaParam;

    if (mode) {
        ReferSemaStatus(semaMcdSync, &semaParam);
        if (semaParam.currentCount == 0) {
            return 1;
        }
    } else {
        WaitSema(semaMcdSync);
        SignalSema(semaMcdSync);
    }
    return lastResult;
}

int mcdGetLastResult() {
    return lastResult;
}

int mcdBeginStream() {
    int r;

    WaitSema(semaMcdCall);
    sceSifCallRpc(&gCd, 0x105, 0, sbuff, 0x40, sbuff, 0x40, 0, 0);
    r = sbuff[0];
    SignalSema(semaMcdCall);
    return r;
}

int mcdEndStream() {
    int r;

    WaitSema(semaMcdCall);
    sceSifCallRpc(&gCd, 0x106, 0, sbuff, 0x40, sbuff, 0x40, 0, 0);
    r = sbuff[0];
    SignalSema(semaMcdCall);
    return r;
}

int mcdReadClock(sceCdCLOCK* rtc) {
    int r;

    WaitSema(semaMcdCall);
    SyncDCache(rbuff, (char*)rbuff + 0x3F);
    sbuff[0] = (unsigned int)rbuff;
    sceSifCallRpc(&gCd, 0x107, 0, sbuff, 0x40, sbuff, 0x40, 0, 0);
    *rtc = *(sceCdCLOCK*)rbuff;
    r = sbuff[0];
    SignalSema(semaMcdCall);
    return r;
}

int mcdChangeMedia(int media) {
    int r;

    WaitSema(semaMcdCall);
    sbuff[0] = media;
    sceSifCallRpc(&gCd, 0x108, 0, sbuff, 0x40, sbuff, 0x40, 0, 0);
    r = sbuff[0];
    SignalSema(semaMcdCall);
    return r;
}

int mcdSetCheckFile(char* file) {
    int r;

    WaitSema(semaMcdCall);
    if (file == 0 || *file == 0) {
        sbuff[0] = 0;
    } else {
        strncpy((char*)sbuff, file, 0x3F);
    }
    sceSifCallRpc(&gCd, 0x109, 0, sbuff, 0x40, sbuff, 0x40, 0, 0);
    r = sbuff[0];
    SignalSema(semaMcdCall);
    return r;
}

int mcdCheckCDTray() {
    int r;

    WaitSema(semaMcdCall);
    sceSifCallRpc(&gCd, 0x110, 0, sbuff, 0x40, sbuff, 0x40, 0, 0);
    r = sbuff[0];
    SignalSema(semaMcdCall);
    return r;
}

int mcdCheckStat() {
    int r;

    WaitSema(semaMcdCall);
    sceSifCallRpc(&gCd, 0x111, 0, sbuff, 0x40, sbuff, 0x40, 0, 0);
    r = sbuff[0];
    SignalSema(semaMcdCall);
    return r;
}

// SOURCE DATA: .data .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .data .rodata
