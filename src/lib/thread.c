/* Thread helpers: interrupt-context wakeup/rotate/suspend through a helper thread, DelayThread, StartThread. */
#include "sdk/eekernel.h"

int kprintf(const char *format, ...);
int WakeupThread(int tid);
int RotateThreadReadyQueue(int priority);
int SuspendThread(int tid);
int _iWakeupThread(int tid);
int _iSuspendThread(int tid);
int _StartThread(int tid, void *arg);
unsigned long TimerUSec2BusClock(unsigned int sec, unsigned int usec);
int SetTimerAlarm(unsigned long clock, unsigned long (*handler)(int, unsigned long, unsigned long, void *), void *arg);

extern char _gp[];

#define KE_CPUDI 0x80008008
#define KE_SEMA_ZERO 0x80008003

enum { TOP_WAKEUP, TOP_ROTATE, TOP_SUSPEND };

/* requests from interrupt handlers, executed by topThread */
typedef struct {
    int rpos;
    int wpos;
    struct {
        unsigned char cmd;
        unsigned char arg;
    } msg[0x200];
} TopArg;

int topId = 0;
static int topSema;
static TopArg topArg;

static inline int iGetThreadId(void)
{
    int tid;

    asm volatile ("li\t$3, -0x2F\n\tsyscall\n\tmove\t%0, $2" : "=r" (tid) : : "$2", "$3");
    return tid;
}

static void topThread(void *arg)
{
    TopArg *top = arg;
    int i;

    for (;;) {
        WaitSema(topSema);
        i = top->rpos & 0x1FF;
        top->rpos = i + 1;
        switch (top->msg[i].cmd) {
        case TOP_WAKEUP:
            WakeupThread(top->msg[i].arg);
            break;
        case TOP_ROTATE:
            RotateThreadReadyQueue(top->msg[i].arg);
            break;
        case TOP_SUSPEND:
            SuspendThread(top->msg[i].arg);
            break;
        default:
            kprintf("## internel error in libkernl.a!\n");
        }
    }
}

int InitThread(void)
{
    static char stack[0x400] __attribute__((aligned(64)));
    struct ThreadParam tp;
    struct SemaParam sp;

    if (topId > 0) {
        return -1;
    }
    sp.maxCount = 0xFF;
    sp.initCount = 0;
    sp.option = (unsigned int)"SceKerneltopThread";
    topSema = CreateSema(&sp);
    if (topSema < 0) {
        return -1;
    }
    tp.entry = topThread;
    tp.stack = stack;
    tp.stackSize = sizeof(stack);
    tp.gpReg = _gp;
    tp.option = (unsigned int)"SceKerneltopThread";
    tp.initPriority = 0;
    topId = CreateThread(&tp);
    if (topId < 0) {
        DeleteSema(topSema);
        return -1;
    }
    topArg.rpos = 0;
    topArg.wpos = 0;
    StartThread(topId, &topArg);
    ChangeThreadPriority(GetThreadId(), 1);
    return topId;
}

/* Waking up / suspending the running thread from an interrupt handler is queued for topThread. */
int iWakeupThread(int tid)
{
    int self = iGetThreadId();
    int i;

    if (self != tid) {
        return _iWakeupThread(tid);
    }
    if ((unsigned int)self >= 256 || topId == 0) {
        return -1;
    }
    i = topArg.wpos & 0x1FF;
    topArg.wpos = i + 1;
    topArg.msg[i].cmd = TOP_WAKEUP;
    topArg.msg[i].arg = self;
    iSignalSema(topSema);
    return self;
}

int iRotateThreadReadyQueue(int priority)
{
    int i;

    if ((unsigned int)priority >= 128 || topId == 0) {
        return -1;
    }
    i = topArg.wpos & 0x1FF;
    topArg.wpos = i + 1;
    topArg.msg[i].cmd = TOP_ROTATE;
    topArg.msg[i].arg = priority;
    iSignalSema(topSema);
    return priority;
}

int iSuspendThread(int tid)
{
    int self = iGetThreadId();
    int i;

    if (self != tid) {
        return _iSuspendThread(tid);
    }
    if ((unsigned int)self >= 256 || topId == 0) {
        return -1;
    }
    i = topArg.wpos & 0x1FF;
    topArg.wpos = i + 1;
    topArg.msg[i].cmd = TOP_SUSPEND;
    topArg.msg[i].arg = self;
    iSignalSema(topSema);
    return self;
}

