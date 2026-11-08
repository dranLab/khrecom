/* Timer counters on EE timer 2 (the system time): 128 counters, each optionally with an alarm handler. */
#include "sdk/eekernel.h"
#include "include_asm.h"

void *memset(void *s, int c, unsigned int n);
int AddIntcHandler2(int cause, int (*handler)(int, void *, void *), int next, void *arg);
int RemoveIntcHandler(int cause, int id);
int EnableIntc(int cause);
int DisableIntc(int cause);
int _InitAlarm(void);

#define T2_COUNT ((volatile unsigned int *)0x10001000)
#define T2_MODE ((volatile unsigned int *)0x10001010)
#define INTC_TIM2 11
#define T2_CUE 0x80   /* count enable */
#define T2_EQUF 0x400 /* compare flag */
#define T2_OVFF 0x800 /* overflow flag */
#define BUSCLK 147456000

#define KE_EXIST_TIMER 0x80008001
#define KE_ILLEGAL_ID 0x80008002
#define KE_NO_COUNTER 0x80008005
#define KE_RELEASE_OR_TIMER_HANDLER 0x80000010
#define KE_NO_INTC_HANDLER 0x80009021

typedef unsigned long (*TimerHandler)(int id, unsigned long schedule, unsigned long actual, void *arg, void *addr);

typedef struct Counter {
    struct Counter *next;
    struct Counter *prev;
    volatile int id;
    unsigned int mode;      /* 1: counting, 2: has a handler */
    unsigned long base;     /* system time when started */
    unsigned long count;    /* counted before that */
    unsigned long schedule; /* the handler's count */
    TimerHandler handler;
    int gp;
    void *arg;
    int pad[3];
} Counter;

static struct {
    unsigned long overflow; /* upper bits of the system time */
    int intc_id;
    volatile int serial;
    volatile int used;
    Counter *free;
    Counter *alarm;         /* counters with handlers, by time */
    volatile int handling;  /* id of the counter whose handler runs */
    Counter *next;          /* the alarm after that one */
} g_Timer = { 0, -1, 1, 0, 0, 0, -1, 0 };

static Counter g_CounterBuf[128] __attribute__((aligned(64)));

/* Write a timer register from kernel mode (sets ERL and erets back to the caller). */
void SetT2(volatile unsigned int *reg, unsigned int value)
{
    int eie;

    asm volatile ("mfc0\t%0, $12" : "=r" (eie));
    eie &= 0x10000;
    if (eie) {
        /* di until Status.EIE reads back clear (the registers as in the original) */
        register int st asm("$2");
        register int mask asm("$6") = 0x10000;

        asm volatile (
            ".set\tnoreorder\n"
            "\t.p2align 3\n"
            "1:\tdi\n\tsync.p\n\tmfc0\t%0, $12\n\tand\t%0, %0, %1\n\tnop\n\tnop\n\tnop\n\tbnez\t%0, 1b\n\tnop\n"
            "\t.set\treorder"
            : "=&r" (st) : "r" (mask));
    }
    asm volatile (
        ".set\tnoreorder\n"
        "\tmfc0\t$2, $12\n\tsync.p\n\tori\t$2, $2, 6\n\txori\t$2, $2, 2\n\tor\t$2, $2, %0\n\tmtc0\t$2, $12\n\tsync.p\n"
        "\tsw\t%2, 0(%1)\n\tmtc0\t$31, $30\n\tsync.p\n\teret\n"
        "\t.set\treorder"
        : : "r" (eie), "r" (reg), "r" (value) : "$2");
}

void SetT2_COUNT(unsigned int value)
{
    SetT2((volatile unsigned int *)0xB0001000, value);
}

void SetT2_MODE(unsigned int value)
{
    SetT2((volatile unsigned int *)0xB0001010, value);
}

void SetT2_COMP(unsigned int value)
{
    SetT2((volatile unsigned int *)0xB0001020, value);
}

int cbTimerHandler(int cause, void *arg, void *addr);