static unsigned long _DelayThreadHandler(int id, unsigned long schedule, unsigned long actual, void *arg);

int DelayThread(unsigned int usec)
{
    struct SemaParam sp;
    int status, sema, ret;

    asm volatile ("mfc0\t%0, $12" : "=r" (status));
    if (!(status & 0x10000)) {
        return KE_CPUDI;
    }
    sp.maxCount = 1;
    sp.option = (unsigned int)"SceKernelDelayThread";
    sp.initCount = 0;
    sema = CreateSema(&sp);
    if (sema < 0) {
        return KE_SEMA_ZERO;
    }
    ret = SetTimerAlarm(TimerUSec2BusClock(0, usec), _DelayThreadHandler, (void *)sema);
    if (ret < 0) {
        DeleteSema(sema);
        return ret;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return 0;
}

int StartThread(int tid, void *arg)
{
    struct ThreadParam info;
    char *frame;
    int ret;

    if (QueryIntrContext()) {
        return -1;
    }
    if (!DIntr()) {
        return -1;
    }
    ret = ReferThreadStatus(tid, &info);
    if (ret < 0) {
        EIntr();
        return ret;
    }
    if (info.status != THS_DORMANT) {
        EIntr();
        return -1;
    }
    /* clear the register frame the kernel starts the thread from (gp/sp/fp/ra kept, 64-bit extended) and
     * put arg in its a0 */
    frame = (char *)info.stack + info.stackSize - 0x2A0;
    asm volatile (
        "sq\t$0, 0x0(%0)\n\tsq\t$0, 0x10(%0)\n\tsq\t$0, 0x20(%0)\n\tsq\t$0, 0x30(%0)\n"
        "\tsq\t$0, 0x40(%0)\n\tsq\t$0, 0x50(%0)\n\tsq\t$0, 0x60(%0)\n\tsq\t$0, 0x70(%0)\n"
        "\tsq\t$0, 0x80(%0)\n\tsq\t$0, 0x90(%0)\n\tsq\t$0, 0xa0(%0)\n\tsq\t$0, 0xb0(%0)\n"
        "\tsq\t$0, 0xc0(%0)\n\tsq\t$0, 0xd0(%0)\n\tsq\t$0, 0xe0(%0)\n\tsq\t$0, 0xf0(%0)\n"
        "\tsq\t$0, 0x100(%0)\n\tsq\t$0, 0x110(%0)\n\tsq\t$0, 0x120(%0)\n\tsq\t$0, 0x130(%0)\n"
        "\tsq\t$0, 0x140(%0)\n\tsq\t$0, 0x150(%0)\n\tsq\t$0, 0x160(%0)\n\tsq\t$0, 0x170(%0)\n"
        "\tsq\t$0, 0x180(%0)\n\tsq\t$0, 0x190(%0)\n\tsq\t$0, 0x1a0(%0)\n\tsq\t$0, 0x1b0(%0)\n"
        "\tsq\t$0, 0x200(%0)\n\tsq\t$0, 0x210(%0)\n\tsq\t$0, 0x220(%0)\n\tsq\t$0, 0x230(%0)\n"
        "\tsq\t$0, 0x240(%0)\n\tsq\t$0, 0x250(%0)\n\tsq\t$0, 0x260(%0)\n\tsq\t$0, 0x270(%0)\n"
        "\tsll\t$8, %1, 0\n\tsd\t$8, 0x40(%0)\n"
        "\tlw\t$8, 0x1c0(%0)\n\tsd\t$8, 0x1c0(%0)\n\tsd\t$0, 0x1c8(%0)\n"
        "\tlw\t$8, 0x1d0(%0)\n\tsd\t$8, 0x1d0(%0)\n\tsd\t$0, 0x1d8(%0)\n"
        "\tlw\t$8, 0x1e0(%0)\n\tsd\t$8, 0x1e0(%0)\n\tsd\t$0, 0x1e8(%0)\n"
        "\tlw\t$8, 0x1f0(%0)\n\tsd\t$8, 0x1f0(%0)\n\tsd\t$0, 0x1f8(%0)"
        : : "r" (frame), "r" (arg) : "$8", "memory");
    EIntr();
    return _StartThread(tid, arg);
}

static unsigned long _DelayThreadHandler(int id, unsigned long schedule, unsigned long actual, void *arg)
{
    iSignalSema((int)arg);
    asm volatile ("sync");
    asm volatile ("ei");
    return 0;
}
// SOURCE DATA: .data .rodata .bss