int InitTimer(int prescale)
{
    unsigned int mode;
    int i, id, eie;

    if (g_Timer.intc_id >= 0) {
        return KE_EXIST_TIMER;
    }
    g_Timer.overflow = 0;
    g_Timer.used = 0;
    memset(g_CounterBuf, 0, sizeof(g_CounterBuf));
    g_Timer.free = g_CounterBuf;
    for (i = 127; i >= 0; i--) {
        g_CounterBuf[i].next = &g_CounterBuf[i + 1];
    }
    g_CounterBuf[127].next = 0;
    _InitAlarm();
    id = AddIntcHandler2(INTC_TIM2, cbTimerHandler, 0, 0);
    if (id < 0) {
        return KE_NO_INTC_HANDLER;
    }
    g_Timer.intc_id = id;
    eie = DIntr();
    mode = *T2_MODE;
    mode = (mode & ~3) | prescale;
    mode |= 0x300;
    if (!(mode & T2_CUE)) {
        mode |= 0xC80;
        SetT2_COUNT(0);
        SetT2_COMP(0xFFFF);
    }
    SetT2_MODE(mode);
    EnableIntc(INTC_TIM2);
    if (eie) {
        EIntr();
    }
    return 0;
}

int EndTimer(void)
{
    int eie;

    if (g_Timer.intc_id < 0) {
        return KE_EXIST_TIMER;
    }
    if (g_Timer.used) {
        return KE_RELEASE_OR_TIMER_HANDLER;
    }
    eie = DIntr();
    if (RemoveIntcHandler(INTC_TIM2, g_Timer.intc_id) == 0) {
        DisableIntc(INTC_TIM2);
        SetT2_MODE(0xC00);
        SetT2_COUNT(0);
    }
    g_Timer.overflow = 0;
    g_Timer.intc_id = -1;
    if (eie) {
        EIntr();
    }
    return 0;
}

int GetTimerPreScaleFactor(void)
{
    if (g_Timer.intc_id < 0) {
        return KE_EXIST_TIMER;
    }
    return *T2_MODE & 3;
}

unsigned long iGetTimerSystemTime(void);
static void SetNextComp(unsigned long now);

int StartTimerSystemTime(void)
{
    unsigned int mode;
    int eie;

    eie = DIntr();
    mode = *T2_MODE;
    if (mode & T2_CUE) {
        if (eie) {
            EIntr();
        }
        return 1;
    }
    SetT2_MODE((mode & ~0xC00) | T2_CUE);
    SetNextComp(iGetTimerSystemTime());
    if (eie) {
        EIntr();
    }
    return 0;
}

int StopTimerSystemTime(void)
{
    unsigned int mode;
    int eie;

    eie = DIntr();
    mode = *T2_MODE;
    if (!(mode & T2_CUE)) {
        if (eie) {
            EIntr();
        }
        return 0;
    }
    SetT2_MODE(mode & ~0xC80);
    if (eie) {
        EIntr();
    }
    return 1;
}

#define ALARM_TIME(c) ((c)->schedule + (c)->base - (c)->count)

/* Program the compare register for the next alarm (alarms closer than 0x7333 to it are merged). */
static void SetNextComp(unsigned long now)
{
    Counter *c;
    unsigned long t, t2;
    unsigned int mode;
    int prescale;

    c = g_Timer.alarm;
    if (g_Timer.handling >= 0) {
        return;
    }
    mode = *T2_MODE;
    if (!c) {
        SetT2_COMP(0);
        SetT2_MODE(mode & ~T2_OVFF);
        return;
    }
    t = ALARM_TIME(c);
    for (c = c->next; c; c = c->next) {
        t2 = ALARM_TIME(c);
        if (t2 < t + 0x7333) {
            t = t2;
        } else {
            break;
        }
    }
    prescale = mode & 3;
    if ((long)(t - now) < 0x7333) {
        SetT2_COMP(*T2_COUNT + (int)(0x7333UL >> (prescale * 4)));
        SetT2_MODE(mode & ~T2_OVFF);
        return;
    }
    SetT2_MODE(mode & ~T2_OVFF);
    SetT2_COMP((int)(t >> (prescale * 4)));
}

static void InsertAlarm(Counter *c)
{
    Counter *prev = 0, *p;
    unsigned long t = ALARM_TIME(c);

    for (p = g_Timer.alarm; p; prev = p, p = p->next) {
        if (t < ALARM_TIME(p)) {
            break;
        }
    }
    c->prev = prev;
    c->next = p;
    if (p) {
        p->prev = c;
    }
    if (prev) {
        prev->next = c;
    } else {
        g_Timer.alarm = c;
    }
}

static Counter *UnlinkAlarm(Counter *c)
{
    Counter *next = c->next;

    if (c->prev) {
        c->prev->next = next;
    } else {
        g_Timer.alarm = next;
    }
    if (next) {
        next->prev = c->prev;
    }
    c->prev = 0;
    return next;
}

/* Parked: scheduling (32 lines; 45 before the handler's time argument in a local, decomp-permuter) */
#if 0
static int cbTimerHandler(int cause, void *arg, void *addr)
{
    Counter *c;
    unsigned int count, mode;
    unsigned long now, t, ret, tt;

    if ((*T2_MODE & T2_EQUF) && (c = g_Timer.alarm)) {
        do {
            count = *T2_COUNT;
            mode = *T2_MODE;
            t = ALARM_TIME(c);
            if (mode & T2_OVFF) {
                g_Timer.overflow++;
                SetT2_MODE(mode & ~T2_EQUF);
                count = *T2_COUNT;
            }
            now = ((g_Timer.overflow << 16) | count) << ((mode & 3) * 4);
            if (now < t) {
                break;
            }
            g_Timer.next = UnlinkAlarm(c);
            tt = now + c->count - c->base;
            g_Timer.handling = ((int)c << 4) | c->id;
            asm volatile ("lw\t$gp, %0" : : "m" (c->gp));
            ret = c->handler(((int)c << 4) | c->id, c->schedule, tt, c->arg, addr);
            if (ret == 0) {
                c->mode &= ~2;
            } else if (ret == (unsigned long)-1) {
                c->id = 0;
                c->mode = 0;
                c->next = g_Timer.free;
                g_Timer.free = c;
                g_Timer.used--;
            } else {
                if (ret <= 0x3998) {
                    ret = 0x3999;
                }
                c->schedule += ret;
                InsertAlarm(c);
            }
            c = g_Timer.next;
        } while (c);
    }
    g_Timer.handling = -1;
    g_Timer.next = 0;
    count = *T2_COUNT;
    mode = *T2_MODE;
    if (mode & T2_OVFF) {
        g_Timer.overflow++;
        SetT2_MODE(mode & ~T2_EQUF);
        count = *T2_COUNT;
    }
    SetNextComp(((g_Timer.overflow << 16) | count) << ((mode & 3) * 4));
    count = *T2_COUNT;
    mode = *T2_MODE;
    if (mode & T2_OVFF) {
        g_Timer.overflow++;
        SetT2_MODE(mode & ~T2_EQUF);
        count = *T2_COUNT;
    }
    now = ((g_Timer.overflow << 16) | count) << ((mode & 3) * 4);
    asm volatile ("sync");
    asm volatile ("ei");
    return 0;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/timer", cbTimerHandler);
#endif

/* The system time: (overflows << 16 | T2_COUNT) << prescale bits (hand-written; reads past an overflow). */
#define GET_SYSTEM_TIME(t)                                                                                       \
    asm volatile (".set\tnoreorder\n"                                                                          \
                  "\tlwu\t%0, 0(%1)\n\tlw\t$8, 0x10(%1)\n\tld\t$10, %2\n\tandi\t$9, $8, 0x800\n"               \
                  "\tbeqz\t$9, 1f\n\taddiu\t$9, $0, 2\n\tlwu\t%0, 0(%1)\n\tdaddiu\t$10, $10, 1\n"             \
                  "1:\tandi\t$8, $8, 3\n\tdsll\t$10, $10, 16\n\tmovz\t$9, $0, $8\n\tor\t%0, %0, $10\n"          \
                  "\tdsllv\t$9, $9, $8\n\tdsllv\t%0, %0, $9\n\t.set\treorder"                                 \
                  : "=&r" (t) : "r" (T2_COUNT), "m" (g_Timer.overflow) : "$8", "$9", "$10")

unsigned long iGetTimerSystemTime(void)
{
    unsigned long t;

    GET_SYSTEM_TIME(t);
    return t;
}

unsigned long GetTimerSystemTime(void)
{
    unsigned long t;
    int eie;

    eie = DIntr();
    GET_SYSTEM_TIME(t);
    if (eie) {
        EIntr();
    }
    return t;
}

int iAllocTimerCounter(void)
{
    Counter *c = g_Timer.free;

    if (!c) {
        return KE_NO_COUNTER;
    }
    g_Timer.used++;
    c->handler = 0;
    c->mode = 0;
    g_Timer.free = c->next;
    c->count = 0;
    g_Timer.serial++;
    c->id = ((g_Timer.serial << 1) & 0x3FE) | 1;
    return ((int)c << 4) | c->id;
}

int AllocTimerCounter(void)
{
    int eie, id;

    eie = DIntr();
    id = iAllocTimerCounter();
    if (eie) {
        EIntr();
    }
    return id;
}

#define COUNTER(id) ((Counter *)(((unsigned int)(id) >> 10) << 6))
#define BAD_ID(id, c) ((id) < 0 || ((id) & 0x3FF) != (c)->id)

int iFreeTimerCounter(int id)
{
    Counter *c = COUNTER(id), *next;

    if (BAD_ID(id, c)) {
        return KE_ILLEGAL_ID;
    }
    if (g_Timer.handling == id) {
        return KE_RELEASE_OR_TIMER_HANDLER;
    }
    if (c->mode & 2) {
        next = UnlinkAlarm(c);
        if (g_Timer.next == c) {
            g_Timer.next = next;
        }
    }
    c->id = 0;
    c->mode = 0;
    c->next = g_Timer.free;
    g_Timer.free = c;
    g_Timer.used--;
    return 0;
}

int FreeTimerCounter(int id)
{
    int eie, ret;

    eie = DIntr();
    ret = iFreeTimerCounter(id);
    if (eie) {
        EIntr();
    }
    return ret;
}

int iGetTimerUsedUnusedCounters(int *used, int *unused)
{
    if (g_Timer.intc_id < 0) {
        return KE_EXIST_TIMER;
    }
    if (used) {
        *used = g_Timer.used;
    }
    if (unused) {
        *unused = 128 - g_Timer.used;
    }
    return 0;
}

int GetTimerUsedUnusedCounters(int *used, int *unused)
{
    int eie, ret;

    eie = DIntr();
    ret = iGetTimerUsedUnusedCounters(used, unused);
    if (eie) {
        EIntr();
    }
    return ret;
}

int iStartTimerCounter(int id)
{
    Counter *c = COUNTER(id);
    unsigned long now;

    if (BAD_ID(id, c)) {
        return KE_ILLEGAL_ID;
    }
    if (g_Timer.handling == id) {
        return KE_RELEASE_OR_TIMER_HANDLER;
    }
    if (c->mode & 1) {
        return 1;
    }
    now = iGetTimerSystemTime();
    c->base = now;
    c->mode |= 1;
    if (c->mode & 2) {
        InsertAlarm(c);
        SetNextComp(now);
    }
    return 0;
}

int StartTimerCounter(int id)
{
    int eie, ret;

    eie = DIntr();
    ret = iStartTimerCounter(id);
    if (eie) {
        EIntr();
    }
    return ret;
}

int iStopTimerCounter(int id)
{
    Counter *c = COUNTER(id), *next;
    unsigned long now;
    unsigned int mode;

    if (BAD_ID(id, c)) {
        return KE_ILLEGAL_ID;
    }
    if (g_Timer.handling == id) {
        return KE_RELEASE_OR_TIMER_HANDLER;
    }
    if (!(c->mode & 1)) {
        return 0;
    }
    now = iGetTimerSystemTime();
    mode = c->mode;
    c->count += now - c->base;
    c->mode = mode & ~1;
    if (mode & 2) {
        next = UnlinkAlarm(c);
        if (g_Timer.next == c) {
            g_Timer.next = next;
        }
        SetNextComp(now);
    }
    return 1;
}

int StopTimerCounter(int id)
{
    int eie, ret;

    eie = DIntr();
    ret = iStopTimerCounter(id);
    if (eie) {
        EIntr();
    }
    return ret;
}

unsigned long SetTimerCount(int id, unsigned long count)
{
    Counter *c = COUNTER(id);
    unsigned long old, now;
    int eie;

    eie = DIntr();
    if (BAD_ID(id, c) || g_Timer.handling == id) {
        if (eie) {
            EIntr();
        }
        return -1;
    }
    old = c->count;
    if (c->mode & 1) {
        now = iGetTimerSystemTime();
        old += now - c->base;
        c->base = now;
        c->count = count;
    } else {
        c->count = count;
    }
    if (eie) {
        EIntr();
    }
    return old;
}

unsigned long iGetTimerBaseTime(int id)
{
    Counter *c = COUNTER(id);

    if (BAD_ID(id, c)) {
        return -1;
    }
    if (c->mode & 1) {
        return c->base - c->count;
    }
    return 0;
}

unsigned long GetTimerBaseTime(int id)
{
    unsigned long ret;
    int eie;

    eie = DIntr();
    ret = iGetTimerBaseTime(id);
    if (eie) {
        EIntr();
    }
    return ret;
}

unsigned long iGetTimerCount(int id)
{
    Counter *c = COUNTER(id);
    unsigned long count;

    if (BAD_ID(id, c)) {
        return -1;
    }
    count = c->count;
    if (c->mode & 1) {
        count += iGetTimerSystemTime() - c->base;
    }
    return count;
}

unsigned long GetTimerCount(int id)
{
    unsigned long ret;
    int eie;

    eie = DIntr();
    ret = iGetTimerCount(id);
    if (eie) {
        EIntr();
    }
    return ret;
}

int iSetTimerHandler(int id, unsigned long schedule, TimerHandler handler, void *arg)
{
    Counter *c = COUNTER(id), *next;

    if (BAD_ID(id, c)) {
        return KE_ILLEGAL_ID;
    }
    if (g_Timer.handling == id) {
        return KE_RELEASE_OR_TIMER_HANDLER;
    }
    if (c->mode & 2) {
        next = UnlinkAlarm(c);
        if (g_Timer.next == c) {
            g_Timer.next = next;
        }
    }
    c->handler = handler;
    if (!handler) {
        c->mode &= ~2;
    } else {
        c->mode |= 2;
        c->schedule = schedule;
        asm volatile ("sw\t$gp, %0" : "=m" (c->gp) : : "memory");
        c->arg = arg;
        if (c->mode & 1) {
            InsertAlarm(c);
        }
    }
    SetNextComp(iGetTimerSystemTime());
    return 0;
}

int SetTimerHandler(int id, unsigned long schedule, TimerHandler handler, void *arg)
{
    int eie, ret;

    eie = DIntr();
    ret = iSetTimerHandler(id, schedule, handler, arg);
    if (eie) {
        EIntr();
    }
    return ret;
}

void TimerBusClock2USec(unsigned long clock, unsigned int *sec, unsigned int *usec)
{
    int s = clock / BUSCLK;

    if (sec) {
        *sec = s;
    }
    if (usec) {
        *usec = (clock - (unsigned long)(unsigned int)s * BUSCLK) * 1000000 / BUSCLK;
    }
}

unsigned long TimerUSec2BusClock(unsigned int sec, unsigned int usec)
{
    return (unsigned long)sec * BUSCLK + (unsigned long)usec * BUSCLK / 1000000;
}

/* the nops before div.s are 991111-01's EE erratum padding (build.yaml) */
float TimerBusClock2Freq(unsigned long clock)
{
    return 147456000.0f / clock;
}

unsigned long TimerFreq2BusClock(float freq)
{
    return 147456000.0f / freq;
}
// SOURCE DATA: .data .bss
